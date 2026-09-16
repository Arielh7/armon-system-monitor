CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11 -Iinclude
TARGET = build/monitor
SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:src/%.c=build/%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build:
	mkdir -p build

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf build

.PHONY: all run clean