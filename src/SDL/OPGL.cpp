
#include <SDL/OPGL.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>

namespace Asteroids
{
    namespace OPGL
    {
        Shader::Shader(const char *vertex_path, const char *fragment_path)
        {
            std::string vertexCode;
            std::string fragmentCode;
            std::ifstream vShaderFile;
            std::ifstream fShaderFile;

            vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

            try
            {
                vShaderFile.open(vertex_path);
                fShaderFile.open(fragment_path);
                std::stringstream vShaderStream, fShaderStream;

                vShaderStream << vShaderFile.rdbuf();
                fShaderStream << fShaderFile.rdbuf();

                vShaderFile.close();
                fShaderFile.close();

                vertexCode = vShaderStream.str();
                fragmentCode = fShaderStream.str();
            }
            catch (std::ifstream::failure e)
            {
                std::cout << "ERROR::SHADER::FILE_NOT_SUCCCESSFULLY_READ" << std::endl;
            }

            const char *vShaderCode = vertexCode.c_str();
            const char *fShaderCode = vertexCode.c_str();

            GLuint vertex, fragment;
            int success;
            char infoLog[512];

            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            glCompileShader(vertex);
            glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(vertex, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                          << infoLog << std::endl;
            };

            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, NULL);
            glCompileShader(fragment);
            glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(vertex, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                          << infoLog << std::endl;
            };

            ID = glCreateProgram();
            glAttachShader(ID, vertex);
            glAttachShader(ID, fragment);
            glLinkProgram(ID);

            glGetProgramiv(ID, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(ID, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                          << infoLog << std::endl;
            }

            glDeleteShader(vertex);
            glDeleteShader(fragment);
        }

        void Shader::use()
        {
            glUseProgram(ID);
        }

        void Shader::setBool(const std::string &name, bool value)
        {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
        }

        void Shader::setInt(const std::string &name, int value)
        {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        }

        void Shader::setFloat(const std::string &name, float value)
        {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        }
        // End Shader
        // SpriteRenderer

        void SpriteRenderer::initRenderData()
        {
            unsigned int VBO;
            float vertices[] = {
                // pos      // tex
                0.0f, 1.0f, 0.0f, 1.0f,
                1.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f,

                0.0f, 1.0f, 0.0f, 1.0f,
                1.0f, 1.0f, 1.0f, 1.0f,
                1.0f, 0.0f, 1.0f, 0.0f};
            
                glGenVertexArrays(1, &this->quadVAO);
                glGenBuffers(1, &VBO);

                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

                glBindVertexArray(this->quadVAO);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0,5,GL_FLOAT,GL_FALSE, 4* sizeof(float), (void*) 0);
                glBindBuffer(GL_ARRAY_BUFFER,0);
                glBindVertexArray(0);

        }

        void SpriteRenderer::DrawSprite(const Texture2D& texture, Vector2 position, Vector2 size, float rotation){
            this->m_shader.use();
            
        }
    }

}