#ifndef SORT_ON_1D_HPP
#define SORT_ON_1D_HPP

#include <cstddef>

namespace simbad
{
namespace models
{

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
}
}
#endif // SORT_ON_1D_HPP
