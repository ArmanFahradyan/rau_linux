all: main

main: main.o matrix.o
	g++ -pthread main.o matrix.o -o main

main.o: main.cpp
	g++ -c main.cpp

matrix.o: matrix.cpp
	g++ -c matrix.cpp

clean:
	rm -rf *.o main
