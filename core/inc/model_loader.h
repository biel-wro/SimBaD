#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include <boost/iterator/transform_iterator.hpp>

#include "model.h"
#include "model_factory.h"

namespace simbad {
namespace core{

class model_loader
{
  model_loader() = default;
public:
  model_loader(model_loader const &) = delete;
  model_loader(model_loader &&) = default;
  model_loader &operator=(model_loader const &) = delete;
  model_loader &operator=(model_loader &&) = default;

  typedef std::string                                     key_type;
  typedef std::unique_ptr<simbad::core::model_factory>    mapped_type;
  typedef std::pair<const key_type,mapped_type>           pair_type;

  typedef std::unordered_map<key_type, mapped_type> factory_map;
  typedef factory_map::const_iterator const_iterator;

  struct name_getter{
    key_type const &operator()(pair_type const &p) const{
      return p.first;
    }
  };

  typedef boost::transform_iterator<name_getter, const_iterator> name_iterator;

  static model_loader const& get_const_instance();

  name_iterator begin_names() const;
  name_iterator end_names() const;

  bool register_factory( mapped_type f);
  void unregister_factory(const std::string &name );

  static bool glob_register_factory( mapped_type f);

  model_factory const &get_model(std::string const &name ) const;

  void reload();

private:
  factory_map factories;
};

}
}

#endif // MODEL_LOADER_H
