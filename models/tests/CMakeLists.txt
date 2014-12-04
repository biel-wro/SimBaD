project(test_${MODELS_NAME})

set( TEST_SRCS
  test_loader.cpp
)



foreach( FILENAME ${TEST_SRCS} )

  GET_FILENAME_COMPONENT( TEST_NAME ${FILENAME} NAME_WE )

  add_executable(${TEST_NAME} ${FILENAME} ${HEADERS} )
  target_link_libraries( ${TEST_NAME} ${LIBS} ${MODELS})
  
  add_test( ${TEST_NAME} ${TEST_NAME} )

endforeach()


