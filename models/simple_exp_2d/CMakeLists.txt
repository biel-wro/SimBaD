set( MODEL_NAME simple_exp_2d )
set( TEST_NAME test_${MODEL_NAME} )

include_directories( inc )

set( MAIN_SOURCE
    src/main.cpp
)

set( SOURCES
  src/simple_exp_2d.cpp
  src/simple_exp_2d_factory.cpp
)

add_library(${MODEL_NAME}_obj OBJECT ${SOURCES} )

add_library(${MODEL_NAME} $<TARGET_OBJECTS:${MODEL_NAME}_obj> )
target_link_libraries( ${MODEL_NAME} ${LIBS} )

set( TEST_SOURCES
)

#add_executable( ${TEST_NAME} ${TEST_SOURCES} ${SOURCES} )
#target_link_libraries( ${TEST_NAME} ${CORE_NAME} ${LIBS} )
