#pragma once
#include <set>
#include <vector>
#include <tuple>
#include <map>

namespace data_types
{
  namespace
  {
    template <typename T>
    struct PairSecondLess
    {
      bool operator()(const T &a, const T &b) const
      {
        return a.second < b.second;
      }
    };
  }

  using ResultPair = std::pair<size_t, double>;
  using ResultSet = std::set<ResultPair, PairSecondLess<ResultPair>>;

  using ResultMapKey = std::pair<size_t, size_t>;
  using ResultMap = std::map<ResultMapKey, double>;
}