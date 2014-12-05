//#define BOOST_RESULT_OF_USE_DECLTYPE

#include "model_loader.h"


namespace simbad {
namespace core {



model_loader const  &model_loader::get_const_instance()
{
  static model_loader global= model_loader();
  return global;
}

model_loader::name_iterator model_loader::begin_names() const{
  return name_iterator( factories.begin(), name_getter() );
}

model_loader::name_iterator model_loader::end_names() const{
  return name_iterator( factories.end(), name_getter() );
}

bool model_loader::register_factory(mapped_type f){
  std::string name = f->model_name();
  return factories.insert( std::make_pair( name, std::move(f) ) ).second;
}

void model_loader::unregister_factory(std::string const &name){
  factories.erase( name );

}

bool model_loader::glob_register_factory(mapped_type f)
{
  model_loader &l = const_cast<model_loader &>(get_const_instance());
  return l.register_factory( std::move(f) );
}

const model_factory &model_loader::get_model(const std::string &name) const
{
  const_iterator it = factories.find(name);
  if( factories.end() == it )
    throw std::range_error(std::string("there is no module named ").append(name));
  model_factory const &f = *(it->second );
  return f;
}

void model_loader::reload()
{

}






}
}

