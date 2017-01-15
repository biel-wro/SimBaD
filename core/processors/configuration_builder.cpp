#include "configuration_builder.hpp"
#include "interface/attribute_list.hpp"
#include <limits>

BEGIN_NAMESPACE_CORE
configuration_builder::configuration_builder(bool store_coords,
                                             ID_POLICY id_policy,
                                             std::size_t nbuckets)
    : m_id_policy(id_policy),
      m_buckets(new bucket_type[nbuckets]),
      m_configuration_buffer(bucket_traits(m_buckets.get(), nbuckets)),
      m_rehash_watchdog(1.0)
{
}

configuration_builder::~configuration_builder()
{
  m_configuration_buffer.clear_and_dispose(std::default_delete<node_type>());
}
void configuration_builder::process_event(const event &e) {}
void configuration_builder::add_particle(std::unique_ptr<node_type> p)
{
  m_configuration_buffer.insert(*p);
  m_rehash_watchdog(m_configuration_buffer, m_buckets);
  p.release();
}
void configuration_builder::visit_configuration(
    particle_visitor visitor) const
{
  //  std::size_t id_attr_idx = particle_id_attr_idx();
  //  std::size_t coord_attr_idx = particle_coord_attr_idx();
  for(generic_particle const &gp : m_configuration_buffer)
  {
    gp.visit_view(visitor);
  }
}

configuration_builder::size_type
configuration_builder::configuration_size() const
{
  return m_configuration_buffer.size();
}
/*
configuration_builder::size_type configuration_builder::dimension() const
{
  return m_dimension;
}

bool configuration_builder::has_unique_id() const
{
  return m_id_policy != ID_POLICY::DO_NOT_STORE;
}
*/
const attribute_descriptor &configuration_builder::new_attr_map() const
{
  return m_attribute_descriptor;
}

void configuration_builder::read_configuration(const configuration_view &conf)
{
  if(ID_POLICY::COPY_IF_UNIQUE == m_id_policy && !conf.has_unique_id())
    m_id_policy = ID_POLICY::DO_NOT_STORE;

  // m_dimension = conf.dimension();

  std::unordered_map<std::size_t, std::size_t> tgt2src;
  tgt2src = m_attribute_descriptor.add_attributes(conf.new_attr_map(), 0);

  conf.visit_configuration([&](attribute_list const &src) {
    std::unique_ptr<node_type> p_tgt(new node_type);
    // store_id(*p_tgt, src);
    // store_coordinates(*p_tgt, src);
    store_attributes(*p_tgt, src, tgt2src);

    add_particle(std::move(p_tgt));
  });
}

void configuration_builder::store_attributes(
    generic_particle &tgt, const attribute_list &src,
    std::unordered_map<std::size_t, std::size_t> tgt2src) const
{
  for(std::pair<std::size_t, std::size_t> const &t2s : tgt2src)
  {
    attribute const &attr = src[t2s.second];
    tgt.set_attribute(t2s.first, attr);
  }
}

std::size_t configuration_builder::generate_id() { return m_id_generator(); }
void configuration_builder::generate_events(event_visitor, size_t) {}
END_NAMESPACE_CORE
