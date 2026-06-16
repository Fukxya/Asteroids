#pragma once
#include <glad/glad.h>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <math/vector.hpp>


namespace Asteroids{
    namespace OPGL{

        class Shader{
            private:
            public:
                unsigned int ID;
                Shader(const char* vertex_path, const char* fragment_path);

                void use();

                void setBool(const std::string &name, bool value);
                void setInt(const std::string &name, int value);
                void setFloat(const std::string &name,float value);
        };

        class Texture2D;

        class SpriteRenderer{
            private:
               Shader m_shader;
               unsigned int quadVAO;
               void initRenderData();
            public:
                SpriteRenderer(Shader &shader);
                ~SpriteRenderer();
                void DrawSprite(const Texture2D& texture, Vector2 position, Vector2 size, float rotation);
        };
        
        
    }
}