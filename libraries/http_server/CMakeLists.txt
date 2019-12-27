#Http_listener library
add_library(http_listener STATIC
		private/http_server.cpp)
target_include_directories(http_listener
		PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/public
		PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/private)
if (CMAKE_BUILD_TYPE MATCHES "Debug")
	target_link_libraries(http_listener file_saving pistache pistache_static gcov pthread)
else ()
	target_link_libraries(http_listener file_saving pistache pistache_static pthread)
endif ()
target_compile_options(http_listener PRIVATE -Wall -Wextra -Werror -std=c++17 -march=native -flto)
add_dependencies(http_listener file_saving pistache pistache_static)