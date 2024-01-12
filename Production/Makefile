CC = gcc
CPP = g++
CFLAGS = -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -std=c99 -D_POSIX_C_SOURCE=200112L
LDFLAGS = -lsqlite3 -lm -lrt -lgpiod

SRCS = starter.c brain.c archivist.c guard.c manager.c sha256.c doorman.c rfid.c
OUT = Visiolock.out

all: $(OUT)

$(OUT): $(SRCS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(OUT)
