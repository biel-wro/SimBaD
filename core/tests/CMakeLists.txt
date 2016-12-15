set(TARGET_NAME test_${CORE_NAME})

set( TEST_MAIN_SOURCE test_main.cpp )

set( TEST_SOURCES
  #test_fixed.cpp
  ut_attribute.cpp
  ut_attributes_descriptor.cpp
  ut_binary_tree.cpp
  #ut_board.cpp
  ut_board_coords.cpp
  ut_board_tile.cpp
  ut_cartesian_spacetime.cpp
  ut_configuration_builder.cpp
  ut_cubic_crystal_configuration.cpp
  ut_generic_particle.cpp
  ut_lazy_set.cpp
  ut_lazy_set_chunk.cpp
  ut_ordered_board.cpp
  ut_pairing_heap.cpp
  ut_parent_from_member.cpp
  ut_poisson_configuration.cpp
  ut_power_law_distribution.cpp
  ut_properties.cpp
  ut_skewed_beta_mutation.cpp
  ut_simple_point_and_event.cpp
  ut_space.cpp
  ut_text_io.cpp
  #ut_spacetime.cpp
  ut_tilers.cpp
  ut_trackee.cpp
)

add_executable( ${TARGET_NAME} ${TEST_MAIN_SOURCE} ${TEST_SOURCES} )
target_link_libraries( ${TARGET_NAME} ${CORE_NAME} ${LIBS} )

add_test(NAME Core COMMAND ${TARGET_NAME} )
