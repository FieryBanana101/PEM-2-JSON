
# Simple Makefile

CC = gcc

CFLAGS = -g -Wall -Werror -Iinclude

SRCS = $(shell find . -name "*.c")

TARGET = pem2json

all:
	@echo [Building...]
	@echo
	@time -p $(CC) -g $(CFLAGS) $(SRCS) -o $(TARGET)
	@echo
	@echo "[Build is done]"

clean:
	@echo [Cleaning...]
	@rm -f $(TARGET)
	@echo "[Cleaning is done]"