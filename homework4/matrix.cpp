#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <climits>
#include <pthread.h>
#include "matrix.h"

// getting number of rows and columns and creating our matrix
ParallelMatrix::ParallelMatrix(size_t row_size, size_t col_size)
{
  ParallelMatrix::matrix = std::vector<std::vector<int>>(row_size, std::vector<int>(col_size));
}

// function initializes every element of matrix with random integer
// function does NOT get any argument
void ParallelMatrix::Init()
{
  std::random_device MyRandomDevice;
  unsigned seed = MyRandomDevice();
  std::default_random_engine engine(seed);
  std::uniform_int_distribution<int> MyUnifIntDist(INT_MIN, INT_MAX);

  for (size_t i = 0; i < ParallelMatrix::matrix.size(); ++i)
  {
    for (size_t j = 0; j < ParallelMatrix::matrix[0].size(); ++j)
    {
      ParallelMatrix::matrix[i][j] = MyUnifIntDist(engine);
    }
  }
}

// function iterates over each element of the matrix and counts the sum
long long ParallelMatrix::Sum()
{
  //variable for storing the result
  long long result = 0;
  for (size_t i = 0; i < ParallelMatrix::matrix.size(); ++i)
  {
    for (size_t j = 0; j < ParallelMatrix::matrix[0].size(); ++j)
    {
      result += ParallelMatrix::matrix[i][j];
    }
  }
  return result;
}

void* plus(void* args)
{
  // this line casts the args from void* to std::pair<ParallelMatrix*, size_t>*
  // so as to get the matrix and the index, wich we will use to know wich
  // part of the matrix we are going to sum
  std::pair<ParallelMatrix*, size_t>* args_ptr = (std::pair<ParallelMatrix*, size_t>*) args;
  // separating the arguments
  ParallelMatrix* obj_ptr = args_ptr->first;
  size_t idx = args_ptr->second;
  // variable for storing the result
  long long sum = 0;
  // dynamic variable for returning the result
  long long * sum_ptr = new long long();
  // iterating over the part of the matrix wich corresponds to current thread
  // and counting the sum
  for (size_t k = idx*(obj_ptr->each_thread); k < (idx+1)*(obj_ptr->each_thread); ++k)
  {
    size_t row = k / obj_ptr->matrix[0].size();
    size_t col = k % obj_ptr->matrix[0].size();
    // checking if we have passed the size of the matrix
    if (row < obj_ptr->matrix.size() && col < obj_ptr->matrix[0].size())
    {
      sum += obj_ptr->matrix[row][col];
    }
    else
    {
      break;
    }
  }
  *sum_ptr = sum;
  return (void*)sum_ptr;
}

// function gets the number of threads as an argument
long long ParallelMatrix::ParallelSum(size_t thread_count)
{
  // variable for storing the result
  long long ans = 0;
  // creating a vector of threads
  std::vector<pthread_t> threads(thread_count);
  unsigned long long element_count = ParallelMatrix::matrix.size()*ParallelMatrix::matrix[0].size();
  // counting how many elements should add every thread
  ParallelMatrix::each_thread = element_count / thread_count;
  // iterating over threads
  for (size_t i = 0; i < thread_count; ++i)
  {
    // preparing the arguments
    std::pair<ParallelMatrix*, size_t> args = {this, 0};
    // creating the thread
    int thread_id = pthread_create(&threads[i], NULL, plus, &args);
    if (thread_id != 0)
    {
      std::cerr << "Error while creating thread" << std::endl;
      exit(thread_id);
    }
  }
  // variable for getting the return value pointer from each thread
  long long * res;
  // joining every thread and getting the return value and adding it
  for (size_t i = 0; i < thread_count; ++i)
  {
    int join_id = pthread_join(threads[i], (void**)&res);
    if (join_id != 0)
    {
      std::cerr << "Error while joining thread" << std::endl;
      exit(join_id);
    }
    ans += *res;
    // freeing dynamicly allocated memory
    delete res;
  }
  return ans;
}

// function iterates over every element and prints it
void ParallelMatrix::print() const
{
  for (size_t i = 0; i < ParallelMatrix::matrix.size(); ++i)
  {
    for (size_t j = 0; j < ParallelMatrix::matrix[0].size(); ++j)
    {
      std::cout << ParallelMatrix::matrix[i][j] << ' ';
    }
    std::cout << std::endl;
  }
}
