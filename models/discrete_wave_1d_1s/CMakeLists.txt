set( MODEL_NAME discrete_wave_1d_1s )
set( TEST_NAME test_${MODEL_NAME} )

include_directories(inc)
include_directories( ../templates/discrete_wave_1D )

set( MAIN_SOURCE
    src/main.cpp
)

set( SOURCES
  src/discrete_wave_1d_1s.cpp
  src/discrete_wave_1d_1s_factory.cpp
  src/discrete_wave_1d_1s_impl.cpp
)

add_library(${MODEL_NAME}_obj OBJECT ${SOURCES} )

add_library(${MODEL_NAME} STATIC $<TARGET_OBJECTS:${MODEL_NAME}_obj>)
target_link_libraries( ${MODEL_NAME}  ${LIBS} )

set( TEST_MAIN_SOURCE test/test_main.cpp )

set( TEST_SOURCES
)

add_executable( ${TEST_NAME}  ${TEST_MAIN_SOURCE} $<TARGET_OBJECTS:${MODEL_NAME}_obj> ${TEST_SOURCES} )
target_link_libraries( ${TEST_NAME}   ${LIBS} )

#add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME} )
