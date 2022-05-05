cmake_minimum_required(VERSION 3.21.1)
project(signalBox VERSION 0.0.1 DESCRIPTION 信号工具箱 LANGUAGES CXX)

find_package(Qt5 COMPONENTS Core Widgets Charts REQUIRED)

set(execName ${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 11)

if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    message("build in debug")
else()
    message("build in resease")
    set(special_flag WIN32)
endif()

set(flex_file
    ${PROJECT_BINARY_DIR}/scanner.cpp
)

set(bison_file
    ${PROJECT_BINARY_DIR}/grammar.tab.cpp
    ${PROJECT_BINARY_DIR}/grammar.tab.hpp
)

set(automatic_gen_src
    ${flex_file}
    ${bison_file}
)

set_source_files_properties(${automatic_gen_src} GENERATED)

add_custom_target(fc
    DEPENDS ${PROJECT_SOURCE_DIR}/compiler/grammar.ypp
    BYPRODUCTS ${flex_file}
    SOURCES ${PROJECT_SOURCE_DIR}/compiler/scanner.l
    # COMMAND flex --debug --noline -o ${PROJECT_BINARY_DIR}/scanner.cpp scanner.l
    COMMAND flex --noline -o ${PROJECT_BINARY_DIR}/scanner.cpp scanner.l
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/compiler/
    COMMENT "flex build"
)

add_custom_target(bc
    DEPENDS ${PROJECT_SOURCE_DIR}/compiler/scanner.l fc
    BYPRODUCTS ${bison_file}
    SOURCES ${PROJECT_SOURCE_DIR}/compiler/grammar.ypp
    COMMAND bison --debug --no-lines --token-table -d -Wcex grammar.ypp -o ${PROJECT_BINARY_DIR}/grammar.tab.cpp
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/compiler/
    COMMENT "bison build"
)

include_directories(.)
include_directories(./ui)
include_directories(./calculator)
include_directories(./compiler)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${Qt5Core_INCLUDE_DIRS})

add_definitions(-DCMAKE_PROJ_NAME="${PROJECT_NAME}")

add_executable(${execName} ${special_flag}
    main.cpp
    ui/mainWindow.cpp
    ui/ChartView.cpp
    compiler/symTable.cpp
    compiler/compiler.cpp
    compiler/ast.cpp
    calculator/innerFun.cpp
    calculator/calculator.cpp
    ${automatic_gen_src}
)

set_target_properties(${execName} PROPERTIES AUTOMOC TRUE AUTOUIC TRUE AUTORCC TRUE)

target_link_directories(${execName} PUBLIC lib)

target_link_libraries(${execName} ${Qt5Widgets_LIBRARIES} ${Qt5Charts_LIBRARIES} libfftw3f-3.dll)

file(GLOB libSrcFiles
    LIST_DIRECTORIES false
    libSrc/*.cpp
)

set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/lib)

foreach(srcFile ${libSrcFiles})
    cmake_path(RELATIVE_PATH srcFile)
    cmake_path(GET srcFile FILENAME fn)
    string(REPLACE "." ";" fn ${fn})
    list(GET fn 0 fn)
    # message("fn:" ${fn} "\nsrc:" ${srcFile} "\n")
    add_library(${fn} SHARED
        ${srcFile}
    )
endforeach()



