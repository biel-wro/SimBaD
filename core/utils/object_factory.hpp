#ifndef OBJECT_FACTORY_HPP
#define OBJECT_FACTORY_HPP
#include "core_fwd.hpp"
#include <string>
#include <memory>
BEGIN_NAMESPACE_CORE
template<class T>
class object_factory
{
public:
    object_factory(){}
    virtual ~object_factory(){}
    virtual std::unique_ptr<T> create_instance(property_tree const &mp) const = 0;
    virtual std::string class_name() const = 0;
};
END_NAMESPACE_CORE

#endif // OBJECT_FACTORY_HPP
