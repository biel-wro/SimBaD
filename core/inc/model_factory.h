#ifndef MODEL_FACTORY_H
#define MODEL_FACTORY_H
#include <boost/program_options.hpp>
#include <model.h>
#include <memory>

namespace po = boost::program_options;

namespace simbad{
namespace core{


class model_factory{
public:
  virtual ~model_factory() noexcept = default;

  virtual std::string model_name() const noexcept = 0;
  virtual int dimension() const noexcept = 0;
  virtual po::options_description const &get_opt_desc() const noexcept= 0;
  virtual std::unique_ptr<model>
    create_model( po::variables_map const &args ) const noexcept = 0;

};

}
}
#endif
