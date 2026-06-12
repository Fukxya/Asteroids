#include <SDL.h>
#include <SDL_image.h>
#include <SDL/SDL.hpp> // SDL Wrapping Classes
#include <math/vector.hpp>
#include <cstdlib>
#include <iostream>
#include <exception>
#include <game.hpp>



int main(int, char**) {
    // SDL initialisieren. Dabei initialisieren wir die Subsysteme "Video" (für
    // die grafische Ausgabe) und "Timer" (zur Zeitmessung).
   
    const auto window = Asteroids::SDL::Window{
        "Asteroids",
        SCREEN_WIDTH,
        SCREEN_HEIGHT
    };
    
    auto renderer = Asteroids::SDL::Renderer{window};
    auto game = Asteroids::Game{window, renderer};
    game.run();
   
    return EXIT_SUCCESS;
    
}
