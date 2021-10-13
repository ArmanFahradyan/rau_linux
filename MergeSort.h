#pragma once
#include <iostream>
#include "Complex.h"

  template <typename T>
  bool smaller(const T& oper1, const T& oper2) {return oper1 < oper2; }
  //default function for comparing operands, that returns true if the first operand is less
  //than the second one and false otherwise

  template <typename it_type, typename f_type>
  void Merge(it_type start, it_type mid, it_type end, f_type compare) // function Merge recieves
  {                                                                   // an interval, where objects
    typedef typename it_type::value_type type_name;                   // are sorted from start to
    auto dist = std::distance(start, end);                      // mid and from mid to end
    type_name* arr = new type_name[dist];                       // its goal is to sort all the 
    it_type it1 = start;                                // interval from start to end.
    it_type it2 = mid;                           // Sorting is done based on function compare
    size_t i=0;
    while (it1 != mid && it2 != end)
    {
      if (compare(*it1, *it2))
      {
        arr[i] = *it1;
        ++it1;
      }
      else
      {
        arr[i] = *it2;
        ++it2;
      }
      ++i;
    }
    while (it1 != mid)
    {
      arr[i] = *it1;
      ++it1;
      ++i;
    }
    while (it2 != end)
    {
      arr[i] = *it2;
      ++it2;
      ++i;
    }
    i = 0;
    it1 = start;
    while (it1 != end)
    {
      *it1 = arr[i];
      ++it1;
      ++i;
    }
    delete[] arr;
  }

  template <typename it_type, typename f_type = decltype (smaller<typename it_type::value_type>)>
  void MergeSort(it_type start, it_type end, f_type compare=smaller<typename it_type::value_type>)
  {
    auto dist = std::distance(start, end);
    if (dist <= 1)
      return;
    it_type mid = start;
    std::advance(mid, dist/2);
    MergeSort(start, mid, compare);
    MergeSort(mid, end, compare);
    Merge(start, mid, end, compare);
  }
  // MergeSort is a recursive function, that recieves an interval, divides it into two equal pieces
  // and calls itself on both parts, after wich merges two parts with function Merge
  // With argument compare you can choose appropriate function according wich will be done the Sorting
