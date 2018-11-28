# How To: Use 'make'
# make 'filename.c' = cc filename.c -o filename
# Makefile needs to be in the same directory as the project .c files
# make sure you only enter TAB characters! NOT: SPACE or mixtures of tabs and spaces
#
# ####################
# Variable Definitions
# ####################
# CFLAGS - Compiler Flags
# -g 		adds debugging information to the executable file
# -Wall 	turns on most, but not all, compiler warnings
# -c		compile source files without linking
# CC - Compiler, tells make which compiler to use
# 'gcc'		use for c
# 'g++'		use for c++
# 'javac'	use for java

CC=gcc
CFLAGS=-Wall -g -I.
TARGET=main

# typing just 'make' will invoke the first target entry in the file
# Convention: 'all' or 'default'
default: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) -c -o obj/$(TARGET).o src/$(TARGET).c
	$(CC) $(CFLAGS) -o bin/$(TARGET) obj/$(TARGET).o

run: $(TARGET)
	bin/$(TARGET) $(a) $(b) $(c) $(d) $(e)

# 'make clean' will execute the code below
# RM - is a predifined macro in make (RM = rm -f)
clean:
	rm -r ./obj/*
	rm -r ./bin/*
	# find . -name $(TARGET) -type f -exec $(RM) {} \;
	# find . -name $(TARGET).dSYM -type d -exec rm -r {} \;
