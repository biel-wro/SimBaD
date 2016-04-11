set(TARGET_NAME test_${CORE_NAME})

set( TEST_MAIN_SOURCE core_test_main.cpp )

set( TEST_SOURCES
  #test_fixed.cpp
  ut_board.cpp
  ut_board_coords.cpp
  ut_board_tile.cpp
  ut_lazy_set.cpp
  ut_lazy_set_chunk.cpp
  ut_power_law_distribution.cpp
  ut_simple_point_and_event.cpp
  ut_trackee.cpp
)


add_executable( ${TARGET_NAME} ${TEST_MAIN_SOURCE} ${TEST_SOURCES} )
target_link_libraries( ${TARGET_NAME} ${CORE_NAME} ${LIBS} )

