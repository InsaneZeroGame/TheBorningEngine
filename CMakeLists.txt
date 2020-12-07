cmake_minimum_required(VERSION 3.0.0)
project(TBE VERSION 0.1.0)

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")
include(cotire)

add_subdirectory(3rdparty)
include_directories(include)

file(GLOB_RECURSE source_files include/*.h src/*.cpp)

file(GLOB_RECURSE shader_files shaders/*.hlsl)


set(vs
   shaders/default_vs.hlsl
)

set(ps
    shaders/default_ps.hlsl
)



set_property(SOURCE ${ps} PROPERTY VS_SHADER_ENTRYPOINT main)
set_property(SOURCE ${ps} PROPERTY VS_SHADER_TYPE Pixel)
set_property(SOURCE ${ps} PROPERTY VS_SHADER_MODEL 5.0)
set_property(SOURCE ${ps} PROPERTY VS_SHADER_FLAGS "/Zi")

set_property(SOURCE ${vs} PROPERTY VS_SHADER_ENTRYPOINT main)
set_property(SOURCE ${vs} PROPERTY VS_SHADER_TYPE Vertex)
set_property(SOURCE ${vs} PROPERTY VS_SHADER_MODEL 5.0)
set_property(SOURCE ${vs} PROPERTY VS_SHADER_FLAGS "/Zi")

#set_property(SOURCE ${gs} PROPERTY VS_SHADER_ENTRYPOINT main)
#set_property(SOURCE ${gs} PROPERTY VS_SHADER_TYPE Geometry)
#set_property(SOURCE ${gs} PROPERTY VS_SHADER_MODEL 5.0)
#set_property(SOURCE ${gs} PROPERTY VS_SHADER_FLAGS "/Zi")

#set_property(SOURCE ${cs} PROPERTY VS_SHADER_ENTRYPOINT main)
#set_property(SOURCE ${cs} PROPERTY VS_SHADER_TYPE Compute)
#set_property(SOURCE ${cs} PROPERTY VS_SHADER_MODEL 5.0)
#set_property(SOURCE ${cs} PROPERTY VS_SHADER_FLAGS "/Zi")

source_group(Shaders FILES ${shader_files})

add_library(TBE STATIC ${source_files} ${shader_files})
target_link_libraries(TBE Cauldron_DX12 D3DCompiler)
set_target_properties(${PROJECT_NAME} PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "include/stdafx.h")
cotire(${PROJECT_NAME})

set(CPCK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
