#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <functional>
#include <cmath>
#include <chrono>
#include <map>
#include <string>
#include <fstream>
#include <iomanip>
#include "omp.h"

namespace helpers
{
  inline void print_omp_info()
  {
    std::cout << "Processors num: " << omp_get_num_procs() << std::endl;
    std::cout << "Max threads num: " << omp_get_max_threads() << std::endl;
    std::cout << "Offload devices num: " << omp_get_num_devices() << std::endl;
  }

  template <typename Period = std::chrono::microseconds, typename Func>
  size_t bench(const std::string label, const Func func)
  {
    auto unit = []() constexpr {
      if constexpr (std::is_same<Period, std::chrono::microseconds>::value)
      {
        return "us";
      }
      if constexpr (std::is_same<Period, std::chrono::milliseconds>::value)
      {
        return "ms";
      }
      if constexpr (std::is_same<Period, std::chrono::nanoseconds>::value)
      {
        return "ns";
      }
      if constexpr (std::is_same<Period, std::chrono::seconds>::value)
      {
        return "s";
      }

      return "unit";
    };
    const auto start1{std::chrono::high_resolution_clock::now()};
    func();
    const auto end1{std::chrono::high_resolution_clock::now()};
    const auto result = std::chrono::duration_cast<Period>(end1 - start1).count();
    std::cout << label << ": " << result << " " << unit() << std::endl;
    return result;
  }
}
