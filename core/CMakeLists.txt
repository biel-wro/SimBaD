set( TARGET_NAME ${CORE_NAME} )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

set( SOURCES
  src/core.cpp
  # computational
  computational/mutations/builtin_mutators.cpp
  computational/mutations/mutator.cpp
  computational/mutations/mutator_register.cpp
  # configuration
  configurations/default_attributed_configuration.cpp
  configurations/enriched_configuration.cpp
  configurations/slice_configuration.cpp
  configurations/cubic_crystal_configuration.cpp
  configurations/poisson_configuration.cpp
  configurations/selected_attributes_configuration.cpp
  configurations/stacked_view_configuration.cpp
  # interface
  interface/attribute.cpp
  interface/attribute_descriptor.cpp
  interface/attribute_list.cpp
  interface/attribute_description.cpp
  interface/configuration_reader.cpp
  interface/configuration_view.cpp
  interface/dataframe.cpp
  interface/event_kind.cpp
  interface/event_sink.cpp
  interface/event_source.cpp
  interface/finite_dataframe.cpp
  interface/property_tree.cpp
  interface/model.cpp
  interface/model_factory.cpp
  interface/model_register.cpp
  # io
  io/csv_printer.cpp
  io/json_configuration_printer.cpp
  io/stream_printer.cpp
  io/text_configuration_printer.cpp
  # processors
  processors/configuration_builder.cpp
  processors/launch_utils.cpp
  processors/snapshotter.cpp
  legacy/simple/simple_event_view.cpp
  legacy/event_partial.cpp
  # utils
  utils/attribute_exceptions.cpp
  utils/counting_id_generator.cpp
  utils/generic_particle.cpp
  utils/properties.cpp
  io/stream_reader.cpp io/stream_reader.hpp io/csv_reader.cpp io/csv_reader.hpp)

set( LIBS
  ${Boost_LIBRARIES}
)

add_library(${TARGET_NAME} ${IF_SHARED_LIBS} ${SOURCES} ${HEADERS})
target_link_libraries( ${TARGET_NAME} ${LIBS} )

subdirs( tests )
