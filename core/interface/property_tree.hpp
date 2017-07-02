#ifndef CORE_PROPERTY_TREE_HPP
#define CORE_PROPERTY_TREE_HPP

#include "interface/interface_fwd.hpp"

#include <boost/property_tree/ptree.hpp>
#include <string>
#include <vector>
BEGIN_NAMESPACE_CORE
class property_tree : public boost::property_tree::ptree
{
public:
  using super = boost::property_tree::ptree;
  using super::super;
  static property_tree const &get_empty();
  property_tree();
  property_tree(super const &s);
  template <class Visitor>
  void visit(Visitor v, bool ignore_empty = true, std::string prefix = "") const
  {
    for(std::pair<std::string, property_tree> child : *this)
    {
      std::string const &name = prefix + child.first;
      property_tree const &st = child.second;
      boost::optional<std::string> opt = st.get_value_optional<std::string>();
      if(opt && (!ignore_empty || !opt.get().empty()))
        v(name, opt.get());
      st.visit(v, ignore_empty, name + ".");
    }
  }
  std::size_t total_size(bool ignore_empty = true) const;

  template <typename T>
  std::vector<T> get_vector(std::string const &key) const
  {
    std::vector<T> r;
    for(std::pair<std::string,super> const &item : get_child(key))
      r.push_back(item.second.get_value<T>());
    return r;
  }
  ~property_tree();

};
END_NAMESPACE_CORE

#endif
