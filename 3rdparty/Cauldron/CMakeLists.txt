cmake_minimum_required(VERSION 3.4)
set(CMAKE_GENERATOR_PLATFORM x64)

project (Cauldron)

list( APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} )

# output exe to bin directory
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_HOME_DIRECTORY}/bin )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

# reference libs used by both backends
add_subdirectory(src/Common)
add_subdirectory(libs/json)
add_subdirectory(libs/ImGUI)
add_subdirectory(libs/stb)
add_subdirectory(libs/DXC)
add_subdirectory(libs/d3d12x)
add_subdirectory(libs/ags)    
add_subdirectory(src/DX12)   


