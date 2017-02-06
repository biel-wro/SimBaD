project(SimBaD)
cmake_minimum_required(VERSION 3.0)

set( CLI_NAME cli )
set( CORE_NAME core )
set( GUI_NAME gui )
set( MODELS_NAME models )
set( MODEL_COMMON model_common )

# C++11
include(CheckCXXCompilerFlag)

#CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
#CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
#if(COMPILER_SUPPORTS_CXX11)
#        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#elseif(COMPILER_SUPPORTS_CXX0X)
#        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
#else()
#        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
#endif()

#set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-std=c++14")
message("compiler=" ${CMAKE_CXX_COMPILER})

#sanitizer
#if ( UNIX)
#  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
#    message("using addres sanitizer in debug")
#      set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
#  endif()
#endif()

#warnings
if( MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W0")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
endif()

#exception handler
if( MSVC )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
endif()

# linking
if( WIN32 )
  set( IF_SHARED_LIBS STATIC )
else()
  set( IF_SHARED_LIBS "" )
endif()

# Boost	
if( WIN32 )
 set(Boost_USE_STATIC_LIBS ON)
 set(Boost_USE_MULTITHREADED ON)
 find_package( Boost 1.54 REQUIRED COMPONENTS
   exception
   program_options
   unit_test_framework
 )
else()
  find_package( Boost 1.54 REQUIRED COMPONENTS
    exception
    program_options
  )
endif()

add_definitions(-DBOOST_RESULT_OF_USE_DECLTYPE)
include_directories( ${Boost_INCLUDE_DIR} )

enable_testing()

file( GLOB_RECURSE ALL_HEADERS ./*.hpp )                            # workaround for headers in QtCreator
add_custom_target(dummy_header_target SOURCES ${ALL_HEADERS})       #

add_subdirectory(core)
add_subdirectory(cli)
#add_subdirectory(gui)
add_subdirectory(models)

install(DIRECTORY examples DESTINATION examples)
