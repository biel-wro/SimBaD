#ifndef SIMBAD_CORE_ADVANCER_HPP
#define SIMBAD_CORE_ADVANCER_HPP

#include "core_def.hpp"
#include "core_fwd.hpp"

#include "interface/advance_estimator.hpp"
#include "interface/model.hpp"

#include <boost/iterator/transform_iterator.hpp>

#include <algorithm>
#include <vector>

BEGIN_NAMESPACE_CORE

class model_advancer
{
public:
  using estimator_ptr = std::unique_ptr<advance_estimator>;
  using estimator_ptr_vec = std::vector<estimator_ptr>;

  // constructors
  explicit model_advancer(model &, estimator_ptr_vec);
  ~model_advancer();

  model const &get_model() const;

  bool advance(model::event_visitor const &visitor, std::size_t num_steps);
  bool advance(std::size_t num_steps);

  using estimator_dereference = advance_estimator &(*)(estimator_ptr const &);

  using iterator = boost::transform_iterator<estimator_dereference,
                                             estimator_ptr_vec::const_iterator>;
  using const_iterator =
      boost::transform_iterator<estimator_dereference,
                                estimator_ptr_vec::const_iterator>;

  iterator begin();
  iterator end();

  const_iterator begin() const;
  const_iterator end() const;

  advance_estimator *const first_advancer_or_null();
  advance_estimator const *const first_advancer_or_null() const;

private:
  model &m_model_ref;

  estimator_ptr_vec const m_all_advancers;
  std::vector<static_advance_estimator *> const m_static_estimators;
  std::vector<dynamic_advance_estimator *> const m_dynamic_estimators;

  std::size_t m_delta_time_attribute_idx;
};

END_NAMESPACE_CORE
#endif
