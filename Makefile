CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -g

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    LDFLAGS =
else ifeq ($(UNAME_S),Darwin)
    LDFLAGS =
else
    LDFLAGS = -lws2_32
endif

SRCS = src/main.c src/builtins.c src/parser.c src/executor.c src/jobs.c src/utils.c src/history.c
OBJS = $(SRCS:.c=.o)

TARGET = shell

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)
