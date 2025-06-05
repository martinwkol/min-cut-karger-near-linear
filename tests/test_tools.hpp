#pragma once

#include <cassert>
#include <cmath>
#include <float.h>

// From https://stackoverflow.com/questions/4915462/how-should-i-do-floating-point-comparison
bool nearlyEqual(
  double a, double b,
  double epsilon = 128 * DBL_EPSILON, double abs_th = DBL_MIN)
  // those defaults are arbitrary and could be removed
{
    assert(std::numeric_limits<double>::epsilon() <= epsilon);
    assert(epsilon < 1.f);

    if (a == b) return true;

    auto diff = std::abs(a-b);
    auto norm = std::min((std::abs(a) + std::abs(b)), std::numeric_limits<double>::max());
    return diff < std::max(abs_th, epsilon * norm);
}