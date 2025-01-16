# Description: Makefile for client

EXECUTE := client.out # Name of the executable file

# Compiler
CC := $(shell which gcc)
# Compiler flags
CFLAGS := -Wall -std=c17 -g -Og -Iinclude
# Linker flags
LDFLAGS := -lpthread -lcrypto -lmysqlclient -ljson-c

# Source files
SRC := $(wildcard **/*.c) $(wildcard *.c)
# Object files
OBJ := $(SRC:.c=.o)

$(EXECUTE): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) $(OBJ)

