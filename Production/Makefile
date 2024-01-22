# Compiler options
CC := gcc
CXX := g++
CFLAGS := -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -Wno-unused-but-set-variable -std=c99 -D_POSIX_C_SOURCE=200112L
CXXFLAGS := -Wall -Wextra

# Source files
SRCS := $(wildcard *.c) $(wildcard *.cpp)

# Object files
OBJS := $(SRCS:.c=.o)
OBJS := $(OBJS:.cpp=.o)

# Target executable
TARGET := Visiolock.out

# Linker flags
LDFLAGS := -lpthread -lrt -lgpiod -lsqlite3 -lm

# Default target
all: $(TARGET)

# Compile C source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile C++ source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link object files
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
	sudo chown root.root $@
	sudo chmod 4755 $@

# Clean build artifacts
clean:
	rm -f $(OBJS) $(TARGET)


