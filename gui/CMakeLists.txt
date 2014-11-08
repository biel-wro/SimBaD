project(${GUI_NAME})

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)



FIND_PACKAGE( Qt5 COMPONENTS Core Gui Widgets OpenGL REQUIRED  )
FIND_PACKAGE(OpenGL REQUIRED)
#FIND_PACKAGE(Qt COMPONENTS QtOpenGL REQUIRED)




#if (NOT QtOpenGL_FOUND)
#message(FATAL_ERROR "Package  and component QtOpenGL required, but not found!")
#endif( NOT QtOpenGL_FOUND )


set( SRC
    src/main.cpp
    src/mainwindow.cpp
    src/dialog_open_model.cpp
    src/simulation_dialog.cpp
    src/my_glwidget.cpp
    src/new_event_dialog.cpp
    src/dialog_for_init_model.cpp
    src/glwidjet_for_init_model.cpp

    #src/Model/dynamics_of_model.cpp
    src/Model/cell_map.cpp
    src/Model/points_of_model.cpp
    src/Model/model_of_space.cpp
    src/Model/result_of_event.cpp
    src/Model/function_for_component_rate.cpp
    src/Model/table_of_component_rates.cpp
    src/Model/distribution_of_particle.cpp
    src/Model/event_of_model.cpp
    src/Model/result_point_of_space.cpp
#    src/Model/point_of_space.cpp
    src/Model/points_of_type.cpp	
)


set( UI
    forms/mainwindow.ui
    forms/dialog_open_model.ui
    forms/simulation_dialog.ui
    forms/new_event_dialog.ui
    forms/dialog_for_init_model.ui
)



include_directories(inc)
include_directories(inc/Model) # please remove this and fix all the haeders


FILE( GLOB HEADERS inc/*.h )  # workaround for headers in QtCreator

include_directories(${${CORE_NAME}_SOURCE_DIR}/inc)



QT5_WRAP_UI(FORMS ${UI})


add_executable(${PROJECT_NAME} ${SRC} ${FORMS} ${HEADERS} )

qt5_use_modules(${PROJECT_NAME} Core Gui Widgets OpenGL)

target_link_libraries(${PROJECT_NAME} ${CORE_NAME} ${OPENGL_LIBRARIES} )

