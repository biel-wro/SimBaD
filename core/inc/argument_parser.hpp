#ifndef ARGUMENT_PARSER_H
#define ARGUMENT_PARSER_H

#include "core_fwd.hpp"

#include <memory>

#include <boost/optional/optional_fwd.hpp>
#include <boost/property_tree/ptree_fwd.hpp>

BEGIN_NAMESPACE_CORE

class argument_parser
{
public:

    using ptree_type = boost::property_tree::ptree;

    argument_parser();
    argument_parser(int argc, const char **argv);
    ~argument_parser();
    void parse(int argc, const char **argv);

    const ptree_type &get_property_tree() const;
    static const std::string &free_param_name();

protected:

    static boost::optional<std::string> get_param_name(std::string const &arg);
    int parse_single_param(int argno, int argc, const char **argv);
private:
    std::unique_ptr<ptree_type> tree_ptr;

};

END_NAMESPACE_CORE
#endif // ARGUMENT_PARSER_H
