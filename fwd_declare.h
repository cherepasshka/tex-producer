#pragma once

#include <cstdint>
#include <vector>

template <class T>
class Fraction;

using Rational = Fraction<int64_t>;
using Column = std::vector<Rational>;