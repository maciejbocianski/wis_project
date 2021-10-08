cmake_minimum_required(VERSION 3.12)
project(proto)


set(CMAKE_CXX_STANDARD 11)

option(ENABLE_RTTI "add rtti flag")
option(ENABLE_EXCEPTIONS "add exceptions flag")

set(PROTO_SOURCES
        ../proto.h
        ../proto.cpp
)

# Append abs path to PROTO_SOURCES
set(PROTO_SOURCES_ABS_PATH ${PROTO_SOURCES})
list(TRANSFORM PROTO_SOURCES_ABS_PATH PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")

add_library(proto
        ${PROTO_SOURCES}
        )

target_include_directories(proto PUBLIC .)
target_include_directories(proto PUBLIC ./..)

if(ENABLE_EXCEPTIONS)
	target_compile_options(proto PUBLIC -fexceptions)
else()
	target_compile_options(proto PUBLIC -fno-exceptions)
endif()
if (ENABLE_RTTI)
	target_compile_options(proto PUBLIC -frtti)
else()
	target_compile_options(proto PUBLIC -fno-rtti)
endif()

target_compile_definitions(proto PUBLIC YOTTA_CFG PROTO_UNITTESTS)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif ()

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(test_app
        proto_test.cpp)

include_directories(test_app ${CMAKE_SOURCE_DIR})

target_link_libraries(test_app gmock_main)
target_link_libraries(test_app proto)


add_test(NAME proto_test COMMAND test_app)
