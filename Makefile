# Directories
K = kernel
C = cpu
D = d_str

# Object files
OBJS1 = \
    $(K)/interrupthndl.o \
    $(K)/lock.o \
    $(K)/proc_simltr.o \
    $(K)/round_robin.o \
    $(K)/io.o \
    $(K)/$(D)/proc_queue.o \
    $(K)/$(D)/tree.o \
	$(K)/main.o

OBJS2 = \
    $(C)/cpu.o

OBJS = $(OBJS1) $(OBJS2)

# Compiler and flags
CC = g++
CFLAGS = -c -Wall -O2 -g -I$(K) -I$(C) -I.

# Build the final executable
all: os

os: $(OBJS)
	$(CC) -g $^ -o $@

# Rules for kernel object files
$(K)/%.o: $(K)/%.cpp
	$(CC) $(CFLAGS) $< -o $@

# Rules for CPU object files
$(C)/%.o: $(C)/%.cpp
	$(CC) $(CFLAGS) $< -o $@

# Clean up object files and the binary
clean:
	rm -f $(OBJS) os
