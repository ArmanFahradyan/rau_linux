#include <iostream>
#include <vector>
#include "Complex.h"
#include "MergeSort.h"

//Sorting vector of complex numbers

int main()
{
  size_t N;
  std::cout << "Enter a natural number N, that will be the size of the vector\n";
  std::cin >> N;
  std::cout << "Then enter " << N << " Complex numbers\n";
  std::vector<Complex> vec(N);
  for (auto& el : vec)
    std::cin >> el;
  MergeSort(vec.begin(), vec.end());
  std::cout << "The same vector but already sorted in ascending order will be\n";
  for (const auto& el : vec)
    std::cout << el << " ";
  std::cout << '\n';
  return 0;
}
