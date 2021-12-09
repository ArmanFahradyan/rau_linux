#pragma once
#include <vector>

// function for counting a partial sum of the matrix
void* plus(void* arg);

class ParallelMatrix
{
  // making function plus friend in order to give access to our class private members
  friend void* plus(void* arg);
private:
  // storing the number of elements, wich must add together each thread
  size_t each_thread;
  // vector of vectors to store our matrix
  std::vector<std::vector<int>> matrix;
public:
  //constructer
  ParallelMatrix(size_t, size_t);

  // function for initializing elements of our matrix randomly(uniformly distributed)
  void Init();
  // function for counting the sum of the elements of the matrix
  long long Sum();
  // function for for counting the sum of elements using threads
  long long ParallelSum(size_t);
  // function for printing our matrix
  void print() const;
};
