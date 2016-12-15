#ifndef AUTO_REHASH_HPP
#define AUTO_REHASH_HPP
#include "core_def.hpp"
#include <memory>

BEGIN_NAMESPACE_CORE
template <class LoadFactorType=float> struct auto_rehash
{
  explicit auto_rehash(LoadFactorType max_load_factor = 1)
      : m_max_load_factor(max_load_factor)
  {
  }
  template <class UnorderedSet>
  void operator()(
      UnorderedSet &set,
      std::unique_ptr<typename UnorderedSet::bucket_type[]> &bucket_ptr) const
  {
    using bucket_traits = typename UnorderedSet::bucket_traits;
    using bucket_type = typename UnorderedSet::bucket_type;
    using size_type = typename UnorderedSet::size_type;

    size_type sz = set.size();
    size_type nbuckets = set.bucket_count();

    LoadFactorType load_factor = LoadFactorType(sz) / LoadFactorType(nbuckets);

    if(load_factor <= m_max_load_factor)
      return;

    size_type new_size = 2 * sz;
    std::unique_ptr<bucket_type[]> new_buckets(new bucket_type[new_size]);
    set.rehash(bucket_traits(new_buckets.get(), new_size));
    bucket_ptr.swap(new_buckets);
  }
  LoadFactorType m_max_load_factor;
};

template <> struct auto_rehash<void>
{
  template <class OrderedBoard> void operator()(OrderedBoard &) const {}
};
END_NAMESPACE_CORE
#endif // AUTO_REHASH_HPP
