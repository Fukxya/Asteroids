include(${PROJECT_SOURCE_DIR}/cmake/warnings.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/sanitizers.cmake)

# the following function was taken from:
# https://github.com/cpp-best-practices/cmake_template/blob/main/ProjectOptions.cmake
macro(check_sanitizer_support)
    if ((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)
        set(supports_ubsan ON)
    else ()
        set(supports_ubsan OFF)
    endif ()

    if ((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
        set(supports_asan OFF)
    else ()
        set(supports_asan ON)
    endif ()
endmacro()

if (PROJECT_IS_TOP_LEVEL)
    option(asteroids_warnings_as_errors "Treat warnings as errors" ON)
    option(asteroids_enable_undefined_behavior_sanitizer "Enable undefined behavior sanitizer" ${supports_ubsan})
    option(asteroids_enable_address_sanitizer "Enable address sanitizer" ${supports_asan})
else ()
    option(asteroids_warnings_as_errors "Treat warnings as errors" OFF)
    option(asteroids_enable_undefined_behavior_sanitizer "Enable undefined behavior sanitizer" OFF)
    option(asteroids_enable_address_sanitizer "Enable address sanitizer" OFF)
endif ()
option(asteroids_build_shared_libs "Build shared libraries instead of static libraries" ON)
set(BUILD_SHARED_LIBS ${asteroids_build_shared_libs})

add_library(asteroids_warnings INTERFACE)
asteroids_set_warnings(asteroids_warnings ${asteroids_warnings_as_errors})

add_library(asteroids_sanitizers INTERFACE)
asteroids_enable_sanitizers(asteroids_sanitizers ${asteroids_enable_address_sanitizer} ${asteroids_enable_undefined_behavior_sanitizer})

add_library(asteroids_project_options INTERFACE)
target_link_libraries(asteroids_project_options
        INTERFACE asteroids_warnings
        INTERFACE asteroids_sanitizers
)
