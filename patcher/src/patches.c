#include "patches.h"
#include "util.h"
#include <string.h>
#include <elf.h>
#include <stdio.h>
#include <bits/types/error_t.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <stdalign.h>

_Alignas(64) static const char* function_names[] = {
    "lua_command_execute"
};

typedef struct function_size_offset_pair {
    size_t size;
    size_t offset;
} function_size_offset_pair_t;

_Alignas(64) static function_size_offset_pair_t function_size_offset_pairs[] = {
    { 335, 0x1ceb0 }
};

typedef struct stat stat_t;

#define CREATING_CHECKSUM_MSG 0xa061b8

static error_t load_files(size_t num_files, const char* const paths[], uint8_t* buffers[]) {
    for (size_t i = 0; i < num_files; i++) {
        stat_t st;
        if (stat(paths[i], &st) != 0) {
            printf("File stat error: %s\n", strerror(errno));
            return 1;
        }
        
        FILE* file = fopen(paths[i], "rb");
        if (file == NULL) {
            printf("File open error: %s\n", strerror(errno));
            fclose(file);
            return 1;
        }

        buffers[i] = malloc(st.st_size);
        if (fread(buffers[i], st.st_size, 1, file) != 1) {
            printf("File read error: %s\n", strerror(errno));
            fclose(file);
            free(buffers[i]);
            return 1;
        }

        fclose(file);
    }

    return 0;
}

typedef struct symbol_string_pairs {
    size_t num_symbols;
    const Elf32_Sym* symbols;
    const char* strings;
} symbol_string_pairs_t;

static symbol_string_pairs_t get_symbol_string_pairs(const uint8_t* patch_data) {
    const Elf32_Ehdr* elf_header = (const Elf32_Ehdr*)patch_data;
    
    size_t num_section_headers = elf_header->e_shnum;
    const Elf32_Shdr* section_headers = (const Elf32_Shdr*)&patch_data[elf_header->e_shoff];

    for (size_t i = 0; i < num_section_headers; i++) {
        const Elf32_Shdr* section_header = &section_headers[i];
        if (section_header->sh_type != SHT_SYMTAB) {
            continue;
        }
        const Elf32_Shdr* symbol_section_header = section_header;
        const Elf32_Shdr* string_section_header = &section_headers[symbol_section_header->sh_link];

        size_t num_symbols = symbol_section_header->sh_size / sizeof(Elf32_Sym);
        const Elf32_Sym* symbols = (const Elf32_Sym*)&patch_data[symbol_section_header->sh_offset];
        const char* strings = (const char*)&patch_data[string_section_header->sh_offset];

        return (symbol_string_pairs_t){ num_symbols, symbols, strings };
    }

    return (symbol_string_pairs_t){ 0, NULL, NULL };
}

void apply_patches(UNUSED size_t size, uint8_t* data) {
    const char confirm_msg[15] = "PATCH_WORKED0";
    memcpy(&data[CREATING_CHECKSUM_MSG], confirm_msg, sizeof(confirm_msg));

    const char* path = "../code/patch.o";
    uint8_t* buffer;
    load_files(1, &path, &buffer);

    const uint8_t* patch_data = buffer;

    const Elf32_Ehdr* elf_header = (const Elf32_Ehdr*)patch_data;

    if (memcmp(elf_header->e_ident, ELFMAG, SELFMAG) != 0) {
        printf("Invalid elf file\n");
        goto end;
    }

    symbol_string_pairs_t pairs = get_symbol_string_pairs(patch_data);
    if (pairs.symbols == NULL) {
        printf("No symbols\n");
        goto end;
    }

    const Elf32_Shdr* section_headers = (const Elf32_Shdr*)&patch_data[elf_header->e_shoff];

    for (size_t i = 0; i < pairs.num_symbols; i++) {
        const Elf32_Sym* symbol = &pairs.symbols[i];
        const char* name = &pairs.strings[symbol->st_name];
        
        for (size_t i = 0; i < NUM_ELEMS(function_names); i++) {
            // Check if our symbol name is the same as a function name
            if (strcmp(name, function_names[i]) != 0) {
                continue;
            }

            function_size_offset_pair_t size_offset_pair = function_size_offset_pairs[i];

            if (symbol->st_size > size_offset_pair.size) {
                printf("Injected code is too big for function: %s\n", name);
                goto end;
            }

            const Elf32_Shdr* text_section_header = &section_headers[symbol->st_shndx];
            const uint8_t* text_section = &patch_data[text_section_header->sh_offset];

            const uint8_t* function_text = &text_section[symbol->st_value];
            // Patch the original file
            memcpy(&data[size_offset_pair.offset], function_text, symbol->st_size);

            printf("%s (0x%lx): ", name, size_offset_pair.offset);
            for (size_t i = 0; i < symbol->st_size; i++) {
                printf("%02x ", function_text[i]);
            }
            break;
        }
    }

    end:
    free(buffer);
}