CC      ?= gcc
# GCC 14+ takes -std=c23; GCC 13 still wants -std=c2x
CSTD    ?= $(shell echo 'int main(void){return 0;}' | $(CC) -std=c23 -x c -c -o /dev/null - 2>/dev/null && echo c23 || echo c2x)
CFLAGS  ?= -std=$(CSTD) -Wall -Wextra -Wpedantic -O2
LDFLAGS ?=

TARGET  = PROJECTNAME
SRCS    = PROJECTNAME.c
OBJS    = $(SRCS:.c=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c PROJECTNAME.h
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
