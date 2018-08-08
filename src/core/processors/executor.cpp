#include "executor.hpp"

BEGIN_NAMESPACE_CORE

template <class SubAdvancer>
static std::vector<SubAdvancer *>
get_filtered_advancers(std::vector<std::unique_ptr<advancer>> const &all)
{
  std::vector<SubAdvancer *> result;
  for(std::unique_ptr<advancer> const &advancer_ptr : all)
  {
    auto casted_ptr = dynamic_cast<SubAdvancer *>(advancer_ptr.get());
    if(nullptr == casted_ptr)
      continue;
    result.push_back(casted_ptr);
  }
  return result;
}

executor::executor(std::unique_ptr<model> model_ptr,
                   std::vector<std::unique_ptr<advancer>> advancers)
    : m_model_ptr(std::move(model_ptr)),
      m_all_advancers(std::move(advancers)),
      m_static_advancers(
          get_filtered_advancers<static_advancer>(m_all_advancers)),
      m_dynamic_advancers(
          get_filtered_advancers<dynamic_advancer>(m_all_advancers))
{
}
bool executor::advance(model::event_visitor const &visitor,
                       std::size_t num_steps)
{
  bool ok;

  auto meta_visitor =
      [this, i(std::size_t(0)), &visitor, num_steps](event const &event) mutable
  {
    visitor(event);
    if(i++ < num_steps)
      return;

    for(dynamic_advancer *dynamic_advancer_ptr : m_dynamic_advancers)
      dynamic_advancer_ptr->event_update(num_steps, event);
  };
  ok = m_model_ptr->run(meta_visitor, num_steps);

  configuration_view const &configuration =
      m_model_ptr->current_configuration();

  for(static_advancer *static_advancer_ptr : m_static_advancers)
    static_advancer_ptr->configuration_update(num_steps, configuration);
  return ok;
}
void executor::collect_estimates(std::vector<std::size_t> &result) const
{
  result.resize(m_all_advancers.size());
  std::transform(m_all_advancers.cbegin(), m_all_advancers.cend(),
                 result.begin(),
                 [](std::unique_ptr<advancer> const &advancer_ptr) {
                   return advancer_ptr->estimate();
                 });
}

END_NAMESPACE_CORE