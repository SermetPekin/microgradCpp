cmake_minimum_required(VERSION 3.10)
project(microgradCpp VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${PROJECT_SOURCE_DIR}/include /usr/local/include)

add_executable(main main.cpp)

find_package(GTest REQUIRED)

file(GLOB TEST_SOURCES tests/test*.cpp)
add_executable(test_output ${TEST_SOURCES})

target_link_libraries(test_output GTest::GTest GTest::Main pthread)

 
add_custom_target(run
    COMMAND ./main
    DEPENDS main
    COMMENT "Running the main executable"
)

add_custom_target(test_run
    COMMAND ./test_output
    DEPENDS test_output
    COMMENT "Running the tests"
)

# Clean target
add_custom_target(clean_all
    COMMAND rm -f main test_output
    COMMENT "Cleaning up build artifacts"
)
