project(gui)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

FIND_PACKAGE( Qt5 COMPONENTS Core Gui OpenGL REQUIRED  )

set(SRC_LIST main.cpp mainwindow.cpp)

#QT5_WRAP_CPP(gui_cpp mainwindow.h)
QT5_WRAP_UI(FORMS mainwindow.ui)


add_executable(${PROJECT_NAME} ${SRC_LIST} ${FORMS})
qt5_use_modules(${PROJECT_NAME} Core Gui Widgets OpenGL )
target_link_libraries(${PROJECT_NAME} engine)
