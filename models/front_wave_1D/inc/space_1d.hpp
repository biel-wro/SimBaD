#ifndef SPACE_1D_HPP
#define SPACE_1D_HPP

#include "particle_1d.hpp"
#include <boost/intrusive/set.hpp>
BEGIN_NAMESPACE_FRONT_WAVE_1D
class space_1d : public boost::intrusive::set<particle_1D>
{
  using boost::intrusive::set<particle_1D>::set;
};
END_NAMESPACE_FRONT_WAVE_1D
#endif // SPACE_1D_HPP
