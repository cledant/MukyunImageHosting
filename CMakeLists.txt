cmake_minimum_required(VERSION 3.13)
project(test_http_server)

#Setup Pistache
option(PISTACHE_ENABLE_NETWORK_TESTS "" OFF)
option(PISTACHE_INSTALL "" OFF)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/pistache)

#Projet libs
add_subdirectory(libraries/http_server)

#Main binary
add_executable(test_http_server binary/private/main.cpp)
target_link_libraries(test_http_server http_listener)
add_dependencies(test_http_server http_listener)
target_compile_options(test_http_server PRIVATE -Wall -Wextra -Werror -std=c++17 -march=native -flto)