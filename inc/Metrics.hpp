#pragma once

namespace metrics
{
class Cosine
{
public:
  Cosine() = default;
  template<typename PtrA, typename PtrB>
  static double eval(const PtrA* a, const PtrB* b, const uint32_t size)
  {
    double dot{.0};
    double lenA{.0};
    double lenB{.0};
    for (size_t i = 0u; i < size; ++i)
    {
      dot += a[i] * b[i];
      lenA += (a[i] * a[i]);
      lenB += (b[i] * b[i]);
    }

    assert(lenA > 0);
    assert(lenB > 0);
    lenA = sqrt(lenA);
    lenB = sqrt(lenB);
    const double sim = 1.0 - (dot / (lenA * lenB));

    return sim;
  }
};

}