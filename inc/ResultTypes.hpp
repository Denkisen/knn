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
    struct pair_less
    {
      bool operator()(const T &a, const T &b) const
      {
        return a.second < b.second;
      }
    };
  }

  using ResultPair = std::pair<size_t, double>;
  using ResultSet = std::set<ResultPair, pair_less<ResultPair>>;

  using ResultTuple = std::tuple<size_t, size_t, double>;
  using ResultVector = std::vector<ResultTuple>;

  using ResultMapKey = std::pair<size_t, size_t>;
  using ResultMap = std::map<ResultMapKey, double>;
}