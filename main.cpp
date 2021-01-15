#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define SFML_STATIC
#include <SFML/Graphics.hpp>
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
unsigned int LoadTexture(const char* path);

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

float CameraSpeed, ActualSpeed = 10.0f;
glm::vec3 CameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 CameraDirection;
glm::vec3 HorizontalVector;

float LastFrame=0.0f;
float CurrentFrame;
float DeltaTime;

float Yaw = -90.0f;
float Pitch = 0.0f;
float lastX = 400.0f, lastY = 300.0f;
float xoffset, yoffset;
float Angle;
const float Sensitivity = 0.05f;
bool FirstMouse = true;
bool CursorControlled = false;


bool OnGround = true;
float Gravity = 20.0f;
float JumpTime, JumpLocalTime, JumpInitialeSpeed;
float CurrentY;

bool Iconified = false;

glm::vec3 LightColor(1.0f, 1.0f, 1.0f);
glm::vec3 SunLightDir(-1.0f, -1.0f, -1.0f);

int main()
{
    cout << "Creating And Configuring Window..." << endl;
    if (!glfwInit())
    {
        cout << "GLFW Initialization Failure!" << endl;
        return EXIT_FAILURE;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (!window)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    GLFWmonitor* ScreenMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* FullScreen = glfwGetVideoMode(ScreenMonitor);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    CursorControlled = true;

    glfwSetWindowPos(window, (1366 - SCR_WIDTH) / 2, 50);

    cout << "--------------------------------------------------------------" << endl;
    cout << "Loading Opengl..." << endl;
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);
    cout << "--------------------------------------------------------------" << endl;
    cout << "Creating And Compiling Shaders..." << endl;
    Shader ContainersShader("resources\\Shaders\\Containers.vert", "resources\\Shaders\\Containers.frag");
    Shader PointLightSourceShader("resources\\Shaders\\Point_Light_Source.vert", "resources\\Shaders\\Point_Light_Source.frag");
    cout << "--------------------------------------------------------------" << endl;
    cout << "Setting Data..." << endl;
    float cubeVertices[] = {
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,    0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,    1.0f, 0.0f,    0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,    1.0f, 1.0f,    0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,    1.0f, 1.0f,    0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,    0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,    0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,    0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,    1.0f, 0.0f,    0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 1.0f,    0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 1.0f,    0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,    0.0f, 1.0f,    0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,    0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,   -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,    1.0f, 1.0f,   -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,   -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,   -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,   -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,   -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,    1.0f, 0.0f,    1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,    1.0f, 1.0f,    1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,    0.0f, 1.0f,    1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,    0.0f, 1.0f,    1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,    0.0f, 0.0f,    1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 0.0f,    1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,    0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,    1.0f, 1.0f,    0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,    1.0f, 0.0f,    0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,    1.0f, 0.0f,    0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,    0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,    0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,    0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,    1.0f, 1.0f,    0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 0.0f,    0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 0.0f,    0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,    0.0f, 0.0f,    0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,    0.0f,  1.0f,  0.0f,

         100.0f, -1.0f,  100.0f,    200.0f,   0.0f,    0.0f,  1.0f,  0.0f,
        -100.0f, -1.0f,  100.0f,      0.0f,   0.0f,    0.0f,  1.0f,  0.0f,
         100.0f, -1.0f, -100.0f,    200.0f, 200.0f,    0.0f,  1.0f,  0.0f,

        -100.0f, -1.0f,  100.0f,      0.0f,   0.0f,    0.0f,  1.0f,  0.0f,
         100.0f, -1.0f, -100.0f,    200.0f, 200.0f,    0.0f,  1.0f,  0.0f,
        -100.0f, -1.0f, -100.0f,      0.0f, 200.0f,    0.0f,  1.0f,  0.0f
    };
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,   0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f,  -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f,  -3.5f),
        glm::vec3(-1.7f,  3.0f,  -7.5f),
        glm::vec3( 1.3f, -2.0f,  -2.5f),
        glm::vec3( 1.5f,  2.0f,  -2.5f),
        glm::vec3( 1.5f,  0.2f,  -1.5f),
        glm::vec3(-1.3f,  1.0f,  -1.5f)
    };

    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    cout << "--------------------------------------------------------------" << endl;
    cout << "Reading And Organizing Data..." << endl;
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    cout << "--------------------------------------------------------------" << endl;
    cout << "Loading Assets..." << endl;
    unsigned int texture1 = LoadTexture("resources\\Textures\\container_diff.png");
    unsigned int texture2 = LoadTexture("resources\\Textures\\container_spec.png");
    unsigned int texture3 = LoadTexture("resources\\Textures\\floor_diff.png");
    unsigned int texture4 = LoadTexture("resources\\Textures\\floor_spec.png");
    glBindTexture(GL_TEXTURE_2D, 0);
    cout << "--------------------------------------------------------------" << endl;

    cout << "Preparing The Sceen And Showing The Window..." << endl;
    glm::mat4 Model = glm::mat4(1.0f);
    glm::mat4 View = glm::mat4(1.0f);
    glm::mat4 Projection = glm::mat4(1.0f);

    Projection = glm::perspective(glm::radians(45.0f), (float)FullScreen->width / (float)FullScreen->height, 0.1f, 500.0f);
    cout << "-----------------------------DONE-----------------------------" << endl;

    glfwSetWindowMonitor(window, ScreenMonitor, 0, 0, FullScreen->width, FullScreen->height, FullScreen->refreshRate);
    glfwFocusWindow(window);
    glfwShowWindow(window);

    while (!glfwWindowShouldClose(window))
    {
        CurrentFrame = (float)glfwGetTime();
        DeltaTime = CurrentFrame - LastFrame;
        LastFrame = CurrentFrame;

        CameraSpeed = ActualSpeed * sqrt(powf(CameraFront.x, 2) + powf(CameraFront.z, 2)) * DeltaTime;

        HorizontalVector = glm::normalize(glm::vec3(CameraFront.x, 0.0f, CameraFront.z));

        processInput(window);

        if (!OnGround)
        {
            JumpLocalTime = (float)glfwGetTime() - JumpTime;
            CurrentY = (-Gravity / 2) * powf(JumpLocalTime, 2) + JumpInitialeSpeed * JumpLocalTime;
            if ( CurrentY < 0.0f )
            {
                OnGround = true;
                CameraPos.y = 0.0f;
                JumpInitialeSpeed = 0.0f;
            }
            else
            {
                CameraPos.y = CurrentY;
            }
        }

        View = glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUp);

        glClearColor(0.0f, 181.0f / 255.0f, 226.0f / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ContainersShader.use();
        glBindVertexArray(VAO);

        ContainersShader.setMat4("View", View);
        ContainersShader.setMat4("Projection", Projection);

        ContainersShader.setInt("material.diffuse", 0);
        ContainersShader.setInt("material.specular", 1);
        ContainersShader.setFloat("material.shininess", 32.0f);

        ContainersShader.setVec3("dirLight.direction", SunLightDir);
        ContainersShader.setVec3("dirLight.ambient",   LightColor * glm::vec3(0.05f));
        ContainersShader.setVec3("dirLight.diffuse",   LightColor * glm::vec3(0.4f));
        ContainersShader.setVec3("dirLight.specular",  LightColor * glm::vec3(0.5f));

        ContainersShader.setVec3("pointLight[0].position", pointLightPositions[0]);
        ContainersShader.setVec3("pointLight[0].ambient", LightColor * glm::vec3(0.05f));
        ContainersShader.setVec3("pointLight[0].diffuse", LightColor * glm::vec3(0.8f));
        ContainersShader.setVec3("pointLight[0].specular", LightColor * glm::vec3(1.0f));
        ContainersShader.setFloat("pointLight[0].constant", 1.0f);
        ContainersShader.setFloat("pointLight[0].linear", 0.09f);
        ContainersShader.setFloat("pointLight[0].quadratic", 0.032f);

        ContainersShader.setVec3("pointLight[1].position", pointLightPositions[1]);
        ContainersShader.setVec3("pointLight[1].ambient", LightColor * glm::vec3(0.05f));
        ContainersShader.setVec3("pointLight[1].diffuse", LightColor * glm::vec3(0.8f));
        ContainersShader.setVec3("pointLight[1].specular", LightColor * glm::vec3(1.0f));
        ContainersShader.setFloat("pointLight[1].constant", 1.0f);
        ContainersShader.setFloat("pointLight[1].linear", 0.09f);
        ContainersShader.setFloat("pointLight[1].quadratic", 0.032f);

        ContainersShader.setVec3("pointLight[2].position", pointLightPositions[2]);
        ContainersShader.setVec3("pointLight[2].ambient", LightColor * glm::vec3(0.05f));
        ContainersShader.setVec3("pointLight[2].diffuse", LightColor * glm::vec3(0.8f));
        ContainersShader.setVec3("pointLight[2].specular", LightColor * glm::vec3(1.0f));
        ContainersShader.setFloat("pointLight[2].constant", 1.0f);
        ContainersShader.setFloat("pointLight[2].linear", 0.09f);
        ContainersShader.setFloat("pointLight[2].quadratic", 0.032f);

        ContainersShader.setVec3("pointLight[3].position", pointLightPositions[3]);
        ContainersShader.setVec3("pointLight[3].ambient", LightColor * glm::vec3(0.05f));
        ContainersShader.setVec3("pointLight[3].diffuse", LightColor * glm::vec3(0.8f));
        ContainersShader.setVec3("pointLight[3].specular", LightColor * glm::vec3(1.0f));
        ContainersShader.setFloat("pointLight[3].constant", 1.0f);
        ContainersShader.setFloat("pointLight[3].linear", 0.09f);
        ContainersShader.setFloat("pointLight[3].quadratic", 0.032f);

        ContainersShader.setVec3("spotLight.position", CameraPos);
        ContainersShader.setVec3("spotLight.direction", CameraFront);
        ContainersShader.setFloat("spotLight.innerCutOff", glm::cos(glm::radians(12.5f)));
        ContainersShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
        ContainersShader.setFloat("spotLight.constant", 1.0f);
        ContainersShader.setFloat("spotLight.linear", 0.09f);
        ContainersShader.setFloat("spotLight.quadratic", 0.032f);
        ContainersShader.setVec3("spotLight.ambient", LightColor * glm::vec3(0.0f));
        ContainersShader.setVec3("spotLight.diffuse", LightColor * glm::vec3(1.0f));
        ContainersShader.setVec3("spotLight.specular", LightColor * glm::vec3(1.0f));

        ContainersShader.setVec3("ViewPos", CameraPos);
        ContainersShader.setVec3("Brightness", glm::vec3(0.0f));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        for (unsigned int i = 0; i < 10; i++)
        {
            Model = glm::mat4(1.0f);
            Model = glm::translate(Model, cubePositions[i]);
            Angle = 20.0f * i;
            Angle *= glfwGetTime();
            Model = glm::rotate(Model, glm::radians(Angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ContainersShader.setMat4("Model", Model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture3);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture4);
        Model = glm::mat4(1.0f);
        ContainersShader.setMat4("Model", Model);
        glDrawArrays(GL_TRIANGLES, 36, 6);

        PointLightSourceShader.use();
        glBindVertexArray(lightVAO);
        PointLightSourceShader.setMat4("Projection", Projection);
        PointLightSourceShader.setMat4("View", View);
        PointLightSourceShader.setVec3("LightColor", LightColor);
        for (int i = 0; i < 4; i++)
        {
            Model = glm::mat4(1.0f);
            Model = glm::translate(Model, pointLightPositions[i]);
            Model = glm::scale(Model, glm::vec3(0.5f));
            PointLightSourceShader.setMat4("Model", Model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (CursorControlled)
    {
        if (FirstMouse)
        {
            lastX = (float)xpos;
            lastY = (float)ypos;
            FirstMouse = false;
        }
        xoffset = (float)xpos - lastX;
        yoffset = lastY - (float)ypos;
        lastX = (float)xpos;
        lastY = (float)ypos;

        xoffset *= Sensitivity;
        yoffset *= Sensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch <= -89.0f)
            Pitch = -89.0f;

        CameraDirection.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        CameraDirection.y = sin(glm::radians(Pitch));
        CameraDirection.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        CameraFront = glm::normalize(CameraDirection);
    }

}
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        CameraPos += HorizontalVector * CameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        CameraPos -= HorizontalVector * CameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        CameraPos -= glm::normalize(glm::cross(CameraFront, CameraUp)) * CameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        CameraPos += glm::normalize(glm::cross(CameraFront, CameraUp)) * CameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && OnGround)
    {
        JumpTime = (float)glfwGetTime();
        JumpInitialeSpeed = 7.5f;
        OnGround = false;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && OnGround)
    {
        CameraPos.y = -0.25f;
        ActualSpeed = 2.5f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE && OnGround)
    {
        CameraPos.y = 0.0f;
        ActualSpeed = 10.0f;
    }
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_F1)
        {
            if (CursorControlled)
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                CursorControlled = false;
            }
            else
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                FirstMouse = true;
                CursorControlled = true;
            }
        }
    }
}
unsigned int LoadTexture(const char* path)
{
    unsigned int ID;
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    sf::Image textureImage;
    if (textureImage.loadFromFile(path))
    {
        textureImage.flipVertically();

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureImage.getSize().x, textureImage.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureImage.getPixelsPtr());

        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        cout << "Texture Loading Failure!" << endl;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    return ID;
}
