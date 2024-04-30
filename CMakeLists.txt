cmake_minimum_required(VERSION 3.27)

project(Regarded)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)   # for clang-tidy

find_package(Boost 1.82 COMPONENTS iostreams REQUIRED)

include(CTest)
enable_testing()

set(HEADERS
    ${PROJECT_SOURCE_DIR}/include/player.hpp
    ${PROJECT_SOURCE_DIR}/include/gameHandler.hpp
    ${PROJECT_SOURCE_DIR}/include/experiments.hpp
    ${PROJECT_SOURCE_DIR}/include/utilities.hpp
    ${PROJECT_SOURCE_DIR}/include/configReader.hpp)

set(SOURCES
    ${PROJECT_SOURCE_DIR}/src/player.cpp
    ${PROJECT_SOURCE_DIR}/src/gameHandler.cpp
    ${PROJECT_SOURCE_DIR}/src/experiments.cpp
    ${PROJECT_SOURCE_DIR}/src/utilities.cpp
    ${PROJECT_SOURCE_DIR}/src/configReader.cpp)

add_executable(${PROJECT_NAME}
               ${SOURCES}
               main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::iostreams)

add_subdirectory(tests)
