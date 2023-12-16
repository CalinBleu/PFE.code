CC = gcc
CFLAGS = -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -std=c99 -D_POSIX_C_SOURCE=199309L
LDFLAGS = -lsqlite3 -lm -lrt -lgpiod

SRCS = starter.c brain.c archivist.c guard.c manager.c sha256.c doorman.c
OUT = Visiolock.out

all: $(OUT)

$(OUT): $(SRCS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(OUT)
