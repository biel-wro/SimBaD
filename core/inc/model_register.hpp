#ifndef MODEL_REGISTER_HPP
#define MODEL_REGISTER_HPP

#include "model_register.hpp"

#include "core_fwd.hpp"

#include <map>
#include <memory>
#include <string>
#include <unordered_map>

#include <boost/iterator/transform_iterator.hpp>

BEGIN_NAMESPACE_CORE

class model_register {
public:
  // types
  using key_type = std::string;
  using mapped_type = std::unique_ptr<const simbad::core::model_factory>;
  using mapped_pair_type = std::pair<const key_type, mapped_type>;

  using factory_map = std::unordered_map<key_type, mapped_type>;
  using factory_iterator = factory_map::const_iterator;

  struct name_list {
    struct name_getter {
      key_type const &operator()(mapped_pair_type const &p) const {
        return p.first;
      }
    };
    using name_iterator =
        boost::transform_iterator<name_getter, factory_iterator>;

    name_list(factory_iterator beg, factory_iterator end);
    name_iterator begin() const;
    name_iterator end() const;

  private:
    name_iterator m_beg, m_end;
  };

  // member functions
  name_list get_model_names() const;

  model_factory &get_model_factory(std::string const &model_name);
  model_factory const &get_model_factory(std::string const &model_name) const;

  bool register_factory(mapped_type f);
  void unregister_factory(std::string const &model_name);

private:
  factory_map factories;
};

END_NAMESPACE_CORE

#endif // MODEL_REGISTER_HPP
