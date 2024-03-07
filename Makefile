
# compiler
CC := gcc

# other variables
OUTPUT := tst.e
CC_FLAGS := -g -O3 -Wall -Wextra -Wpedantic -mavx2 -mfma -mcx16
LD_FLAGS := -lm -lpthread -O3

HEADERS := ./graphy/cryptography.h ./graphy/stream/stream.h ./graphy/bits.h ./analysis/brute-force/brute_force.h
FILES := ./graphy/DES/des.c ./graphy/FEAL/feal.c ./graphy/GOST/gost.c ./graphy/IDEA/idea.c ./graphy/LOKI91/loki.c ./graphy/MMB/mmb.c ./graphy/stream/stream.c ./analysis/brute-force/brute_force.c

# build benchmarks
clock:
	$(CC) $(CC_FLAGS) $(LD_FLAGS) -o clock.e $(HEADERS) $(FILES) ./graphy/speed_test.c

# build tests
test:
	$(CC) $(CC_FLAGS) $(LD_FLAGS) -o test.e $(HEADERS) $(FILES) ./graphy/functionality_test.c

# build brute force attacks
force:
	$(CC) $(CC_FLAGS) $(LD_FLAGS) -o force.e $(HEADERS) $(FILES) ./analysis/brute_force_test.c
