cmake_minimum_required (VERSION 3.16)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(PROJECT_NAME "profanityd")

project(${PROJECT_NAME})
aux_source_directory("src" coresrc)
add_executable(${PROJECT_NAME} ${coresrc})

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
)

set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

set(CMAKE_CXX_FLAGS "-g -O2 -Wall -Wno-unused-private-field -Wno-psabi -Wempty-body -Wignored-qualifiers -Wimplicit-fallthrough -Wmissing-field-initializers -Wsign-compare -Wtype-limits -Wuninitialized -Wshift-negative-value")
target_compile_definitions(${PROJECT_NAME} PUBLIC SPDLOG_FMT_EXTERNAL=ON)

target_include_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
	/usr/include/jsoncpp
	${DPP_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
	drogon
	jsoncpp
	trantor
	z
	uuid
	ssl
	crypto
	fmt
	spdlog
	${CMAKE_THREAD_LIBS_INIT}
)
