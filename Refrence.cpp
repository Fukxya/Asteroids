#include <SDL.h>
#include <SDL_image.h>
#include <cstdlib>
#include <iostream>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main(int, char**) {
    // SDL initialisieren. Dabei initialisieren wir die Subsysteme "Video" (für
    // die grafische Ausgabe) und "Timer" (zur Zeitmessung).
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << '\n';
        return EXIT_FAILURE;
    }

    // Ein neues Fenster erzeugen.
    SDL_Window* const window =
        SDL_CreateWindow(
            "Asteroids",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN
        );
    if (window == nullptr) {
        std::cerr << "Failed to create window: " << SDL_GetError() << '\n';
        // SDL beenden.
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // Renderer erzeugen. Dieser ist notwendig, um Grafiken in das Fenster zu zeichnen.
    SDL_Renderer* const renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << '\n';
        // Fenster zerstören/freigeben.
        SDL_DestroyWindow(window);

        // SDL beenden.
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // Grafik von der Festplatte in eine SDL_Texture laden.
    SDL_Texture* const ship_image = IMG_LoadTexture(renderer, "assets/ship01.png");
    if (ship_image == nullptr) {
        std::cerr << "Failed to load graphic.\n";

        // Renderer zerstören/freigeben.
        SDL_DestroyRenderer(renderer);

        // Fenster zerstören/freigeben.
        SDL_DestroyWindow(window);

        // SDL beenden.
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // Größe des geladenen Bilds abfragen.
    int ship_image_width;
    int ship_image_height;
    if (SDL_QueryTexture(ship_image, nullptr, nullptr, &ship_image_width, &ship_image_height) != 0) {
        std::cerr << "Failed to get image dimensions.\n";

        // Grafik freigeben.
        SDL_DestroyTexture(ship_image);

        // Renderer zerstören/freigeben.
        SDL_DestroyRenderer(renderer);

        // Fenster zerstören/freigeben.
        SDL_DestroyWindow(window);

        // SDL beenden.
        SDL_Quit();

        return EXIT_FAILURE;
    }

    // Game-Loop. In einer Schleife werden solange Events (z. B. Tastendrücke)
    // abgefragt, bis die Escape-Taste gedrückt wird oder das Fenster geschlossen
    // wird.
    // Tastatureingaben können auf zwei Arten erfasst werden:
    // 1. Beim Drücken und Loslassen entstehen Events, die wir untersuchen können.
    // 2. Wir können den aktuellen Zustand einer Taste erfragen (gedrückt/nicht gedrückt).
    bool running = true;
    while (running) {
        // Zeichen-Farbe auf schwarz ändern. Das müssten wir eigentlich nicht in jeder Schleifenwiederholung tun.
        if (SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255) != 0) {
            std::cerr << "Failed to set render color.\n";
            break;
        }
        // Fenster mit schwarzer Farbe füllen.
        if (SDL_RenderClear(renderer) != 0) {
            std::cerr << "Failed to clear screen.\n";
            break;
        }

        SDL_Event event;
        // Events abfragen:
        while (SDL_PollEvent(&event) == 1) {
            // Soll das Fenster geschlossen werden (SDL_QUIT)?
            // Wurde die Escape-Taste gedrückt?
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                running = false;
            }
        }

        // Aktuellen Zustand der Leertaste abfragen:
        const Uint8* const keys = SDL_GetKeyboardState(nullptr);
        if (keys[SDL_GetScancodeFromKey(SDLK_SPACE)] == 1) {
            // Leertaste wird gerade gedrückt => Raumschiff zeichnen.
            // Ziel-Rechteck, in das wir hineinzeichnen wollen:
            const auto target_rect = SDL_Rect{
                SCREEN_WIDTH / 2 - ship_image_width / 2,
                SCREEN_HEIGHT / 2 - ship_image_height / 2,
                ship_image_width,
                ship_image_height,
            };
            // Raumschiff-Textur auf den Bildschirm kopieren.
            SDL_RenderCopyEx(
                renderer,
                ship_image,
                nullptr,
                &target_rect,
                0.0, // Rotation in Grad
                nullptr,
                SDL_FLIP_NONE
            );
        }

        // Alles, was wir gezeichnet haben, anzeigen.
        SDL_RenderPresent(renderer);
    }

    // Grafik freigeben.
    SDL_DestroyTexture(ship_image);

    // Renderer zerstören/freigeben.
    SDL_DestroyRenderer(renderer);

    // Fenster zerstören/freigeben.
    SDL_DestroyWindow(window);

    // SDL beenden.
    SDL_Quit();
    return EXIT_SUCCESS;
}
