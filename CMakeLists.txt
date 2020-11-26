cmake_minimum_required(VERSION 3.0.0)
project(TBE VERSION 0.1.0)

add_subdirectory(3rdparty)

file(GLOB_RECURSE source_files include/*.h src/*.cpp)

add_library(TBE STATIC ${source_files})

target_link_libraries(TBE Cauldron_DX12)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
