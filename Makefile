CC=gcc
CFLAGS=-std=c89 -Wall -Wall -Werror -Wextra -Wpedantic -Wshadow -Wformat=2 -Wcast-align -Wconversion \
	-Wsign-conversion -Wnull-dereference -Wdouble-promotion -Wfloat-equal -Wstrict-prototypes -Wmissing-prototypes \
	-Wuninitialized -Wundef -Wwrite-strings -Wvla -Wswitch-default
TARGET=arvds

all: $(TARGET)
	@mkdir -p bin
	$(CC) $(CFLAGS) -o bin/$(TARGET) arvds.c

clean:
	rm -f bin/$(TARGET)
