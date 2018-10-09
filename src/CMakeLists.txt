project(SimBaD)
cmake_minimum_required(VERSION 3.0)

set( CLI_NAME cli )
set( CORE_NAME core )
set( GUI_NAME gui )
set( MODELS_NAME models )
set( MODEL_COMMON model_common )

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_FLAGS "-std=c++14")
message("compiler=" ${CMAKE_CXX_COMPILER})

set(SANITIZER "" CACHE STRING "use ADDRESS or MEMORY sanitizer when building" )

if( SANITIZER STREQUAL ADDRESS )
  message("using ADDRESS sanitizer")
  add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
elseif( SANITIZER STREQUAL MEMORY)
  message("using MEMORY sanitizer")
  add_compile_options(-fsanitize=memory -fno-omit-frame-pointer)
endif()

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
 find_package( Boost 1.61 REQUIRED COMPONENTS
   exception
   program_options
   unit_test_framework
 )
else()
  find_package( Boost 1.61 REQUIRED COMPONENTS
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
add_subdirectory(plugins)

install(DIRECTORY ../examples DESTINATION examples)
