
# compiler
CC := gcc

# other variables
OUTPUT := tst.e
CC_FLAGS := -g -O3 -Wall -Wextra -Wpedantic -mavx2 -mfma -mcx16
LD_FLAGS := -O3

SRC_FILES := $(wildcard *.c)
H_FILES := $(wildcard *.h)
OBJ_FILES := $(SRC_FILES:.c=.o)

all: $(OUTPUT)

$(OUTPUT): $(OBJ_FILES) Makefile
	$(CC) $(OBJ_FILES) $(H_FILES) $(LD_FLAGS) -o ./$@

.c.o:
	$(CC) -c $(CC_FLAGS) $< -o $@

clean:
	rm -f $(OBJ_FILES) ./tst.e

run: $(OUTPUT)
	./tst.e
