#include "advancer.hpp"

#include "interface/attribute_description.hpp"
#include "interface/attribute_descriptor.hpp"
#include "interface/attribute_list.hpp"
#include "interface/dynamic_advance_estimator.hpp"
#include "interface/static_advance_estimator.hpp"

#include <boost/optional.hpp>

BEGIN_NAMESPACE_CORE

template <class SubEstimator>
static std::vector<SubEstimator *>
get_filtered_estimators(advancer::estimator_ptr_vec const &all)
{
  std::vector<SubEstimator *> result;
  for(advancer::estimator_ptr const &estimator_ptr : all)
  {
    auto casted_ptr = dynamic_cast<SubEstimator *>(estimator_ptr.get());
    if(nullptr == casted_ptr)
      continue;
    result.push_back(std::move(casted_ptr));
  }
  return result;
}

advancer::advancer(model &model_ref, estimator_ptr_vec advancers)
    : m_model_ref(model_ref),
      m_all_advancers(std::move(advancers)),
      m_static_estimators(
          get_filtered_estimators<static_advance_estimator>(m_all_advancers)),
      m_dynamic_estimators(
          get_filtered_estimators<dynamic_advance_estimator>(m_all_advancers)),
      m_delta_time_attribute_idx(model_ref.event_description()
                                     .get_descriptor(ATTRIBUTE_KIND::DELTA_TIME)
                                     .get()
                                     .attribute_idx())
{
  for(dynamic_advance_estimator *estimator_ptr : m_dynamic_estimators)
    estimator_ptr->set_description(m_model_ref.event_description());
}

advancer::~advancer() = default;

bool advancer::advance(model::event_visitor const &visitor,
                       std::size_t num_steps)
{
  auto meta_visitor =
      [this, i(std::size_t(0)), &visitor, num_steps](event const &e) mutable
  {
    assert(i < num_steps);
    visitor(e);
    if(0 == e[m_delta_time_attribute_idx].get_int_val())
      ++i;

    if(i < num_steps)
      return;

    for(dynamic_advance_estimator *estimator_ptr : m_dynamic_estimators)
      estimator_ptr->event_update(num_steps, e);
  };

  bool ok;
  if(m_dynamic_estimators.empty())
    ok = m_model_ref.run(visitor, num_steps);
  else
    ok = m_model_ref.run(meta_visitor, num_steps);

  if(m_static_estimators.empty())
    return ok;

  configuration_view const &configuration = m_model_ref.current_configuration();
  for(static_advance_estimator *estimator_ptr : m_static_estimators)
    estimator_ptr->configuration_update(num_steps, configuration);

  return ok;
}

bool advancer::advance(std::size_t num_steps)
{
  auto visitor = [this, i(std::size_t(0)), num_steps](event const &e) mutable
  {
    assert(i < num_steps);

    if(0 == e[m_delta_time_attribute_idx].get_int_val())
      ++i;
    if(i < num_steps)
      return;
    for(dynamic_advance_estimator *estimator_ptr : m_dynamic_estimators)
      estimator_ptr->event_update(num_steps, e);
  };
  bool ok;
  if(m_dynamic_estimators.empty())
    ok = m_model_ref.run(num_steps);
  else
    ok = m_model_ref.run(visitor, num_steps);
  if(m_static_estimators.empty())
    return ok;

  configuration_view const &configuration = m_model_ref.current_configuration();
  for(static_advance_estimator *estimator_ptr : m_static_estimators)
    estimator_ptr->configuration_update(num_steps, configuration);

  return ok;
}

static advance_estimator &dereference(advancer::estimator_ptr const &ptr)
{
  return *ptr;
}

advancer::iterator advancer::begin()
{
  return {m_all_advancers.begin(), dereference};
}
advancer::iterator advancer::end()
{
  return {m_all_advancers.end(), dereference};
}

advancer::const_iterator advancer::begin() const
{
  return {m_all_advancers.begin(), dereference};
}
advancer::const_iterator advancer::end() const
{
  return {m_all_advancers.end(), dereference};
}
model const &advancer::get_model() const { return m_model_ref; }

END_NAMESPACE_CORE