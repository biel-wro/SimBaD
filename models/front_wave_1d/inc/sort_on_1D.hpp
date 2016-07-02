#ifndef SORT_ON_1D_HPP
#define SORT_ON_1D_HPP

#include "front_wave_1d_fwd.hpp"

BEGIN_NAMESPACE_FRONT_WAVE_1D

struct sort_on_1D
{
    template <class P, size_t DIM = 0>
    bool operator()(const P &lhs, const P &rhs) const
    {
        return lhs.template get_coordinate<DIM>() <
               rhs.template get_coordinate<DIM>();
    }
    template <class P, size_t DIM = 0>
    bool operator()(const P &p, float pos) const
    {
        return p.template get_coordinate<DIM>() < pos;
    }

    template <class P, size_t DIM = 0>
    bool operator()(const float pos, const P &p) const
    {
        return pos < p.template get_coordinate<DIM>();
    }
};
END_NAMESPACE_FRONT_WAVE_1D
#endif // SORT_ON_1D_HPP
