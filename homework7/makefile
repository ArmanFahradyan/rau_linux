all: main

main: main.o
	g++ main.o -pthread -o main

main.o: main.cpp
	g++ main.cpp -pthread -c

clean:
	rm -rf *.o main
