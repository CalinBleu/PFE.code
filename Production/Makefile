CC = gcc
CFLAGS = -std=c99
LDFLAGS = -lsqlite3 -lm

SRCS = starter.c brain.c archivist.c
OUT = Visiolock

all: $(OUT)

$(OUT): $(SRCS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(OUT)
