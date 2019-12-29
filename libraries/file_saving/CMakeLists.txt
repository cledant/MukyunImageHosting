cmake_minimum_required(VERSION 3.13)
project(mukyun_file_saving)

#File saving library
add_library(file_saving STATIC
		private/file_saving.cpp)
target_include_directories(file_saving
		PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/public
		PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/private)
target_link_libraries(file_saving nlohmann_json::nlohmann_json)
target_compile_options(file_saving PRIVATE -Wall -Wextra -Werror -std=c++17 -march=native -flto)