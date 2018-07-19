set(PLUGIN_NAME cbor_lite_io)

include_directories(include)
include_directories(extern/cbor-lite/include)
set(SOURCES
  src/cbor_lite_io.cpp
)

add_library(${PLUGIN_NAME} ${SOURCES})
target_link_libraries(${PLUGIN_NAME} ${CORE_NAME})


set(TEST_SOURCES
  test/ut_stream_printer.cpp)

add_executable(test_${PLUGIN_NAME} ${SOURCES} ${TEST_SOURCES})
target_link_libraries(test_${PLUGIN_NAME} ${PLUGIN_NAME})

add_test(NAME ${PLUGIN_NAME} COMMAND test_${PLUGIN_NAME})
