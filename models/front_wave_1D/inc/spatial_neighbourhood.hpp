#ifndef SPATIAL_NEIGHBOURHOOD_HPP
#define SPATIAL_NEIGHBOURHOOD_HPP
#include "sort_on_1D.hpp"
#include "space_1d.hpp"
namespace simbad
{
namespace models
{
class spatial_neighbourhood
{
  public:
    using iterator = space_1d::iterator;
    using const_iterator = space_1d::const_iterator;

    spatial_neighbourhood(iterator beg, iterator end) : m_beg(beg), m_end(end)
    {
    }
    spatial_neighbourhood(space_1d &space, float center, float range)
        : m_beg(space.lower_bound(center - range, sort_on_1D())),
          m_end(space.upper_bound(center + range, sort_on_1D()))
    {
    }

    iterator begin() { return m_beg; }
    iterator end() { return m_end; }
    const_iterator begin() const { return m_beg; }
    const_iterator end() const { return m_end; }
  protected:
    iterator m_beg;
    iterator m_end;
};
}
}
#endif // SPATIAL_NEIGHBOURHOOD_HPP
