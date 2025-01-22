#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"

#include <sstream>
#include <fstream>
#include <iostream>
#include <filesystem>

ResourceManager::ResourceManager(const std::string& executablePath)
{
    size_t found = executablePath.find_last_of("/\\");
    m_path = executablePath.substr(0, found);
}

/*std::string ResourceManager::getFileString(const std::string& relativeFilePath) const
{
    std::ifstream f;
    f.open(m_path + "/" + relativeFilePath.c_str(), std::ios::in | std::ios::binary);
    if(!f.is_open())
    {
        std::cerr << "Failed to open file: " << relativeFilePath << std::endl;
        std::cerr << "Attempting to load shader from: " << m_path + "/" + relativeFilePath << std::endl;
        return std::string{};
    }

    std::stringstream buffer;
    buffer << f.rdbuf();
    return buffer.str();
}
*/

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


/*std::shared_ptr<Renderer::ShaderProgram> ResourceManager::loadShaders(const std::string& shaderName, const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{ 

   std::shared_ptr<Renderer::ShaderProgram>& newShader = m_ShaderPrograms.emplace(shaderName, std::make_shared<Renderer::ShaderProgram>(vertexShaderPath, fragmentShaderPath)).first->second;
    if(newShader -> isCompiled())
    {
        return newShader;
    }
    else
    {
        std::cerr << "Can't load shader program:\n"
            << "Vertex: " << vertexShaderPath << "\n"
            << "Fragment: " << fragmentShaderPath << std::endl;

        return nullptr;
    }
}

*/

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
          std::cerr << "Can't find shaderprogram: " << shaderName << std::endl;  
          return nullptr;
        }

}