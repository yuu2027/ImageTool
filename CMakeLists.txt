cmake_minimum_required(VERSION 3.8)

set(OpenCV_DIR "C:/Users/yuu20/opencv/build")

project(imgtool)

find_package(OpenCV REQUIRED)

add_executable(imgtool
    src/main.cpp
    src/Config.cpp
    src/FileScanner.cpp
    src/ImageProcessor.cpp
    src/NameGenerator.cpp
    src/BatchRunner.cpp
    src/Logger.cpp
)

target_include_directories(imgtool PRIVATE
    include
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(imgtool PRIVATE
    ${OpenCV_LIBS}
)

if(CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET imgtool PROPERTY CXX_STANDARD 20)
endif()