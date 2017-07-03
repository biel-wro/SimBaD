#ifndef FIXED_MATH_H
#define FIXED_MATH_H

#include "fixed.h"

namespace core
{
namespace fixed
{
template <int Bits, int Offset> Fixed<Bits, Offset> exp(Fixed<Bits, Offset> x)
{
  typedef Fixed<Bits, Offset> fixed;

  for(size_t b = Offset; b < Offset + Bits; b++)
  {
  }

  throw "not implemented yet";
}
}
}

#endif // FIXED_MATH_H
