OS := $(shell uname)
CC = gcc
CFLAGS = -std=gnu99 -Wall -I./headers

ifeq ($(OS),Darwin)
	LDLIBS =
else
	LDLIBS = -lm -pthread
endif

C_SOURCES = $(wildcard ./src/*.c)
C_OBJECTS = ./*.o
EXECUTABLE = ./dist/raytracing

all: create-dist compile

create-dist:
	mkdir -p ./dist

compile: $(C_SOURCES)
		$(CC) $(CFLAGS) $(C_SOURCES) $(LDLIBS) -o $(EXECUTABLE)

clean:
	rm -rf $(EXECUTABLE)
