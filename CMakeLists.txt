cmake_minimum_required(VERSION 3.5)
project(ieditor VERSION 1.1.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLACS} -Wall -fpic -g -no-pie")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLACS} -std=c++17 -Wall -fpic -g -no-pie")
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLACS} -Wall -fpic -O2")
SET(CMAKE_CXX_FLACS_RELEASE "${CMAKE_CXX_FLACS} -std=c++17 -Wall -fpic -O2")
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

ADD_DEFINITIONS(-Wall -Wno-unused-parameter -Wextra -fno-tree-pre)

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

include_directories(
    .
)

set (SRCS
    ConfigStruct.cpp
    ConfigStruct.h
    FindDialog.cpp
    FindDialog.h
    imagse.qrc
    LICENSE
    Main.cpp
    MainWindow.cpp
    MainWindow.h
    NewWizard.cpp
    NewWizard.h
    OptionDialog.cpp
    OptionDialog.h
    OutputDack.cpp
    OutputDock.h
    Project.cpp
    Project.h
    ProjectDock.cpp
    ProjectDock.h
    ProjectTree.cpp
    ProjectTree.h
    README.md
    SaveDialog.cpp
    SaveDialog.h
    TextEdit.cpp
    TextEdit.h
    TextEditTab.cpp
    TextEditTab.h
)


add_executable(${CMAKE_PROJECT_NAME} ${SRCS})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)
