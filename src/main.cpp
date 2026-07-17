#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const unsigned int scrWidth {800};
const unsigned int scrHeight {600};
bool wireFrameMode {false};

//C source code for a vertex shader
const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

//C source code for fragment shader colour
const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColour;\n"
    "void main ()\n"
    "{\n"
    "   FragColour = vec4(1.0f, 0.5f, 0.2f, 1.0f);"
    "}\0";

const char* fragmentShaderSourceYellow = "#version 330 core\n"
    "out vec4 FragColour;\n"
    "void main ()\n"
    "{\n"
    "   FragColour = vec4(1.0f, 1.0f, 0.0f, 1.0f);"
    "}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
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
}

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

    //Glad manages function pointers for OpenGL so we want to make sure it is initialized
    // before we call any OpenGL functions 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //This is a float array with three vertices with each vertex having a 3D position.
    // X Y Z, Z is set to 0 as we are rendering a 2D triangle
    float triangleOne[] = {
    -1.0f, -0.5f, 0.0f, //bottom left
     0.0f, -0.5f, 0.0f, //bottom right
    -0.5f,  0.5f, 0.0f, // top
    };

    float triangleTwo[] = {
     0.0f, -0.5f, 0.0f, //bottom left
     1.0f, -0.5f, 0.0f, //bottom right
     0.5f,  0.5f, 0.0f, // top
    };

    unsigned int indices[] = {
        0, 1, 2, // first triangle
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleOne), triangleOne, GL_STATIC_DRAW);

    /*
    Instead of creating a seperate VBO like this, you can create multiple at once:

    unsigned int VBOs[2], VAOs[2];
    glGenVertexArrays(2, VAOs); // we can also generate multiple VAOs or buffers at the same time
    glGenBuffers(2, VBOs);

    Then call it like an array:
    glBindVertexArray(VAOs[0]); etc
    */
    unsigned int VBO2;
    glGenBuffers(1, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleTwo), triangleTwo, GL_STATIC_DRAW);

    //Creating a shader object and giving it an ID
    unsigned int vertexShader{glCreateShader(GL_VERTEX_SHADER)};
    //Attaching the shader source code to the shader object and compling the shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    //checking if vertex shader compilation was successfull
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //This works the same way as the vertex shader above
    unsigned int fragmentShader {glCreateShader(GL_FRAGMENT_SHADER)};
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    //checking if fragment shader complilation was successfull
    // TO DO - Create 1 helper function that can be used for both fragment and vertex
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //Creating program object
    unsigned int shaderProgram{glCreateProgram()};
    //Attach and link previously created shaders to program object
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //checking if shader program linked
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
    }
    //After linking, the program contains its own copy of the compiled shader code, 
    // so the individual shader objects can be deleted.
    glDeleteShader(fragmentShader);

    unsigned int fragmentShaderYellow {glCreateShader(GL_FRAGMENT_SHADER)};
    glShaderSource(fragmentShaderYellow, 1, &fragmentShaderSourceYellow, NULL);
    glCompileShader(fragmentShaderYellow);

    
    unsigned int shaderProgram2{glCreateProgram()};
    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram2, fragmentShaderYellow);
    glLinkProgram(shaderProgram2);

    glDeleteShader(fragmentShaderYellow);
    glDeleteShader(vertexShader);

    //Create VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleOne), triangleOne, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    unsigned int VAO2;
    glGenVertexArrays(1, &VAO2);
    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleTwo), triangleTwo, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //CTRL+F on "https://learnopengl.com/Getting-started/Hello-Triangle" to explain this function

    //Create EBO
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    //Similar to VBO, just using GL_ELEMENT_ARRAY_BUFFER instead
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //This is the render loop to keep the window open
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glfwSetKeyCallback(window, processInputOnce);

        wireFrameMode ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgram2);

        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //This cleans/deletes all of GLFW's resources that were allocated
    glfwTerminate();
    return 0;
}