
# Simple Makefile

CC = gcc

CFLAGS = -g -Wall -Werror -Iinclude

LINKFLAGS = -lgmp

SRCS = $(shell find . -name "*.c")

TARGET = pem2json

all:
	@echo [Building...]
	@echo
	@time -p $(CC) $(CFLAGS) $(SRCS) $(LINKFLAGS) -o $(TARGET)
	@echo
	@echo "[Build is done]"

clean:
	@echo [Cleaning...]
	@rm -f $(TARGET)
	@echo "[Cleaning is done]"