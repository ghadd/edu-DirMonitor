cmake_minimum_required(VERSION 3.5)

project(edu-DirMonitor-Test LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_SOURCE_DIR ../src)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

find_package(CxxTest)
if(CXXTEST_FOUND)
    include_directories(${CXXTEST_INCLUDE_DIR})
    enable_testing()
    CXXTEST_ADD_TEST(edu-DirMonitor-Test tests.cpp
                      ${CMAKE_CURRENT_SOURCE_DIR}/dirmonitor_test.h
                      ${CMAKE_SOURCE_DIR}/dirmonitor.h
                      ${CMAKE_SOURCE_DIR}/dirmonitor.cpp)
endif()

target_link_libraries(edu-DirMonitor-Test PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
