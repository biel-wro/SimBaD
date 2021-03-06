set( TARGET_NAME ${CORE_NAME} )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
include_directories( core )
set( SOURCES

  # computational
  computational/time_dependent_scalars/time_dependent_scalar.cpp
  computational/time_dependent_scalars/identity.cpp
  computational/time_dependent_scalars/repository.cpp
  computational/time_dependent_scalars/segment_multiplied.cpp
  computational/time_dependent_scalars/teeth.cpp

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
  interface/attribute_kind.cpp
  interface/attribute_list.cpp
  interface/attribute_description.cpp
  interface/configuration_reader.cpp
  interface/configuration_view.cpp
  interface/dataframe.cpp
  interface/dynamic_advance_estimator.cpp
  interface/event_kind.cpp
  interface/event_sink.cpp
  interface/event_source.cpp
  interface/extractor.cpp
  interface/finite_dataframe.cpp
  interface/advance_estimator.cpp
  interface/property_tree.cpp
  interface/model.cpp
  interface/model_factory.cpp
  interface/model_register.cpp
  interface/mutator.cpp
  interface/static_advance_estimator.cpp
  interface/stream_printer.cpp
  interface/stream_reader.cpp

  # io
  io/csv_printer.cpp
  io/csv_reader.cpp
  io/detail/csv_header_parser.cpp
  io/detail/csv_record_parser.cpp
  io/json_configuration_printer.cpp
  io/null_printer.cpp
  io/text_printer.cpp

  # advance_estimators
  processors/advance_estimators/num_events.cpp
  processors/advance_estimators/in_simulation_time.cpp
  processors/advance_estimators/population_size.cpp

  # processors
  processors/abstract_builder.cpp
  processors/chronicler.cpp
  processors/chronicles_builder.cpp
  processors/configuration_builder.cpp
  processors/model_advancer.cpp
  processors/final_snapshot.cpp
  processors/printing_chronicle_builder.cpp
  processors/snapshoter.cpp
  processors/streamer.cpp
#  processors/tree_builder
  processors/dataframe_tracker.cpp
  processors/tracker_record.cpp

  # repositories
  repositories/advance_estimator_repository.cpp
  repositories/configuration_repository.cpp
  repositories/extractor_repository.cpp
  repositories/mutator_repository.cpp
  repositories/stream_printer_repository.cpp
  repositories/stream_reader_repository.cpp

  #legacy
  legacy/event_partial.cpp
  # utils
  utils/attribute_exceptions.cpp
  utils/attribute_vector.cpp
  utils/counting_id_generator.cpp
  utils/generic_particle.cpp
  utils/properties.cpp
  utils/record_transformation.cpp
  utils/stream_as_configuration.cpp
  utils/stream_as_model.cpp
  utils/stream_from_name.cpp

)

set( LIBS
  ${Boost_LIBRARIES}
  core_version
)

add_library(${TARGET_NAME} ${IF_SHARED_LIBS} ${SOURCES} ${HEADERS})
target_link_libraries( ${TARGET_NAME} ${LIBS} )

subdirs( tests version )
