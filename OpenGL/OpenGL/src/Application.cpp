#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader_s.h"
#include "Texture.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    {
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSwapInterval(1);

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return -1;
        }

        float vertices[] = {
            // positions          // colors           // texture coords
             0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
             0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        std::cout << glGetString(GL_VERSION) << std::endl;

        // 1. bind Vertex Array Object
        VertexArray va;
        VertexBufferLayout layout;
        
        // 2. copy our vertices array in a vertex buffer for OpenGL to use
        VertexBuffer vb(vertices, 8 * 4 * sizeof(float));
        vb.Bind();

        // 3. copy our index array in a element buffer for OpenGL to use
        IndexBuffer ib(indices, 6);
        ib.Bind();

        // 4. then set the vertex attributes pointers
        // position attribute
        layout.Push<float>(3);
        // color attribute
        layout.Push<float>(3);
        // texture attribute
        layout.Push<float>(2);
        va.Bind();

        // always do this after pushing to layout
        va.AddBuffer(vb, layout);

        // load and create a texture 
        // -------------------------
        Texture texture1("res/textures/container.jpg");
        Texture texture2("res/textures/awesomeface.png");
        
        texture1.Bind(0);
        texture2.Bind(1);

        // build and compile our shader zprogram
        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.setInt("texture1", 0);
        shader.setInt("texture2", 1);
        
        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;
        

        // render loop
        // -----------
        while (!glfwWindowShouldClose(window))
        {
            // input
            // -----
            processInput(window);

            // render
            // ------
            renderer.Clear();

            renderer.Draw(va, ib, shader);

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }   
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}