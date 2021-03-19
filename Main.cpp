#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp> //core glm functionality
#include <glm/gtc/matrix_transform.hpp> //glm extension for generating common transformation matrices
#include <glm/gtc/matrix_inverse.hpp> //glm extension for computing inverse matrices
#include <glm/gtc/type_ptr.hpp> //glm extension for accessing the internal data structure of glm types

#include "Window.h"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Model3D.hpp"

#include <iostream>
#include <windows.h>
#include <mmsystem.h>

// window
gps::Window myWindow;
// matrices
glm::mat4 model, model2, model3, modelGate, modelGate2, modelElice, modelHelicopter;
glm::mat4 view;
glm::mat4 projection;
glm::mat3 normalMatrix;

// light parameters
glm::vec3 lightDir;
glm::vec3 lightColor;
glm::vec3 lightColor2;
glm::vec3 gunLight = glm::vec3(0.0f,0.0f,0.0f);

// shader uniform locations
GLuint modelLoc;
GLuint modelLocGate;
GLuint viewLoc;
GLuint fulgerLoc;
GLuint viewLocGate;
GLuint projectionLoc;
GLuint projectionLocGate;
GLuint normalMatrixLoc;
GLuint lightDirLoc;
GLuint lightColorLoc;
GLuint lightColorLoc2;
GLuint gunLightLoc;

// camera
float cameraX = 90.0f;
float cameraY = 10.0f;
float cameraZ = 20.0f;
float carAngle = 0.0f;

gps::Camera myCamera(
    glm::vec3(cameraX, cameraY, cameraZ),
    glm::vec3(80.0f, 10.0f, 20.0f),
    glm::vec3(0.0f, 1.0f, 0.0f));

gps::Camera myCamera2(
    glm::vec3(cameraX, cameraY, cameraZ),
    glm::vec3(80.0f, 10.0f, 20.0f),
    glm::vec3(0.0f, 1.0f, 0.0f));

GLfloat cameraSpeed = 0.2f;

GLboolean pressedKeys[1024];

// models
gps::Model3D teapot;
gps::Model3D rulota;
gps::Model3D tree;
gps::Model3D gate;
gps::Model3D gate2;
gps::Model3D elice;
gps::Model3D helicopter;
gps::Model3D eliceMica;
gps::Model3D gunAndTarget;
gps::Model3D muzzleFlash;
gps::Model3D hole1;
gps::Model3D hole2;
gps::Model3D hole3;
gps::Model3D hole4;
gps::Model3D tragaci;
gps::Model3D cort;
gps::Model3D masina;
GLfloat angle;

// shaders
gps::Shader myBasicShader;
gps::Shader treeShader;
gps::Shader gateShader;

//Mouse
int glWindowWidth = 800;
int glWindowHeight = 600;
float lastX = 0.0f;
float lastY = 0.0f;
float yaw = 0.0f;
float pitch = 180.0f;
int preview = 0;
int statePreview = 0;
glm::vec3 frog = glm::vec3(0.0f, 0.0f, 0.0f);
float angleGate = 0.0f;
float angleTragaci = 0.0f;
float angleGate2 = 0.0f;
float angleElice = 0.0f;
int eliceState = 0;
float helicopterZ = 0.0f;
int automatic = 0;
int manualGate = 0;
int tragaciState = 0;

//shadows
int retina_width, retina_height;
const unsigned int SHADOW_WIDTH = 2048;
const unsigned int SHADOW_HEIGHT = 2048;
glm::mat4 lightRotation;
GLfloat lightAngle;
gps::Model3D lightCube;
gps::Shader depthMapShader;
gps::Shader lightShader;
GLuint shadowMapFBO;
GLuint depthMapTexture;
glm::mat4 lightView;
int lightOK;
bool showDepthMap;
int fulgerState = 0;
GLfloat specularStrength2 = 0.05f;
GLfloat ambientStrength2 = 0.05f;
glm::vec3 fulger;
int holeNumber = 0;
float helicopterMove;
float helicopterAngle;
float helicopterMoveZ;
int decolat = 0;
int aterizare = 0;
int driving = 0;
glm::vec3 masinaCoord = glm::vec3(149.82f, 1.84f, 23.1f);
glm::vec3 masinaLastCoord = glm::vec3(0.0f, 0.0f, 0.0f);
float masinaX = 149.82f;
float masinaY = 1.84f;
float masinaZ = 23.1f;
float lastRotate = 0.0f;
//float masinaX = 150.13f;
//float masinaY = 2.15f;
//float masinaZ = 23.13f;
glm::vec3 lastView(149.82f, 1.84f, 23.1f);


GLenum glCheckError_(const char* file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        std::string error;
        switch (errorCode) {
        case GL_INVALID_ENUM:
            error = "INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            error = "INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            error = "INVALID_OPERATION";
            break;
        case GL_STACK_OVERFLOW:
            error = "STACK_OVERFLOW";
            break;
        case GL_STACK_UNDERFLOW:
            error = "STACK_UNDERFLOW";
            break;
        case GL_OUT_OF_MEMORY:
            error = "OUT_OF_MEMORY";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            error = "INVALID_FRAMEBUFFER_OPERATION";
            break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void windowResizeCallback(GLFWwindow* window, int width, int height) {
    fprintf(stdout, "Window resized! New width: %d , and height: %d\n", width, height);
    //TODO
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            pressedKeys[key] = true;
        }
        else if (action == GLFW_RELEASE) {
            pressedKeys[key] = false;
        }
    }
}

bool firstMouse = true;
void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    //TODO

    float MouseSensitivity = 0.1;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    yaw += yoffset * MouseSensitivity;
    pitch += xoffset * MouseSensitivity;

    myCamera.rotate(yaw, pitch);
    view = myCamera.getViewMatrix();
    myBasicShader.useShaderProgram();
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    // compute normal matrix for teapot
    normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
}

void processMovement() {

    if (pressedKeys[GLFW_KEY_R]) {
        cameraSpeed = 1.0f;
        driving = 1;

        glm::vec3 v = myCamera.getCameraPosition();
        v.x = lastView.x;
        v.y = lastView.y;
        v.z = lastView.z;
        myCamera.setCameraPosition(v);
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for teapot
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));

    }
    if (pressedKeys[GLFW_KEY_T]) {
        cameraSpeed = 0.2f;
        driving = 0;

        glm::vec3 v = myCamera.getCameraPosition();
       
        v.z = v.z + 1.5f;
        myCamera.setCameraPosition(v);
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for teapot
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));

        carAngle += 0.03f;

    }

    if (pressedKeys[GLFW_KEY_J]) {
        preview = 0;
    }
    if (pressedKeys[GLFW_KEY_F]) {
        helicopterAngle += 0.3f;
    }

    if (pressedKeys[GLFW_KEY_UP]) {
        if(decolat == 1)
        helicopterMove = helicopterMove + 0.08f;
    }

    if (pressedKeys[GLFW_KEY_DOWN]) {
        if (decolat == 1)
        helicopterMove = helicopterMove - 0.08f;
    }
    if (pressedKeys[GLFW_KEY_LEFT]) {
        if (decolat == 1)
        helicopterMoveZ = helicopterMoveZ - 0.08f;
    }

    if (pressedKeys[GLFW_KEY_RIGHT]) {
        if (decolat == 1)
        helicopterMoveZ = helicopterMoveZ + 0.08f;
    }
    if (pressedKeys[GLFW_KEY_SPACE]) {
       
        glm::vec3 v = myCamera.getCameraPosition();
        v.y += 0.1f;
        myCamera.setCameraPosition(v);
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for teapot
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    }
    if (pressedKeys[GLFW_KEY_LEFT_CONTROL]) {
        glm::vec3 v = myCamera.getCameraPosition();
        v.y -= 0.1f;
        myCamera.setCameraPosition(v);
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for teapot
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    }
    if (pressedKeys[GLFW_KEY_L]) {
        aterizare = 1;
        eliceState = 2;
    }
    if (pressedKeys[GLFW_KEY_N]) {
        manualGate = 0;
    }
    if (pressedKeys[GLFW_KEY_M]) {
        manualGate = 1;
    }

    if (pressedKeys[GLFW_KEY_W]) {
        myCamera.move(gps::MOVE_FORWARD, cameraSpeed);
        //update view matrix
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for teapot
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));

       
    }

    if (pressedKeys[GLFW_KEY_S]) {
        myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
        //update view matrix
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for teapot
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    }

    if (pressedKeys[GLFW_KEY_A]) {
       
        if (driving == 0) {
            myCamera.move(gps::MOVE_LEFT, cameraSpeed);
            //update view matrix
            view = myCamera.getViewMatrix();
            myBasicShader.useShaderProgram();
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            // compute normal matrix for teapot
            normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        }
    }

    if (pressedKeys[GLFW_KEY_D]) {

        if (driving == 0) {
            myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
            //update view matrix
            view = myCamera.getViewMatrix();
            myBasicShader.useShaderProgram();
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            // compute normal matrix for teapot
            normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        }
       
    }

   


    if (pressedKeys[GLFW_KEY_E]) {
        angle += 1.0f;
        // update model matrix for teapot
        model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));
        // update normal matrix for teapot
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    }

    if (pressedKeys[GLFW_KEY_C]) {
        
        frog.x = 1.0f;
       

        myBasicShader.useShaderProgram();
        int frogLoc = glGetUniformLocation(myBasicShader.shaderProgram, "frog");
        glUniform3fv(frogLoc, 1, glm::value_ptr(frog));
    }
    if (pressedKeys[GLFW_KEY_B]) {

        frog.x = 0.0f;


        myBasicShader.useShaderProgram();
        int frogLoc = glGetUniformLocation(myBasicShader.shaderProgram, "frog");
        glUniform3fv(frogLoc, 1, glm::value_ptr(frog));
    }
    if (pressedKeys[GLFW_KEY_H]) {

        fulgerState = 1;
        myBasicShader.useShaderProgram();
        fulger = glm::vec3(1.5f, 1.5f, 0.0f);
        fulgerLoc = glGetUniformLocation(myBasicShader.shaderProgram, "fulger");
        glUniform3fv(fulgerLoc, 1, glm::value_ptr(fulger));
        PlaySound(TEXT("thunder.wav"), NULL, SND_ASYNC);
       
    }

    if (pressedKeys[GLFW_KEY_V]) {

        myCamera = myCamera2;
        yaw = 0.0f;
        if (preview == 0)
        {
            preview = 1;
            statePreview = 1;
            myCamera.setCameraPosition(glm::vec3(70.0f, 10.0f, 25.0f));


            myCamera.rotate(0, 90.0f);
            pitch = 90.0f;
            view = myCamera.getViewMatrix();
            myBasicShader.useShaderProgram();
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            // compute normal matrix for teapot
            normalMatrix = glm::mat3(glm::inverseTranspose(view * model));

        }



    }

    if (preview == 1)
    {
        if (statePreview == 1)
        {


            yaw;
            pitch += 0.3f;




            if (pitch > 230)
            {
                statePreview = 2;
                myCamera.setCameraPosition(glm::vec3(50.0f, 10.0f, 25.0f));
                pitch = 90.0f;
            }

            myCamera.rotate(yaw, pitch);
            view = myCamera.getViewMatrix();
            myBasicShader.useShaderProgram();
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            // compute normal matrix for teapot
            normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        }

        if (statePreview == 2)
        {

            myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
            //update view matrix
            view = myCamera.getViewMatrix();
            myBasicShader.useShaderProgram();
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            // compute normal matrix for teapot
            normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
            if (myCamera.getCameraPosition().x < -50)
            {
                statePreview = 3;
                myCamera.setCameraPosition(glm::vec3(-10.0f, 10.0f, 25.0f));
                pitch = 180.0f;
                myCamera.rotate(yaw, pitch);
            }
        }
        if (statePreview == 3)
        {
            myCamera.move(gps::MOVE_BACKWARD, cameraSpeed * 0.8f);
            //update view matrix
            view = myCamera.getViewMatrix();
            myBasicShader.useShaderProgram();
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            // compute normal matrix for teapot
            normalMatrix = glm::mat3(glm::inverseTranspose(view * model));

            if (myCamera.getCameraPosition().x > 30)
            {
                statePreview = 4;
                myCamera.setCameraPosition(glm::vec3(50.0f, 10.0f, 20.0f));
                pitch = -90.0f;
                myCamera.rotate(yaw, pitch);
            }
        }

        if (statePreview == 4)
        {
            myCamera.move(gps::MOVE_LEFT, cameraSpeed);
            //update view matrix
            view = myCamera.getViewMatrix();
            myBasicShader.useShaderProgram();
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            // compute normal matrix for teapot
            normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
            if (myCamera.getCameraPosition().x < -60)
            {
                statePreview = 0;
                myCamera.setCameraPosition(glm::vec3(90.0f, 10.0f, 20.0f));
                pitch = 180.0f;
                myCamera.rotate(yaw, pitch);
                view = myCamera.getViewMatrix();
                myBasicShader.useShaderProgram();
                glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
                // compute normal matrix for teapot
                normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
                preview = 0;
            }
        }
    }

    

    if (pressedKeys[GLFW_KEY_O]) {

        if (angleGate < 2.2f)
            angleGate += 0.03f;


    }
    if (pressedKeys[GLFW_KEY_K]) {

        if (angleGate > 0.0f)
            angleGate -= 0.03f;


    }
    if (pressedKeys[GLFW_KEY_P]) {



       
           eliceState = 1; 
            PlaySound(TEXT("helicopter.wav"), NULL, SND_ASYNC);


    }

    if (pressedKeys[GLFW_KEY_Z]) {


        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    }

    if (pressedKeys[GLFW_KEY_X]) {


        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


    }
    if (pressedKeys[GLFW_KEY_Q]) {


        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

        

    }

    if (eliceState == 1)
    {   
        
        angleElice -= 0.15f;

       

        if (angleElice < -50)
        {
            angleElice -= 0.2f;
            if (helicopterZ < 20)
                helicopterZ += 0.06f;
            else decolat = 1;
        }


    }
    if (eliceState == 2)
    {

        angleElice -= 0.15f;



        if (angleElice < -50)
        {
            angleElice -= 0.2f;
            if (helicopterZ > 0)
                helicopterZ -= 0.06f;
            else { decolat = 0; eliceState = 0; angleElice = 0.0f; }
        }


    }

    //tragaci
    if (pressedKeys[GLFW_KEY_I]) {


        tragaciState = 1;
        angleTragaci += 0.03f;
       


    }
}




void initOpenGLWindow() {
    //myWindow.Create(1024, 768, "OpenGL Project Core");
    myWindow.Create(1920, 1080, "OpenGL Project Core");
    glfwSetCursorPosCallback(myWindow.getWindow(), mouseCallback);
    glfwSetInputMode(myWindow.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwGetFramebufferSize(myWindow.getWindow(), &retina_width, &retina_height);
}

void setWindowCallbacks() {
    glfwSetWindowSizeCallback(myWindow.getWindow(), windowResizeCallback);
    glfwSetKeyCallback(myWindow.getWindow(), keyboardCallback);
    glfwSetCursorPosCallback(myWindow.getWindow(), mouseCallback);
}

void initOpenGLState() {
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
    glViewport(0, 0, myWindow.getWindowDimensions().width, myWindow.getWindowDimensions().height);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    glEnable(GL_CULL_FACE); // cull face
    glCullFace(GL_BACK); // cull back face
    glFrontFace(GL_CCW); // GL_CCW for counter clock-wise
}

void initModels() {
    teapot.LoadModel("models/teapot/teapot20segUT.obj");
    rulota.LoadModel("models/scena/scena.obj");
    gate.LoadModel("models/gate/gate.obj");
    gate2.LoadModel("models/gate/gate2.obj");
    tree.LoadModel("models/tree/tree.obj");
    elice.LoadModel("models/elice/elice.obj");
    helicopter.LoadModel("models/elice/helicopter.obj");
    lightCube.LoadModel("models/cube/cube.obj");
    eliceMica.LoadModel("models/elice/eliceMica.obj");
    gunAndTarget.LoadModel("models/gun and target/gun.obj");
    hole1.LoadModel("models/gun hole/h1.obj");
    hole2.LoadModel("models/gun hole/h2.obj");
    hole3.LoadModel("models/gun hole/h3.obj");
    hole4.LoadModel("models/gun hole/h4.obj");
    muzzleFlash.LoadModel("models/muzzle-flash/muzzle_flash.obj");
    tragaci.LoadModel("models/tragaci/tragaci.obj");
    cort.LoadModel("models/cort/cort.obj");
    masina.LoadModel("models/masina/masina.obj");

}

void initShaders() {
    myBasicShader.loadShader(
        "shaders/basic.vert",
        "shaders/basic.frag");

    treeShader.loadShader(
        "shaders/tree.vert",
        "shaders/tree.frag");
    gateShader.loadShader(
        "shaders/gate.vert",
        "shaders/gate.frag");

    depthMapShader.loadShader("shaders/depthMap.vert", "shaders/depthMap.frag");
    depthMapShader.useShaderProgram();

    lightShader.loadShader("shaders/lightCube.vert", "shaders/lightCube.frag");
    lightShader.useShaderProgram();
}

void initUniforms() {
    myBasicShader.useShaderProgram();

    // create model matrix for teapot
    model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
    modelLoc = glGetUniformLocation(myBasicShader.shaderProgram, "model");

    //2 randuri rulota
    model2 = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
    // modelLoc = glGetUniformLocation(myBasicShader.shaderProgram, "model2");

    //2 randuri hangar
    model3 = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));


    // get view matrix for current camera

    view = myCamera.getViewMatrix();
    viewLoc = glGetUniformLocation(myBasicShader.shaderProgram, "view");
    // send view matrix to shader
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    // compute normal matrix for teapot
    normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    normalMatrixLoc = glGetUniformLocation(myBasicShader.shaderProgram, "normalMatrix");



    // compute normal matrix for rulota
    normalMatrix = glm::mat3(glm::inverseTranspose(view * model2));
    normalMatrixLoc = glGetUniformLocation(myBasicShader.shaderProgram, "normalMatrix");


    // compute normal matrix for tree
    normalMatrix = glm::mat3(glm::inverseTranspose(view * model3));
    normalMatrixLoc = glGetUniformLocation(myBasicShader.shaderProgram, "normalMatrix");

    // create projection matrix
    projection = glm::perspective(glm::radians(45.0f),
        (float)myWindow.getWindowDimensions().width / (float)myWindow.getWindowDimensions().height,
        0.1f, 800.0f);
    projectionLoc = glGetUniformLocation(myBasicShader.shaderProgram, "projection");
    // send projection matrix to shader
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));



    //set the light direction (direction towards the light)
    lightDir = glm::vec3(15.76f, 17.62f, 15.34f);
    //lightDirLoc = glGetUniformLocation(myBasicShader.shaderProgram, "lightDir");
   // lightDir = glm::vec3(-15.76f, 1.32f, 63.54f);
    lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    lightDirLoc = glGetUniformLocation(myBasicShader.shaderProgram, "lightDir");
    //glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));
    // send light dir to shader
    glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDir));

    //set light color
    lightColor = glm::vec3(1.0f, 1.0f, 1.0f); //white light
    lightColorLoc = glGetUniformLocation(myBasicShader.shaderProgram, "lightColor");
    // send light color to shader
    glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

    int frogLoc = glGetUniformLocation(myBasicShader.shaderProgram, "frog");
    glUniform3fv(frogLoc, 1, glm::value_ptr(frog));

    


    //set light color
    lightColor = glm::vec3(0.5f,0.5f, 0.5f); //white light
    
    lightColorLoc = glGetUniformLocation(myBasicShader.shaderProgram, "lightColor");
    // send light color to shader
    glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));


    //fulger

     
    fulger = glm::vec3(0.05f, 0.05f, 0.0f);
    fulgerLoc = glGetUniformLocation(myBasicShader.shaderProgram, "fulger");
    glUniform3fv(fulgerLoc, 1, glm::value_ptr(fulger));
   


    lightShader.useShaderProgram();
    glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    gunLightLoc = glGetUniformLocation(myBasicShader.shaderProgram, "gunLight");
    glUniform3fv(gunLightLoc, 1, glm::value_ptr(gunLight));

}


void initFBO() {
    //TODO - Create the FBO, the depth texture and attach the depth texture to the FBO
    glGenFramebuffers(1, &shadowMapFBO);
    glGenTextures(1, &depthMapTexture);
    glBindTexture(GL_TEXTURE_2D, depthMapTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture,
        0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}


glm::mat4 computeLightSpaceTrMatrix() {
   
    //start : -15.76f, 7.62f, 55.54f
    //stop :  -15.76f, 1.31f, 73.34f
    if (lightOK == 0)
    {
        lightView = glm::lookAt(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir, glm::vec3(-15.76f, 1.31f, 73.34f), glm::vec3(0.0f, 1.0f, 0.0f));
        lightOK = 1;
    }
    const GLfloat near_plane = 0.1f, far_plane = 68.0f;
    glm::mat4 lightProjection = glm::ortho(-7.0f, 6.5f, -6.5f, 6.5f, near_plane, far_plane);
    glm::mat4 lightSpaceTrMatrix = lightProjection * lightView;

    return lightSpaceTrMatrix;
}


void renderTeapot(gps::Shader shader, bool depthPass) {
    // select active shader program
    shader.useShaderProgram();
    
    model = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
    //send teapot model matrix data to shader
   // glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

    //send teapot normal matrix data to shader
    //glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    if (!depthPass) {
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    }
    // draw teapot
   // teapot.Draw(shader);

    // draw scena
    
    model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

    // do not send the normal matrix if we are rendering in the depth map
    if (!depthPass) {
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    }
   
    rulota.Draw(shader);
    //shader.useShaderProgram();

    //modelGate = glm::rotate(glm::mat4(1.0f), glm::radians(angleGate), glm::vec3(0.0f, 1.0f, 0.0f));
    //glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    shader.useShaderProgram();

    //send teapot model matrix data to shader

     // update model matrix for teapot

    //modelGate = glm::rotate(model, angleGate, glm::vec3(0, 1, 0));

    modelGate = glm::mat4(1.0f);

    modelGate = glm::translate(modelGate, glm::vec3(77.79, 2.62, 19.72));
    //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelGate));
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelGate));


    modelGate = glm::rotate(modelGate, angleGate, glm::vec3(0, 1, 0));
    //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelGate));
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelGate));

    modelGate = glm::translate(modelGate, glm::vec3(-77.79, -2.62, -19.72));
    //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelGate));
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelGate));


    //send teapot normal matrix data to shader
    //glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    if (!depthPass) {
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    }
    gate.Draw(shader);

    //gate2

    modelGate2 = glm::mat4(1.0f);

    modelGate2 = glm::translate(modelGate2, glm::vec3(77.79, 2.62, 27.138));
    //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelGate2));
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelGate2));

    modelGate2 = glm::rotate(modelGate2, -angleGate, glm::vec3(0, 1, 0));
    //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelGate2));
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelGate2));


    modelGate2 = glm::translate(modelGate2, glm::vec3(-77.79, -2.62, -27.138));
    //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelGate2));
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelGate2));

   
    if (!depthPass) {
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    }
    gate2.Draw(shader);

    //Elice

    modelElice = glm::mat4(1.0f);
    

    modelElice = glm::translate(modelElice, glm::vec3(-59.89 + helicopterMove, 7.029, 23.189 + helicopterMoveZ));
    //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelElice));
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelElice));


    modelElice = glm::rotate(modelElice, -angleElice, glm::vec3(0, 1, 0));
    //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelElice));
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelElice));

    modelElice = glm::translate(modelElice, glm::vec3(59.89 - helicopterMove, -7.029, -23.189 - helicopterMoveZ));
    //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelElice));
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelElice));

    modelElice = glm::translate(modelElice, glm::vec3(0.0f, helicopterZ, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelElice));
    modelElice = glm::translate(modelElice, glm::vec3(helicopterMove, 0, helicopterMoveZ));
   
   
    //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelElice));
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelElice));

    
    if (!depthPass) {
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    }
    elice.Draw(shader);

    //helicopter
    modelHelicopter = glm::mat4(1.0f);

    modelHelicopter = glm::translate(modelHelicopter, glm::vec3(helicopterMove, helicopterZ, helicopterMoveZ));
    //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelHelicopter));
   
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelHelicopter));
    helicopter.Draw(shader);


    
    //EliceMica

    modelElice = glm::mat4(1.0f);

    modelElice = glm::translate(modelElice, glm::vec3(-75.963 + helicopterMove, 7.51 + helicopterZ, 22.322));
    //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelElice));
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelElice));


    modelElice = glm::rotate(modelElice, -angleElice, glm::vec3(0, 0, 1));
    //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelElice));
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelElice));

    modelElice = glm::translate(modelElice, glm::vec3(75.963 - helicopterMove, -7.51 - helicopterZ, -22.322));
    //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelElice));
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelElice));

    modelElice = glm::translate(modelElice, glm::vec3(0.0f, helicopterZ, 0.0f));
   // glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelElice));
    modelElice = glm::translate(modelElice, glm::vec3(helicopterMove, 0.0f, helicopterMoveZ));
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelElice));


    if (!depthPass) {
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    }
    eliceMica.Draw(shader);

    //gun and target
    shader.useShaderProgram();

    model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));  
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

    if (!depthPass) {
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    }
    
    gunAndTarget.Draw(shader);

    //muzzle flash
    shader.useShaderProgram();

    model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

    if (!depthPass) {
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    }

  

    //holes
    shader.useShaderProgram();

    model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

    if (!depthPass) {
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    }

    if(holeNumber > 0 )
        hole1.Draw(shader);
    if (holeNumber > 1)
    hole2.Draw(shader);
    
    if (holeNumber > 2)
    hole3.Draw(shader);
    
    if (holeNumber > 3)
    hole4.Draw(shader);

    if (holeNumber == 6)
        holeNumber = 0;

    //tragaci
    shader.useShaderProgram();

    model = glm::mat4(1.0f);
    if (tragaciState == 1)
    {
      

        if (angleTragaci < 0.5f)
            angleTragaci += 0.02f;
        else
        {
            tragaciState = 2;
            
            holeNumber++;
            PlaySound(TEXT("preview.wav"), NULL, SND_ASYNC);
        }

        if (angleTragaci > 0.4f)
        {
            muzzleFlash.Draw(shader);
            gunLight.x = 1.0f;
            
        }
        

    }

    if (tragaciState == 2)
    {
        gunLight.x = 0.0f;

        if (angleTragaci > 0.0f)
            angleTragaci -= 0.02f;
        else
            tragaciState = 0;
    }
        model = glm::translate(model, glm::vec3(-6.987f, 1.39f, -9.344f));
        //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelElice));
        glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));


        model = glm::rotate(model, -angleTragaci, glm::vec3(1, 0, 0));
        //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelElice));
        glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

        model = glm::translate(model, glm::vec3(6.987f, -1.39f, 9.344f));
        //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelElice));
        glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

      
    

    if (!depthPass) {
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
        gunLightLoc = glGetUniformLocation(myBasicShader.shaderProgram, "gunLight");
        glUniform3fv(gunLightLoc, 1, glm::value_ptr(gunLight));
    }
    tragaci.Draw(shader);

    model = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    if (!depthPass) {
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    }
   
    cort.Draw(shader);

    model = glm::mat4(1.0f);
   
    glm::vec3 camCoord = myCamera.getCameraPosition();
    if (driving )
    {   
        
        camCoord.y = 2.03f;
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        myCamera.setCameraPosition(camCoord);
        
        model = glm::translate(model, glm::vec3(149.82f + camCoord.x - masinaCoord.x,1.84f,23.1f + camCoord.z - masinaCoord.z));
        model = glm::rotate(model, -pitch/60 + 9.3f, glm::vec3(0, 1, 0));
        model = glm::translate(model, glm::vec3(-149.82f -(camCoord.x - masinaCoord.x), -1.84f, -23.1f - (camCoord.z - masinaCoord.z)));

        model = glm::translate(model, glm::vec3(camCoord.x - masinaCoord.x, 0, camCoord.z - masinaCoord.z));
        
        
        masinaLastCoord.x = camCoord.x - masinaCoord.x;
         masinaLastCoord.y = 0.0f;
         masinaLastCoord.z = camCoord.z - masinaCoord.z;

         lastRotate = -pitch / 60 + 9.3f;

         lastView.x = camCoord.x;
         lastView.y = camCoord.y;
         lastView.z = camCoord.z;
        
    }
    else {
        model = glm::translate(model, glm::vec3(149.82f + lastView.x - masinaCoord.x, 1.84f, 23.1f + lastView.z - masinaCoord.z));
        model = glm::rotate(model,lastRotate, glm::vec3(0, 1, 0));
        model = glm::translate(model, glm::vec3(-149.82f - (lastView.x - masinaCoord.x), -1.84f, -23.1f - (lastView.z - masinaCoord.z)));
        model = glm::translate(model, glm::vec3(masinaLastCoord));
    }
   
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
   
    if (!depthPass) {
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    }

    masina.Draw(shader);

    


}
void renderTree(gps::Shader shader) {

    treeShader.useShaderProgram();
    glUniformMatrix4fv(glGetUniformLocation(treeShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));




    glUniformMatrix4fv(glGetUniformLocation(treeShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model3));


    glUniformMatrix4fv(glGetUniformLocation(treeShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model3));
    tree.Draw(treeShader);

    // glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
     //glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix)); 
     //tree.Draw(shader);





}
void renderScene() {
   // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    depthMapShader.useShaderProgram();
    glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "lightSpaceTrMatrix"),
        1,
        GL_FALSE,
        glm::value_ptr(computeLightSpaceTrMatrix()));
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    renderTeapot(depthMapShader, true);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    glViewport(0, 0, retina_width, retina_height);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    myBasicShader.useShaderProgram();

    view = myCamera.getViewMatrix();
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
   // glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));

    //bind the shadow map
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, depthMapTexture);
    glUniform1i(glGetUniformLocation(myBasicShader.shaderProgram, "shadowMap"), 3);

    glUniformMatrix4fv(glGetUniformLocation(myBasicShader.shaderProgram, "lightSpaceTrMatrix"),
        1,
        GL_FALSE,
        glm::value_ptr(computeLightSpaceTrMatrix()));

    renderTeapot(myBasicShader, false);

    //draw a white cube around the light
    
    lightShader.useShaderProgram();

    glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

    model = lightRotation;
    model = glm::translate(model, 1.0f * lightDir);
    model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
    glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

    //lightCube.Draw(lightShader);

    //renderTeapot(myBasicShader);

    //renderTree(treeShader);

    //automatic open
    if (manualGate == 0)
    {
        if (myCamera.getCameraPosition().x > 66 && myCamera.getCameraPosition().x < 90 && myCamera.getCameraPosition().z < 27 && myCamera.getCameraPosition().z > 20 && myCamera.getCameraPosition().y > -1 && myCamera.getCameraPosition().y < 7)
            automatic = 1;
        else automatic = 2;
    }
    else automatic = 0;

    if(automatic == 1)
        if (angleGate < 2.2f)
            angleGate += 0.03f;
    if(automatic == 2 )
        if (angleGate > 0.0f)
            angleGate -= 0.03f;

    //fulger
    if (fulgerState == 1)
    {
        if (fulger.x < 0.5)
            fulgerState = 2;

        myBasicShader.useShaderProgram();
        fulger.x -= 0.5f;
        fulger.y -= 0.5f;
        fulgerLoc = glGetUniformLocation(myBasicShader.shaderProgram, "fulger");
        glUniform3fv(fulgerLoc, 1, glm::value_ptr(fulger));

    }

    if (fulgerState == 2)
    {
        if (fulger.x > 2.5)
            fulgerState = 3;

        myBasicShader.useShaderProgram();
        fulger.x += 0.3f;
        fulger.y += 0.3f;
        fulgerLoc = glGetUniformLocation(myBasicShader.shaderProgram, "fulger");
        glUniform3fv(fulgerLoc, 1, glm::value_ptr(fulger));

    }

    if (fulgerState == 3)
    {   
        fulgerState = 0;

        myBasicShader.useShaderProgram();
        fulger.x = 0.05f;
        fulger.y = 0.05f;
        fulgerLoc = glGetUniformLocation(myBasicShader.shaderProgram, "fulger");
        glUniform3fv(fulgerLoc, 1, glm::value_ptr(fulger));
    }

    

}

void cleanup() {
    myWindow.Delete();
    //cleanup code for your own data
}

int main(int argc, const char* argv[]) {

    

   
    
     

    try {
        initOpenGLWindow();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    initOpenGLState();
    initModels();
    initShaders();
    initUniforms();
    initFBO();
    setWindowCallbacks();

    // compute normal matrix for teapot
    normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    glCheckError();
    // application loop
    while (!glfwWindowShouldClose(myWindow.getWindow())) {
        processMovement();
        renderScene();

        glfwPollEvents();
        glfwSwapBuffers(myWindow.getWindow());

        glCheckError();
    }

    cleanup();

    return EXIT_SUCCESS;
}
