#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <stdlib.h>
#include <algorithm>

// function for printing logical size of a file
void print_log_size(const char* filename)
{
	// opening the file in write only mode(actually we could choose
	// read only or read/write mode)
        int fd = open(filename, O_WRONLY);
	// checking if we managed to open the file, if we failed, just
	// exiting the program
        if (fd < 0)
        {
                printf("failed to open %s to write\n", filename);
                exit(-1);
        }
	// moving the offset to the end of the file
        int lse = lseek(fd, 0, SEEK_END);
	// checking if we managed to move offset
        if (lse < 0)
        {
                printf("failed to move offset\n");
                exit(-1);
        }
	// printing the count of bytes, over wich offset has "traveled"
	printf("logical size of file %s is: ", filename);
        printf("%d\n", lse);
	// closing the file
	close(fd);
}

// function for printing physical size of a file
void print_phys_size(const char* filename)
{
	// variable in wich we will store the answer
        int overall_bytes = 0;
	// opening the file in read only mode
        int fd = open(filename, O_RDONLY);
	// checking if we have managed to open the file
        if (fd < 0)
        {
                printf("failed to open %s for reading\n", filename);
                exit(-1);
        }
        bool flag = false;
        while (1)
        {
		// reaching the first datapoint from our current position
		// thus counting the bytes of current hole
                int hole = lseek(fd, 0, SEEK_DATA);
                if (hole < 0)
                {
                        printf("failed to move offset\n");
                        exit(-1);
                }
                if (hole == 0 && flag)
                        break;
                flag = true;
		// reaching the start of a hole thus counting the bytes of
		// current data
                int data = lseek(fd, 0, SEEK_HOLE);
                if (data < 0)
                {
                        printf("failed to move offset\n");
                        exit(-1);
                }
		// if we count 0 bytes, we finish
                if (data == 0)
                        break;
		// adding newly counted bytes to the answer
                overall_bytes += data;
        }
	// printing the result
	printf("physical size of file %s is: ", filename);
        printf("%d\n", overall_bytes);
}

// this main function prints the physical and logical sizes of sorce file
// then copies the sorce file into destination file and then prints
// the physical and logical sizes of destination file
int main(int argc, const char** argv)
{
	// checking if we recieved appropriate number of arguments
	if (argc != 3)
	{
		printf("invalid arguments\n");
		exit(-1);
	}
	// printing logical and physical sizes of sorce file
	print_log_size(argv[1]);
	print_phys_size(argv[1]);
	// opening sorce file for reading
	int fd1 = open(argv[1], O_RDONLY);
	// checking if we managed to open
	if (fd1 < 0)
	{
		printf("failed to open %s to read\n", argv[1]);
		exit(-1);
	}
	// this variable stores how many bytes we have counted
	// since the beginning
	int curr_overall_bytes = 0;
	// opning destination file for writing, we empty it if it stores
	// data and if there is no such file, we create one
	int fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	// checking if we managed to open
	if (fd2 < 0)
	{
		printf("failed to open %s to write\n", argv[2]);
		exit(-1);
	}
	// buffer size
	long long buff_size = 1 << 20;
	// a lambda function for counting the minimum of two numbers
	auto minimum = [](long long a, int b) {
			 return (a < b)*a + (a >= b)*b; };
	bool flag = false;
	while (1)
	{
		// buffer for transporting the data
		char buff[buff_size];
		// reaching the data thus counting the bytes of a hole
		int hole1 = lseek(fd1, 0, SEEK_DATA);
		// checking if we managed to move offset
		if (hole1 < 0)
		{
			printf("failed to move offset\n");
			exit(-1);
		}
		if (hole1 == 0 && flag)
			break;
		// adding the number of hole bytes over wich we have
		// just passed
		curr_overall_bytes += hole1;
		// putting a hole of the same size in destination file
		int hole2 = lseek(fd2, hole1, SEEK_CUR);
		// checking if we managed to do it
		if (hole2 < 0)
		{
			printf("failed to move offset\n");
		}
		flag = true;
		// reaching the next hole thus counting the size of a data
		// over wich we have just passed
		int curr_data_bytes = lseek(fd1, 0, SEEK_HOLE);
		// checking if we managed to move
		if (curr_data_bytes < 0)
		{
			printf("failed to move the offset\n");
			exit(-1);
		}
		// if size of the data is 0, we finish the program
		if (curr_data_bytes == 0)
			break;
		// putting the offset back as we have moved it forward
		// in order to count the size of current data block
		int back = lseek(fd1, curr_overall_bytes, SEEK_SET);
		if (back < 0)
		{
			printf("failed to move the offset\n");
			exit(-1);
		}

		// reading current data block
		int r_bytes = 0;
		while (r_bytes < curr_data_bytes)
		{
		// we either read the hole block or a part of it with buffer
		// and repeat until the end of data block
		int r_count = read(fd1, buff, minimum(sizeof(buff), (curr_data_bytes - r_bytes)));
		if (r_count == 0)
		{
			break;
		}
		if (r_count < 0)
		{
			printf("error while reading\n");
			exit(-1);
		}
		r_bytes += r_count;
		// writing in destination file
		int w_count = write(fd2, buff, r_count);
		if (w_count < 0)
		{
			printf("error while writing\n");
			exit(-1);
		}
		}
		// adding read bytes
		curr_overall_bytes += curr_data_bytes;
	}
	// closing both files
	close(fd2);
	close(fd1);
	// printing logical and physical sizes of destination file
	print_log_size(argv[2]);
	print_phys_size(argv[2]);

	return 0;
}
