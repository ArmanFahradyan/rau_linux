#include <iostream>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char** argv)
{
	// Checking that we do not get any argument
	if (argc != 1)
	{
		std::cerr << "Wrong arguments!\n";
		exit(-1);
	}
	// Variable in which we will store the sum of numbers
	int sum = 0;
	// Array of numbers
	int arr_size;
	// Getting size of array from the input
	std::cin >> arr_size;
	// Allocating space for array
	int* arr = new int[arr_size];
	// Getting each element of the array from input
	for (int i=0; i < arr_size; ++i)
	{
		std::cin >> arr[i];
	}
	// Creating a child process
	pid_t pid = fork();
	// Handling possible error
	if (pid < 0)
	{
		std::cerr << "Failed to create child process\n";
		exit(-1);
	}
	// Checking that we are in parent process
	else if (pid > 0)
	{
		// Iterating on the array and checking if current number is even or odd
		for (int i=0; i < arr_size; ++i)
		{
			if (arr[i] % 2 == 0)
			{
				// Adding only even numbers
				sum += arr[i];
			}
		}
		// Printing the sum of even numbers
		std::cout << "parent: " + std::to_string(sum) << '\n';

		// Waiting the child process
		int wait_state = wait(NULL);
		// Handling possible error
		if (wait_state < 0)
		{
			std::cerr << "Error while waiting a child process\n";
			exit(-1);
		}
	}
	// Checking that we are in child process
	else if (pid == 0)
	{
		// Iterating on the array and checking if current number is even or odd
		for (int i=0; i < arr_size; ++i)
		{
			if (arr[i] % 2 == 1)
			{
				// Adding only odd numbers
				sum += arr[i];
			}
		}
		// Printing the sum of odd numbers
		std::cout << "child: " + std::to_string(sum) << '\n';
	}



	return 0;
}
