#include <boost/test/auto_unit_test.hpp>

#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>



namespace pt = boost::property_tree;

BOOST_AUTO_TEST_CASE(ptree_experiments)
{
  pt::ptree tree;

  tree.put("model.class","test model name");

  tree.put("model.parameters.alpha", 3.5 );
  tree.put("model.parameters.k", 10 );
  tree.put("model.time", 30 );

  tree.put("output.format","text");
  tree.put("output.file","test.txt");
  tree.put("output.compression",false);

  pt::ptree subtree = tree.get_child("model");

  pt::xml_writer_settings<char> xml_settings(' ',4);
  //pt::xml_parser::write_xml(std::cout, subtree, xml_settings);

  //std::cout<< subtree.get<double>("parameters.alpha") << std::endl;
}
