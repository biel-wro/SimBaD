set( MODEL_NAME parameter_evolution_3d )
set( TEST_NAME test_${MODEL_NAME} )

include_directories( BEFORE inc )

set( MAIN_SOURCE
  src/main.cpp
)

set( SOURCES
  src/parameter_evolution_3d.cpp
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
