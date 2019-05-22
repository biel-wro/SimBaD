#ifndef PROPERTIES_HPP
#define PROPERTIES_HPP

#include "interface/interface_fwd.hpp"

#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

#include "interface/property_tree.hpp"
//#include <boost/property_tree/ptree_fwd.hpp>

BEGIN_NAMESPACE_CORE

class properties
{
public:
  using ptree_type = boost::property_tree::ptree;

  // using ptree_type = property_tree;
  properties();
  ~properties();

  void add_properties(const ptree_type &tree);

  void add_properties_info(std::string const &filepath);
  void add_properties_info(std::istream &is);
  void add_properites_ini(std::string const &filepath);
  void add_properites_ini(std::istream &is);
  void add_properties_json(std::string const &filepath);
  void add_properties_json(std::istream &input);
  void add_properties_xml(std::string const &filepath);
  void add_properties_xml(std::istream &is);

  void add_properties_auto(std::string const &filepath);
  void add_properties_auto(std::vector<std::string> const &filepaths);

  ptree_type const &get_property_tree() const;

  static void merge_trees(ptree_type &target, ptree_type const &source,
                          bool overwrite=false);

private:
  std::unique_ptr<ptree_type> m_tree_ptr;
};

END_NAMESPACE_CORE

#endif // PROPERTIES_HPP
