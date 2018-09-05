#include "stream_as_configuration.hpp"

#include "interface/attribute_list.hpp"
#include "interface/stream_reader.hpp"

BEGIN_NAMESPACE_CORE

stream_as_configuration::stream_as_configuration(
    std::unique_ptr<stream_reader> reader_ptr)
    : m_description(reader_ptr->read_header()), m_reverse_mapping{}, m_data{}
{
  std::vector<std::size_t> mapping = m_description.unpack_indices();
  m_reverse_mapping = attribute_description::reverse_mapping(mapping);

  auto visitor = [this, &mapping](attribute_list const &input_record) {
    std::size_t record_size = mapping.size();

    record data_record(record_size);
    for(std::size_t idx = 0; idx < record_size; ++idx)
    {
      std::size_t source_idx = mapping[idx];
      data_record[idx] = input_record[source_idx];
    }
    m_data.push_back(std::move(data_record));
  };
  reader_ptr->visit_entries(visitor, 0);
}
stream_as_configuration::~stream_as_configuration() = default;

std::size_t stream_as_configuration::size() const { return m_data.size(); }
attribute_description const &stream_as_configuration::description() const
{
  return m_description;
}

namespace
{
struct attribute_list_view : public attribute_list
{
  attribute_list_view(
      std::unordered_map<std::size_t, std::size_t> const &reversed_mapping)
      : m_reversed_mapping(reversed_mapping)
  {
  }

  attribute get_attribute(std::size_t idx) const override
  {
    std::size_t vec_idx = m_reversed_mapping.at(idx);
    return attribute_ptr->at(vec_idx);
  }

  std::unordered_map<std::size_t, std::size_t> const &m_reversed_mapping;
  std::vector<attribute> const *attribute_ptr;
};
}

void stream_as_configuration::visit_records(record_visitor visitor) const
{
  attribute_list_view view{m_reverse_mapping};
  for(record const &data_record : m_data)
  {
    view.attribute_ptr = &data_record;
    visitor(view);
  }
}
END_NAMESPACE_CORE