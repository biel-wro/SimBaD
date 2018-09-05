#include "attribute_descriptor.hpp"

#include <boost/bimap.hpp>

BEGIN_NAMESPACE_CORE

attribute_descriptor::attribute_descriptor(std::size_t attr_id,
                                           std::string name,
                                           ATTRIBUTE_KIND kind,
                                           ATTRIBUTE_SCALAR scalar,
                                           size_t attribute_dimension)
    : m_attribute_id(attr_id),
      m_attribute_name(std::move(name)),
      m_kind(kind),
      m_scalar(scalar),
      m_attribue_dimension(attribute_dimension)
{
}

const std::string &attribute_descriptor::attribute_name() const
{
  return m_attribute_name;
}

void attribute_descriptor::set_attribute_name(std::string attribute_name)
{
  m_attribute_name = std::move(attribute_name);
}

std::size_t attribute_descriptor::attribute_idx() const
{
  return m_attribute_id;
}

void attribute_descriptor::set_attribute_id(std::size_t attribute_id)
{
  m_attribute_id = attribute_id;
}

ATTRIBUTE_KIND attribute_descriptor::kind() const { return m_kind; }
void attribute_descriptor::set_kind(ATTRIBUTE_KIND kind) { m_kind = kind; }
std::size_t attribute_descriptor::attribute_dimension() const
{
  return m_attribue_dimension;
}

void attribute_descriptor::set_attribute_dimension(
    std::size_t attribute_dimension)
{
  m_attribue_dimension = attribute_dimension;
}

ATTRIBUTE_SCALAR attribute_descriptor::scalar() const { return m_scalar; }
void attribute_descriptor::set_scalar(ATTRIBUTE_SCALAR scalar)
{
  m_scalar = scalar;
}

using kind_to_default_name = boost::bimap<ATTRIBUTE_KIND, std::string>;

static std::unique_ptr<kind_to_default_name> make_mapping()
{
  std::vector<kind_to_default_name::value_type> pairs{
      {ATTRIBUTE_KIND::PARTICLE_UID, "particle.uid"},
      {ATTRIBUTE_KIND::PARTICLE_POSITION, "particle.position"},
      {ATTRIBUTE_KIND::EVENT_UID, "event.uid"},
      {ATTRIBUTE_KIND::EVENT_TIME, "event.time"},
      {ATTRIBUTE_KIND::EVENT_DELTA_TIME, "event.delta_time"},
      {ATTRIBUTE_KIND::EVENT_KIND, "event.kind"}};

  return std::unique_ptr<kind_to_default_name>(
      new kind_to_default_name(pairs.begin(), pairs.end()));
}

static kind_to_default_name const &get_kind_to_default_name_mapping()
{
  static std::unique_ptr<kind_to_default_name> ptr(make_mapping());
  return *ptr;
}

ATTRIBUTE_KIND attribute_descriptor::default_kind(std::string const &name)
{
  kind_to_default_name const &mapping = get_kind_to_default_name_mapping();
  kind_to_default_name ::right_const_iterator it = mapping.right.find(name);

  if(mapping.right.end() == it)
    return ATTRIBUTE_KIND ::UNKNOWN;
  return it->second;
}

std::string
attribute_descriptor::default_name(simbad::core::ATTRIBUTE_KIND kind)
{
  kind_to_default_name const &mapping = get_kind_to_default_name_mapping();
  kind_to_default_name::left_const_iterator it = mapping.left.find(kind);
  if(mapping.left.end() == it)
    return "";
  return it->second;
}

std::size_t
attribute_descriptor::default_dimension(simbad::core::ATTRIBUTE_KIND kind)
{
  switch(kind)
  {
  case ATTRIBUTE_KIND::PARTICLE_UID: return 1;
  case ATTRIBUTE_KIND::PARTICLE_POSITION: return 0;

  case ATTRIBUTE_KIND::EVENT_UID: return 1;
  case ATTRIBUTE_KIND::EVENT_TIME: return 1;
  case ATTRIBUTE_KIND::EVENT_DELTA_TIME: return 1;
  case ATTRIBUTE_KIND::EVENT_KIND: return 1;

  default: return 0;
  }
}

ATTRIBUTE_SCALAR
attribute_descriptor::default_scalar(simbad::core::ATTRIBUTE_KIND kind)
{
  switch(kind)
  {
  case ATTRIBUTE_KIND::PARTICLE_UID: return ATTRIBUTE_SCALAR::INT;
  case ATTRIBUTE_KIND::PARTICLE_POSITION: return ATTRIBUTE_SCALAR::UNKNOWN;
  case ATTRIBUTE_KIND::EVENT_UID: return ATTRIBUTE_SCALAR ::INT;
  case ATTRIBUTE_KIND::EVENT_TIME: return ATTRIBUTE_SCALAR ::UNKNOWN;
  case ATTRIBUTE_KIND::EVENT_KIND: return ATTRIBUTE_SCALAR ::INT;

  default: return ATTRIBUTE_SCALAR::UNKNOWN;
  }
}
bool attribute_descriptor::set_default_by_name(bool force)
{
  kind_to_default_name const &mapping = get_kind_to_default_name_mapping();

  kind_to_default_name::right_const_iterator it =
      mapping.right.find(m_attribute_name);
  if(mapping.right.end() == it)
    return false;

  m_kind = it->second;
  set_default_by_kind(force);
  return true;
}

void attribute_descriptor::set_default_by_kind(bool force)
{
  if(force || 0 == m_attribue_dimension)
    m_attribue_dimension = default_dimension(m_kind);
  if(force || ATTRIBUTE_SCALAR::UNKNOWN == m_scalar)
    m_scalar = default_scalar(m_kind);
}

std::ostream &operator<<(std::ostream &os, attribute_descriptor const &desc)
{
  os << "id=" << desc.attribute_idx()         //
     << " name=" << desc.attribute_name()     //
     << " dim=" << desc.attribute_dimension() //
     << " scalar=" << desc.scalar()           //
     << " kind=" << desc.kind();              //

  return os;
}

END_NAMESPACE_CORE
