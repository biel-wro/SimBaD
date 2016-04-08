set( MODEL_NAME integrated_wave_1D )
set( TEST_NAME test_${MODEL_NAME} )


file( GLOB_RECURSE HEADERS inc/*.hpp )  # workaround for headers in QtCreator
include_directories( inc )

set( MAIN_SOURCE
    src/main.cpp
)

set( SOURCES
#    src/front_wave_1d_impl.cpp
#    src/front_wave_1d_algorithms.cpp
    src/driver.cpp
    src/particle_1d.cpp
#    src/saturation_distance_computer.cpp
)


set( TEST_MAIN_SOURCE
  test/test_main.cpp)

set( TEST_SOURCES
  test/ut_space_1d.cpp
  test/ut_intensity_accumulator.cpp
)
add_executable(${MODEL_NAME} ${MAIN_SOURCE} ${SOURCES} ${HEADERS} )
target_link_libraries( ${MODEL_NAME} ${LIBS} )


add_executable( ${TEST_NAME} ${TEST_MAIN_SOURCE} ${TEST_SOURCES} ${SOURCES} )
target_link_libraries( ${TEST_NAME} ${CORE_NAME} ${LIBS} )
