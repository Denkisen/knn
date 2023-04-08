#include <gtest/gtest.h>
#include "helpers.hpp"
#include "test_helpers.hpp"
#include "Metrics.hpp"
#include "Knn.hpp"
#include "Matrix.hpp"
#include "Normalizers.hpp"

namespace testing
{
  using namespace metrics;
  using namespace models;
  using namespace data_types;
  using namespace normalizers;

  struct TestTrain
  {
    template <typename T>
    static void test()
    {
      const size_t rows{10u};
      const size_t cols{5u};
      Knn<T, Cosine, None> knn;
      std::vector<T> v1{test_helpers::randomVector<T>(rows * cols)};
      Matrix<T> e{rows, cols, v1};
      knn.train(e);
      auto &data = knn.getFeatures();
      for (size_t i = 0u; i < e.size(); ++i)
      {
        EXPECT_EQ(data.at(i), e.at(i));
      }
    }
  };

  TEST(TestKnn, train)
  {
    test_helpers::cycleTypes<TestTrain>();
  }

  struct TestFindOne
  {
    template <typename T>
    static void test()
    {
      const size_t rows{5u};
      const size_t cols{5u};
      Knn<T, Cosine, None> knn;
      std::vector<std::vector<T>> v1;
      v1.push_back({(T) 1, (T) 0, (T) 1, (T) 0, (T) 2});
      v1.push_back({(T) 0, (T) 0, (T) 0, (T) 0, (T) 2});
      v1.push_back({(T) 7, (T) 8, (T) 2, (T) 4, (T) 3});
      v1.push_back({(T) 1, (T) 0, (T) 1, (T) 0, (T) 2});
      v1.push_back({(T) 5, (T) 9, (T) 3, (T) 2, (T) 2});

      Matrix<T> e{v1};
      knn.train(e);
      ResultSet result1 = knn.find(0u, 1u, 0.5);
      EXPECT_EQ(result1.size(), 1u);
      EXPECT_EQ((*result1.begin()).first, 3u);

      ResultMap result2 = knn.find(2u, 0.18);
      EXPECT_EQ(result2.size(), 2u);
      EXPECT_EQ((*result2.begin()).first.first, 0u);
      EXPECT_EQ((*result2.begin()).first.second, 3u);
    }
  };

  TEST(TestKnn, testFindOne)
  {
    test_helpers::cycleTypes<TestFindOne>();
  }
}