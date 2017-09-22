cmake_minimum_required(VERSION 3.2)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-O2 -Wall -Wextra")
project(test_project)

add_executable(test_p test.cpp)
add_executable(main main.cpp)
add_executable(simple_test simple_test.cpp)

find_package(SQLiteCpp REQUIRED)
target_link_libraries(test_p SQLiteCpp sqlite3)
target_link_libraries(main SQLiteCpp sqlite3)
target_link_libraries(simple_test SQLiteCpp sqlite3)
