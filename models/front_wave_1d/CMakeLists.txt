set( MODEL_NAME front_wave_1d )
set( TEST_NAME test_${MODEL_NAME} )

include_directories( inc )

set( MAIN_SOURCE
    src/main.cpp
)

set( SOURCES
    src/birth_rate_accumulator.cpp
    src/event_rate_accumulator.cpp
    src/front_wave_1d.cpp
    src/front_wave_1d_impl.cpp
    src/front_wave_1d_factory.cpp
    src/particle_1d.cpp
    src/saturation_distance_computer.cpp
    src/spatial_neighbourhood.cpp
)

add_library(${MODEL_NAME}_obj OBJECT ${SOURCES} )

add_library(${MODEL_NAME} $<TARGET_OBJECTS:${MODEL_NAME}_obj> )
target_link_libraries( ${MODEL_NAME} ${LIBS} )

set( TEST_SOURCES
  test/ut_front_wave_1d.cpp
  test/ut_front_wave_1d_impl.cpp
)

add_executable( ${TEST_NAME} ${TEST_SOURCES} ${SOURCES} )
target_link_libraries( ${TEST_NAME} ${CORE_NAME} ${LIBS} )

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME} )
