project( ${CLI_NAME})

include_directories( inc )
include_directories( ${CMAKE_SOURCE_DIR}/core/inc )
include_directories( ${CMAKE_SOURCE_DIR}/models/common/inc )
include_directories( ${Boost_INCLUDE_DIR} )

set( MAIN_SOURCE src/main.cpp)

set(SOURCES
  src/launcher.cpp
)

set( LIBS
  ${Boost_LIBRARIES}
  ${MODEL_COMMON}
  ${CORE_NAME}
)

add_library(${PROJECT_NAME}_obj OBJECT ${SOURCES} )

add_executable(${PROJECT_NAME} ${MAIN_SOURCE} $<TARGET_OBJECTS:${PROJECT_NAME}_obj> )
target_link_libraries( ${PROJECT_NAME} ${LIBS} )

set( TEST_MAIN_SOURCE  test/test_main.cpp)

set( TEST_SOURCES
    test/ut_model_chooser.cpp
    test/ut_snapshotting.cpp
    )

add_executable( ${PROJECT_NAME}_test ${TEST_MAIN_SOURCE} $<TARGET_OBJECTS:${PROJECT_NAME}_obj> ${TEST_SOURCES} )
target_link_libraries( ${PROJECT_NAME}_test ${LIBS} )
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME}_test )
