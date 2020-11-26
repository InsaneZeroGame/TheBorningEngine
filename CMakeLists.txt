cmake_minimum_required(VERSION 3.0.0)
project(TBE VERSION 0.1.0)

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")
include(cotire)

add_subdirectory(3rdparty)
include_directories(3rdparty)
include_directories(include)

file(GLOB_RECURSE source_files include/*.h src/*.cpp)

add_library(TBE STATIC ${source_files})
target_link_libraries(TBE Cauldron_DX12)
set_target_properties(${PROJECT_NAME} PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "include/stdafx.h")
cotire(${PROJECT_NAME})

set(CPCK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
