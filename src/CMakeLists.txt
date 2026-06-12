
add_executable(asteroids
    main.cpp
    SDL/SDL.cpp
    game/game.cpp
)
target_include_directories(asteroids PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_dependencies(asteroids copy_assets)

target_link_system_libraries(asteroids PRIVATE SDL2::SDL2 SDL2::SDL2main SDL2_image::SDL2_image-static)
