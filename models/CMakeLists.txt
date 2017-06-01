include_directories( ${CMAKE_SOURCE_DIR}/core/ )

set( MODEL_ROOT_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR} )

set( MODELS
#  empty_model
#  bio2D
#  adhesion_2d
#  discrete_wave_1d_1s
#  discrete_wave_1d_2s
#  front_wave_1d
  parameter_evolution_3d
#  simple_exp_2d
)

set( LIBS
  ${Boost_LIBRARIES}
  ${CORE_NAME}
)

if( !msvc )
  set(LIBS $(LIBS)
    -lstdc++
    -lm)
endif()

subdirs(common)

foreach( MODEL ${MODELS} )
  subdirs( ${MODEL} )
endforeach()

#subdirs( tests )

