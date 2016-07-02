set( MODEL_NAME discrete_wave_1d_2s )
set( TEST_NAME test_${MODEL_NAME} )

include_directories( inc )
include_directories( ../templates/discrete_wave_1D )

set( SOURCES
  src/discrete_wave_1d_2s.cpp
  src/discrete_wave_1d_2s_factory.cpp
  src/discrete_wave_1d_2s_impl.cpp
)

add_library(${MODEL_NAME}_obj OBJECT ${SOURCES} ${HEADERS} )

add_library(${MODEL_NAME} $<TARGET_OBJECTS:${MODEL_NAME}_obj>)
target_link_libraries( ${MODEL_NAME}  ${LIBS} )


set( TEST_MAIN_SOURCE
  test/test_main.cpp)

set( TEST_SOURCES

)

add_executable( ${TEST_NAME}  ${TEST_MAIN_SOURCE}
  $<TARGET_OBJECTS:${MODEL_NAME}_obj> ${TEST_SOURCES} )

target_link_libraries( ${TEST_NAME}   ${LIBS} )
