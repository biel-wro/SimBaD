set( MODEL_NAME adhesion_2d )
set( TEST_NAME test_${MODEL_NAME} )

include_directories( BEFORE inc )

set( MAIN_SOURCE
  src/main.cpp
)

set( SOURCES
  src/model_parameters.cpp
  src/adhesion_2d.cpp
  src/particle.cpp
)

add_library(${MODEL_NAME}_obj OBJECT ${SOURCES} )

add_library(${MODEL_NAME} $<TARGET_OBJECTS:${MODEL_NAME}_obj> )
target_link_libraries( ${MODEL_NAME} ${LIBS} )

set( TEST_MAIN_SOURCE
  test/test_main.cpp)

set( TEST_SOURCES
  test/ut_adhesion_2d.cpp
)

add_executable( ${TEST_NAME} ${TEST_MAIN_SOURCE}
  $<TARGET_OBJECTS:${MODEL_NAME}_obj> ${TEST_SOURCES} )

target_link_libraries( ${TEST_NAME} ${LIBS} )
