project(${CORE_NAME})

FILE( GLOB HEADERS inc/*.h )  # workaround for headers in QtCreator
include_directories( inc )

set( SRC
    src/core.cpp
)

add_library(${PROJECT_NAME} ${SHARED_LIBS} ${SRC} ${HEADERS} )

target_link_libraries( ${PROJECT_NAME} ${Boost_LIBRARIES} )

