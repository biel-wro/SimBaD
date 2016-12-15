#ifndef COUNTING_ID_GENERATOR_HPP
#define COUNTING_ID_GENERATOR_HPP
#include "core_def.hpp"
#include <cstddef>

BEGIN_NAMESPACE_CORE
class counting_id_generator
{
public:
  counting_id_generator(std::size_t prev_id=0);
  std::size_t operator()();

private:
  std::size_t m_last_assigned_id;
};
END_NAMESPACE_CORE
#endif // COUNTING_ID_GENERATOR_HPP
