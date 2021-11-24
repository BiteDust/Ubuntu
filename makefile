SOURCE = $(wildcard *.c)
TARGETS = $(patsubst %.c, %, $(SOURCE))


CC = gcc
CFLAGS += -g -o

all:$(TARGETS)

$(TARGETS):%:%.c
	$(CC) $^ $(CFLAGS) $@

.PHONY:clean
clean:
	rm -rf $(TARGETS)
