CC = gcc
CPPFLAGS = -D_DEFAULT_SOURCE
CFLAGS = -Wall -Wextra -Werror -std=c99 -Wvla
LDFLAGS =
VPATH = src

TARGET_LIB = libmalloc.so
OBJS = ./src/malloc.o ./src/my_malloc.o ./src/buckets/bucket.o ./src/tools.o

all: library

library: $(TARGET_LIB)
$(TARGET_LIB): CFLAGS += -pedantic -fvisibility=hidden -fPIC
$(TARGET_LIB): LDFLAGS += -Wl,--no-undefined -shared
$(TARGET_LIB): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

debug: CFLAGS += -g
debug: clean $(TARGET_LIB)

check: CFLAGS += -DMY_MALLOC -g
check: LDFLAGS += -lcriterion -g
check: $(OBJS) ./tests/crit.o
	$(CC) $(LDFLAGS) -o testcrit $(OBJS) ./tests/crit.o

test: CFLAGS += -DMY_MALLOC -g
test: LDFLAGS += -g -fsanitize=address
test: $(OBJS) ./tests/test.o
	$(CC) $(LDFLAGS) -o btest $(OBJS) ./tests/test.o

clean:
	$(RM) $(TARGET_LIB) $(OBJS) testcrit ./tests/crit.o ./btest ./tests/test.o

.PHONY: all $(TARGET_LIB) clean
