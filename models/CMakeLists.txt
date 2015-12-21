include_directories( ../core/inc )


set( MODELS
#  empty_model
#  bio2D
  prototype
)

set( LIBS
	${Boost_LIBRARIES}
  ${CORE_NAME}
)




foreach( MODEL ${MODELS} )
  subdirs( ${MODEL} )

endforeach()


#subdirs( tests )

