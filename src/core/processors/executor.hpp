#ifndef SIMBAD_CORE_EXECUTOR_HPP
#define SIMBAD_CORE_EXECUTOR_HPP

#include "core_def.hpp"

#include "interface/advancer.hpp"
#include "interface/dynamic_advancer.hpp"
#include "interface/model.hpp"
#include "interface/static_advancer.hpp"

#include <algorithm>
#include <vector>

BEGIN_NAMESPACE_CORE

class executor
{
public:
  explicit executor(std::unique_ptr<model> model_ptr,
                    std::vector<std::unique_ptr<advancer>> advancers);

  bool advance(model::event_visitor const &visitor, std::size_t num_steps);
  void collect_estimates(std::vector<std::size_t> &result) const;

private:
  std::unique_ptr<model> const m_model_ptr;

  std::vector<std::unique_ptr<advancer>> const m_all_advancers;
  std::vector<static_advancer *> const m_static_advancers;
  std::vector<dynamic_advancer *> const m_dynamic_advancers;
};

END_NAMESPACE_CORE
#endif // SIMBAD_CORE_EXECUTOR_HPP
