#include "spatial_neighbourhood.hpp"

#include "sort_on_1D.hpp"

BEGIN_NAMESPACE_FRONT_WAVE_1D
spatial_neighbourhood::spatial_neighbourhood(
    spatial_neighbourhood::iterator beg, spatial_neighbourhood::iterator end)
    : m_beg(beg), m_end(end)
{
}

spatial_neighbourhood::spatial_neighbourhood(space_1d &space, float center,
                                             float range)
    : m_beg(space.lower_bound(center - range, sort_on_1D())),
      m_end(space.upper_bound(center + range, sort_on_1D()))
{
}

END_NAMESPACE_FRONT_WAVE_1D
