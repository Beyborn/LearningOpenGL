#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const unsigned int scrWidth {800};
const unsigned int scrHeight {600};
bool wireFrameMode {false};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window);
void processInputOnce(GLFWwindow* window, int key, int scancode, int action, int mods);


float mixValue {0.2f};

int main ()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Creating window using a function and assigning it to a variable
    GLFWwindow* window = glfwCreateWindow(scrWidth, scrHeight, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Glad manages function pointers for OpenGL so we want to make sure it is initialized
    // before we call any OpenGL functions 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    Shader ourShader ("/home/byron/Documents/LearningOpenGL/src/shaders/3.3.shader.vs", "/home/byron/Documents/LearningOpenGL/src/shaders/3.3.shader.fs"); 

    // stuff for creating textures

    //start of creating a texture
    unsigned int texture1 {};
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    int width {};
    int height {};
    int nrChannels{};

    //set the texture options (on the texure bond above)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_set_flip_vertically_on_load(true);
    unsigned char *data {stbi_load("/home/byron/Documents/LearningOpenGL/src/container.jpg", &width, &height, &nrChannels, 0)};
    if (data)
    {
        //glTexImage2D function explination
        // 1.Texture Target, 2.mipmap level, 3.what format, 4/5. self explanitory, 6.Always 0 (legacy), 7/8.specify format & datatype, 9.image data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load container texture" << std::endl;
    }
    stbi_image_free(data);

    unsigned int texture2 {};
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    //set the texture options (on the texure bond above)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_set_flip_vertically_on_load(true);
    data =  {stbi_load("/home/byron/Documents/LearningOpenGL/src/awesomeface.png", &width, &height, &nrChannels, 0)};
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load face texture" << std::endl;
    }
    stbi_image_free(data);

    //This is a float array with three vertices with each vertex having a 3D position.
    // X Y Z, Z is set to 0 as we are rendering a 2D triangle
    float vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    //Creating a vertex buffer object
    unsigned int VBO;
    //Just like any object in OpenGL, this buffer has a unique ID corresponding to that buffer, 
    // so we can generate one with a buffer ID using the glGenBuffers function
    glGenBuffers(1, &VBO);
    //Binding the newley created buffer to GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //Copying vertex data into buffer's memory
    // glBufferData(1. type of buffer, 2. size of the data, 3. actual data we want to send, 4. how we want the GPU to manage the data)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /*
    Instead of creating a seperate VBOs, you can create multiple at once:

    unsigned int VBOs[2], VAOs[2];
    glGenVertexArrays(2, VAOs); // we can also generate multiple VAOs or buffers at the same time
    glGenBuffers(2, VBOs);
open gl gl_position
    Then call it like an array:
    glBindVertexArray(VAOs[0]); etc
    */

    //Create VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //colour attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    //texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6* sizeof(float)));
    glEnableVertexAttribArray(2);

    //Create EBO
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    //Similar to VBO, just using GL_ELEMENT_ARRAY_BUFFER instead
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    /*
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertx attributes supported: " << nrAttributes << std::endl;
    */
    
    //Telling OpenGL which texture unt each shader smapler belongs to
    ourShader.use();
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0); //set manually
    ourShader.setInt("texture2", 1); //or via the function created in the shader class

    //This is the render loop to keep the window open
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glfwSetKeyCallback(window, processInputOnce);

        wireFrameMode ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //ourShader.horizontalOffset(0.5f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        ourShader.setFloat("mixValue", mixValue);
        
        ourShader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //This cleans/deletes all of GLFW's resources that were allocated
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void processInputOnce(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
    {
        wireFrameMode = !wireFrameMode;
    }

    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    {
        mixValue += 0.1f;
        if (mixValue >= 1)
        {
            mixValue = 1;
        }
    }

    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
        mixValue -= 0.1f;
        if (mixValue <= 0)
        {
            mixValue = 0;
        }
    }
}