project(${MODELS_NAME})
include_directories( ../core/inc )

set( LIBS
	${Boost_LIBRARIES}
#  ${CORE_NAME}
)

foreach( MODEL ${MODELS} )
  subdirs( ${MODEL} )

endforeach()


subdirs( tests )
