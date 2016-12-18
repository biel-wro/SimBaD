#ifndef ATTRIBUTE_DESCRIPTOR_HPP
#define ATTRIBUTE_DESCRIPTOR_HPP
#include "core_fwd.hpp"
#include "utils/attribute_exceptions.hpp"

#include <boost/range/any_range.hpp>

#include <cstddef>
#include <string>
#include <vector>

BEGIN_NAMESPACE_CORE
enum class ATTRIBUTE_KIND : std::uint32_t
{
  INTRINSIC,   // attributes stored in particle, non-recomputable
  ACCUMULATED, // attributes stored in particle, recomputable
  OBSERVABLE,  // attributes not-stored in particle, with physical meaning
  INFO         // other attributes
};

class attribute_descriptor
{
public:
  explicit attribute_descriptor(
      std::size_t attr_id, std::string name,
      ATTRIBUTE_KIND kind = ATTRIBUTE_KIND::INTRINSIC);
  std::string const &attribute_name() const;
  void set_attribute_name(std::string const &attribute_name);
  std::size_t attribute_idx() const;
  void set_attribute_id(std::size_t attribute_idx);
  ATTRIBUTE_KIND attribute_kind() const;
  void set_attribute_kind(ATTRIBUTE_KIND attribute_kind);

private:
  std::size_t m_attribute_id;
  std::string m_attribute_name;
  ATTRIBUTE_KIND m_attribute_kind;
};

END_NAMESPACE_CORE
#endif // ATTRIBUTE_DESCRIPTOR_HPP
