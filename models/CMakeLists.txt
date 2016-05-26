include_directories( ../core/inc )


set( MODELS
#  empty_model
#  bio2D
  discrete_wave_1D
  discrete_wave_1D_2s
  front_wave_1D
  integrated_wave_1D
#  prototype
)

set( LIBS
	${Boost_LIBRARIES}
  ${CORE_NAME}
 -lstdc++
 -lm
)




foreach( MODEL ${MODELS} )
  subdirs( ${MODEL} )

endforeach()


#subdirs( tests )

