#ifndef CONFIGURATION_BUILDER_HPP
#define CONFIGURATION_BUILDER_HPP
#include "core_fwd.hpp"

#include "interface/attribute_mapping.hpp"
#include "interface/configuration_reader.hpp"
#include "interface/configuration_view.hpp"
#include "interface/event_sink.hpp"
#include "interface/event_source.hpp"
#include "utils/auto_rehash.hpp"
#include "utils/counting_id_generator.hpp"
#include "utils/generic_particle.hpp"

#include <boost/intrusive/unordered_set.hpp>

#include <unordered_map>
BEGIN_NAMESPACE_CORE
class configuration_builder : public event_sink,
                              public event_source,
                              public configuration_view,
                              public configuration_reader
{
public:
  using hook_type = boost::intrusive::unordered_set_base_hook<>;
  struct node_type : public hook_type, public generic_particle
  {
  };
  using set_type = boost::intrusive::unordered_set<
      node_type, boost::intrusive::constant_time_size<true>,
      boost::intrusive::hash<std::hash<generic_particle>>>;
  using bucket_type = typename set_type::bucket_type;
  using bucket_traits = typename set_type::bucket_traits;
  enum class ID_POLICY : char
  {
    DO_NOT_STORE,
    COPY_IF_UNIQUE,
    REASSIGN
  };

  explicit configuration_builder(
      bool store_coords = true, ID_POLICY id_policy = ID_POLICY::COPY_IF_UNIQUE,
      std::size_t nbuckets = 1024);
  ~configuration_builder() override;

  void process_event(event const &e) override;

  void add_particle(std::unique_ptr<node_type> p);

  size_type configuration_size() const override;
  size_type dimension() const override;
  bool has_unique_id() const override;
  attribute_mapping const &attr_map() const override;
  void visit_configuration(particle_visitor visitor) const override;
  void generate_events(event_visitor, size_t) override;

  // attribute_range attribute_descriptors() const override;

protected:
  void read_configuration(const configuration_view &conf) override;

  std::size_t particle_id_attr_idx() const;
  std::size_t particle_coord_attr_idx() const;
  std::size_t extra_attr_idx() const;
  void store_id(generic_particle &tgt, particle const &src);
  void store_coordinates(generic_particle &tgt, const particle &src) const;
  void
  store_attributes(generic_particle &tgt, const particle &src,
                   std::unordered_map<std::size_t, std::size_t> tgt2src) const;
  std::size_t generate_id();

private:
  std::size_t m_dimension;
  bool m_store_coords;
  ID_POLICY m_id_policy;
  // std::vector<attribute_descriptor> m_attribute_descriptors;
  attribute_mapping m_attribute_mapping;
  std::unique_ptr<bucket_type[]> m_buckets;
  set_type m_configuration_buffer;
  auto_rehash<float> m_rehash_watchdog;
  counting_id_generator m_id_generator;
};
END_NAMESPACE_CORE
#endif // CONFIGURATION_BUILDER_HPP
