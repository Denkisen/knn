#include <gtest/gtest.h>
#include "helpers.hpp"
#include "Matrix.hpp"
#include "test_helpers.hpp"

namespace testing
{
  using namespace data_types;

  template <typename T>
  void checkDims(const Matrix<T> &obj, const size_t rows, const size_t cols)
  {
    EXPECT_EQ(obj.size(), cols * rows);
    EXPECT_EQ(obj.n_cols(), cols);
    EXPECT_EQ(obj.n_rows(), rows);
  }

  template<typename T>
  void checkMatrixData(const Matrix<T> &obj, const std::vector<T> &data)
  {
    EXPECT_EQ(obj.size(), data.size());
    EXPECT_EQ(obj.n_cols() * obj.n_rows(), data.size());

    for (uint32_t i = 0u; i < obj.size(); ++i)
    {
      EXPECT_EQ(obj.at(i), data.at(i));
    }
  }

  template<typename T>
  void checkMatrixData(const Matrix<T> &obj, const std::vector<std::vector<T>> &data)
  {
    EXPECT_EQ(obj.size(), data.size() * data.at(0u).size());
    EXPECT_EQ(obj.n_cols() * obj.n_rows(), data.size() * data.at(0u).size());

    for (uint32_t i = 0u; i < obj.n_rows(); ++i)
    {
      for (uint32_t j = 0u; j < obj.n_cols(); ++j)
      {
        EXPECT_EQ(obj.at(i, j), data.at(i).at(j));
      }
    }
  }

  struct TestConstructors
  {
    template <typename T>
    static void test()
    {
      using type_name = T;
      const size_t rows{10u};
      const size_t cols{5u};
      Matrix<type_name> a;
      checkDims(a, 0u, 0u);

      Matrix<type_name> b{rows, cols};
      checkDims(b, rows, cols);

      Matrix<type_name> c{b};
      checkDims(c, rows, cols);

      Matrix<type_name> d{std::move(c)};
      checkDims(d, rows, cols);

      std::vector<type_name> v1{test_helpers::randomVector<type_name>(rows * cols)};
      Matrix<type_name> e{rows, cols, v1};
      checkDims(e, rows, cols);
      checkMatrixData(e, v1);

      std::vector<type_name> v1_copy{v1};
      Matrix<type_name> f{rows, cols, std::move(v1_copy)};
      checkDims(f, rows, cols);
      checkMatrixData(f, v1);

      std::vector<std::vector<type_name>> v2{test_helpers::randomVector<type_name>(rows, cols)};

      Matrix<type_name> g{v2};
      checkDims(g, rows, cols);
      checkMatrixData(g, v2);
    }
  };

  TEST(TestMatrix, constructors)
  {
    test_helpers::cycleTypes<TestConstructors>();
  }

  struct TestAssignOperators
  {
    template <typename T>
    static void test()
    {
      using type_name = T;
      const size_t rows{10u};
      const size_t cols{5u};
      std::vector<type_name> v1{test_helpers::randomVector<type_name>(rows * cols)};
      Matrix<type_name> a{rows, cols, v1};
      Matrix<type_name> b;
      Matrix<type_name> c;
      b = a;
      checkDims(b, rows, cols);
      checkMatrixData(b, v1);

      c = std::move(a);
      checkDims(c, rows, cols);
      checkMatrixData(c, v1);
    }
  };

  TEST(TestMatrix, assignOperators)
  {
    test_helpers::cycleTypes<TestAssignOperators>();
  }

  struct TestMathOperators
  {
    template <typename T>
    static void test()
    {
      using type_name = T;
      const auto rows{1000u};
      const auto cols{1000u};
      std::vector<std::vector<type_name>> v1{test_helpers::randomVector<type_name>(rows, cols)};
      std::vector<std::vector<type_name>> v2{test_helpers::randomVector<type_name>(rows, cols)};

      Matrix<type_name> a{v1};
      Matrix<type_name> b{v2};
      checkDims(a, rows, cols);
      checkDims(b, rows, cols);

      helpers::bench("Sum bench", [&]()
                     { a = a + b; });

      for (size_t i = 0u; i < rows; ++i)
      {
        for (size_t j = 0u; j < cols; ++j)
        {
          EXPECT_EQ(a.at(i, j), v1.at(i).at(j) + v2.at(i).at(j));
        }
      }
    }
  };

  TEST(TestMatrix, mathOperators)
  {
    test_helpers::cycleTypes<TestMathOperators>();
  }

  struct TestFill
  {
    template <typename T>
    static void test()
    {
      using type_name = T;
      const auto rows{10u};
      const auto cols{5u};
      std::vector<std::vector<type_name>> v1{test_helpers::randomVector<type_name>(rows, cols)};

      Matrix<type_name> a{v1};
      checkDims(a, rows, cols);
      a.fill(v1);
      checkMatrixData(a, v1);
    }
  };

  TEST(TestMatrix, fill)
  {
    test_helpers::cycleTypes<TestFill>();
  }

  struct TestResize
  {
    template <typename T>
    static void test()
    {
      using type_name = T;
      const auto rows{10u};
      const auto cols{5u};
      Matrix<type_name> a{rows, cols};
      EXPECT_EQ(a.size(), rows * cols);
      checkDims(a, rows, cols);
      a.resize(5u, 12u);
      checkDims(a, 5u, 12u);
    }
  };

  TEST(TestMatrix, resize)
  {
    test_helpers::cycleTypes<TestResize>();
  }

  struct TestMinMaxValues
  {
    template <typename T>
    static void test()
    {
      using type_name = T;
      const auto rows{1000u};
      const auto cols{1000u};
      std::vector<type_name> v1{test_helpers::randomVector<type_name>(rows * cols)};
      Matrix<type_name> a{rows, cols, v1};
      checkDims(a, rows, cols);

      type_name min_val{0u};
      helpers::bench("MinValue", [&]()
                     { min_val = a.min_value(); });
      EXPECT_EQ(min_val, *std::min_element(v1.begin(), v1.end()));

      type_name max_val{0u};
      helpers::bench("MaxValue", [&]()
                     { max_val = a.max_value(); });
      EXPECT_EQ(max_val, *std::max_element(v1.begin(), v1.end()));
    }
  };

  TEST(TestMatrix, findMinMaxValue)
  {
    test_helpers::cycleTypes<TestMinMaxValues>();
  }

  struct TestRawData
  {
    template <typename T>
    static void test()
    {
      using type_name = T;
      const auto rows{1000u};
      const auto cols{1000u};
      std::vector<type_name> v1{test_helpers::randomVector<type_name>(rows * cols)};
      Matrix<type_name> a{rows, cols, v1};
      checkDims(a, rows, cols);
      for (size_t i = 0u; i < v1.size(); ++i)
      {
        EXPECT_EQ(a.raw_data()[i], v1.at(i));
      }
    }
  };

  TEST(TestMatrix, rawData)
  {
    test_helpers::cycleTypes<TestRawData>();
  }
}