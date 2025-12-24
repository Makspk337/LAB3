cmake_minimum_required(VERSION 3.14)
project(DataStructures VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(datastructures STATIC
    array.cpp
    doubly_list.cpp
    forward_list.cpp
    hash_table.cpp
    queue.cpp
    stack.cpp
    tree.cpp
)

target_include_directories(datastructures
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(benchmark benchmark.cpp)
target_link_libraries(benchmark datastructures)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    find_package(GTest REQUIRED)
    include(GoogleTest)
    
    add_executable(test_array GoogleTest/test_array.cpp array.cpp)
    add_executable(test_doubly_list GoogleTest/test_doubly_list.cpp doubly_list.cpp)
    add_executable(test_forward_list GoogleTest/test_forward_list.cpp forward_list.cpp)
    add_executable(test_hash_table GoogleTest/test_hash_table.cpp hash_table.cpp)
    add_executable(test_queue GoogleTest/test_queue.cpp queue.cpp)
    add_executable(test_stack GoogleTest/test_stack.cpp stack.cpp)
    add_executable(test_tree GoogleTest/test_tree.cpp tree.cpp)
    
    target_link_libraries(test_array GTest::gtest GTest::gtest_main datastructures)
    target_link_libraries(test_doubly_list GTest::gtest GTest::gtest_main datastructures)
    target_link_libraries(test_forward_list GTest::gtest GTest::gtest_main datastructures)
    target_link_libraries(test_hash_table GTest::gtest GTest::gtest_main datastructures)
    target_link_libraries(test_queue GTest::gtest GTest::gtest_main datastructures)
    target_link_libraries(test_stack GTest::gtest GTest::gtest_main datastructures)
    target_link_libraries(test_tree GTest::gtest GTest::gtest_main datastructures)
    
    gtest_discover_tests(test_array)
    gtest_discover_tests(test_doubly_list)
    gtest_discover_tests(test_forward_list)
    gtest_discover_tests(test_hash_table)
    gtest_discover_tests(test_queue)
    gtest_discover_tests(test_stack)
    gtest_discover_tests(test_tree)
    
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(test_array PRIVATE --coverage -fprofile-arcs -ftest-coverage)
        target_compile_options(test_doubly_list PRIVATE --coverage -fprofile-arcs -ftest-coverage)
        target_compile_options(test_forward_list PRIVATE --coverage -fprofile-arcs -ftest-coverage)
        target_compile_options(test_hash_table PRIVATE --coverage -fprofile-arcs -ftest-coverage)
        target_compile_options(test_queue PRIVATE --coverage -fprofile-arcs -ftest-coverage)
        target_compile_options(test_stack PRIVATE --coverage -fprofile-arcs -ftest-coverage)
        target_compile_options(test_tree PRIVATE --coverage -fprofile-arcs -ftest-coverage)
        
        target_link_options(test_array PRIVATE --coverage -fprofile-arcs -ftest-coverage)
        target_link_options(test_doubly_list PRIVATE --coverage -fprofile-arcs -ftest-coverage)
        target_link_options(test_forward_list PRIVATE --coverage -fprofile-arcs -ftest-coverage)
        target_link_options(test_hash_table PRIVATE --coverage -fprofile-arcs -ftest-coverage)
        target_link_options(test_queue PRIVATE --coverage -fprofile-arcs -ftest-coverage)
        target_link_options(test_stack PRIVATE --coverage -fprofile-arcs -ftest-coverage)
        target_link_options(test_tree PRIVATE --coverage -fprofile-arcs -ftest-coverage)
    endif()
endif()