CFLAGS=-I/opt/local/include/ -I/usr/local/include
LDFLAGS=-I/opt/local/include/ -L/usr/local/lib/
LDLIBS=-lGLEW -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit

all: main.o
	clang main.c LoadShaders.c -g -o main $(CFLAGS) $(LDFLAGS) $(LDLIBS)

clean: rm -f *.o main
