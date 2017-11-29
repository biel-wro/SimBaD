#ifndef FIXEDPRECISION_H
#define FIXEDPRECISION_H

#include <boost/operators.hpp>

#include "conversion.hpp"
#include "int_base.hpp"

namespace simbad
{
namespace core
{
namespace fixed
{
template <int Bits, int Offset>
class Fixed : boost::ordered_field_operators1<Fixed<Bits, Offset>>
{
  struct enabler
  {
  };

public:
  // types & definitions
  static constexpr const int bits = Bits;
  static constexpr const int offset = Offset;

  typedef typename int_base<bits, true>::type base;
  typedef typename int_base<2 * bits, true>::type dbase;

  // constructor and assignment operators
  Fixed() : m_base(0) {}
  template <class numeric>
  Fixed(numeric num) : m_base(num_to_int().operator()<offset, base>(num))
  {
  }

  Fixed(Fixed const &) = default;
  Fixed(Fixed &&) = default;

  Fixed &operator=(Fixed const &) = default;
  Fixed &operator=(Fixed &&) = default;

  // comparison operators
  bool operator<(Fixed const &other) const { return m_base < other.m_base; }
  bool operator==(Fixed const &other) const { return m_base == other.m_base; }

  // arithmetic operators
  Fixed operator+=(Fixed const &other) { return m_base += other.m_base; }
  Fixed operator+() { return *this; }
  Fixed operator-=(Fixed const &other) { return m_base -= other.m_base; }
  Fixed operator-() { return Fixed() - *this; }
  Fixed operator*=(Fixed const &other)
  {
    dbase r = static_cast<dbase>(m_base) * static_cast<dbase>(other.m_base);
    return m_base = static_cast<base>(offset_unshift<offset>(r));
  }
  Fixed operator/=(Fixed const &other)
  {
    dbase divident = offset_shift<offset>(static_cast<dbase>(m_base));
    dbase divisor = static_cast<dbase>(other.m_base);
    return m_base = static_cast<base>(divident / divisor);
  }

  // conversion to other types
  template <class target_type> target_type to() const
  {
    return int_to_num().operator()<offset, target_type>(m_base);
  }

protected:
  base m_base;
};
}
}
}
#endif
