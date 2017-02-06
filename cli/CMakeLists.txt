include_directories( inc )
include_directories( ${CMAKE_SOURCE_DIR}/core/ )
include_directories( ${CMAKE_SOURCE_DIR}/models/common/inc )
include_directories( ${Boost_INCLUDE_DIR} )

set( MAIN_SOURCE src/main.cpp)

set(SOURCES
  src/argument_parser.cpp
  src/launcher.cpp
)

set( LIBS
  ${Boost_LIBRARIES}
  ${MODEL_COMMON}
  ${CORE_NAME}
)

add_library(${CLI_NAME}_obj OBJECT ${SOURCES} )

add_executable(${CLI_NAME} ${MAIN_SOURCE} $<TARGET_OBJECTS:${CLI_NAME}_obj> )
target_link_libraries( ${CLI_NAME} ${LIBS} )

set( TEST_MAIN_SOURCE  test/test_main.cpp)

set( TEST_SOURCES
    test/ut_argument_parser.cpp
    test/ut_model_chooser.cpp
    test/ut_snapshotting.cpp
    )

set_target_properties(${CLI_NAME} PROPERTIES OUTPUT_NAME SimBaD-cli)
install(TARGETS ${CLI_NAME} RENAME SimBaD-cli DESTINATION bin)


add_executable( ${CLI_NAME}_test ${TEST_MAIN_SOURCE} $<TARGET_OBJECTS:${CLI_NAME}_obj> ${TEST_SOURCES} )
target_link_libraries( ${CLI_NAME}_test ${LIBS} )
add_test(NAME ${CLI_NAME} COMMAND ${CLI_NAME}_test )


