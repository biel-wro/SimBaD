include_directories(${CMAKE_SOURCE_DIR}/core)

add_library(plugins plugin_common.cpp)
target_link_libraries(plugins ${CORE_NAME})

set(ALL_PLUGINS
  cbor_lite_io)


set(NUM_ACTIVE_PLUGINS 0)
foreach(PLUGIN ${ALL_PLUGINS})
  #option(USE_PLUGIN_${PLUGIN} "activates plugin ${PLUGIN}")
  #if(USE_PLUGIN_${PLUGIN})
    add_subdirectory(${PLUGIN})
    target_link_libraries(plugins ${PLUGIN})

    add_definitions( -DSIMBAD_PLUGIN${NUM_ACTIVE_PLUGINS}=${PLUGIN} )
    math(EXPR NUM_ACTIVE_PLUGINS ${NUM_ACTIVE_PLUGINS}+1 )

  #endif(USE_PLUGIN_${PLUGIN})
endforeach()
add_definitions(-DSIMBAD_NPLUGINS=${NUM_ACTIVE_PLUGINS})





