#ifndef SIMBAD_PROCESSORS_CHRONICLES_BUILDER_HPP
#define SIMBAD_PROCESSORS_CHRONICLES_BUILDER_HPP

#include "processors/abstract_builder.hpp"

#include <boost/optional.hpp>

#include <memory>
#include <unordered_map>

BEGIN_NAMESPACE_CORE
class chronicles_builder : public abstract_builder
{
public:
  chronicles_builder(attribute_description const &event_description,
                     std::string const &key_name,
                     std::vector<std::string> const &nonkey_attribute_names);
  ~chronicles_builder() override;

  void read_configuration(configuration_view const &view) override;
  void emit_alive();

protected:
  std::size_t generate_next_id();

  void on_event_group_start(std::size_t num_subevents) override;
  void on_event_group_end() override;

  void on_create(attribute const &key, attribute_list const &event) override;
  void on_remove(attribute const &key, attribute_list const &event) override;
  void on_transform(attribute const &key, attribute_list const &event) override;

  struct chronicle_datum
  {
    std::size_t id;
    std::size_t parent_id;
    double birth_time;
  };

  virtual void emit_particle(tracker_record const &record,
                             chronicle_datum const &datum,
                             double death_time) = 0;

  chronicle_datum &get_chronicle_or_throw(attribute const &key);
  tracker_record &get_record_or_throw(attribute const &key);

  using chronicle_map = std::unordered_map<attribute, chronicle_datum>;

  struct interpreter_context
  {
    boost::optional<double> m_maybe_time;
    boost::optional<std::size_t> m_maybe_parent;
    std::vector<attribute> m_keys;
    void reset();
  };
  void fetch_time_if_not_set(interpreter_context &context,
                             attribute_list const &event);

private:
  interpreter_context m_interpreter_context;

  std::size_t m_event_time_idx;
  chronicle_map m_chronicle_data;
  std::size_t m_next_id;
};
END_NAMESPACE_CORE
#endif // SIMBAD_PROCESSORS_CHRONICLES_BUILDER_HPP
