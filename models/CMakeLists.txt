include_directories( ${CMAKE_SOURCE_DIR}/core/inc )

set( MODEL_ROOT_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR} )

set( MODELS
#  empty_model
#  bio2D
  discrete_wave_1d_1s
  discrete_wave_1d_2s
  front_wave_1d

  simple_exp_2d
)

set( LIBS
  ${Boost_LIBRARIES}
  ${CORE_NAME}
 -lstdc++
 -lm
)

subdirs(common)

subdirs(parameter_evolution_3d)
foreach( MODEL ${MODELS} )
  subdirs( ${MODEL} )
endforeach()

#subdirs( tests )

