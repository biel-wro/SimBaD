#ifndef SPATIAL_NEIGHBOURHOOD_HPP
#define SPATIAL_NEIGHBOURHOOD_HPP

#include "space_1d.hpp"

BEGIN_NAMESPACE_FRONT_WAVE_1D

class spatial_neighbourhood
{
  public:
    using iterator = space_1d::iterator;
    using const_iterator = space_1d::const_iterator;

    spatial_neighbourhood(iterator beg, iterator end);
    spatial_neighbourhood(space_1d &space, float center, float range);

    iterator begin() { return m_beg; }
    iterator end() { return m_end; }
    const_iterator begin() const { return m_beg; }
    const_iterator end() const { return m_end; }
  protected:
    iterator m_beg;
    iterator m_end;
};

END_NAMESPACE_FRONT_WAVE_1D
#endif // SPATIAL_NEIGHBOURHOOD_HPP
