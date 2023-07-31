#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp> 
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <iostream>
#include <vector>

#define M_PI 3.14159265358979323846
#define N_FANTASMAS 1

std::vector<float> verticesPacman;
std::vector<unsigned int> indicesPacman;

std::vector <std::vector<float> >verticesFantasmasEsfera;
std::vector <std::vector<unsigned int> >indicesFantasmasEsfera;

std::vector <std::vector<float> >verticesFantasmasCubo;
std::vector <std::vector<unsigned int> >indicesFantasmasCubo;

// Variables para la rotacion
float rotationSpeed = 0.1f;
float rotationAngle = 0.0f;

// Variables para el movimiento
float movementSpeed = 0.001f;
float xPos = 0.0f;
float yPos = 0.0f;

// Variables para la escala
float scaleSpeed = 0.001f;
float scale = 1.0f;


void generarIndicesSphere(int sectorCount, int stackCount, std::vector<unsigned int>& indices) {
    int k1, k2;
    for (int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);   // índice inicial de la pila actual
        k2 = k1 + sectorCount + 1;    // índice inicial de la siguiente pila

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // Triángulo 1
            indices.push_back(k1);
            indices.push_back(k1 + 1);
            indices.push_back(k2);

            // Triángulo 2
            indices.push_back(k1 + 1);
            indices.push_back(k2 + 1);
            indices.push_back(k2);
        }
    }
}

void buildSphere(float radius, int sectorCount, int stackCount, std::vector<float>& vertices, std::vector<unsigned int>& indices) {
    // Generar vértices
    float x, y, z, xy;
    float sectorStep = 2 * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount; ++i)
    {
        stackAngle = M_PI / 2 - i * stackStep;  // [pi/2, -pi/2]
        xy = radius * cosf(stackAngle);         // r * cos(u)
        z = radius * sinf(stackAngle);          // r * sin(u)

        for (int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;       // [0, 2pi]
            x = xy * cosf(sectorAngle);         // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);         // r * cos(u) * sin(v)

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            vertices.push_back(1.0);
        }
    }

    // Generar índices para los triángulos
    generarIndicesSphere(sectorCount, stackCount, indices);
}

void buildFantasma() {
    std::vector<float> verticesFantasmaEsfera;
    std::vector<unsigned int> indicesFantasmaEsfera;
    const float radius = 0.5f;
    const int sectorCount = 36;
    const int stackCount = 18;

    // Construir esphera
    buildSphere(radius, sectorCount, stackCount, verticesFantasmaEsfera, indicesFantasmaEsfera);

    verticesFantasmasEsfera.push_back(verticesFantasmaEsfera);
    indicesFantasmasEsfera.push_back(indicesFantasmaEsfera);

    std::vector<float> verticesFantasmaCubo;
    std::vector<unsigned int> indicesFantasmaCubo;

    //Esta fallando el cubo de abajo :c
    float vertices[] = {
        // Front face
        -0.5f, -0.5f, 0.5f, 1.0f,   // Bottom Left
        0.5f, -0.5f, 0.5f, 1.0f,    // Bottom Right
        0.5f, 0.0f, 0.5f, 1.0f,     // Top Right 
        -0.5f, 0.0f, 0.5f, 1.0f,    // Top Left 

        // Back face
        -0.5f, -0.5f, -0.5f, 1.0f,  // Bottom Left
        0.5f, -0.5f, -0.5f, 1.0f,   // Bottom Right
        0.5f, 0.0f, -0.5f, 1.0f,    // Top Right 
        -0.5f, 0.0f, -0.5f, 1.0f,   // Top Left 

        // Right face
        0.5f, -0.5f, 0.5f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f,
        0.5f, 0.0f, -0.5f, 1.0f,
        0.5f, 0.0f, 0.5f, 1.0f,

        // Left face
        -0.5f, -0.5f, -0.5f, 1.0f,
        -0.5f, -0.5f, 0.5f, 1.0f,
        -0.5f, 0.0f, 0.5f, 1.0f,
        -0.5f, 0.0f, -0.5f, 1.0f,

        // Bottom face
        -0.5f, -0.5f, -0.5f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f,
        -0.5f, -0.5f, 0.5f, 1.0f,

        // Top face
        -0.5f, 0.0f, 0.5f, 1.0f,    // Top Left 
        0.5f, 0.0f, 0.5f, 1.0f,     // Top Right 
        0.5f, 0.0f, -0.5f, 1.0f,    // Bottom Right
        -0.5f, 0.0f, -0.5f, 1.0f,   // Bottom Left
    };

    unsigned int indices[] = {
        0, 1, 2,  // Front face
        2, 3, 0,
        4, 5, 6,  // Right face
        6, 7, 4,
        8, 9, 10,  // Back face
        10, 11, 8,
        12, 13, 14,  // Left face
        14, 15, 12,
        16, 17, 18,  // Bottom face
        18, 19, 16,
        20, 21, 22,  // Top face
        22, 23, 20
    };

    std::cout << verticesFantasmaCubo.size() << std::endl;
    std::cout << indicesFantasmaCubo.size() << std::endl;

    const int tamVertEsfera = verticesFantasmaCubo.size();
    const int numVerticesCubo = 24;
    const int numIndicesCubo = 36;

    for (int i = 0; i < numVerticesCubo; i++) {
        verticesFantasmaCubo.push_back(vertices[i]);
    }

    for (int j = 0; j < numIndicesCubo; j++) {
        indicesFantasmaCubo.push_back(indices[j] + tamVertEsfera);
    }
    std::cout << verticesFantasmaCubo.size() << std::endl;
    std::cout << indicesFantasmaCubo.size() << std::endl;


    verticesFantasmasCubo.push_back(verticesFantasmaCubo);
    indicesFantasmasCubo.push_back(indicesFantasmaCubo);
}

// Dimensiones de la ventana
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Prototipos de funciones
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Shader de vértices
const char* vertexShaderSourcePacman = R"(
    #version 330 core
    layout (location = 0) in vec4 aPos;
    uniform mat4 model;
    void main()
    {
        gl_Position = model * aPos;
    }
)";

// Shader de fragmentos
const char* fragmentShaderSourcePacman = R"(
    #version 330 core
    out vec4 FragColor;
    void main()
    {
        FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
    }
)";



const char* vertexShaderSourceFantasma = R"(
    #version 330 core
    layout (location = 0) in vec4 aPos;
    uniform mat4 model;
    void main()
    {
        gl_Position = model * aPos;
    }
)";

//"   gl_Position = multiplicar();\n"
//"   gl_Position = transformacion*vec4(aPos,1.0);\n"
//"   gl_Position = vec4(aPos.x+transformacion[0][3],aPos.y+transformacion[1][3],aPos.z+transformacion[2][3], 1.0);\n"
const char* fragmentShaderSourceFantasma = R"(
    #version 330 core
    out vec4 FragColor;
    uniform vec4 ourColor;
    void main()
    {
        FragColor = ourColor;
    }
)";


int main()
{
    // Inicializar GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Crear una ventana GLFW
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Esfera OpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Inicializar GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Compilar y enlazar shaders
    GLuint vertexShaderPacman = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderPacman, 1, &vertexShaderSourcePacman, NULL);
    glCompileShader(vertexShaderPacman);

    GLuint fragmentShaderPacman = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderPacman, 1, &fragmentShaderSourcePacman, NULL);
    glCompileShader(fragmentShaderPacman);

    GLuint shaderProgramPacman = glCreateProgram();
    glAttachShader(shaderProgramPacman, vertexShaderPacman);
    glAttachShader(shaderProgramPacman, fragmentShaderPacman);
    glLinkProgram(shaderProgramPacman);
    glDeleteShader(vertexShaderPacman);
    glDeleteShader(fragmentShaderPacman);


    //Program y shaders para fantasma

    unsigned int vertexShaderFantasma = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderFantasma, 1, &vertexShaderSourceFantasma, NULL);
    glCompileShader(vertexShaderFantasma);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShaderFantasma, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShaderFantasma, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShaderFantasma = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderFantasma, 1, &fragmentShaderSourceFantasma, NULL);
    glCompileShader(fragmentShaderFantasma);
    // check for shader compile errors
    glGetShaderiv(fragmentShaderFantasma, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderFantasma, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shaderProgramFantasma = glCreateProgram();
    glAttachShader(shaderProgramFantasma, vertexShaderFantasma);
    glAttachShader(shaderProgramFantasma, fragmentShaderFantasma);
    glLinkProgram(shaderProgramFantasma);
    // check for linking errors
    glGetProgramiv(shaderProgramFantasma, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgramFantasma, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShaderFantasma);
    glDeleteShader(fragmentShaderFantasma);


    // Datos de la esfera
    const float radius = 0.5f;
    const int sectorCount = 36;
    const int stackCount = 18;

    // Construir pacman
    buildSphere(radius, sectorCount, stackCount, verticesPacman, indicesPacman);
    // Construir fantasmas
    for (int i = 0;i < N_FANTASMAS;i++) {
        buildFantasma();
    }

    // Crear VAOPacman, VBOPacman y EBOPacman
    GLuint VAOPacman, VBOPacman, EBOPacman;
    glGenVertexArrays(1, &VAOPacman);
    glGenBuffers(1, &VBOPacman);
    glGenBuffers(1, &EBOPacman);

    glBindVertexArray(VAOPacman);
    glBindBuffer(GL_ARRAY_BUFFER, VBOPacman);
    glBufferData(GL_ARRAY_BUFFER, verticesPacman.size() * sizeof(float), &verticesPacman[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOPacman);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesPacman.size() * sizeof(unsigned int), &indicesPacman[0], GL_STATIC_DRAW);

    // Configurar atributos de vértices
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Desvincular VAOPacman y VBOPacman
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Crear VAOFantasma, VBOFantasma y EBOFantasma
    unsigned int VBOFantasmaEsfera[N_FANTASMAS], VAOFantasmaEsfera[N_FANTASMAS], EBOFantasmaEsfera[N_FANTASMAS];
    glGenVertexArrays(N_FANTASMAS, VAOFantasmaEsfera);
    glGenBuffers(N_FANTASMAS, VBOFantasmaEsfera);
    glGenBuffers(N_FANTASMAS, EBOFantasmaEsfera);

    for (int i = 0;i < N_FANTASMAS;i++) {
        glBindVertexArray(VAOFantasmaEsfera[i]);

        glBindBuffer(GL_ARRAY_BUFFER, VBOFantasmaEsfera[i]);
        glBufferData(GL_ARRAY_BUFFER, (verticesFantasmasEsfera[i].size()) * sizeof(float), &verticesFantasmasEsfera[i][0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOFantasmaEsfera[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesFantasmasEsfera[i].size() * sizeof(unsigned int), &indicesFantasmasEsfera[i][0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }


    unsigned int VBOFantasmaCubo[N_FANTASMAS], VAOFantasmaCubo[N_FANTASMAS], EBOFantasmaCubo[N_FANTASMAS];
    glGenVertexArrays(N_FANTASMAS, VAOFantasmaCubo);
    glGenBuffers(N_FANTASMAS, VBOFantasmaCubo);
    glGenBuffers(N_FANTASMAS, EBOFantasmaCubo);

    for (int i = 0;i < N_FANTASMAS;i++) {
        glBindVertexArray(VAOFantasmaCubo[i]);

        glBindBuffer(GL_ARRAY_BUFFER, VBOFantasmaCubo[i]);
        glBufferData(GL_ARRAY_BUFFER, (verticesFantasmasCubo[i].size()) * sizeof(float), &verticesFantasmasCubo[i][0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOFantasmaCubo[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesFantasmasCubo[i].size() * sizeof(unsigned int), &indicesFantasmasCubo[i][0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    // Configurar estado de renderizado
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Entrada de usuario
        processInput(window);

        // Actualizar la matriz de transformación de la esfera
        rotationAngle += rotationSpeed;
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(xPos, yPos, 0.0f));
        model = glm::scale(model, glm::vec3(scale, scale, scale));
        //model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));

        // Renderizado
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgramPacman);
        glBindVertexArray(VAOPacman);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgramPacman, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawElements(GL_TRIANGLES, indicesPacman.size(), GL_UNSIGNED_INT, 0);

        glUseProgram(shaderProgramFantasma);
        for (int i = 0;i < N_FANTASMAS;i++) {
            //esfera del fantasma
            glBindVertexArray(VAOFantasmaEsfera[i]);
            int vertexColorLocation = glGetUniformLocation(shaderProgramFantasma, "ourColor");
            glUniform4f(vertexColorLocation, 1.0f, 0.0f, 1.0f, 1.0f);

            glUniformMatrix4fv(glGetUniformLocation(shaderProgramPacman, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glDrawElements(GL_TRIANGLES, indicesFantasmasEsfera[i].size(), GL_UNSIGNED_INT, 0);

            //cubo del fantasma

            glBindVertexArray(VAOFantasmaCubo[i]);
            //int vertexColorLocation = glGetUniformLocation(shaderProgramFantasma, "ourColor");
            glUniform4f(vertexColorLocation, 1.0f, 0.0f, 1.0f, 1.0f);
            glUniformMatrix4fv(glGetUniformLocation(shaderProgramPacman, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glDrawElements(GL_TRIANGLES, indicesFantasmasCubo[i].size(), GL_UNSIGNED_INT, 0);
        }

        // Intercambio de buffers y eventos de GLFW
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Liberar recursos
    glDeleteVertexArrays(1, &VAOPacman);
    glDeleteBuffers(1, &VBOPacman);
    glDeleteBuffers(1, &EBOPacman);
    glDeleteProgram(shaderProgramPacman);

    glDeleteVertexArrays(N_FANTASMAS, VAOFantasmaEsfera);
    glDeleteBuffers(N_FANTASMAS, VBOFantasmaEsfera);
    glDeleteBuffers(N_FANTASMAS, EBOFantasmaEsfera);
    glDeleteProgram(shaderProgramFantasma);

    glDeleteVertexArrays(N_FANTASMAS, VAOFantasmaCubo);
    glDeleteBuffers(N_FANTASMAS, VBOFantasmaCubo);
    glDeleteBuffers(N_FANTASMAS, EBOFantasmaCubo);
    // glDeleteProgram(shaderProgramFantasma);

    // Terminar GLFW
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        yPos += movementSpeed;

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        yPos -= movementSpeed;

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        xPos -= movementSpeed;

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        xPos += movementSpeed;

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        scale += scaleSpeed;

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        scale -= scaleSpeed;
}