set( MODEL_NAME simple_exp_2d )
set( TEST_NAME test_${MODEL_NAME} )

include_directories( BEFORE inc )

set( MAIN_SOURCE
    src/main.cpp
)

set( SOURCES
  src/dispersion.cpp
  src/kernel.cpp
  src/particle_2d.cpp
  src/saturation_intensity.cpp
  src/simple_exp_2d.cpp
  src/simple_exp_2d_factory.cpp
)

add_library(${MODEL_NAME}_obj OBJECT ${SOURCES} )

add_library(${MODEL_NAME} $<TARGET_OBJECTS:${MODEL_NAME}_obj> )
target_link_libraries( ${MODEL_NAME} ${LIBS} )

set( TEST_MAIN_SOURCE
  test/test_main.cpp)

set( TEST_SOURCES
  test/ut_simple_exp_2d.cpp
  test/ut_simple_exp_2d_structures.cpp
  test/ut_kernel.cpp
)

add_executable( ${TEST_NAME} ${TEST_MAIN_SOURCE}
  $<TARGET_OBJECTS:${MODEL_NAME}_obj> ${TEST_SOURCES} )

target_link_libraries( ${TEST_NAME} ${LIBS} )
