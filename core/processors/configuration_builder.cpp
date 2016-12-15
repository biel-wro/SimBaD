#include "configuration_builder.hpp"
#include "interface/attribute_descriptor.hpp"
#include <limits>

BEGIN_NAMESPACE_CORE
configuration_builder::configuration_builder(bool store_coords,
                                             ID_POLICY id_policy,
                                             std::size_t nbuckets)
    : m_dimension(0),
      m_store_coords(store_coords),
      m_id_policy(id_policy),
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
void configuration_builder::visit_configuration(particle_visitor visitor) const
{
  std::size_t id_attr_idx = particle_id_attr_idx();
  std::size_t coord_attr_idx = particle_coord_attr_idx();
  for(generic_particle const &gp : m_configuration_buffer)
  {
    gp.visit_view(visitor, id_attr_idx, coord_attr_idx);
  }
}

configuration_builder::size_type
configuration_builder::configuration_size() const
{
  return m_configuration_buffer.size();
}

configuration_builder::size_type configuration_builder::dimension() const
{
  return m_dimension;
}

bool configuration_builder::has_unique_id() const
{
  return m_id_policy != ID_POLICY::DO_NOT_STORE;
}

const attribute_mapping &configuration_builder::attr_map() const
{
  return m_attribute_mapping;
}

void configuration_builder::read_configuration(const configuration_view &conf)
{
  if(ID_POLICY::COPY_IF_UNIQUE == m_id_policy && !conf.has_unique_id())
    m_id_policy = ID_POLICY::DO_NOT_STORE;

  m_dimension = conf.dimension();

  std::unordered_map<std::size_t, std::size_t> tgt2src;
  tgt2src = m_attribute_mapping.add_attributes(conf.attr_map(),extra_attr_idx());

  conf.visit_configuration([&](particle const &src) {
    std::unique_ptr<node_type> p_tgt(new node_type);
    store_id(*p_tgt, src);
    store_coordinates(*p_tgt, src);
    store_attributes(*p_tgt, src, tgt2src);

    add_particle(std::move(p_tgt));
  });
}

std::size_t configuration_builder::particle_id_attr_idx() const
{
  return ID_POLICY::DO_NOT_STORE != m_id_policy
             ? 0
             : std::numeric_limits<std::size_t>::max();
}

std::size_t configuration_builder::particle_coord_attr_idx() const
{
  return std::size_t(ID_POLICY::DO_NOT_STORE != m_id_policy);
}

std::size_t configuration_builder::extra_attr_idx() const
{
  return particle_coord_attr_idx() + 1;
}
void configuration_builder::store_id(generic_particle &tgt, const particle &src)
{
  std::unique_ptr<generic_particle> p_particle(new generic_particle);

  if(ID_POLICY::COPY_IF_UNIQUE == m_id_policy)
    tgt.set_attribute(0, std::int64_t(src.id()));
  else if(ID_POLICY::REASSIGN == m_id_policy)
    tgt.set_attribute(0, std::int64_t(generate_id()));
}
void configuration_builder::store_coordinates(generic_particle &tgt,
                                              particle const &s) const
{
  if(!m_store_coords)
    return;
  const std::size_t attr_id = particle_coord_attr_idx();
  if(1 == m_dimension)
    tgt.set_attribute(attr_id, s.coord(0));
  else if(2 == m_dimension)
    tgt.set_attribute(attr_id, attribute{s.coord(0), s.coord(1)});
  else if(3 == m_dimension)
    tgt.set_attribute(attr_id, attribute{s.coord(0), s.coord(1), s.coord(2)});
  else
  {
    attribute_array<double> pos(m_dimension);
    for(std::size_t d = 0; d < m_dimension; ++d)
      pos[d] = s.coord(d);
    tgt.set_attribute(attr_id, pos);
  }
}

void configuration_builder::store_attributes(
    generic_particle &tgt, const particle &src,
    std::unordered_map<std::size_t, std::size_t> tgt2src) const
{
  for(std::pair<std::size_t, std::size_t> const &t2s : tgt2src)
  {
    attribute const &attr = src.get_attribute(t2s.second);
    tgt.set_attribute(t2s.first, attr);
  }
}

std::size_t configuration_builder::generate_id() { return m_id_generator(); }
void configuration_builder::generate_events(event_visitor, size_t) {}
END_NAMESPACE_CORE
