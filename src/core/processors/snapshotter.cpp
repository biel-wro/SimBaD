#include "snapshotter.hpp"
#include <assert.h>

#include <boost/optional.hpp>

#include "interface/attribute.hpp"
#include "interface/attribute_description.hpp"
#include "interface/attribute_descriptor.hpp"
#include "interface/attribute_list.hpp"
#include "interface/model.hpp"

BEGIN_NAMESPACE_CORE
snapshotter::snapshotter(model *model_ptr, time_type time_step)
    : m_model_ptr(model_ptr),
      m_time_attr_id(model_ptr->event_descriptor()
                         .get_descriptor(ATTRIBUTE_KIND::TIME)
                         .get()
                         .attribute_idx()),
      m_current_step(0),
      m_time_step(time_step),
      m_current_time(0.0)
{
}
snapshotter::~snapshotter() {}
const model &snapshotter::get_model() const { return *m_model_ptr; }
void snapshotter::set_model(model *model_ptr) { m_model_ptr = model_ptr; }
void snapshotter::set_time_step(snapshotter::time_type time_step)
{
  m_time_step = time_step;
}
const snapshotter::time_type &snapshotter::get_time_step() const
{
  return m_time_step;
}

snapshotter::time_type snapshotter::get_current_time() const
{
  return m_current_time;
}
std::size_t snapshotter::get_current_step() const { return m_current_step; }
const configuration_view &snapshotter::operator()()
{
  time_type expected_time = m_current_step * m_time_step;
  while(get_current_time() < expected_time)
    next_event();

  ++m_current_step;
  return get_model().current_configuration();
}

void snapshotter::next_event()
{
  m_model_ptr->run(
      [this](event const &e) {
        time_type new_time = e[m_time_attr_id].get_real_val();
        assert(new_time >= m_current_time);
        m_current_time = new_time;
      },
      1);
}
END_NAMESPACE_CORE