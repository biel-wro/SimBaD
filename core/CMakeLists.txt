set( TARGET_NAME ${CORE_NAME} )

include_directories( inc )

set( SOURCES
  src/core.cpp
  src/event.cpp
  src/event_kind.cpp
  src/event_partial.cpp
  src/model.cpp
  src/model_factory.cpp
  src/model_loader.cpp
  src/model_register.cpp
  src/particle.cpp
  src/simple_event_view.cpp
)

set( LIBS
  ${Boost_LIBRARIES}
)

add_library(${TARGET_NAME} ${IF_SHARED_LIBS} ${SOURCES} ${HEADERS})
target_link_libraries( ${TARGET_NAME} ${LIBS} )

subdirs( tests )
