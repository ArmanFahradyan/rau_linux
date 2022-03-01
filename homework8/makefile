all: test_blocking_queue

test_blocking_queue: main.o
	g++ main.o -pthread -o test_blocking_queue

main.o: main.cpp blocking_queue.h
	g++ -c main.cpp blocking_queue.h -pthread

clean:
	rm -rf *.o test_blocking_queue blocking_queue.h.gch
