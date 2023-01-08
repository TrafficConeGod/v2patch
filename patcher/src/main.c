#include "patches.h"
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

typedef struct stat stat_t;

int main() {
    stat_t from_stat;
    if (stat("v2game.exe", &from_stat) != 0) {
        printf("From file stat error: %s\n", strerror(errno));
        return 1;
    }

    FILE* from_file = fopen("v2game.exe", "rb");
    if (from_file == NULL) {
        printf("From file open error: %s\n", strerror(errno));
        fclose(from_file);
        return 1;
    }

    void* data = malloc(from_stat.st_size);
    if (fread(data, from_stat.st_size, 1, from_file) != 1) {
        printf("From file read error: %s\n", strerror(errno));
        fclose(from_file);
        free(data);
        return 1;
    }

    fclose(from_file);

    FILE* to_file = fopen("v2patch.exe", "wb");
    if (to_file == NULL) {
        printf("To file open error: %s\n", strerror(errno));
        fclose(to_file);
        free(data);
        return 1;
    }

    apply_patches(from_stat.st_size, data);

    if (fwrite(data, from_stat.st_size, 1, to_file) != 1) {
        printf("To file write error: %s\n", strerror(errno));
        fclose(to_file);
        free(data);
        return 1;
    }

    fclose(to_file);
    free(data);
    
    return 0;
}