set( MODEL_NAME integrated_wave_1D )
set( TEST_NAME test_${MODEL_NAME} )

include_directories( inc )

set( MAIN_SOURCE
    src/main.cpp
)

set( SOURCES
    src/driver.cpp
    src/birth_ccdf_computer.cpp
    src/birth_pdf_computer.cpp
    src/particle_1d.cpp
#    src/saturation_distance_computer.cpp
)

add_library(${MODEL_NAME}_obj OBJECT ${SOURCES} ${HEADERS} )

add_executable(${MODEL_NAME} $<TARGET_OBJECTS:${MODEL_NAME}_obj> ${MAIN_SOURCE})
target_link_libraries( ${MODEL_NAME}  ${LIBS} )

set( TEST_MAIN_SOURCE
  test/test_main.cpp)

set( TEST_SOURCES
  test/ut_space_1d.cpp
  test/ut_intensity_accumulator.cpp
)

add_executable( ${TEST_NAME}  ${TEST_MAIN_SOURCE}
  $<TARGET_OBJECTS:${MODEL_NAME}_obj> ${TEST_SOURCES} )

target_link_libraries( ${TEST_NAME}   ${LIBS} )
