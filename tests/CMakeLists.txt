include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.11.0 # Adjust to the desired version
)

FetchContent_MakeAvailable(googletest)

add_executable(test1 ${CMAKE_SOURCE_DIR}/src/player.cpp testing.cpp )
target_include_directories(test1 PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(test1 gtest gtest_main)

add_test(NAME test1 COMMAND test1)
