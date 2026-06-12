#pragma once
#include <exception>
#include <string>

namespace Asteroids{
    
class SDLInitializationException : public std::exception{
    std::string m_msg;
    public:
        SDLInitializationException() : m_msg{"SDL Initialization Error"}{};
        explicit SDLInitializationException(const std::string& message): m_msg{message}{}
        const char* what() const override {
            return m_msg.c_str();
        }
};

class SDLRendererException : std::exception{
    std::string m_msg;
    public:
        SDLRendererException() : m_msg{"Renderer Exception"}{};
        explicit SDLRendererException(const std::string& message): m_msg{message}{}
        const char* what() const override {
            return m_msg.c_str();
        }
};

}//end Asteroids
