#include <glad/glad.h> 
#include <glfw3.h>
#include "shader.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

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

    
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
  
        
    


    unsigned int VAO,LIGHTVAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    int width, length, nrchannels;
    unsigned char* data = stbi_load("steelborder.png", &width, &length, &nrchannels,0);
  

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, length, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "the texture cannot be loaded" << "\n";
    }

    int specwidth, speclength, specchannels;
    unsigned char* steeldata = stbi_load("steelspecular.png", &specwidth, &speclength, &specchannels,0);
   
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (steeldata) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, specwidth, speclength, 0, GL_RGBA, GL_UNSIGNED_BYTE, steeldata);
        glGenerateMipmap(GL_TEXTURE_2D);

    }
    else {
        std::cout << "the 2nd one could not load into site"<<"\n";
    }

   //light cube
    glGenVertexArrays(1, &LIGHTVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindVertexArray(LIGHTVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    Shader shader("vertexshader.glsl", "fragmentshader.glsl");
    Shader theshader("lightvert.glsl", "lightfrag.glsl");
    glEnable(GL_DEPTH_TEST);
     
    glm::vec3 cubepositions[]{
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f) };

    while (!glfwWindowShouldClose(window)) {
      

        double currentframe = glfwGetTime();
        deltatime = currentframe - lastframe;
        lastframe = currentframe;
        processInput(window);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     
        shader.use();
        glBindVertexArray(VAO);
        glm::mat4 view(1.0f);
        view = glm::lookAt(cameraposition, cameraposition + camerafront, cameraup);
        shader.setmatrixuniform("view", view);
        glm::mat4 projection(1.0f);
        projection = glm::perspective(glm::radians(fov), 1280.0f / 720.0f, 0.1f, 100.0f);
        shader.setmatrixuniform("projection", projection);
   
        glm::vec3 lightcolor{ 1.0f, 1.0f,1.0f };
      
        shader.setvec3("lightcolor", lightcolor);
       
        shader.setfloat("material.shininess", 64.0f);
        shader.setvec3("viewposition", cameraposition);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        
        shader.setint("material.diffuse", 0);
        shader.setint("material.specular", 1);

        glm::vec3 lightdiffuse{ 0.5f, 0.5f, 0.5f };
        glm::vec3 lightambient{ 0.4f,0.4f,0.4f };
        glm::vec3 lightspecular{ 0.8f,0.8f,0.8f };
        glm::vec3 lightposition{ 0.0f,0.5f,0.0f};

        shader.setvec3("light.position", lightposition);
        shader.setvec3("light.ambient", lightambient);
        shader.setvec3("light.diffuse", lightdiffuse);
        shader.setvec3("light.specular", lightspecular);
        shader.setfloat("light.constant", 1.0f);
        shader.setfloat("light.linear", 0.09f);
        shader.setfloat("light.quadratic", 0.032f);
        for (int i{ 0 }; i < 10; i++) {
            glm::mat4 model(1.0f);

            model = glm::translate(model, cubepositions[i]);
            float angle{ 20.0f * i };
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setmatrixuniform("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        theshader.use();
        glm::mat4 lightmodel(1.0f);
        lightmodel = glm::translate(lightmodel, lightposition);
        lightmodel = glm::scale(lightmodel,glm::vec3(0.2f));
        theshader.setmatrixuniform("model", lightmodel);
        
        glBindVertexArray(LIGHTVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);  

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

