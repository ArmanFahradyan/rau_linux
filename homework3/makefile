
all: my_shell

my_shell: my_shell.o
	g++ my_shell.o -o my_shell

my_shell.o: my_shell.cpp
	g++ -c my_shell.cpp

clean:
	rm -rf *.o my_shell
