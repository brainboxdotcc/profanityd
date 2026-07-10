cmake_minimum_required(VERSION 3.16)

project(profanityd LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

aux_source_directory(src coresrc)
add_executable(${PROJECT_NAME} ${coresrc})

find_package(Threads REQUIRED)
find_package(Drogon CONFIG REQUIRED)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        SPDLOG_FMT_EXTERNAL=ON
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        /usr/include/jsoncpp
)

target_compile_options(${PROJECT_NAME}
    PRIVATE
        -g
        -O2
        -Wall
        -Wno-unused-private-field
        -Wno-psabi
        -Wempty-body
        -Wignored-qualifiers
        -Wimplicit-fallthrough
        -Wmissing-field-initializers
        -Wsign-compare
        -Wtype-limits
        -Wuninitialized
        -Wshift-negative-value
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Drogon::Drogon
        Threads::Threads
)

# GCC < 9 requires a separate filesystem library.
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU"
    AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9)
    target_link_libraries(${PROJECT_NAME} PRIVATE stdc++fs)
endif()
