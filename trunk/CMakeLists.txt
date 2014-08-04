project(SimBaD)
cmake_minimum_required(VERSION 2.8)

set( CLI_NAME cli )
set( CORE_NAME core )
set( GUI_NAME gui )


if( WIN32 )
    set( $SHARED_LIBS )
else()
    set( $SHARED_LIBS )
endif()

# Boost	
if( WIN32 )
 set(Boost_USE_STATIC_LIBS ON)
 set(Boost_USE_MULTITHREADED ON)
endif()



find_package( Boost 1.49 COMPONENTS program_options exception REQUIRED )

include_directories( ${Boost_INCLUDE_DIR} )

subdirs( core cli gui)

