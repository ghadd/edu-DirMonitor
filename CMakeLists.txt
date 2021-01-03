cmake_minimum_required(VERSION 3.5)

project(edu-DirMonitor LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "-Wall -W -Werror -fPIC -pthread")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

add_executable(edu-DirMonitor
  main.cpp
  app.cpp
  app.h
  app.ui

  assets.qrc

  src/dirmonitor.h
  src/dirmonitor.cpp

  test/dirmonitor_test.h
  )

find_path(HIREDIS_HEADER hiredis)
find_library(HIREDIS_LIB hiredis)

find_path(REDIS_PLUS_PLUS_HEADER sw)
find_library(REDIS_PLUS_PLUS_LIB redis++)

target_include_directories(edu-DirMonitor PUBLIC ${HIREDIS_HEADER})
target_link_libraries(edu-DirMonitor PRIVATE ${HIREDIS_LIB})

target_include_directories(edu-DirMonitor PUBLIC ${REDIS_PLUS_PLUS_HEADER})
target_link_libraries(edu-DirMonitor PRIVATE ${REDIS_PLUS_PLUS_LIB})

find_package(nlohmann_json 3.2.0 REQUIRED)
target_link_libraries(edu-DirMonitor PRIVATE nlohmann_json::nlohmann_json)

target_link_libraries(edu-DirMonitor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

