set( SOURCES
    src/builtin_models.cpp
)
include_directories(inc)

set( LIBS ${LIBS}
    ${CORE_NAME}
    ${MODELS}
    )

add_library( ${MODEL_COMMON} STATIC ${SOURCES} )
target_link_libraries(${MODEL_COMMON} ${LIBS} )


include_directories(${MODEL_ROOT_SRC_DIR} )

set(LOOP_COUNT 0)

foreach(MODEL ${MODELS} )
    add_definitions(-DSIMBAD_MODEL${LOOP_COUNT}=${MODEL} )
    math(EXPR LOOP_COUNT ${LOOP_COUNT}+1 )
endforeach()

add_definitions(-DSIMBAD_NMODELS=${LOOP_COUNT})
