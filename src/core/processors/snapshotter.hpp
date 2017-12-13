#ifndef SNAPSHOTTER_HPP
#define SNAPSHOTTER_HPP
#include "interface/interface_fwd.hpp"

#include <memory>

BEGIN_NAMESPACE_CORE
class snapshotter
{
public:
  using time_type = double;
  explicit snapshotter(model *model_ptr = nullptr, time_type time_step = 1);
  ~snapshotter();

  model const &get_model() const;
  void set_model(model *model_ptr);

  void set_time_step(time_type time_step);
  time_type const &get_time_step() const;

  time_type get_current_time() const;
  std::size_t get_current_step() const;

  configuration_view const &operator()();

protected:
  void next_event();

private:
  model *m_model_ptr;
  std::size_t m_time_attr_id;
  std::size_t m_current_step;
  time_type m_time_step;
  time_type m_current_time;
};
END_NAMESPACE_CORE
#endif // SNAPSHOTTER_HPP
