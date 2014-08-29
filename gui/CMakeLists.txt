project(${GUI_NAME})

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

FIND_PACKAGE( Qt5 COMPONENTS Core Gui OpenGL REQUIRED  )

set( SRC
    src/main.cpp
    src/mainwindow.cpp
    src/dialog_open_model.cpp
    src/simulation_dialog.cpp
    src/my_glwidget.cpp
    src/new_event_dialog.cpp
)


set( UI
    forms/mainwindow.ui
    forms/dialog_open_model.ui
    forms/simulation_dialog.ui
    forms/new_event_dialog.ui
)



include_directories(inc)
FILE( GLOB HEADERS inc/*.h )  # workaround for headers in QtCreator

include_directories(${${CORE_NAME}_SOURCE_DIR}/inc)

QT5_WRAP_UI(FORMS ${UI})


add_executable(${PROJECT_NAME} ${SRC} ${FORMS} ${HEADERS})

qt5_use_modules(${PROJECT_NAME} Core Gui Widgets OpenGL )

target_link_libraries(${PROJECT_NAME} ${CORE_NAME})
