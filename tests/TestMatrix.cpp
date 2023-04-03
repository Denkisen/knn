#include <gtest/gtest.h>
#include "helpers.hpp"
#include "Matrix.hpp"

using namespace data_types;

TEST (Matrix, constructors)
{
  Matrix<double> a;
  EXPECT_EQ(a.size(), 0u);
  EXPECT_EQ(a.n_cols(), 0u);
  EXPECT_EQ(a.n_rows(), 0u);

  Matrix<double> b{10u, 8u};
  EXPECT_EQ(b.size(), 10u * 8u);
  EXPECT_EQ(b.n_rows(), 10u);
  EXPECT_EQ(b.n_cols(), 8u);

  Matrix<double> c{b};
  EXPECT_EQ(c.size(), 10u * 8u);
  EXPECT_EQ(c.n_rows(), 10u);
  EXPECT_EQ(c.n_cols(), 8u);

  Matrix<double> d{std::move(c)};
  EXPECT_EQ(d.size(), 10u * 8u);
  EXPECT_EQ(d.n_rows(), 10u);
  EXPECT_EQ(d.n_cols(), 8u);

  std::vector<uint8_t> v1(10u, 2u);
  Matrix<uint8_t> e{5u, 2u, v1};
  EXPECT_EQ(e.size(), 5u * 2u);
  EXPECT_EQ(e.n_rows(), 5u);
  EXPECT_EQ(e.n_cols(), 2u);
  for (uint32_t i = 0u; i < 5u; ++i)
  {
    for (uint32_t j = 0u; j < 2u; ++j)
    {
      EXPECT_EQ(e.at(i, j), 2u);
    }
  }

  Matrix<uint8_t> f{5u, 2u, std::move(v1)};
  EXPECT_EQ(f.size(), 5u * 2u);
  EXPECT_EQ(f.n_rows(), 5u);
  EXPECT_EQ(f.n_cols(), 2u);
  for (uint32_t i = 0u; i < 5u; ++i)
  {
    for (uint32_t j = 0u; j < 2u; ++j)
    {
      EXPECT_EQ(f.at(i, j), 2u);
    }
  }

  std::vector<std::vector<uint8_t>> v2;
  v2.resize(10u, std::vector<uint8_t>(5u, 4u));

  Matrix<uint8_t> g{v2};
  EXPECT_EQ(g.size(), 10u * 5u);
  EXPECT_EQ(g.n_rows(), 10u);
  EXPECT_EQ(g.n_cols(), 5u);

  for (uint32_t i = 0u; i < 10u; ++i)
  {
    for (uint32_t j = 0u; j < 5u; ++j)
    {
      EXPECT_EQ(g.at(i, j), 4u);
    }
  }
}

TEST (Matrix, assignOperators)
{
  Matrix<double> a{10u, 8u};
  Matrix<double> b;
  Matrix<double> c;
  b = a;
  EXPECT_EQ(b.size(), 10u * 8u);
  EXPECT_EQ(b.n_rows(), 10u);
  EXPECT_EQ(b.n_cols(), 8u);

  c = std::move(a);
  EXPECT_EQ(c.size(), 10u * 8u);
  EXPECT_EQ(c.n_rows(), 10u);
  EXPECT_EQ(c.n_cols(), 8u);
}

TEST (Matrix, mathOperators)
{
  const auto rows = 1000u;
  const auto cols = 1000u;
  std::vector<std::vector<uint32_t>> v1;
  v1.resize(rows, std::vector<uint32_t>(cols, 0u));
  std::vector<std::vector<uint32_t>> v2;
  v2.resize(rows, std::vector<uint32_t>(cols, 0u));
  for (uint32_t i = 0u; i < rows; ++i)
  {
    for (uint32_t j = 0u; j < cols; ++j)
    {
      v1.at(i).at(j) = i * j;
      v2.at(i).at(j) = i * j;
    }
  }
  
  Matrix<uint32_t> a{rows, cols};
  Matrix<uint32_t> b{rows, cols};

  helpers::bench("Fill bench", [&]() {
    a.fill(v1);
  });

  b.fill(v2);

  helpers::bench("Sum bench", [&]() {
    a = a + b;
  });

  for (uint32_t i = 0u; i < rows; ++i)
  {
    for (uint32_t j = 0u; j < cols; ++j)
    {
      EXPECT_EQ(a.at(i,j), 2 * i * j);
    }
  }
}

TEST (Matrix, fill)
{
  const auto rows = 10u;
  const auto cols = 5u;
  std::vector<std::vector<uint8_t>> v;
  v.resize(rows, std::vector<uint8_t>(cols, 0u));
  for (uint32_t i = 0u; i < rows; ++i)
  {
    for (uint32_t j = 0u; j < cols; ++j)
    {
      v.at(i).at(j) = i * j;
    }
  }
  Matrix<uint8_t> a{rows, cols};
  EXPECT_EQ(a.size(), rows * cols);\

  a.fill(v);

  for (uint32_t i = 0u; i < rows; ++i)
  {
    for (uint32_t j = 0u; j < cols; ++j)
    {
      EXPECT_EQ(a.at(i, j), i * j);
    }
  }
}

TEST (Matrix, resize)
{
  const auto rows = 10u;
  const auto cols = 5u;
  Matrix<uint8_t> a{rows, cols};
  EXPECT_EQ(a.size(), rows * cols);
  a.resize(5u, 12u);
  EXPECT_EQ(a.size(), 5u * 12u);
  EXPECT_EQ(a.n_rows(), 5u);
  EXPECT_EQ(a.n_cols(), 12u);
}

TEST (Matrix, findMinMaxValue)
{
  const auto rows = 1000u;
  const auto cols = 1000u;
  std::vector<std::vector<uint32_t>> v1;
  v1.resize(rows, std::vector<uint32_t>(cols, 0u));
  uint32_t k{10u};
  for (uint32_t i = 0u; i < rows; ++i)
  {
    for (uint32_t j = 0u; j < cols; ++j)
    {
      v1.at(i).at(j) = k++;
    }
  }
  
  Matrix<uint32_t> a{rows, cols};
  a.fill(v1);
  uint32_t min_val{0u};
  helpers::bench("MinValue", [&]() {
    min_val = a.min_value();
  });
  EXPECT_EQ(min_val, 10u);

  uint32_t max_val{0u};
  helpers::bench("MaxValue", [&]() {
    max_val = a.max_value();
  });
  EXPECT_EQ(max_val, k - 1);
}