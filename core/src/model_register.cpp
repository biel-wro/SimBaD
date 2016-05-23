//#define BOOST_RESULT_OF_USE_DECLTYPE

#include "model_register.hpp"
#include "model_factory.hpp"

BEGIN_NAMESPACE_CORE

model_register::name_list model_register::get_model_names() const
{
  return name_list(factories.begin(), factories.end());
}

bool model_register::register_factory(mapped_type f)
{
  std::string name = f->model_name();
  return factories.insert(std::make_pair(name, std::move(f))).second;
}

void model_register::unregister_factory(std::string const &name)
{
  factories.erase(name);
}

const model_factory &
model_register::get_model_factory(const std::string &name) const
{
  factory_iterator it = factories.find(name);
  if (factories.end() == it)
    throw std::range_error(std::string("no module named ") + name +
                           " was found");

  model_factory const &f = *(it->second);
  return f;
}

model_register::name_list::name_list(model_register::factory_iterator beg,
                                     model_register::factory_iterator end)
    : m_beg{beg}, m_end{end}
{
}

model_register::name_list::name_iterator
model_register::name_list::begin() const
{
  return m_beg;
}

model_register::name_list::name_iterator model_register::name_list::end() const
{
  return m_end;
}

END_NAMESPACE_CORE
