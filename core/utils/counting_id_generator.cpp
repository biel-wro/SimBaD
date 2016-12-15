#include "counting_id_generator.hpp"
BEGIN_NAMESPACE_CORE
counting_id_generator::counting_id_generator(std::size_t prev_id)
    : m_last_assigned_id(prev_id)
{
}

std::size_t counting_id_generator::operator()(){
  return ++m_last_assigned_id;
}
END_NAMESPACE_CORE
