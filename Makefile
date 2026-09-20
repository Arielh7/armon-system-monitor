CC = gcc
CFLAGS = -Wall -Wextra -g -std=gnu11 -Iinclude
TARGET = build/armon
SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:src/%.c=build/%.o)

# Installation paths
PREFIX ?= $(HOME)/.local
BINDIR = $(PREFIX)/bin
BINNAME = armon

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build:
	mkdir -p build

run: $(TARGET)
	./$(TARGET)

install: $(TARGET)
	mkdir -p $(BINDIR)
	cp $(TARGET) $(BINDIR)/$(BINNAME)
	@echo "Installed $(BINNAME) to $(BINDIR)/$(BINNAME)"

uninstall:
	rm -f $(BINDIR)/$(BINNAME)
	@echo "Removed $(BINDIR)/$(BINNAME)"

reinstall: uninstall install

clean:
	rm -rf build

.PHONY: all run clean install uninstall reinstall