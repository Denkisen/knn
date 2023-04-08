#pragma once
#include <random>
#include <limits>
#include <type_traits>
#include <algorithm>

namespace test_helpers
{
  template <typename T>
  constexpr void cycleTypes()
  {
    T::template test<uint16_t>();
    T::template test<uint32_t>();
    T::template test<uint64_t>();
    T::template test<float>();
    T::template test<double>();
  }

  template<typename T>
  auto getDistribution()
  {
    if constexpr (std::is_integral_v<T>)
    {
      constexpr T min = (std::numeric_limits<T>::denorm_min() - (std::numeric_limits<T>::min() / 2));
      constexpr T max = std::numeric_limits<T>::max() / 2;
      return std::uniform_int_distribution<T>{min, max};
    }
    else if constexpr (std::is_floating_point_v<T>)
    {
      constexpr auto min = std::numeric_limits<T>::min();
      constexpr auto max = std::numeric_limits<T>::max();
      return std::uniform_real_distribution<T>{min, max};
    }
    else
    {
      throw;
    }
  }

  template <typename T>
  std::vector<T> randomVector(const size_t size)
  {
    std::mt19937 gen(999u);
    auto distr{test_helpers::getDistribution<T>()};
    auto rnd = [&]() { return distr(gen); };
    std::vector<T> result(size);
    std::generate(result.begin(), result.end(), rnd);
    return result;
  }

  template <typename T>
  std::vector<std::vector<T>> randomVector(const size_t rows, const size_t cols)
  {
    std::mt19937 gen(999u);
    auto distr{test_helpers::getDistribution<T>()};
    auto rnd = [&]() { return distr(gen); };
    std::vector<std::vector<T>> result(rows);
    for (size_t i = 0u; i < result.size(); ++i)
    {
      result.at(i).resize(cols);
      std::generate(result.at(i).begin(), result.at(i).end(), rnd);
    }
    return result;
  }
}