cmake_minimum_required(VERSION 3.21.1)
project(signalBox VERSION 0.0.1 DESCRIPTION 信号工具箱 LANGUAGES CXX)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
find_package(FFTW3)

set(execName ${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 11)

include_directories(.)
include_directories(./ui)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${FFTW3_INCLUDE_DIRS})

add_definitions(-DCMAKE_PROJ_NAME="${PROJECT_NAME}")

add_executable(${execName} WIN32 main.cpp ui/mainWindow.cpp)

set_target_properties(${execName} PROPERTIES AUTOMOC TRUE AUTOUIC TRUE AUTORCC TRUE)

target_link_directories(${execName} PUBLIC ${FFTW3_LIBRARY_DIRS})

target_link_libraries(${execName} ${Qt5Widgets_LIBRARIES} ${FFTW3_LIBRARIES})
