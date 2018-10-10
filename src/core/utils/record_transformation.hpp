#ifndef RECORD_TRANSFORMATION_HPP
#define RECORD_TRANSFORMATION_HPP

#include "core_def.hpp"

#include "interface/attribute_description.hpp"
#include "interface/attribute_list.hpp"

#include <functional>
#include <unordered_map>

BEGIN_NAMESPACE_CORE

class record_transformation : public attribute_list
{
public:
  record_transformation();

protected:
  // attribute get_attribute(std::size_t idx) const override;

  void add_forwarding(attribute_description const &input_description,
                      std::string const &name);
  // proposed:
  // void add_

private:
  using transform_function =
      std::function<attribute(attribute_list const &, attribute const &)>;
  std::unordered_map<std::size_t, transform_function> m_getter_mapping;
  attribute_description m_output_description;
};

END_NAMESPACE_CORE

#endif // RECORD_TRANSFORMATION_HPP
