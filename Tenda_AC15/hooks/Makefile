TOOLCHAIN = /root/buildroot-2020.02.9/output/host/usr/bin

export PATH := $(TOOLCHAIN):$(PATH)

# CC = arm-linux-gnueabihf-gcc
# uClibc
CC = arm-buildroot-linux-uclibcgnueabi-gcc
LD = arm-buildroot-linux-uclibcgnueabi-ld

CFLAGS = -O2 -fPIC -Wall
CFLAGS += -DVERBOSE
# CFLAGS += -DHOOK_SYSTEM
LDFLAGS = -shared -nostdlib

TEST_CFLAGS = -O2 -fPIC -Wall
TEST_LDFLAGS = -shared

BIN=bin
SRC=src
OBJ=obj

SOURCES=tenda_hooks.c
OBJECTS=$(SOURCES:.c=.o)
TARGET=tenda_hooks.so

all: directories $(SRC)/$(SOURCES) $(BIN)/$(TARGET)

$(BIN)/$(TARGET): $(OBJ)/$(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJ)/$(OBJECTS) -o $@

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJ)/* $(BIN)/*

directories: $(BIN) $(OBJ)

$(BIN):
	mkdir -p $(BIN)

$(OBJ):
	mkdir -p $(OBJ)
