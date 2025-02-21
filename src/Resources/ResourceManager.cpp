#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"

#include <sstream>
#include <fstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

#include <filesystem>

ResourceManager::ResourceManager(const std::string& executablePath)
{
    size_t found = executablePath.find_last_of("/\\");
    m_path = executablePath.substr(0, found);
}

std::string ResourceManager::getFileString(const std::string& relativeFilePath) const
{
    std::ifstream file(m_path + "/" + relativeFilePath, std::ios::in);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << relativeFilePath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}


std::shared_ptr<Renderer::ShaderProgram> ResourceManager::loadShaders(const std::string& shaderName, const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
    std::string vertexSource = getFileString(vertexShaderPath);
    std::string fragmentSource = getFileString(fragmentShaderPath);

    if (vertexSource.empty() || fragmentSource.empty()) {
        std::cerr << "Failed to load shaders: " << vertexShaderPath << ", " << fragmentShaderPath << std::endl;
        return nullptr;
    }

    // auto shader = std::make_shared<Renderer::ShaderProgram>(vertexSource, fragmentSource);
    // if (!shader->isCompiled()) {
    //     std::cerr << "Shader program compilation failed: " << shaderName << std::endl;
    //     return nullptr;
    // }

    // m_ShaderPrograms[shaderName] = shader;
    // return shader;

    auto shader = std::make_shared<Renderer::ShaderProgram>(vertexSource, fragmentSource);

    if (shader->isCompiled()) 
    {
        // Добавляем шейдер в контейнер только если компиляция успешна
        auto [it, inserted] = m_ShaderPrograms.emplace(shaderName, shader);
        if (!inserted) {
            std::cerr << "Shader program with name \"" << shaderName << "\" already exists in the container." << std::endl;
            return nullptr;
        }
        return it->second;
    } else 
    {
        // Если компиляция неуспешна, выводим ошибку
        std::cerr << "Shader program compilation failed: " << shaderName << std::endl;
        return nullptr;
    }
}



std::shared_ptr<Renderer::ShaderProgram> ResourceManager::getShaderProgram(const std::string& shaderName)
{
        ShaderProgramsMap::const_iterator it = m_ShaderPrograms.find(shaderName);
        if(it != m_ShaderPrograms.end())
        {
            return it->second;
        }
        else
        {
          std::cerr << "Can't find shader program: " << shaderName << std::endl;  
          return nullptr;
        }

}

std::shared_ptr<Renderer::Texture2D> ResourceManager::loadTexture(const std::string& textureName, const std::string& texturePath)
{
    int channels{0};
    int width {0};
    int height {0};

    stbi_set_flip_vertically_on_load(true);
    unsigned char* pixels = stbi_load(std::string(m_path + "/" + texturePath).c_str(), &width, &height, &channels, 0);
    if(!pixels)
    {
        std::cerr << "ERROR::TEXTURES:: Can't load image " << texturePath << std::endl;
        return nullptr;
    }

    std::shared_ptr<Renderer::Texture2D> newTexture = m_textures.emplace(textureName, std::make_shared<Renderer::Texture2D>(width,
                                                                                                                            height,
                                                                                                                            pixels,
                                                                                                                            channels, 
                                                                                                                            GL_NEAREST, 
                                                                                                                            GL_CLAMP_TO_EDGE)).first->second;

    stbi_image_free(pixels);

    return newTexture;
}

std::shared_ptr<Renderer::Texture2D> ResourceManager::getTexture(const std::string& textureName)
{
    TexturesMap::const_iterator it = m_textures.find(textureName);
        if(it != m_textures.end())
        {
            return it->second;
        }
        else
        {
          std::cerr << "Can't find texture: " << textureName << std::endl;  
          return nullptr;
        }




}