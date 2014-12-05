#include "factory_impl.h"

#include "model_impl.h"

namespace simbad {
namespace models {
namespace empty_model{

static volatile bool is_registered = core::model_loader::glob_register_factory(
      std::unique_ptr<core::model_factory>(new factory_impl()) );

factory_impl::factory_impl():
  opt_desc("Biological model 2D")
{
  }

std::string factory_impl::model_name() const noexcept{
  return model_impl::name_str;
}

int factory_impl::dimension() const noexcept{
  return 0;
}

const po::options_description &
factory_impl::get_opt_desc() const noexcept{
  return opt_desc;
}

std::unique_ptr<core::model>
factory_impl::create_model(const po::variables_map &args) const noexcept
{
  (void) args;
  model_impl *p_model = new model_impl();
  std::unique_ptr<core::model> uptr(p_model);
  return std::move(uptr);
}

}}}
