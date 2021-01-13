cmake_minimum_required(VERSION 3.11)
include(FetchContent)


project(edu-DirMonitor LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "-fPIC -pthread")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

add_executable(edu-DirMonitor
        main.cpp

        src/dirmonitor.h
        src/dirmonitor.cpp

        src/server.h
        src/server.cpp

        test/dirmonitor_test.h

        src/logger.h
        src/logger.cpp
        )

find_path(HIREDIS_HEADER hiredis)
find_library(HIREDIS_LIB hiredis)

find_path(REDIS_PLUS_PLUS_HEADER sw)
find_library(REDIS_PLUS_PLUS_LIB redis++)

target_include_directories(edu-DirMonitor PUBLIC ${HIREDIS_HEADER})
target_link_libraries(edu-DirMonitor PRIVATE ${HIREDIS_LIB})

target_include_directories(edu-DirMonitor PUBLIC ${REDIS_PLUS_PLUS_HEADER})
target_link_libraries(edu-DirMonitor PRIVATE ${REDIS_PLUS_PLUS_LIB})

FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
  GIT_TAG v3.9.1)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

target_link_libraries(edu-DirMonitor PRIVATE nlohmann_json::nlohmann_json)


target_link_libraries(edu-DirMonitor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)


