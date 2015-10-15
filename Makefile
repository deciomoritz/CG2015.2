make run:
	./exec;

make:
	g++ `pkg-config --cflags gtk+-3.0` -o exec -g -O0 *.cpp `pkg-config --libs gtk+-3.0` -std=c++11 -rdynamic -w;

