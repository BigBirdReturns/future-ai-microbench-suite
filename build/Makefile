# Makefile for Future AI Microbench Suite (FAMBS)

# Toolchain
CC = riscv64-unknown-elf-gcc
OBJDUMP = riscv64-unknown-elf-objdump
OBJCOPY = riscv64-unknown-elf-objcopy

# Flags
CFLAGS = -march=rv64gc -mabi=lp64d -O2 -nostdlib -nostartfiles -ffreestanding
LDFLAGS = -T linker.ld -nostdlib

# Paths
SRC_DIR = ../src
BUILD_DIR = .

# Source files
SRCS = $(SRC_DIR)/harness.c \
       $(SRC_DIR)/mak_micro_attention.c \
       $(SRC_DIR)/mal_micro_autonomy.c \
       $(SRC_DIR)/mbw_bandwidth.c \
       $(SRC_DIR)/mgt_micro_graph.c \
       $(SRC_DIR)/mrk_micro_retrieval.c \
       $(SRC_DIR)/pck_pointer_chase.c

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Target ELF
TARGET = fambs.elf

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^
	$(OBJDUMP) -d $@ > fambs.dis

# Compile
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJS) $(TARGET) fambs.dis

# Phony targets
.PHONY: all clean
