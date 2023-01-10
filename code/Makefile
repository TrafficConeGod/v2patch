TARGET := patch

SOURCES := $(wildcard src/*.c) $(wildcard src/*.cpp)
LIBS := 
WARNS := -Wall -Wextra -Wdouble-promotion -Wduplicate-decl-specifier -Wformat=2 -Wnull-dereference -Wimplicit-fallthrough
OBJECTS := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))
DEPENDS := $(patsubst %.c,%.d,$(patsubst %.cpp,%.d,$(SOURCES)))

override CFLAGS += -fno-pic -nostdlib -m32 -O3 -Werror -std=c2x
CXXFLAGS = $(CFLAGS) -fno-exceptions -std=c++2a

.PHONY: build run clean

build: $(OBJECTS)
	ld -nostdlib -melf_i386 -E -r $(OBJECTS) -o $(TARGET).o

clean:
	$(RM) $(OBJECTS) $(DEPENDS)

-include $(DEPENDS)

%.o: %.c Makefile
	$(CC) $(CFLAGS) $(WARNS) -MMD -MP -c $< -o $@

%.o: %.cpp Makefile
	$(CXX) $(CXXFLAGS) $(WARNS) -MMD -MP -c $< -o $@