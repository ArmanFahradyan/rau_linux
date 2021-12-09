#include <iostream>
#include "matrix.h"

int main()
{
  ParallelMatrix PM(2, 2);
  PM.Init();
  std::cout << PM.Sum() << std::endl;
  std::cout << PM.ParallelSum(2) << std::endl;
  PM.print();
  std::cout << std::endl;

  return 0;
}
