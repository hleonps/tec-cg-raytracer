OS := $(shell uname)
CC = gcc
CFLAGS = -std=gnu99 -Wall -I./headers

ifeq ($(OS),Darwin)
	LDLIBS = -framework GLUT -framework OpenGL -framework Cocoa
else
	LDLIBS = -lX11 -lglut -lGLU -lGL -lm
endif

C_SOURCES = $(wildcard ./src/*.c)
C_OBJECTS = ./*.o
EXECUTABLE = ./dist/raytracing

all: compile

compile: $(C_SOURCES)
		$(CC) $(CFLAGS) $(C_SOURCES) $(LDLIBS) -o $(EXECUTABLE)

clean:
	rm -rf $(EXECUTABLE)
