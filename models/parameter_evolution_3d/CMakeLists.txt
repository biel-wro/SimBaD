set( MODEL_NAME parameter_evolution_3d )
set( TEST_NAME test_${MODEL_NAME} )

include_directories( BEFORE ${CMAKE_CURRENT_SOURCE_DIR} )

set( MAIN_SOURCE
  src/main.cpp
)

set( SOURCES
  configuration_view.cpp
  intrinsic_params.cpp
  model_parameters.cpp
  parameter_evolution_3d.cpp
  particle.cpp
  particle_view.cpp
)

add_library(${MODEL_NAME}_obj OBJECT ${SOURCES} )

add_library(${MODEL_NAME} $<TARGET_OBJECTS:${MODEL_NAME}_obj> )
target_link_libraries( ${MODEL_NAME} ${LIBS} )

set( TEST_MAIN_SOURCE
  test/test_main.cpp)

set( TEST_SOURCES
  test/ut_parameter_evolution_3d.cpp
)

add_executable( ${TEST_NAME} ${TEST_MAIN_SOURCE}
  $<TARGET_OBJECTS:${MODEL_NAME}_obj> ${TEST_SOURCES} )

target_link_libraries( ${TEST_NAME} ${LIBS} )
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME} )
