
# compiler
CC := gcc

# other variables
OUTPUT := tst.e
CC_FLAGS := -g -O3 -Wall -Wextra -Wpedantic -mavx2 -mfma -mcx16
LD_FLAGS := -O3 -lm

HEADERS := ./graphy/cryptography.h ./graphy/stream/stream.h ./graphy/bits.h
FILES := ./graphy/DES/des.c ./graphy/FEAL/feal.c ./graphy/GOST/gost.c ./graphy/IDEA/idea.c ./graphy/LOKI91/loki.c ./graphy/MMB/mmb.c ./graphy/stream/stream.c

# build benchmarks
clock:
	$(CC) $(CC_FLAGS) $(LD_FLAGS) -o clock.e $(HEADERS) $(FILES) ./graphy/speed_test.c



# build tests
test:
	$(CC) $(CC_FLAGS) $(LD_FLAGS) -o test.e $(HEADERS) $(FILES) ./graphy/functionality_test.c
