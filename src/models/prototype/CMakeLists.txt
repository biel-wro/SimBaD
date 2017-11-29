file( GLOB_RECURSE HEADERS inc/*.hpp )  # workaround for headers in QtCreator
include_directories( inc )

set( SRC
    src/main.cpp
)

add_executable(prototype ${SRC} ${HEADERS} )
target_link_libraries( prototype ${LIBS} )

