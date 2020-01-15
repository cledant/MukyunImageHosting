cmake_minimum_required(VERSION 3.13)
project(mukyun_image_hosting)

#Setup Pistache
option(PISTACHE_ENABLE_NETWORK_TESTS "" OFF)
option(PISTACHE_INSTALL "" OFF)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/pistache)

#Setup nlohmann json
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/json)

#Projet libs
add_subdirectory(libraries/http_server)
add_subdirectory(libraries/file_saving)

#Main binary
add_executable(mukyun_image_hosting binary/private/main.cpp)
target_link_libraries(mukyun_image_hosting http_listener)
add_dependencies(mukyun_image_hosting http_listener)
target_compile_options(mukyun_image_hosting PRIVATE -Wall -Wextra -Werror -std=c++17 -flto)