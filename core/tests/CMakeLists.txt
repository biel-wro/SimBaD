project(test_${CORE_NAME})

set( TEST_SRCS
  #test_fixed.cpp
  test_lazy_set_chunk.cpp
  test_lazy_set.cpp
  test_board.cpp
  test_board_coords.cpp
  test_board_tile.cpp
  test_template_instantiation.cpp
  test_tracking.cpp
)



foreach( FILENAME ${TEST_SRCS} )

  GET_FILENAME_COMPONENT( TEST_NAME ${FILENAME} NAME_WE )

  add_executable(${TEST_NAME} ${FILENAME} ${HEADERS} )
  target_link_libraries( ${TEST_NAME} ${LIBS} )
  
  add_test( ${TEST_NAME} ${TEST_NAME} )

endforeach()


