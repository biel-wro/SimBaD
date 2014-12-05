project(SimBaD)
cmake_minimum_required(VERSION 2.8)


set( CLI_NAME cli )
set( CORE_NAME core )
set( GUI_NAME gui )
set( MODELS_NAME models )


# C++11
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()


#warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")


# linking
if( WIN32 )
    set( SHARED_LIBS " ")
else()
    set( SHARED_LIBS SHARED )
endif()


# Boost	
if( WIN32 )
 set(Boost_USE_STATIC_LIBS ON)
 set(Boost_USE_MULTITHREADED ON)
endif()

find_package( Boost 1.40 COMPONENTS exception program_options REQUIRED )
add_definitions(-DBOOST_RESULT_OF_USE_DECLTYPE)
include_directories( ${Boost_INCLUDE_DIR} )

enable_testing()

set( MODELS
  empty_model
  bio2D
)


subdirs( core cli gui models)

