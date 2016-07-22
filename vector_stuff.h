/////////////////////////////////////////////////////////////
// some (rather generic) stuff to work easily with vectors //
/////////////////////////////////////////////////////////////

#include <vector>
#include <iostream>
#include <utility>
#include <algorithm>

#ifndef __VECTOR_HELPER_PERSONAL__
#define __VECTOR_HELPER_PERSONAL__

/** stream operator for std::vector */
template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
  if (v.empty()) {
    std::cout << "-empty-";
    return os;
  }
  os << "[";
#if __cplusplus <= 199711L
  typedef typename std::vector<T>::const_iterator vecIt;
  vecIt it = v.begin();
#else
  auto it = v.cbegin();
#endif
  for(; it != v.end() -1; ++it) { os << *it << ", "; }

  os << v.back();
  os << "]";
  return os;
}

/** stream operator for std::vector<std::pair<A,B> > */
template<class A, class B>
std::ostream& operator<<(std::ostream& os, const std::vector<std::pair<A,B> >& v)
{
  if (v.empty()) {
    std::cout << "-empty-";
    return os;
  }
  os << "[";
#if __cplusplus <= 199711L
  typedef typename std::vector<std::pair<A,B> > vecPaIt;
  vecPaIt itp = v.begin();
#else
  auto itp = v.cebgin();
#endif
  for(; itp != v.end() - 1; ++itp) {
    os << "(" << itp->first << ", " << itp->second << "), ";
  }
  os << "(" << v.back().first << ", " << v.back().second << ")";
  os << "]";
  return os;
}

#if __cplusplus > 199711L // almost any of the following will only work with c++11

/** calculuate the average value of all values stored in the vector */
template<class T>
double mean(const std::vector<T>& v)
{
  size_t nEntries = v.size();
  return (double) std::accumulate(v.begin(), v.end(), T{}) / (double) nEntries;
}

/** return only the unique values of the vector. NOTE: as a side product this will sort the vector in ascending order. */
template<class T>
void uniquify(std::vector<T>& v)
{
  std::sort(v.begin(), v.end());
  auto nEnd = std::unique(v.begin(), v.end());
  v.resize(std::distance(v.begin(), nEnd));
}

/** get the unique values of the vector in a new vector. */
template<class T>
std::vector<T> unique_entries(std::vector<T> v)
{
  std::sort(v.begin(), v.end());
  auto nEnd = std::unique(v.begin(), v.end());
  v.resize(std::distance(v.begin(), nEnd));
  return v;
}

/** check if any of the elements in the vector fulfill the condition stated in eqFun. */
template<class T, class F>
bool any_equal(const std::vector<T>& v, F eqFun) {
  for (const auto& e : v) {
    if (eqFun(e)) return true;
  }
  return false;
}

/** check if any of the entries in a vector of bools is true. */
bool any(const std::vector<bool>& v) {
  for (const bool b : v) {
    if (b) return true;
  }
  return false;
}
#endif // c++11 support needed

#endif
