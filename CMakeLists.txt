cmake_minimum_required(VERSION 3.15)
project(asteroids)

set(BUILD_SHARED_LIBS OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(dependencies.cmake)
asteroids_setup_dependencies()

include(project_options.cmake)

add_subdirectory(src bin)

set(ASSETS_SOURCE_DIR ${CMAKE_SOURCE_DIR}/assets)
set(ASSETS_BINARY_DIR ${CMAKE_BINARY_DIR}/bin/assets)

add_custom_command(
    OUTPUT ${ASSETS_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${ASSETS_SOURCE_DIR} ${ASSETS_BINARY_DIR}
    DEPENDS ${ASSETS_SOURCE_DIR}
    COMMENT "Copying assets to binary directory"
)

add_custom_target(copy_assets ALL DEPENDS ${ASSETS_BINARY_DIR})
