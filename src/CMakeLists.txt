# This is my first attempt to use CMake for TBE...


cmake_minimum_required (VERSION 2.8.6)
project (tbe)

SET(main_SOURCES
    backtrace.cpp
    main.cpp
)
SET (main_HEADERS
    tbe_global.h
)

SUBDIRS(box2d)
INCLUDE(control/control.txt)
INCLUDE(loadsave/loadsave.txt)
INCLUDE(model/model.txt)
INCLUDE(view/view.txt)

include_directories(
    box2d
    control
    loadsave
    model
    view
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_WORK_DIR}
    ${QT_INCLUDES}
)


SET(tbe_SOURCES ${control_SOURCES} ${loadsave_SOURCES} ${main_SOURCES} ${model_SOURCES} ${view_SOURCES})
SET(tbe_HEADERS ${control_HEADERS} ${loadsave_HEADERS} ${main_HEADERS} ${model_HEADERS} ${view_HEADERS})
SET(tbe_UIFORMS ${control_UIFORMS} ${loadsave_UIFORMS} ${main_UIFORMS} ${model_UIFORMS} ${view_UIFORMS})

set(CMAKE_CXX_FLAGS "-g -Wall -Wextra")

# all Qt-specifics
find_package(Qt4 REQUIRED)
ADD_DEFINITIONS(${QT_DEFINITIONS})

#QT4_WRAP_CPP(tbe_HEADERS_MOC ${tbe_SOURCES})
qt4_automoc(tbe_SOURCES)
QT4_WRAP_UI(tbe_FORMS_HEADERS ${tbe_UIFORMS})
#QT4_ADD_RESOURCES(tbe_RESOURCES_RCC ${view_RESOURCES})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

SET(QT_USE_QTXML TRUE)
SET(QT_USE_QTSVG TRUE)

ADD_EXECUTABLE(tbe 
    ${tbe_SOURCES}
    ${tbe_HEADERS_MOC}
    ${tbe_FORMS_HEADERS}
)

INCLUDE(${QT_USE_FILE})
TARGET_LINK_LIBRARIES(tbe ${QT_LIBRARIES} box2d)
