#include <SDL/SDL.hpp>
#include <SDL.h>
#include <SDL_image.h>
#include <game.hpp>
#include <exceptions/sdlexceptions.hpp>
#include <string>
#include <iostream>
#include <SDL_video.h>

namespace Asteroids::SDL{
    //Sdl
    Sdl::Sdl(){
        //TODO: Make Fleixble with variadic functions for arbitary amount of flags
        if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0){
            //Throw custom 
            throw SDLInitializationException{"Failed to initialize SDL Context:" + std::string(SDL_GetError())};
        }

    }
    Sdl::~Sdl(){
        SDL_Quit();
    }
    //End Sdl
    //Window
    Window::Window(const char* name,  const int width, const int height): 
    m_sdl{},
    m_window_ptr{
        SDL_CreateWindow(
            name,
            SDL_WINDOWPOS_CENTERED, 
            SDL_WINDOWPOS_CENTERED,
            width,
            height,
            SDL_WINDOW_OPENGL
        )
    },
    m_gl_context{
        NULL
    }
    {
        
        if(m_window_ptr == nullptr){
            throw SDLInitializationException("Failed to create Window: " + std::string(SDL_GetError()));
        }
        m_gl_context = SDL_GL_CreateContext(m_window_ptr);
        if(m_gl_context == NULL){
            throw SDLInitializationException("Failed to create Open GL Context: " + std::string(SDL_GetError()));
        }
        
        
    }
    Window::~Window(){
        SDL_DestroyWindow(m_window_ptr);
        SDL_GL_DeleteContext(m_gl_context);
    }

    SDL_Window* Window::window_ptr()const{
        return m_window_ptr;
    }
    SDL_GLContext Window::gl_context() const{
        return m_gl_context;
    }
    //End Window
    //Renderer
    Renderer::Renderer(const Window& window) :
    m_renderer_ptr{
        SDL_CreateRenderer(window.window_ptr(), -1 , SDL_RENDERER_PRESENTVSYNC)}
    {
        if(m_renderer_ptr == nullptr){
            throw SDLInitializationException("Failed to create Renderer: " + std::string(SDL_GetError()));
        }
        if(SDL_GL_MakeCurrent(window.window_ptr(), window.gl_context()) != 0){
            throw SDLInitializationException("Failed to set Open GL context: " + std::string(SDL_GetError()));
        }
    }
    Renderer::~Renderer(){
        SDL_DestroyRenderer(m_renderer_ptr);
        
    }
    SDL_Renderer* Renderer::renderer_ptr() const{
        return m_renderer_ptr;
    }

    void Renderer::clear(const SDL_Color& color)const{
        
        if(SDL_SetRenderDrawColor(m_renderer_ptr,color.r, color.g, color.b, color.a) != 0){
            throw SDLRendererException{"Failed to set Renderer Color: " + std::string(SDL_GetError())};
        }

        if(SDL_RenderClear(m_renderer_ptr) != 0){
            throw SDLRendererException{"Failed to clear Renderer: " + std::string(SDL_GetError())};
        }
    }
    void Renderer::render(const Image& image, Vector2 position, double rotation_degrees)const{
            const auto rect = SDL_Rect{
                static_cast<int>(position.x) - image.width() / 2,
                static_cast<int>(position.y) - image.height() / 2,
                image.width(),
                image.height()    
            };

            SDL_RenderCopyEx(m_renderer_ptr, image.texture(),nullptr, &rect, rotation_degrees, nullptr, SDL_FLIP_NONE);
    }

    void Renderer::render(const Asteroids::Entity& entity) const{
        render(*(entity.sprite()), entity.position(), entity.rotation());
    }

    void Renderer::present() const{
        SDL_RenderPresent(m_renderer_ptr);
    }
    //End Renderer

    //Image 
    Image::Image(const Renderer& renderer, const char* path): 
    m_texture_ptr{IMG_LoadTexture(renderer.renderer_ptr(), path)}
    {
       if(m_texture_ptr == nullptr){
            //TODO: Throw Exception
            throw SDLRendererException{"Failed to Load Image: " + std::string(SDL_GetError())};
       }
       if(SDL_QueryTexture(m_texture_ptr, nullptr, nullptr, &m_width, &m_height) != 0){
            throw SDLRendererException{"Failed to get image dimensions: " + std::string(SDL_GetError())};
       }
    }

    const int Image::width() const{
        return m_width;
    }
    const int Image::height() const{
        return m_height;
    }
    SDL_Texture* Image::texture() const{
        return m_texture_ptr;
    }
    Image::~Image(){
        SDL_DestroyTexture(m_texture_ptr);
    }
    //End Image
    //Application
    Application::Application(const Window& window, Renderer& renderer): m_window{window}, m_renderer{renderer}, m_running{true}, m_last_ticks{0}{}

    void Application::run(){
        m_last_ticks = SDL_GetTicks64();
        while(m_running){
            SDL_Event event;
                while(SDL_PollEvent(&event) == 1){
                    if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                    m_running = false;
                }
            }
            auto current_ticks = SDL_GetTicks64();
            double deltaTime = static_cast<double>(current_ticks - m_last_ticks) / 1000.0; 
            update(deltaTime);
            m_renderer.clear(SDL_Color{5,5,5,255});
            render(m_renderer);
            m_renderer.present();
            m_last_ticks = current_ticks;

        }
    }

    bool Application::is_key_down(SDL_Keycode key) const{
        const Uint8* keyboard_state = SDL_GetKeyboardState(nullptr);
        return keyboard_state[SDL_GetScancodeFromKey(key)] != 0;
    }

    Image Application::load_image(const char* filepath)const{
        return Image{m_renderer, filepath};
    }
    void Application::quit(){
        m_running = false;
    }
    //End Application
}//End Asteroids SDL