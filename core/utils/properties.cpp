#include "properties.hpp"

//#include "argument_parser.hpp"

#include <boost/property_tree/detail/ptree_utils.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <stdexcept>

BEGIN_NAMESPACE_CORE

properties::properties() : m_tree_ptr(new ptree_type) {}

properties::~properties() {}

void properties::add_properties(const ptree_type &tree)
{
  merge_trees(*m_tree_ptr, tree);
}

void properties::add_properties_info(const std::string filepath)
{
  boost::property_tree::read_info(filepath, *m_tree_ptr);
}

void properties::add_properites_ini(const std::string filepath)
{
  boost::property_tree::read_ini(filepath, *m_tree_ptr);
}
void properties::add_properties_json(std::string const &filepath)
{
  boost::property_tree::read_json(filepath, *m_tree_ptr);
}

void properties::add_properties_xml(std::string const &filepath)
{
  boost::property_tree::read_xml(filepath, *m_tree_ptr);
}

void properties::add_properties_auto(const std::string &filepath)
{
  std::string extension = filepath.substr(filepath.find_last_of('.') + 1);

  if(extension == "info" || extension == "simbad")
    add_properties_info(filepath);
  else if(extension == "ini")
    add_properites_ini(filepath);
  else if(extension == "json" || extension == "js")
    add_properties_json(filepath);
  else if(extension == "xml")
    add_properties_xml(filepath);
  else
    throw std::runtime_error("unrecognized extension \"" + extension + "\"");
}

void properties::add_properties_auto(const std::vector<std::string> &filepaths)
{
  for(std::string const &filepath : filepaths)
    add_properties_auto(filepath);
}

const properties::ptree_type &properties::get_property_tree() const
{
  return *m_tree_ptr;
}

static void merge_trees_recrsively(properties::ptree_type &target,
                                   const properties::ptree_type &source,
                                   std::string const &prefix)
{
  std::string const &data = source.data();

  target.put(prefix, data);

  for(auto const &value : source)
  {
    properties::ptree_type::key_type key = value.first;
    properties::ptree_type subtree = value.second;

    std::string new_prefix = prefix.empty() ? key : prefix + '.' + key;
    merge_trees_recrsively(target, subtree, new_prefix);
  }
}

void properties::merge_trees(properties::ptree_type &target,
                             const properties::ptree_type &source)
{
  merge_trees_recrsively(target, source, "");
}

END_NAMESPACE_CORE
