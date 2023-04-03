#pragma once
#include "Matrix.hpp"

namespace normalizers
{
  class MinMax
  {
  public:
    MinMax() = delete;
    template <typename T>
    static auto eval(const data_types::Matrix<T> &data)
    {
      data_types::Matrix<T> result{data.n_rows(), data.n_cols()};
      const T min{data.min_value()};
      const T delta{data.max_value() - min};

      for (size_t i = 0u; i < result.size(); ++i)
      {
        result.at(i) = (data.at(i) - min) / delta;
      }

      return std::move(result);
    }
  };

  class None
  {
  public:
    None() = delete;
    template <typename T>
    static auto eval(const data_types::Matrix<T> &data)
    {
      return std::move(data);
    }
  };
}