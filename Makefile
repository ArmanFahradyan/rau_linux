all: main.o Complex.o
	g++ main.o Complex.o -o complex

main.o: main.cpp MergeSort.h
	g++ -c main.cpp

Complex.o: Complex.cpp Complex.h
	g++ -c Complex.cpp

clean:
	rm -rf *.o all
