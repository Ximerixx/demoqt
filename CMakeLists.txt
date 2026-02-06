cmake_minimum_required(VERSION 3.16)
project(demoqt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Sql)

qt_standard_project_setup()

qt_add_executable(demoqt
    src/main.cpp
    src/database.cpp
    src/database.h
    src/passwordhash.cpp
    src/passwordhash.h
    src/dbselectwindow.cpp
    src/dbselectwindow.h
    src/newuserdialog.cpp
    src/newuserdialog.h
    src/loginwindow.cpp
    src/loginwindow.h
    src/mainwindow.cpp
    src/mainwindow.h
    src/tabletab.cpp
    src/tabletab.h
)

target_link_libraries(demoqt PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Sql
)
