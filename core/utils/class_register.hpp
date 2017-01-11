#ifndef CLASS_REGISTER_HPP
#define CLASS_REGISTER_HPP

#include "core_def.hpp"

#include "interface/property_tree.hpp"
#include "object_factory.hpp"
#include <boost/iterator/transform_iterator.hpp>

#include <memory>
#include <string>
#include <unordered_map>

BEGIN_NAMESPACE_CORE

template <class T> class class_register
{
public:
  // types
  using key_type = std::string;
  using factory_type = object_factory<T>;
  using mapped_type = std::unique_ptr<factory_type>;
  using mapped_pair_type = std::pair<const key_type, mapped_type>;

  using factory_map = std::unordered_map<key_type, mapped_type>;
  using factory_iterator = typename factory_map::const_iterator;

  struct classname_list
  {
    struct name_getter
    {
      key_type const &operator()(mapped_pair_type const &p) const
      {
        return p.first;
      }
    };
    using name_iterator =
        boost::transform_iterator<name_getter, factory_iterator>;

    classname_list(factory_iterator beg, factory_iterator end)
        : m_beg(beg), m_end(end)
    {
    }
    name_iterator begin() const { return m_beg; }
    name_iterator end() const { return m_end; }
  private:
    name_iterator m_beg, m_end;
  };

  // member functions
  classname_list get_model_names() const
  {
    return classname_list(m_factories.begin(), m_factories.end());
  }

  factory_type const &get_factory(std::string const &name) const
  {
    factory_iterator it = m_factories.find(name);
    if(m_factories.end() == it)
      throw std::range_error(std::string("no factory for class named ") + name +
                             " was found");
    factory_type const &f = *(it->second);
    return f;
  }
  std::unique_ptr<T> create_instance(property_tree const &pt,
                                     std::string const &path) const
  {
    return create_instance(pt.get_child(path));
  }
  std::unique_ptr<T> create_instance(property_tree const &pt) const
  {
    std::string const &class_name = pt.get<std::string>("class");
    property_tree const &parameters = pt.get_child("parameters");
    return create_instance(class_name, parameters);
  }
  std::unique_ptr<T> create_instance(std::string const &class_name,
                                     property_tree const &pt) const
  {
    factory_type const &f = get_factory(class_name);
    return f.create_instance(pt);
  }

  bool register_factory(mapped_type f)
  {
    std::string name = f->class_name();
    return m_factories.emplace(std::move(name), std::move(f)).second;
  }

  void unregister_factory(std::string const &name) { m_factories.erase(name); }
private:
  factory_map m_factories;
};
END_NAMESPACE_CORE

#endif // CLASS_REGISTER_HPP
