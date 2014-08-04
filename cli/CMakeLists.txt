project( ${CLI_NAME})


include_directories( inc )
include_directories( ${core_SOURCE_DIR}/inc )

FILE( GLOB HEADERS inc/*.h )  # workaround for headers in QtCreator

set(SRC
    src/main.cpp
    src/launcher.cpp
)

add_executable(${PROJECT_NAME} ${SRC} ${HEADERS} )
target_link_libraries( ${PROJECT_NAME} ${Boost_LIBRARIES} )
target_link_libraries(${PROJECT_NAME} ${CORE_NAME})

