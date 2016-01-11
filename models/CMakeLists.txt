include_directories( ../core/inc )


set( MODELS
#  empty_model
#  bio2D
  front_wave_1D
#  prototype
)

set( LIBS
	${Boost_LIBRARIES}
  ${CORE_NAME}
)




foreach( MODEL ${MODELS} )
  subdirs( ${MODEL} )

endforeach()


#subdirs( tests )

