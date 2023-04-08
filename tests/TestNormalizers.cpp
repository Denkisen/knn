#include <gtest/gtest.h>
#include "helpers.hpp"
#include "test_helpers.hpp"
#include "Normalizers.hpp"

namespace testing
{
  using namespace normalizers;
  using namespace data_types;

  struct TestMinMax
  {
    template <typename T>
    static void test()
    {
      const size_t rows{10u};
      const size_t cols{5u};
      Matrix<T> a{test_helpers::randomVector<T>(rows, cols)};
      Matrix<T> b{MinMax::eval(a)};
      for (size_t i = 0u; i < a.size(); ++i)
      {
        EXPECT_GE(b.at(i), (T) 0);
        EXPECT_LE(b.at(i), (T) 1);
      }
    }
  };

  TEST(TestNormalizers, minMax)
  {
    test_helpers::cycleTypes<TestMinMax>();
  }

  struct TestNone
  {
    template <typename T>
    static void test()
    {
      const size_t rows{10u};
      const size_t cols{5u};
      Matrix<T> a{test_helpers::randomVector<T>(rows, cols)};
      Matrix<T> b{None::eval(a)};
      for (size_t i = 0u; i < a.size(); ++i)
      {
        EXPECT_EQ(b.at(i), a.at(i));
      }
    }
  };

  TEST(TestNormalizers, none)
  {
    test_helpers::cycleTypes<TestNone>();
  }
}