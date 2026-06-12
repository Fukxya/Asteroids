include("${CMAKE_SOURCE_DIR}/cmake/CPM.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/system_link.cmake")

function(asteroids_setup_dependencies)
    CPMAddPackage(
        NAME SDL2
        GITHUB_REPOSITORY libsdl-org/SDL
        GIT_TAG c98c4fbff6d8f3016a3ce6685bf8f43433c3efcc
        OPTIONS
        "SDL_TEST_LIBRARY OFF"
        "SDL_SHARED OFF"
        "SDL_STATIC ON"
    )
    CPMAddPackage(
        NAME SDL2_Image
        GITHUB_REPOSITORY libsdl-org/SDL_Image
        GIT_TAG abcf63aa71b4e3ac32120fa9870a6500ddcdcc89
        OPTIONS
        "SDL2IMAGE_BUILD_SHARED_LIBS OFF"
    )
endfunction()
