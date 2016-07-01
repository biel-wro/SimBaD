#include "argument_parser.hpp"

#include <string>

#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>

BEGIN_NAMESPACE_CORE

argument_parser::argument_parser() : tree_ptr(new ptree_type) {}

argument_parser::argument_parser(int argc, const char **argv)
    : argument_parser() {
  parse(argc, argv);
}

argument_parser::~argument_parser() {}

void argument_parser::parse(int argc, const char **argv) {
  namespace po = boost::program_options;

  po::options_description desc;
  po::command_line_parser cl_parser(argc, argv);
  cl_parser.options(desc);
  cl_parser.allow_unregistered();
  po::parsed_options parsed_options = cl_parser.run();

  //tree_ptr -> put("positional_cmd_arg","");

  for (auto const &option : parsed_options.options) {
    std::string string_key = option.string_key;
    int position = option.position_key;

    std::vector<std::string> values = option.value;

    if (string_key == "") {
      tree_ptr->put("positional_cmd_arg." + boost::lexical_cast<std::string>(position),
                    values[0]);
    } else {
      for (std::string const &value : values)
        tree_ptr->put(string_key, value);
    }
  }
}

boost::optional<std::string>
argument_parser::get_param_name(const std::string &arg) {
  if (arg.length() >= 2 && arg[0] == '-' && arg[1] == '-')
    return arg.substr(2);

  if (arg.length() >= 1 && arg[0] == '-')
    return arg.substr(1);

  return boost::optional<std::string>();
}

int argument_parser::parse_single_param(int argno, int argc,
                                        const char **argv) {
  std::string leading_arg = argv[argno];

  boost::optional<std::string> opt_param_name = get_param_name(leading_arg);

  std::string param_name = opt_param_name.get_value_or(free_param_name());

  if (opt_param_name)
    ++argno;

  for (; argno < argc; ++argno) {
    std::string param_argument = argv[argno];
    if (get_param_name(param_argument)) // if is not a parameter argument
      return argno;

    tree_ptr->put(param_name, param_argument);
  }
  return argno;
}

argument_parser::ptree_type const &argument_parser::get_property_tree() const {
  return *tree_ptr;
}

std::string const &argument_parser::free_param_name() {
  static std::string name("free_param");
  return name;
}
END_NAMESPACE_CORE
