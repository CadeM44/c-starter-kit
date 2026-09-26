# System raylib build. If pkg-config cannot find raylib, use:
#   cmake -S . -B build && cmake --build build
# which will fetch raylib 5.5.

CC      ?= gcc
CFLAGS  ?= -std=c99 -O2 -Wall -Wextra
RAYLIB_CFLAGS := $(shell pkg-config --cflags raylib 2>/dev/null)
RAYLIB_LIBS   := $(shell pkg-config --libs raylib 2>/dev/null)
ifeq ($(RAYLIB_LIBS),)
RAYLIB_LIBS := -lraylib -lm -ldl -lpthread
endif

.PHONY: all clean cmake clay.h

all: cstarter-gui

third_party/clay.h:
	curl -fsSL -o $@ https://raw.githubusercontent.com/nicbarker/clay/main/clay.h

cstarter-gui: main.c app.c layout.c theme.h app.h layout.h third_party/clay.h third_party/clay_renderer_raylib.c
	$(CC) $(CFLAGS) -I . -I third_party $(RAYLIB_CFLAGS) -o $@ main.c app.c layout.c $(RAYLIB_LIBS)

cmake:
	cmake -S . -B build
	cmake --build build

clean:
	rm -f cstarter-gui
	rm -rf build
