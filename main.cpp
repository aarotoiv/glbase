#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <stdlib.h>
#include <cmath>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CommonValues.h"

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "Model.h"

// Screen sizes
#define SCREEN_SIZE_X 1366
#define SCREEN_SIZE_Y 768

Window mainWindow;

std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;
Shader directionalShadowShader;

Camera camera;

// Create textures
Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;

Model xwing;
Model blackhawk;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

Material shinyMaterial;

const float toRadians = 3.14159265f / 180.0f;

GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
uniformSpecularIntensity = 0, uniformShininess = 0;

unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;

// Vertex Shader
static const char* vShader = "./shaders/shader.vert";
// Fragment Shader
static const char* fShader = "./shaders/shader.frag";

void calcNormals(unsigned int *indices, unsigned int indiceCount, GLfloat *vertices, unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset) {
    for (size_t i = 0; i < indiceCount; i += 3) {
        unsigned int in0 = indices[i] * vLength;
        unsigned int in1 = indices[i + 1] * vLength;
        unsigned int in2 = indices[i + 2] * vLength;
        glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
        glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
        glm::vec3 normal = glm::cross(v1, v2);
        normal = glm::normalize(normal);

        in0 += normalOffset;
        in1 += normalOffset;
        in2 += normalOffset;

        vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
        vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
        vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
    }

    for (size_t i = 0; i < verticeCount / vLength; i++) {
        unsigned int nOffset = i * vLength + normalOffset;
        glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
        vec = glm::normalize(vec);
        vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
    }
}

void createTriangle() {
    // Create indices
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };
    // 3 per row for vertex coords, 2 texture UV values, 3 for normals
	GLfloat vertices[] = {
		-1.0f, -1.0f, -0.6f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, -1.0f, 1.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -0.6f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f
	};

    unsigned int floorIndices[] = {
        0, 2, 1,
        1, 2, 3
    };

    GLfloat floorVertices[] = {
        -10.0f, 0.0f, -10.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        10.0f, 0.0f, -10.0f, 10.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        -10.0f, 0.0f, 10.0f, 0.0f, 10.0f, 0.0f, -1.0f, 0.0f,
        10.0f, 0.0f, 10.0f, 10.0f, 10.0f, 0.0f, -1.0f, 0.0f
    };


    calcNormals(indices, 12, vertices, 32, 8, 5);

	Mesh *obj = new Mesh();
    obj->createMesh(vertices, indices, 32, 12);
    meshList.push_back(obj);

    Mesh *obj2 = new Mesh();
    obj2->createMesh(floorVertices, floorIndices, 32, 6);
    meshList.push_back(obj2);
}

void createShader() {
    Shader *shader = new Shader();
    shader->createFromFiles(vShader, fShader);
    shaderList.push_back(shader);

    directionalShadowShader = Shader();
    shader->createFromFiles("./shaders/directional_shadow_map.vert", "./shaders/directional_shadow_map.frag");
}

void renderScene() {
    // TRIANGLE
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    // USE TEXTURE
    brickTexture.useTexture();
    // Use material
    shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
    meshList[0]->renderMesh();

    // FLOOR
    model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    // USE TEXTURE
    plainTexture.useTexture();
    // Use material
    shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
    meshList[1]->renderMesh();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-15.0f, -2.0f, 15.0f));
    model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
    xwing.renderModel();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-5.0f, -2.0f, 2.0f));
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
    model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
    blackhawk.renderModel();
}

void directionalShadowMapPass(DirectionalLight *light) {
    directionalShadowShader.useShader();
    
    glViewport(0, 0, light->getShadowMap()->getShadowWidth(), light->getShadowMap()->getShadowHeight());
    
    light->getShadowMap()->write();
    glClear(GL_DEPTH_BUFFER_BIT);

    uniformModel = directionalShadowShader.getModelLocation();
    
    glm::mat4 val = light->calculateLightTransform();

    directionalShadowShader.setDirectionalLightTransform(&val);

    renderScene();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void renderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix) {
    shaderList[0]->useShader();
    uniformModel = shaderList[0]->getModelLocation();
    uniformProjection = shaderList[0]->getProjectionLocation();
    uniformView = shaderList[0]->getViewLocation();
    uniformEyePosition = shaderList[0]->getEyePositionLocation();
    uniformSpecularIntensity = shaderList[0]->getSpecularIntensityLocation();
    uniformShininess = shaderList[0]->getShininessLocation();

    glViewport(0, 0, 1366, 768);

    // Drawings here
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //spotLights[0].setFlash(camera.getCameraPosition(), camera.getCameraDirection());

    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

    shaderList[0]->setDirectionalLight(&mainLight);
    shaderList[0]->setPointLights(pointLights, pointLightCount);
    shaderList[0]->setSpotLights(spotLights, spotLightCount);
    glm::mat4 val = mainLight.calculateLightTransform();
    shaderList[0]->setDirectionalLightTransform(&val);

    mainLight.getShadowMap()->read(GL_TEXTURE1);
    shaderList[0]->setTexture(0);
    shaderList[0]->setDirectionalShadowMap(1);

    renderScene();
}
 
int main (int argc, char* argv[])
{   

    mainWindow = Window(SCREEN_SIZE_X, SCREEN_SIZE_Y);
    mainWindow.initialize();
    
    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double deltaTime = 0;
    
    // Create stuff
    createTriangle();
    createShader();

    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 0.01f, 1.0f);

    brickTexture = Texture("./textures/brick.png");
    brickTexture.loadTextureA();
    dirtTexture = Texture("./textures/dirt.png");
    dirtTexture.loadTextureA();
    plainTexture = Texture("./textures/plain.png");
    plainTexture.loadTextureA();

    xwing = Model();
    xwing.loadModel("models/x-wing.obj");
    blackhawk = Model();
    blackhawk.loadModel("models/uh60.obj");


    shinyMaterial = Material(1.0f, 32.0f);
    mainLight = DirectionalLight(
    1024, 1024,
    1.0f, 1.0f, 1.0f,
    0.3f, 0.6f,
    2.0f, 1.0f, -2.0f);

    pointLights[0] = PointLight(0.0f, 0.0f, 1.0f, 
                                0.0f, 0.1f,
                                4.0f, 0.0f, 0.0f,
                                0.3f, 0.2f, 0.1f);
    pointLightCount++;

    pointLights[1] = PointLight(0.0f, 1.0f, 0.0f, 
                                0.0f, 0.1f,
                                -4.0f, 2.0f, 0.0f,
                                0.3f, 0.1f, 0.1f);
    pointLightCount++;

    spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f, 
                            0.0f, 1.0f,
                            0.0f, 0.0f, 0.0f,
                            0.0f, -1.0f, 0.0f,
                            1.0f, 0.0f, 0.0f, 20.0f);
    spotLightCount++;

    spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f, 
                            0.0f, 1.0f,
                            0.0f, 1.5f, 0.0f,
                            -5.0f, -1.0f, 0.0f,
                            1.0f, 0.0f, 0.0f, 20.0f);
    spotLightCount++;

    

    glm::mat4 projection = glm::perspective(45.0f, (GLfloat) mainWindow.getBufferWidth() / (GLfloat) mainWindow.getBufferHeight(), 0.1f, 100.0f);

    // Game loop
    while (!mainWindow.getShouldClose()) {
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();

        deltaTime = (double)((NOW - LAST)*1000 / (double)SDL_GetPerformanceFrequency());
        //std::cout << deltaTime << std::endl;
        mainWindow.pollEvent();

        
        camera.keyControl(mainWindow.getKeys(), deltaTime);
        camera.mouseControl(mainWindow.getMouseX(), mainWindow.getMouseY());
        
        directionalShadowMapPass(&mainLight);
        renderPass(projection, camera.calculateViewMatrix());

        glUseProgram(0);

        mainWindow.swapBuffers();
    }

    return 0;
}