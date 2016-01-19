set( MODEL_NAME front_wave_1D )
set( TEST_NAME test_${MODEL_NAME} )


file( GLOB_RECURSE HEADERS inc/*.hpp )  # workaround for headers in QtCreator
include_directories( inc )

set( MAIN_SOURCE
    src/main.cpp
)

set( SOURCES
    src/event_rate_accumulator.cpp
    src/front_wave_1d.cpp
    src/front_wave_1d_impl.cpp
    src/front_wave_1d_algorithms.cpp
    src/particle_1d.cpp
)


set( TEST_SOURCES
  test/ut_front_wave_1d.cpp
  test/ut_front_wave_1d_impl.cpp
)
add_executable(${MODEL_NAME} ${MAIN_SOURCE} ${SOURCES} ${HEADERS} )
target_link_libraries( ${MODEL_NAME} ${LIBS} )


add_executable( ${TEST_NAME} ${TEST_SOURCES} ${SOURCES} )
target_link_libraries( ${TEST_NAME} ${CORE_NAME} ${LIBS} )
