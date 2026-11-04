/**
 * SearchSort
 */

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>


constexpr int LABEL_WIDTH = 25;


/* ================= SEARCH ================= */


// Linear search O(n)
template <typename It, typename Predicate>
It LinearSearchIf(It it_first, It it_last, Predicate predicate)
{
  while ((it_first != it_last) && (!predicate(*it_first)))
  {
    ++it_first;
  }

  return it_first;
}


template <typename It, typename T>
It LinearSearch(It it_first, It it_last, const T & x)
{
  return LinearSearchIf(it_first, it_last, [&](const auto & v) { return v == x; });
}


// RandomAccessIterator → O(log n)
// ForwardIterator      → O(n) (linear fallback)
template <typename It, typename Predicate>
It BinarySearchFirstFalse(It it_first, It it_last, Predicate predicate)
{
  using category = typename std::iterator_traits<It>::iterator_category;

  if constexpr (std::is_base_of_v<std::random_access_iterator_tag, category>)
  {
    // O(log N)
    while (it_first != it_last)
    {
      It it_mid = it_first + (it_last - it_first) / 2;

      if (predicate(*it_mid))
      {
        it_first = it_mid + 1;
      }
      else
      {
        it_last = it_mid;
      }
    }
  }
  else
  {
    // fallback O(N) linear scan for ForwardIterator (predicate must be monotonic)
    while ((it_first != it_last) && predicate(*it_first))
    {
      ++it_first;
    }
  }

  return it_first;
}


template <typename It, typename T>
It LowerBound(It it_first, It it_last, const T & x)
{
  return BinarySearchFirstFalse(it_first, it_last, [&](const auto & v) { return v < x; });
}


template <typename It, typename T>
It UpperBound(It it_first, It it_last, const T & x)
{
  return BinarySearchFirstFalse(it_first, it_last, [&](const auto & v) { return v <= x; });
}


// Binary search:
// Array must be sorted
// RandomAccessIterator → O(log n)
// ForwardIterator      → O(n) (linear fallback)
// Returns it_last if not found
template <typename It, typename T>
It BinarySearch(It it_first, It it_last, const T & x)
{
  It it = LowerBound(it_first, it_last, x);

  if ((it != it_last) && (*it == x))
  {
    // Ok
  }
  else
  {
    it = it_last;
  }

  return it;
}


/* ================= SORT ================= */


// Bubble Sort O(n^2)
template <typename It, typename Compare>
void BubbleSort(It it_first, It it_last, Compare comp)
{
  It it_end = it_last;

  while (it_end != it_first)
  {
    It it_curr = it_first;
    It it_next = std::next(it_curr);
    bool swapped = false;

    while (it_next != it_end)
    {
      if (comp(*it_next, *it_curr))
      {
        std::iter_swap(it_curr, it_next);
        swapped = true;
      }

      ++it_curr;
      ++it_next;
    }

    if (swapped == false)
    {
      break;
    }

    it_end = it_curr;
  }
}


template <typename It>
void BubbleSort(It it_first, It it_last)
{
  BubbleSort(it_first, it_last, [](const auto & a, const auto & b) { return a < b; } );
}


// Insertion Sort O(n^2)
template <typename It, typename Compare>
void InsertionSort(It it_first, It it_last, Compare comp)
{
  for (It it_curr = it_first; it_curr != it_last; ++it_curr)
  {
    auto Value = *it_curr;
    It it_insert = it_curr;

    while ((it_insert != it_first) && comp(Value, *std::prev(it_insert)))
    {
      *it_insert = *std::prev(it_insert);
      --it_insert;
    }

    *it_insert = Value;
  }
}


template <typename It>
void InsertionSort(It it_first, It it_last)
{
  InsertionSort(it_first, it_last, [](const auto & a, const auto & b) { return a < b; } );
}


// Selection Sort O(n^2)
template <typename It, typename Compare>
void SelectionSortIf(It it_first, It it_last, Compare compare)
{
  for (It it_curr = it_first;it_curr != it_last; ++it_curr)
  {
    It it_min = it_curr;
    It it_scan = std::next(it_curr);

    while (it_scan != it_last)
    {
      if (compare(*it_scan, *it_min))
      {
        it_min = it_scan;
      }

      ++it_scan;
    }

    std::iter_swap(it_curr, it_min);
  }
}

// Selection Sort O(n^2)
template <typename It>
void SelectionSort(It it_first, It it_last)
{
  SelectionSortIf(it_first, it_last, [&](const auto & a, const auto & b) { return a < b; });
}


// Quick sort O(n * log(n))
template <typename It, typename Compare>
void QuickSort(It it_first, It it_last, Compare comp)
{
  static_assert(
    std::is_base_of<std::random_access_iterator_tag, typename std::iterator_traits<It>::iterator_category>::value,
    "QuickSort requires RandomAccessIterator"
  );

  if (it_first == it_last || it_first + 1 == it_last)
  {
    // Base Case (0 or 1 element)
  }
  else
  {
    It it_mid = it_first + (it_last - it_first) / 2;
    It it_last_elem = std::prev(it_last);

    // median of three
    if (comp(*it_mid, *it_first))
    {
      std::iter_swap(it_mid, it_first);
    }

    if (comp(*it_last_elem, *it_first))
    {
      std::iter_swap(it_last_elem, it_first);
    }

    if (comp(*it_last_elem, *it_mid))
    {
      std::iter_swap(it_last_elem, it_mid);
    }

    auto pivot = *it_mid;

    It it_pivot = std::prev(it_last);
    std::iter_swap(it_mid, it_pivot);

    // partition
    It it_i = it_first;
    It it_j = it_first;

    while (it_j != it_pivot)
    {
      if (comp(*it_j, pivot))
      {
        std::iter_swap(it_i, it_j);
        ++it_i;
      }

      ++it_j;
    }

    std::iter_swap(it_i, it_pivot);

    // recursion
    QuickSort(it_first, it_i, comp);
    QuickSort(std::next(it_i), it_last, comp);
  }
}



template <typename It>
void QuickSort(It it_first, It it_last)
{
  QuickSort(it_first, it_last, [](const auto & a, const auto & b) { return a < b; } );
}


/* ================ HELPER ================ */

void PrintLine(const std::string& label, const std::vector<int>& a)
{
  std::cout << std::left << std::setw(LABEL_WIDTH) << label;

  for (int x : a)
  {
    std::cout << x << " ";
  }

  std::cout << std::endl;
}


/* ================= MAIN ================= */

int main(int argc, const char* argv[])
{
  std::vector<int> data = {7, 3, 9, 1, 5};

  PrintLine("Original:", data);

  {
    auto a = data;
    BubbleSort(a.begin(), a.end());
    PrintLine("Bubble sort:", a);
  }

  {
    auto a = data;
    InsertionSort(a.begin(), a.end());
    PrintLine("Insertion sort:", a);
  }

  {
    auto a = data;
    SelectionSort(a.begin(), a.end());
    PrintLine("Selection sort:", a);
  }

  {
    auto a = data;
    QuickSort(a.begin(), a.end());
    PrintLine("Quick sort:", a);
  }

  {
    auto a = data;
    std::sort(a.begin(), a.end());
    PrintLine("std::sort (n log n):", a);

    // Exact binary search via lower_bound
    auto itbs = BinarySearch(a.begin(), a.end(), 5);
    std::cout << std::left << std::setw(LABEL_WIDTH)
              << "Binary search 5 -> "
              << (itbs != a.end() ? std::to_string(*itbs) : "end")
              << std::endl;

    // lower_bound
    auto itlb = LowerBound(a.begin(), a.end(), 4);
    std::cout << std::left << std::setw(LABEL_WIDTH)
              << "First >= 4 -> "
              << (itlb != a.end() ? std::to_string(*itlb) : "end")
              << std::endl;

    // upper_bound
    auto itub = UpperBound(a.begin(), a.end(), 4);
    std::cout << std::left << std::setw(LABEL_WIDTH)
              << "First > 4 -> "
              << (itub != a.end() ? std::to_string(*itub) : "end")
              << std::endl;
  }

  {
    std::unordered_set<int> s(data.begin(), data.end());
    std::cout << std::left << std::setw(LABEL_WIDTH)
              << "Hash search 5 -> "
              << (s.count(5) ? "found" : "not found")
              << std::endl;
  }

  return 0;
}
