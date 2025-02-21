#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vec2.hpp>
#include <mat4x4.hpp>
#include <gtc/matrix_transform.hpp>

#include "Renderer/ShaderProgram.h"
#include "Resources/ResourceManager.h"
#include "Renderer/Texture2D.h"


GLfloat point[] = {
   0.0f, 50.f, 0.0f,  
   50.f, -50.f, 0.0f, 
   -50.f, -50.f, 0.0f 
};

GLfloat colour[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

GLfloat tex_coord[] = {
    0.5f, 1.0f, 
    1.0f, 0.0f, 
    0.0f, 0.0f
};

glm::ivec2 g_windowSize(640, 480);

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    g_windowSize.x = width;
    g_windowSize.y = height;
    glViewport(0, 0, width, height);

}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}

int main(int argc, char** argv)
{

    //Initialize the library 
    if (!glfwInit())
    {
        std::cout << "glfwInit failed" << std::endl;
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Create a windowed mode window and its OpenGL context 
    GLFWwindow* pWindow = glfwCreateWindow(g_windowSize.x, g_windowSize.y, "Battle", nullptr, nullptr);
    if (!pWindow)
    {
        std::cout << "pWindow failed" << std::endl;
        glfwTerminate();  
        return -1;
    }

    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);


    //Make the window's context current 
    glfwMakeContextCurrent(pWindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    {
    std::cerr << "Failed to initialize OpenGL loader!" << std::endl;
    return -1;
    }


    if(!gladLoadGL())
    {
        std::cout << "Can't load GLAD" << std::endl;
        return -1;
    }

    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    glClearColor(204 / 255.0f, 204 / 255.0f, 255 / 255.0f, 1.0f);

    

    {
        ResourceManager resourceManager(argv[0]);
        auto pDefaultShaderProgram = resourceManager.loadShaders("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
        if(!pDefaultShaderProgram)
        {
            std::cerr << "Can't create shader profram: " << "DefaultShader" << std::endl;
            return -1;
        }

        auto tex = resourceManager.loadTexture("DefaultTexture", "res/textures/map_16x16.png");

        GLuint points_vbo = 0;
        glGenBuffers(1, &points_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);

        GLuint colours_vbo = 0;
        glGenBuffers(1, &colours_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colour), colour, GL_STATIC_DRAW);

        GLuint vao = 0;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        GLuint tex_coord_vbo = 0;
        glGenBuffers(1, &tex_coord_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, tex_coord_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coord), tex_coord, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, tex_coord_vbo);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr); 

        pDefaultShaderProgram->use();
        pDefaultShaderProgram->setInt("tex", 0);

        glm::mat4 modelMatrix_1 = glm::mat4(1.f);
        modelMatrix_1 = glm::translate(modelMatrix_1, glm::vec3(100.f, 50.f, 0.f));

        glm::mat4 modelMatrix_2 = glm::mat4(1.f);
        modelMatrix_2 = glm::translate(modelMatrix_2, glm::vec3(590.f, 50.f, 0.f));

        glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(g_windowSize.x), 0.f, static_cast<float>(g_windowSize.y), -100.f, 100.f);

        pDefaultShaderProgram->setMatrix4("projectionMat", projectionMatrix);

        // Loop until the user closes the window 
        while (!glfwWindowShouldClose(pWindow))
        {
            // Render here 
            glClear(GL_COLOR_BUFFER_BIT);
            
            pDefaultShaderProgram->use();
            glBindVertexArray(vao);
            tex->bind();

            pDefaultShaderProgram->setMatrix4("modelMat", modelMatrix_1);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            pDefaultShaderProgram->setMatrix4("modelMat", modelMatrix_2);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            GLenum error = glGetError();
            if (error != GL_NO_ERROR)
            {
                std::cerr << "OpenGL error: " << error << std::endl;
            }

            // Swap front and back buffers 
            glfwSwapBuffers(pWindow);

            // Poll for and process events 
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
} 



