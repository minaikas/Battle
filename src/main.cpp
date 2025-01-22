#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Renderer/ShaderProgram.h"
#include "Resources/ResourceManager.h"

GLfloat point[] = {
   0.0f, 0.5f, 0.0f,  
   0.5f, -0.5f, 0.0f, 
   -0.5f, -0.5f, 0.0f 
};

GLfloat colour[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

// const char* vertex_shader = 
// "#version 460\n"
// "layout(location = 0) in vec3 vertex_position;"
// "layout(location = 1) in vec3 vertex_colour;"
// "out vec3 colour;"
// "void main(){"
// "   colour = vertex_colour;"
// "   gl_Position = vec4(vertex_position, 1.0);"
// "}";

// const char* fragment_shader = 
// "#version 460\n"
// "in vec3 colour;"
// "out vec4 frag_colour;"
// "void main() {"
// "   frag_colour = vec4(colour, 1.0);"
// "}";

int g_windowSizeX = 640;
int g_windowSizeY = 480;



void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    g_windowSizeX = width;
    g_windowSizeY = height;
    glViewport(0, 0, g_windowSizeX, g_windowSizeY);

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
    GLFWwindow* pWindow = glfwCreateWindow(g_windowSizeX, g_windowSizeY, "Battle", nullptr, nullptr);
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

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        // Loop until the user closes the window 
        while (!glfwWindowShouldClose(pWindow))
        {
            // Render here 
            glClear(GL_COLOR_BUFFER_BIT);
            
            pDefaultShaderProgram->use();
            glBindVertexArray(vao);
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

// int main(int argc, char** argv) 
// {
//     if (!glfwInit()) {
//         std::cerr << "Failed to initialize GLFW" << std::endl;
//         return -1;
//     }

//     GLFWwindow* window = glfwCreateWindow(g_windowSizeX, g_windowSizeY, "OpenGL Window", nullptr, nullptr);
//     if (!window) {
//         std::cerr << "Failed to create GLFW window" << std::endl;
//         glfwTerminate();
//         return -1;
//     }

//     glfwMakeContextCurrent(window);
//     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
//         std::cerr << "Failed to initialize GLAD" << std::endl;
//         return -1;
//     }

//     ResourceManager resourceManager(argv[0]);
//     auto shaderProgram = resourceManager.loadShaders("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
//     if (!shaderProgram) {
//         std::cerr << "Failed to load shaders" << std::endl;
//         return -1;
//     }

//     shaderProgram->use();

//     // Основной цикл
//     while (!glfwWindowShouldClose(window)) {
//         glClear(GL_COLOR_BUFFER_BIT);

//         // Рендеринг

//         glfwSwapBuffers(window);
//         glfwPollEvents();
//     }

//     glfwTerminate();
//     return 0;
// }

