project(bio2D)

FILE( GLOB_RECURSE HEADERS inc/*.h )  # workaround for headers in QtCreator
include_directories( inc )

set( SRC
    src/model_impl.cpp
    src/factory_impl.cpp
)

add_library(${PROJECT_NAME} ${SHARED_LIBS} ${SRC} ${HEADERS} )

target_link_libraries( ${PROJECT_NAME} ${LIBS} )

