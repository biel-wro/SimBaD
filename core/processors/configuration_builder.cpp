#include "configuration_builder.hpp"

#include "interface/attribute.hpp"
#include "interface/attribute_descriptor.hpp"
#include "interface/attribute_list.hpp"
#include "interface/event_source.hpp"

#include <boost/intrusive/set.hpp>
#include <boost/intrusive/unordered_set.hpp>

#include <cstddef>
#include <limits>

BEGIN_NAMESPACE_CORE

namespace
{
struct map_node : public boost::intrusive::unordered_set_base_hook<
                      boost::intrusive::store_hash<true>>
{
  std::vector<attribute> attributes() { return m_attributes; }
  std::vector<attribute> const attributes() const { return m_attributes; }

private:
  std::vector<attribute> m_attributes;
};
}

struct configuration_builder::particle_set
{
  static constexpr std::size_t initial_bucket_size = 1 << 10;
  using hasher_type = std::function<bool(map_node const &)>;
  using comparator_type =
      std::function<std::size_t(map_node const &, map_node const &)>;
  using base_set = boost::intrusive::unordered_set< //
      map_node,                                     //
      boost::intrusive::equal<comparator_type>,     //
      boost::intrusive::hash<hasher_type>,          //
      boost::intrusive::compare_hash<true>          //
      >;

  using iterator = base_set::iterator;
  using const_iterator = base_set::const_iterator;

  particle_set(std::size_t keylen)
      : m_buckets_ptr(new base_set::bucket_type[initial_bucket_size]),
        m_bucket_traits(m_buckets_ptr.get(), initial_bucket_size),
        // hasher should just hash key attributes
        m_hasher([keylen](map_node const &node) {
          std::vector<attribute> const &attributes(node.attributes());
          return boost::hash_range(attributes.begin(),
                                   std::next(attributes.begin(), keylen));
        }),
        // comparator should just compare key attributes
        m_comparator([keylen](map_node const &lhs, map_node const &rhs) {
          std::vector<attribute>::const_iterator left(lhs.attributes().begin());
          std::vector<attribute>::const_iterator right(
              rhs.attributes().begin());

          return std::equal(left, std::next(left, keylen), //
                            right, std::next(right, keylen));
        }),
        m_set(m_bucket_traits, m_hasher, m_comparator)
  {
  }

  ~particle_set() { m_set.clear_and_dispose(std::default_delete<map_node>()); }



private:
  std::unique_ptr<base_set::bucket_type[]> m_buckets_ptr;
  base_set::bucket_traits m_bucket_traits;
  hasher_type m_hasher;
  comparator_type m_comparator;
  base_set m_set;
};

configuration_builder::configuration_builder(
    event_source &source, std::vector<std::string> const &key_names,
    std::vector<std::string> const &value_names)
    : m_source(source),
      m_key_size(key_names.size()),
      m_particle_set_ptr(new particle_set(m_key_size))
{
  // get all the names together in the right order
  std::vector<std::string> all_names(key_names);
  all_names.insert(all_names.end(), value_names.begin(), value_names.end());

  // set attribute mapping
  std::unordered_map<std::size_t, std::size_t> attribute_map =
      m_attribute_description.add_attributes(source.event_descriptor(),
                                             all_names);

  m_attribute_mapping.resize(attribute_map.size());
  for(std::pair<std::size_t, std::size_t> const &key_value : attribute_map)
    m_attribute_mapping[key_value.first] = key_value.second;
}

configuration_builder::~configuration_builder() {}

void configuration_builder::operator()(std::size_t size)
{
  auto visitor = [this](attribute_list const &event_attriubtes) {

  };
}

END_NAMESPACE_CORE
