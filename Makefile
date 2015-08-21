make:
	g++ `pkg-config --cflags gtk+-3.0` -o exec *.cpp `pkg-config --libs gtk+-3.0` -std=c++11 -rdynamic;
	./exec