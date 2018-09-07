#ifndef SIMBAD_STREAM_AS_MODEL_HPP
#define SIMBAD_STREAM_AS_MODEL_HPP

#include "interface/model.hpp"

#include "core_fwd.hpp"

#include <string>
#include <boost/optional.hpp>

BEGIN_NAMESPACE_CORE
class stream_as_model : public model
{
public:
  explicit stream_as_model(std::unique_ptr<stream_reader>,
                          std::string const &key_name,
                          std::vector<std::string> const &nonkey_observables );

  explicit stream_as_model(property_tree const &pt);
  ~stream_as_model();

  configuration_view const &current_configuration() const override;
  bool generate_events(event_visitor, std::size_t) override;
  attribute_description const &event_description() const override;
  void read_configuration(configuration_view const &) override;

private:
  std::unique_ptr<stream_reader> const m_stream_reader_ptr;
  std::unique_ptr<attribute_description> const m_event_description_ptr;
  std::unique_ptr<configuration_builder> const m_configuration_builder_ptr;
  boost::optional<std::size_t> const m_maybe_delta_time_idx;
};

END_NAMESPACE_CORE
#endif // SIMBAD_INPUT_AS_MODEL_HPP
