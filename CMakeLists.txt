cmake_minimum_required(VERSION 3.21.1)
project(signalBox VERSION 0.0.1 DESCRIPTION 信号工具箱 LANGUAGES CXX)

find_package(Qt5 COMPONENTS Core Widgets Charts REQUIRED)
find_package(FFTW3)

set(execName ${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 11)


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
    DEPENDS ${PROJECT_SOURCE_DIR}/calculator/grammar.ypp
    BYPRODUCTS ${flex_file}
    SOURCES ${PROJECT_SOURCE_DIR}/calculator/scanner.l
    # COMMAND flex --debug --noline -o ${PROJECT_BINARY_DIR}/scanner.cpp scanner.l
    COMMAND flex --noline -o ${PROJECT_BINARY_DIR}/scanner.cpp scanner.l
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/calculator/
    COMMENT "flex build"
)


add_custom_target(bc
    DEPENDS ${PROJECT_SOURCE_DIR}/calculator/scanner.l fc
    BYPRODUCTS ${bison_file}
    SOURCES ${PROJECT_SOURCE_DIR}/calculator/grammar.ypp
    COMMAND bison --debug --no-lines --token-table -d -Wcex grammar.ypp -o ${PROJECT_BINARY_DIR}/grammar.tab.cpp
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/calculator/
    COMMENT "bison build"
)


include_directories(.)
include_directories(./ui)
include_directories(./calculator)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${FFTW3_INCLUDE_DIRS})
include_directories(${Qt5Core_INCLUDE_DIRS})

add_definitions(-DCMAKE_PROJ_NAME="${PROJECT_NAME}")

add_executable(${execName}
    main.cpp
    ui/mainWindow.cpp
    calculator/ast.cpp
    calculator/symTable.cpp
    calculator/innerFun.cpp
    ${automatic_gen_src}
)

set_target_properties(${execName} PROPERTIES AUTOMOC TRUE AUTOUIC TRUE AUTORCC TRUE)

target_link_directories(${execName} PUBLIC ${FFTW3_LIBRARY_DIRS})

target_link_libraries(${execName} ${Qt5Widgets_LIBRARIES} ${Qt5Charts_LIBRARIES} ${FFTW3_LIBRARIES})
