project(${CORE_NAME})

FILE( GLOB_RECURSE HEADERS inc/*.h )  # workaround for headers in QtCreator
include_directories( inc )

set( SRC
    src/core.cpp
#    src/core_old.cpp
    src/generic_particle.cpp
#    src/model_loader.cpp
    src/particle_interface.cpp
)

set( LIBS
	${Boost_LIBRARIES}
)

add_library(${PROJECT_NAME} ${SHARED_LIBS} ${SRC} ${HEADERS} )

target_link_libraries( ${PROJECT_NAME} ${LIBS} )

subdirs( tests )
