#ifndef SPACE_1D_HPP
#define SPACE_1D_HPP

#include <boost/intrusive/set.hpp>
#include "particle_1d.hpp"
namespace simbad
{
namespace models
{
class space_1d : public boost::intrusive::set<particle_1D>{
    using boost::intrusive::set<particle_1D>::set;
};

}}
#endif // SPACE_1D_HPP
