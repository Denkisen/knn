#include <gtest/gtest.h>
#include "helpers.hpp"
#include "test_helpers.hpp"
#include "Metrics.hpp"

namespace testing
{
  using namespace metrics;

  struct TestCosine
  {
    template <typename T>
    static void test()
    {
      std::vector<T> v1{(T) 1, (T) 2, (T) 3};
      std::vector<T> v2{(T) 2, (T) 3, (T) 4};
      EXPECT_NEAR(Cosine::eval(v1.data(), v2.data(), v1.size()), 0.00741667, 0.00000001);
    }
  };

  TEST(TestMetrics, testCosine)
  {
    test_helpers::cycleTypes<TestCosine>();
  }
}