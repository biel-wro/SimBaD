#include "result_of_event.h"
#include "result_point_of_space.h"

simbad::gui::result_of_event::result_of_event()
{
  global_time_of_event = -1;
  Vector_of_dead_points.reserve(2);
  Vector_of_new_points.reserve(2);
}

simbad::gui::result_of_event::result_of_event(
    int time, QVector<result_point_of_space> New_points,
    QVector<result_point_of_space> Dead_points)
{
  this->global_time_of_event = time;
  this->Vector_of_dead_points = Dead_points;
  this->Vector_of_new_points = New_points;
}
simbad::gui::result_of_event::result_of_event(const result_of_event &that)
    : global_time_of_event(that.global_time_of_event),
      Vector_of_dead_points(that.Vector_of_dead_points),
      Vector_of_new_points(that.Vector_of_new_points)

{
}

simbad::gui::result_of_event::~result_of_event() {}
QVector<simbad::gui::result_point_of_space>
simbad::gui::result_of_event::get_vector_of_dead_points()
{
  return Vector_of_dead_points;
}

QVector<simbad::gui::result_point_of_space>
simbad::gui::result_of_event::get_vector_of_new_points()
{
  return Vector_of_new_points;
}

void simbad::gui::result_of_event::set_vector_of_new_points(
    QVector<result_point_of_space> New_points)
{
  Vector_of_new_points = New_points;
}
void simbad::gui::result_of_event::set_vector_of_dead_points(
    QVector<result_point_of_space> Dead_points)
{
  Vector_of_dead_points = Dead_points;
}

long double simbad::gui::result_of_event::get_global_time_of_result_event()
{
  return global_time_of_event;
}
void simbad::gui::result_of_event::set_global_time_of_result_event(
    long double time)
{
  global_time_of_event = time;
}
