#ifndef RESOURCES_H
#define RESOURCES_H

#include <map>
#include <string>

#include "GL/glew.h"

#include "Texture2D.h"
#include "Shader.h"

class Resources
{
public:
    static std::map<std::string, Shader> Shaders;
    static std::map<std::string, Texture2D> Textures;

    static Shader LoadShader(const GLchar* vShaderPath, const GLchar* fShaderPath, std::string name);
    static Shader GetShader(std::string name);

    static Texture2D LoadTexture(const GLchar* file, GLboolean alpha, std::string name);
    static Texture2D GetTexture(std::string name);

    static void Clear();
private:
    Resources() {}
    static Shader loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile);
    static Texture2D loadTextureFromFile(const GLchar *file, GLboolean alpha);
};

#endif
