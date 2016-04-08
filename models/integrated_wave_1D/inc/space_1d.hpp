#ifndef SPACE_1D_HPP
#define SPACE_1D_HPP

#include <boost/intrusive/set.hpp>

#include "particle_1d.hpp"

BEGIN_NAMESPACE_INTEGRATED_WAVE_1D
class space_1d : public boost::intrusive::set<particle_1d>{
    using boost::intrusive::set<particle_1d>::set;
};

END_NAMESPACE_INTEGRATED_WAVE_1D

#endif // SPACE_1D_HPP
