#ifndef SIMBAD_STREAM_AS_CONFIGURATION_HPP
#define SIMBAD_STREAM_AS_CONFIGURATION_HPP

#include "interface/attribute.hpp"
#include "interface/attribute_description.hpp"
#include "interface/configuration_view.hpp"

#include <memory>

BEGIN_NAMESPACE_CORE
class stream_as_configuration : public configuration_view
{
public:
  explicit stream_as_configuration(std::unique_ptr<stream_reader> reader_ptr);
  ~stream_as_configuration();

  std::size_t size() const override;
  attribute_description const &description() const override;
  void visit_records(record_visitor) const override;

private:
  attribute_description m_description;
  std::unordered_map<std::size_t, std::size_t> m_reverse_mapping;
  using record = std::vector<attribute>;
  using data_storage = std::vector<record>;
  data_storage m_data;
};
END_NAMESPACE_CORE
#endif // SIMBAD_STREAM_AS_CONFIGURATION_HPP
