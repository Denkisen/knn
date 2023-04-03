#pragma once
#include <tuple>
#include "Matrix.hpp"
#include "ResultTypes.hpp"
#include "omp.h"

namespace models
{
  template <typename valueType, typename Metric, typename Normalizer>
  class Knn
  {
  public:
    using elem_type = valueType;
    using metric = Metric;
    using normalizer = Normalizer;
    Knn() = default;
    Knn(const Knn &) = delete;
    Knn(Knn &&) = delete;
    ~Knn() = default;
    Knn &operator=(const Knn &) = delete;
    Knn &operator=(Knn &&) = delete;

    void train(const data_types::Matrix<elem_type> &data)
    {
      features = normalizer::eval(data);
    }

    data_types::ResultSet find(const size_t index, const size_t max_neighbors = 1, const float threshold = 0.5f)
    {
      data_types::ResultSet result{};
      for (size_t i = 0u; i < features.n_rows(); ++i)
      {
        if (i == index)
          continue;
        auto eval = std::make_pair(i, metric::eval(&features.at(i, 0u), &features.at(index, 0u), features.n_cols()));
        if (eval.second < threshold)
        {
          result.emplace(eval);
          if (result.size() > max_neighbors)
          {
            result.erase(std::prev(result.end()));
          }
        }
      }
      return result;
    }

    data_types::ResultMap find(const size_t max_neighbors = 1, const float threshold = 0.5f)
    {
      std::vector<data_types::ResultMap> results{static_cast<size_t>(omp_get_num_procs())};
      #pragma omp parallel for default(shared)
      for (size_t i = 0u; i < features.n_rows(); ++i)
      {
        auto res = find(i, max_neighbors, threshold);
        for (auto &item : res)
        {
          const auto first = std::min(i, item.first);
          const auto second = std::max(i, item.first);
          results.at(omp_get_thread_num()).emplace(data_types::ResultMapKey{first, second}, item.second);
        }
      }

      data_types::ResultMap result{};
      for (auto &item : results)
      {
        result.merge(item);
      }

      return result;
    }

  private:
    data_types::Matrix<elem_type> features;
  };
};
