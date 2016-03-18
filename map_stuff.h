//////////////////////////////////////////////////////////////////////////////////////
// some (not yet so generic) helper functions and overloads for operating with maps //
//////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <map>

// // template<typename M, typename K, typename V>
// template<typename T, typename U>
// std::ostream& operator<<(std::ostream& os, const std::multimap<T,U>& map)
// // std::ostream& operator<<(std::ostream& os, const typename M<typename T, typename V>& map)
// {
//   os << "[";
//   for (const auto& KVpair : map) {
//     os << KVpair.first << ": " << KVpair.second << ", ";
//   }
//   os << "]";
//   return os;
// }

template<typename T, typename U>
std::ostream& operator<<(std::ostream& os, const std::multimap<T,U>& map)
{
  os << "{";
  for (const auto& P : map) {
    os << "(" << P.first << ": " << P.second.partnerInds << ", " << P.second.narrow << ")" << std::endl;
  }
  os << "}";
  return os;
}
