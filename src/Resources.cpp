#include "Resources.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include <SOIL/SOIL.h>

std::map<std::string, Shader> Resources::Shaders;
std::map<std::string, Texture2D> Resources::Textures;

Shader Resources::LoadShader(const GLchar *vShaderPath, const GLchar *fShaderPath, std::string name)
{
    Shaders[name] = loadShaderFromFile(vShaderPath, fShaderPath);
    return Shaders[name];
}

Shader Resources::GetShader(std::string name)
{
    return Shaders[name];
}

Texture2D Resources::LoadTexture(const GLchar* file, GLboolean alpha, std::string name)
{
    Textures[name] = loadTextureFromFile(file, alpha);
    return Textures[name];
}

Texture2D Resources::GetTexture(std::string name)
{
    return Textures[name];
}

void Resources::Clear()
{
    //for(auto iter : Shaders)
    //    glDeleteProgram(iter.second.ID);
}

Shader Resources::loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile)
{
    std::string vertexCode;
    std::string fragmentCode;
    try
    {
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();

        vertexShaderFile.close();
        fragmentShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const GLchar *vShaderCode = vertexCode.c_str();
    const GLchar *fShaderCode = fragmentCode.c_str();
    
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode);
    return shader;
}

Texture2D Resources::loadTextureFromFile(const GLchar *file, GLboolean alpha)
{
    Texture2D texture;
    if (alpha)
    {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }

    int width, height;
    unsigned char* image = SOIL_load_image(file, &width, &height, 0, texture.Image_Format == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);

    texture.Generate(width, height, image);

    SOIL_free_image_data(image);
    return texture;
}
