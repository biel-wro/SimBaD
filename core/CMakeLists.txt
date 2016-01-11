set( TARGET_NAME ${CORE_NAME} )

include_directories( inc )

file( GLOB_RECURSE HEADERS inc/*.hpp )  # workaround for headers in QtCreator

set( SOURCES
  src/core.cpp
  src/event_kind.cpp
  src/generic_particle.cpp
  src/model.cpp
  src/model_loader.cpp
  src/particle_interface.cpp
)

set( LIBS
	${Boost_LIBRARIES}
)

add_library(${TARGET_NAME} ${IF_SHARED_LIBS} ${SOURCES} ${HEADERS})
target_link_libraries( ${TARGET_NAME} ${LIBS} )

subdirs( tests )
