#include <glad/glad.h> 
#include <glfw3.h>
#include "shader.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scrollback(GLFWwindow* window, double xoffset, double yoffset);
glm::vec3 cameraposition{ glm::vec3(0.0f, 0.0f, 3.0f) };
glm::vec3 cameratarget{ glm::vec3(0.0f,0.0f,0.0f) };
glm::vec3 cameradirection{ glm::normalize(cameraposition - cameratarget) };
glm::vec3 up{ glm::vec3(0.0f, 1.0f, 0.0f) };
glm::vec3 cameraright{ glm::normalize(glm::cross(up,cameradirection)) };
glm::vec3 cameraup{ glm::cross(cameradirection,cameraright) };
glm::vec3 camerafront{ glm::normalize(glm::vec3(0.0f,0.0f,-2.0f)) };
double deltatime{ 0.0f };
double lastframe{ 0.0f };
double currentframe{glfwGetTime()};


double g_yaw{ -90.0f };
double g_pitch{0.0f};
float fov = 45.0f;
bool firstmouse{ true };



int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Mustbe the best", NULL, NULL);
    if (window == NULL) {
        std::cout << "failed to create glfwwindow" << "\n";
        glfwTerminate();
        return -1;

    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    framebuffer_size_callback(window, 1280, 720);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scrollback);
    std::cout << glGetString(GL_VERSION) << "\n";

    

 
    glEnable(GL_DEPTH_TEST);


    while (!glfwWindowShouldClose(window)) {
      


        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    std::cout << "cleaning resources" << "\n";
    std::cout << "sizeof double is " << sizeof(double) << "\n";
  

    glfwTerminate();
   
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
   
    glViewport(0, 0, width, height);

}
void processInput(GLFWwindow* window) {
    
   

    float cameraspeed{ 2.5f * static_cast<float>(deltatime) };

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
   
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraposition += cameraspeed * camerafront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraposition -= cameraspeed * camerafront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraposition -= glm::normalize(glm::cross(camerafront, cameraup)) * cameraspeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraposition += glm::normalize(glm::cross(camerafront, cameraup)) * cameraspeed;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static double lastx{ 720 };
    static double lasty{ 360 };
    if (firstmouse) {
        lastx = xpos;
        lasty = ypos;
        firstmouse = false;
    }
    double xoffset{ xpos - lastx };
    double yoffset{ lasty - ypos };
    lastx = xpos;
    lasty = ypos;
    constexpr double sensitivity{ 0.05f };
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    g_yaw += xoffset;
    g_pitch += yoffset;

    if (g_pitch > 89.0f)
        g_pitch = 89.0f;
    if (g_pitch < -89.0f)
        g_pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(g_yaw)) * cos(glm::radians(g_pitch));
    direction.y = sin(glm::radians(g_pitch));
    direction.z = sin(glm::radians(g_yaw)) * cos(glm::radians(g_pitch));
    camerafront = glm::normalize(direction);
}

void scrollback(GLFWwindow* window, double xoffset, double yoffset) {
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

