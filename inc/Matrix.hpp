#pragma once
#include <vector>
#include <algorithm>
#include <assert.h>

namespace data_types
{
  template <typename valueType>
  class Matrix
  {
  public:
    using elem_type = valueType;
    Matrix() = default;
    Matrix(const Matrix &other) : data(other.data), cols(other.cols), rows(other.rows) {}
    Matrix(Matrix &&other) : data(std::move(other.data)), cols(other.cols), rows(other.rows) {}
    Matrix(const size_t rows, const size_t cols, const elem_type fill_value = (valueType)0) : cols(cols), rows(rows)
    {
      assert(cols > 0u);
      assert(rows > 0u);
      data.resize(cols * rows, fill_value);
    }
    Matrix(const std::vector<std::vector<elem_type>> &data) : rows(data.size()), cols(data.at(0).size())
    {
      assert(cols > 0u);
      assert(rows > 0u);
      this->data.resize(cols * rows, (valueType)0);
      fill(data);
    }
    Matrix(const size_t rows, const size_t cols, const std::vector<elem_type> &data) : rows(rows), cols(cols), data(data)
    {
      assert(cols > 0u);
      assert(rows > 0u);
      assert(data.size() == rows * cols);
    }
    Matrix(const size_t rows, const size_t cols, std::vector<elem_type> &&data) : rows(rows), cols(cols), data(std::move(data))
    {
      assert(cols > 0u);
      assert(rows > 0u);
      assert(this->data.size() == rows * cols);
    }
    ~Matrix() = default;
    Matrix &operator=(const Matrix &other)
    {
      cols = other.cols;
      rows = other.rows;
      data = other.data;
      return *this;
    }
    Matrix &operator=(Matrix &&other)
    {
      cols = other.cols;
      rows = other.rows;
      data = std::move(other.data);
      return *this;
    }
    Matrix &operator+(const Matrix &other)
    {
      assert(cols == other.cols);
      assert(rows == other.rows);
      assert(size() == other.size());

      std::transform(other.data.begin(), other.data.end(), data.begin(), data.begin(), std::plus<elem_type>());
      return *this;
    }

    size_t size() const { return data.size(); }
    void resize(uint32_t rows, uint32_t cols, const elem_type fill_value = (valueType)0)
    {
      this->cols = cols;
      this->rows = rows;
      data.resize(cols * rows, fill_value);
    }
    void fill(const std::vector<std::vector<elem_type>> &input_data)
    {
      assert(input_data.size() == rows);
      for (uint32_t row = 0u; row < input_data.size(); ++row)
      {
        assert(input_data.at(row).size() == cols);
        const uint32_t i = (cols * row);
        std::copy(input_data.at(row).begin(), input_data.at(row).end(), &data.at(i));
      }
    }

    void fill(const elem_type fill_value)
    {
      std::fill(data.begin(), data.end(), fill_value);
    }

    size_t n_cols() const { return cols; }
    size_t n_rows() const { return rows; }

    elem_type &at(uint32_t row, uint32_t col)
    {
      const uint32_t i = (cols * row) + col;
      return data.at(i);
    }
    const elem_type &at(uint32_t row, uint32_t col) const
    {
      const uint32_t i = (cols * row) + col;
      return data.at(i);
    }
    elem_type &at(uint32_t index) { return data.at(index); }
    const elem_type &at(uint32_t index) const { return data.at(index); }

    elem_type min_value() const
    {
      return *std::min_element(data.begin(), data.end());
    }

    elem_type max_value() const
    {
      return *std::max_element(data.begin(), data.end());
    }

    auto raw_data() { return data.data(); }

  private:
    std::vector<elem_type> data;
    size_t cols{0u};
    size_t rows{0u};
  };
}

