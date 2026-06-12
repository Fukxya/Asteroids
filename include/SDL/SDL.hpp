#pragma once
#include <SDL.h>
#include <cstdarg>
#include <cstdlib>
#include <exceptions/sdlexceptions.hpp>
#include <iostream>
#include <math/vector.hpp>

namespace Asteroids{
    class Entity;
    namespace SDL{
        class Image;
           
        class Sdl{
            public:
                explicit Sdl();
                Sdl(const Sdl&) = delete;
                Sdl& operator=(const Sdl&) = delete;
                ~Sdl();
        };

        class Window{
            private:
                const Sdl m_sdl;
                SDL_Window* m_window_ptr;
            public:
                Window(const char* name,  const int width, const int height);
                Window(const Window&) = delete;
                Window& operator=(const Window&) = delete;
                ~Window();

                SDL_Window* window_ptr() const;
              
        };

        class Renderer{
            private:
                SDL_Renderer* m_renderer_ptr;
            public:
                explicit Renderer(const Window& window);
                Renderer(const Renderer&) = delete;
                Renderer& operator=(const Renderer&) = delete;
                ~Renderer();
                
                SDL_Renderer* renderer_ptr() const;
                void render(const Image& image, Vector2 position, double rotation_degrees)const;
                void render(const Entity& entity)const;
                void clear(const SDL_Color& color) const;
                void present()const;
        };

        class Image{
            private:
                int m_width;
                int m_height;
                SDL_Texture* m_texture_ptr;
            public:
                Image(const Renderer& renderer, const char* path);
                Image(const Image&) = delete;
                Image& operator=(const Image&) = delete;
                ~Image();
                //Move Operator for the Load function
                Image(Image&& other) noexcept : 
                m_texture_ptr{other.m_texture_ptr},
                m_width{other.m_width},
                m_height{other.m_height}{
                    other.m_texture_ptr = nullptr;
                }

                SDL_Texture* texture() const;
                const int height() const; 
                const int width() const;
        };

        class Application{
            private: 
                const Window& m_window;
                Renderer& m_renderer;
                bool m_running;
                Uint64 m_last_ticks; 
            public:
                Application(const Window& window, Renderer& renderer);
                Application(const Application&) = delete;
                Application& operator=(const Application&) = delete;
                virtual ~Application() = default;

                virtual void update(double delta_seconds) = 0;
                virtual void render(Renderer& renderer) const = 0;
                void run();
            protected:
                bool is_key_down(SDL_Keycode key)const;
                Image load_image(const char* filepath)const;
                void quit();

        };

    }//End SDL  
    
}//endAsteroids