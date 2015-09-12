#ifndef LAZY_SET_ITER_H
#define LAZY_SET_ITER_H

#include <boost/iterator/iterator_facade.hpp>
#include <type_traits>
#include <algorithm>

#include "lazy_set_chunk.h"

namespace simbad
{
namespace core
{

typedef size_t size_type;

typedef boost::intrusive::list_base_hook<> list_hook;
typedef boost::intrusive::avl_set_base_hook<
    boost::intrusive::optimize_size<true>> tree_hook;

struct ChunkContains
{
    bool operator()(LazySetNode const &node, D const &data) const
    {
        return static_cast<const void *>(&node) <
               static_cast<const void *>(&data - LazySetNode::max_occupancy());
    }
    bool operator()(D const &data, LazySetNode const &node) const
    {
        return static_cast<const void *>(&data + LazySetNode::length()) <
               static_cast<const void *>(&node);
    }
};

struct DifferentFromNodePred
{
    DifferentFromNodePred(LazySetNode &node) : pNode(&node) {}
    bool operator()(LazySetNode const &node) const { return &node != pNode; }
    LazySetNode *pNode;
};
}
}
#endif
