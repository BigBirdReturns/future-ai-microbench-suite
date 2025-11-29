RISCV_PREFIX ?= riscv64-unknown-elf
CC      := $(RISCV_PREFIX)-gcc
CFLAGS  := -O2 -std=c11 -I../include
LDFLAGS :=

SRC := \
  ../src/harness.c \
  ../src/svk_sparse_vec.c \
  ../src/mgt_micro_graph.c \
  ../src/mak_micro_attention.c \
  ../src/mrk_micro_retrieval.c \
  ../src/pck_pointer_chase.c \
  ../src/mbw_bandwidth.c \
  ../src/mal_micro_autonomy.c

all: fambs.elf

fambs.elf: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $@ $(LDFLAGS)

clean:
	rm -f fambs.elf
