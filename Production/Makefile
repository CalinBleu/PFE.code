CC = gcc
CFLAGS = -std=c99
LDFLAGS = -lsqlite3 -lm -lrt -lgpiod

SRCS = starter.c brain.c archivist.c guard.c manager.c sha256.c
OUT = Visiolock

all: $(OUT)

$(OUT): $(SRCS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(OUT)
