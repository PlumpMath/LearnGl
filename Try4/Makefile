CFLAGS=-I/opt/local/include/
LDFLAGS=-L/usr/local/lib/ -I/opt/local/include/
LDLIBS=-lGLEW -lglfw -framework OpenGL -framework Cocoa

all: main.o; ./main
	clang main.c LoadShaders.c -o main $(CFLAGS) $(LDFLAGS) $(LDLIBS)

clean: rm -f *.o main
