#include "lab_m1/Tema3/Tema3.h"

#include <vector>
#include <string>
#include <iostream>
#include <random>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema3::Tema3()
{
}


Tema3::~Tema3()
{
}

void Tema3::deleteObjectsScene(int& i, int type)
{
    std::pair<glm::vec3, int> pos = renderObjects[i];
    int val = 25;
    if (pos.first.x <= position.x - 2 * val || pos.first.x >= position.x + 2 * val || pos.first.z <= position.z - val) {
        if (i < renderObjects.size())
        renderObjects.erase(renderObjects.begin() + i);
        i--;
        if (type == 3) {
            lightDir.erase(lightDir.begin());
            lightDir.erase(lightDir.begin());
            lightPos.erase(lightPos.begin());
            lightPos.erase(lightPos.begin());
        }

        if (type == 0 || type == 1) {
            pointLightPos.erase(pointLightPos.begin());
            pointLightCol.erase(pointLightCol.begin());
        }
    }
}

void Tema3::decisionCollision(int type, std::pair<glm::vec3, int> pos, int& i)
{
    switch (type)
    {
    case 0:
    {
        if (
            pos.first.z < position.z + 1 &&
            pos.first.z + 1 > position.z && pos.first.x < position.x + 1 &&
            pos.first.x + 1 > position.x
            )
            if (!gameOver)
                finalGame();
    }
    break;
    case 1:
    {
        if (
            pos.first.z < position.z + 1 &&
            pos.first.z + 2 > position.z && pos.first.x < position.x + 1 &&
            pos.first.x + 2 > position.x
            )
        {
            if(i < renderObjects.size())
            renderObjects.erase(renderObjects.begin() + i);
            vector<glm::vec3>::iterator position = find(pointLightPos.begin(), pointLightPos.end(), pos.first + glm::vec3(0, 1.25f, 0));
            int index = std::distance(pointLightPos.begin(), position);
            pointLightPos.erase(position);
            if(index < pointLightCol.size())
            pointLightCol.erase(pointLightCol.begin() + index);
            score++;
            i--;
        }
    }
    break;
    case 2:
    {
        if (
            pos.first.z < position.z + 1 &&
            pos.first.z + 2 > position.z && pos.first.x < position.x + 1 &&
            pos.first.x + 2.5f > position.x
            )
            if (!gameOver)
                finalGame();
    }
    break;
    case 3:
    {
        if (
            pos.first.z < position.z + 1 &&
            pos.first.z + 0.5f > position.z && pos.first.x < position.x + 1 &&
            pos.first.x + 0.5f > position.x
            )
            if (!gameOver)
                finalGame();
    }
    break;
    default:
    {

    }
   }
}

void Tema3::renderRocks()
{
    for (int i = 0; i < renderObjects.size(); i++) {
        std::pair<glm::vec3, int> pos = renderObjects[i];

        if (pos.second == 2)
        {
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, pos.first + glm::vec3(0.5f, 0, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(pos.second));
                RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, glm::vec3(1), mapTextures["rock"]);
            }

            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, pos.first + glm::vec3(1.5f, 0, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(pos.second));
                RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, glm::vec3(1), mapTextures["rock"]);

            }

            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, pos.first);
                modelMatrix = glm::scale(modelMatrix, glm::vec3(pos.second));
                RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, glm::vec3(1), mapTextures["rock"]);
            }

            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, pos.first + glm::vec3(2.5f, 0, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(pos.second));
                RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, glm::vec3(1), mapTextures["rock"]);
            }
            int dummy;
            decisionCollision(pos.second, pos, dummy);
        }
        deleteObjectsScene(i, pos.second);
    }
}

void Tema3::renderGifts()
{
    for (int i = 0; i < renderObjects.size(); i++) {
        std::pair<glm::vec3, int> pos = renderObjects[i];
        if (pos.second == 1)
        {
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, pos.first + glm::vec3(0, 1.25f, 0));
                modelMatrix = glm::rotate(modelMatrix, RADIANS(angle), glm::vec3(1, 0, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(pos.second + 1));
                RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(1), mapTextures["gift"]);
            }

            if (find(pointLightPos.begin(), pointLightPos.end(), pos.first + glm::vec3(0, 1.25f, 0)) == pointLightPos.end()) {
                pointLightPos.push_back(pos.first + glm::vec3(0, 1.25f, 0));
                float red = (float)(rand() % 80 + 10) / 100;
                float green = (float)(rand() % 80 + 10) / 100;
                float blue = (float)(rand() % 80 + 10) / 100;
                pointLightCol.push_back(glm::vec3(red, green, blue));
            }

            decisionCollision(pos.second, pos, i);
        }
        deleteObjectsScene(i, pos.second);
    }

}

void Tema3::renderLight()
{
    for (int i = 0; i < renderObjects.size(); i++) {
        std::pair<glm::vec3, int> pos = renderObjects[i];
        if (pos.second == 3)
        {
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, pos.first + glm::vec3(0, 4.5f, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 10.f, 0.5f));
                RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(1), mapTextures["lightpole"]);
            }

            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, pos.first + glm::vec3(0, 9.75f, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(5, 0.5f, 0.5f));
                RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(1), mapTextures["lightpole"]);
            }

            if (find(lightPos.begin(), lightPos.end(), pos.first + glm::vec3(2.5f, 9.75f, 0)) == lightPos.end()) {
                int var = 1;
                lightPos.push_back(pos.first + glm::vec3(2.5f, 9.75f, 0));
                lightPos.push_back(pos.first + glm::vec3(-2.5f, 9.75f, 0));
                lightDir.push_back(glm::vec3(var / 2, -var, 0));
                lightDir.push_back(glm::vec3(-var / 2, -var, 0));
            }
            int dummy;
            decisionCollision(pos.second, pos, dummy);
        }
        deleteObjectsScene(i, pos.second);
    }
}

void Tema3::renderTrees()
{
    for (int i = 0; i < renderObjects.size(); i++) {
        std::pair<glm::vec3, int> pos = renderObjects[i];
        if (pos.second == 0)
        {
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, pos.first + glm::vec3(0, 1.5f, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 4.f, 1));
                RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(1), mapTextures["tree1"]);
            }

            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, pos.first + glm::vec3(0, 4, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(2));
                RenderSimpleMesh(meshes["cone"], shaders["LabShader"], modelMatrix, glm::vec3(1), mapTextures["tree"]);
            }

            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, pos.first + glm::vec3(0, 6.5, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(1.5));
                RenderSimpleMesh(meshes["cone"], shaders["LabShader"], modelMatrix, glm::vec3(1), mapTextures["tree"]);
            }

            if (find(pointLightPos.begin(), pointLightPos.end(), pos.first + glm::vec3(0, 1.5f, 0)) == pointLightPos.end()) {
                float red = (float)(rand() % 65 + 10) / 100;
                float green = (float)(rand() % 65 + 10) / 100;
                float blue = (float)(rand() % 65 + 10) / 100;
                pointLightPos.push_back(pos.first + glm::vec3(0, 1.5f, 0));
                pointLightCol.push_back(glm::vec3(red, green, blue));
            }
            int dummy;
            decisionCollision(pos.second, pos, dummy);
        }
        deleteObjectsScene(i, pos.second);
    }
}

void Tema3::Init()
{
    {
        counter = 0;

        string textureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "textures");

        {
            Texture2D* texture = new Texture2D();
            texture->Load2D(PATH_JOIN(textureDir, "snow.jpg").c_str(), GL_REPEAT);
            mapTextures["snow"] = texture;
        }

        {
            Texture2D* texture = new Texture2D();
            texture->Load2D(PATH_JOIN(textureDir, "ski.jpg").c_str(), GL_REPEAT);
            mapTextures["tool"] = texture;
        }

        {
            Texture2D* texture = new Texture2D();
            texture->Load2D(PATH_JOIN(textureDir, "gift.jpg").c_str(), GL_REPEAT);
            mapTextures["gift"] = texture;
        }

        {
            Texture2D* texture = new Texture2D();
            texture->Load2D(PATH_JOIN(textureDir, "rock1.jpg").c_str(), GL_CLAMP_TO_EDGE);
            mapTextures["rock"] = texture;
        }

        {
            Texture2D* texture = new Texture2D();
            texture->Load2D(PATH_JOIN(textureDir, "earth.png").c_str(), GL_CLAMP_TO_EDGE);
            mapTextures["gamePlayer"] = texture;
        }

        {
            Texture2D* texture = new Texture2D();
            texture->Load2D(PATH_JOIN(textureDir, "tree1.png").c_str(), GL_REPEAT);
            mapTextures["tree1"] = texture;
        }

        {
            Texture2D* texture = new Texture2D();
            texture->Load2D(PATH_JOIN(textureDir, "light.jpg").c_str(), GL_REPEAT);
            mapTextures["lightpole"] = texture;
        }

        {
            Texture2D* texture = new Texture2D();
            texture->Load2D(PATH_JOIN(textureDir, "tree.jpg").c_str(), GL_CLAMP_TO_EDGE);
            mapTextures["tree"] = texture;
        }
    }

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("cone");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "cone.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a simple quad
    {
        vector<glm::vec3> vertices
        {
            glm::vec3(0.5f,   0.0f, 0.5f),    // top right
            glm::vec3(0.5f,   0.0f, -0.5f),    // bottom right
            glm::vec3(-0.5f,  0.0f, -0.5f),    // bottom left
            glm::vec3(-0.5f,  0.0f, 0.5f),    // top left
        };

        vector<glm::vec3> normals
        {
            glm::vec3(0, 1, 1),
            glm::vec3(1, 0, 1),
            glm::vec3(1, 0, 0),
            glm::vec3(0, 1, 0)
        };

        vector<glm::vec2> textureCoords
        {
            glm::vec2(0.0f, 0.0f),
            glm::vec2(0.f, 1.f),
            glm::vec2(1.f, 1.f),
            glm::vec2(1.f, 0.f),

        };

        vector<unsigned int> indices =
        {
            0, 1, 3,
            1, 2, 3
        };

        Mesh* mesh = new Mesh("square");
        mesh->InitFromData(vertices, normals, textureCoords, indices);
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        second_camera = new implemented::CameraT3();
        int val = 10;
        second_camera->Set(glm::vec3(0, val, 25.f), glm::vec3(0, -val / 2, 0), glm::vec3(0, 1, 0));
        camera = new implemented::CameraT3();
        camera->Set(glm::vec3(0, 1, val), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
        fov = RADIANS(60);
        left = -8.f;
        right = -left;
        bottom = -4.5f;
        top = -bottom;
        zNear = 0.01f;
        zFar = 200.f;
    }

    {
        angle = 30.f;
        speed = 14.5f;
        scale = 75.f;
        score = 0;
        gameOver = false;
        slope = -tan(RADIANS(angle));
        direction = glm::normalize(glm::vec3(0, slope, 1));
        position = glm::vec3(0, 1.2f, 0);
    }

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader* shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Light & material properties
    {

        lightDirec = glm::vec3(0, -1, 0);
        materialShininess = 60;
        materialKd = 0.5;
        materialKs = 0.5;
    }

    projectionMatrix = glm::perspective(fov, window->props.aspectRatio, zNear, zFar);
    main_camera = second_camera;
    begin = std::chrono::high_resolution_clock::now();
}

void Tema3::renderPlayer(float var)
{
    {
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, position);
            modelMatrix = glm::rotate(modelMatrix, RADIANS(angle), glm::vec3(1, 0, 0));
            modelMatrix = glm::rotate(modelMatrix, atan(direction.x / direction.z), glm::vec3(0, 1, 0));
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0.25f, -1.f, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f, 0.1f, var + 0.8));
            RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(1), mapTextures["tool"]);
        }

        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, position);
            modelMatrix = glm::rotate(modelMatrix, RADIANS(angle), glm::vec3(1, 0, 0));
            modelMatrix = glm::rotate(modelMatrix, atan(direction.x / direction.z), glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(1, var + 0.8, var - 0.2));
            RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(1), mapTextures["gamePlayer"]);
        }

        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, position);
            modelMatrix = glm::rotate(modelMatrix, RADIANS(angle), glm::vec3(1, 0, 0));
            modelMatrix = glm::rotate(modelMatrix, atan(direction.x / direction.z), glm::vec3(0, 1, 0));
            modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.25f, -1.f, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f, 0.1f, var + 0.8));
            RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(1), mapTextures["tool"]);
        }
    }
}

void Tema3::prepareRender()
{
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - begin;
    if (diff.count() >= 0.35f)
    {
        begin = std::chrono::high_resolution_clock::now();
        int type;
        float x, y, z;
        prepareObject(type, x, y, z);

        glm::vec3 pos = glm::vec3(x, y, z);
        pair<glm::vec3, int> elem = { pos, type };
        renderObjects.push_back(elem);
    }
}

void Tema3::prepareObject(int& type, float& x, float& y, float& z)
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> dist4(0, 4);
    type = dist4(rng);
    int var = 61, var1 = 15, var2 = 95;
    int aux = rand();
    x = position.x + (aux % var - var1 + ((aux % 307) - var2) / 10);
    z = second_camera->position.z + bottom;
    y = -tan(RADIANS(angle)) * z;
}


void Tema3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.8, 0.8, 0.8, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema3::finalGame() {
    cout << "Your score: " << score << "\n";
    cout << "Spacebar -> restart!.\n";
    speed = 0;
    gameOver = true;
}

void Tema3::Update(float deltaTimeSeconds)
{

    float var = 1.2f;

    renderPlayer(var);

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, position - glm::vec3(0, var, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(angle), glm::vec3(var - 0.2, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));
        RenderSimpleMesh(meshes["square"], shaders["LabShader"], modelMatrix, glm::vec3(1), mapTextures["snow"]);

    }

    renderRocks();

    renderTrees();

    renderGifts();

    renderLight();

    if (!gameOver) {
        position += speed * direction * deltaTimeSeconds;

        second_camera->Set(glm::vec3(0, 10, 25.f) + position, glm::vec3(0, -5, 0) + position, glm::vec3(0, 1, 0));

        {
            prepareRender();
        }
    }

}


void Tema3::FrameEnd()
{
 
}


void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, Texture2D* texture)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    if (lightPos.size()) {
        int lightPositions = glGetUniformLocation(shader->program, "lightPositions");
        glUniform3fv(lightPositions, lightPos.size(), glm::value_ptr(lightPos[0]));

        int lightDirections = glGetUniformLocation(shader->program, "lightDirections");
        glUniform3fv(lightDirections, lightDir.size(), glm::value_ptr(lightDir[0]));
    }

    if (pointLightPos.size()) {

        int lightColors = glGetUniformLocation(shader->program, "pointLightColors");
        glUniform3fv(lightColors, pointLightCol.size(), glm::value_ptr(pointLightCol[0]));

        int lightPositions = glGetUniformLocation(shader->program, "pointLightPositions");
        glUniform3fv(lightPositions, pointLightPos.size(), glm::value_ptr(pointLightPos[0]));

    }
    int pointLights = glGetUniformLocation(shader->program, "pointLights");
    glUniform1i(pointLights, pointLightPos.size());

    int spotLights = glGetUniformLocation(shader->program, "spotLights");
    glUniform1i(spotLights, lightPos.size());

    int lightDirection = glGetUniformLocation(shader->program, "lightDirection");
    glUniform3f(lightDirection, lightDirec.x, lightDirec.y, lightDirec.z);

    // Set eye position (camera position) uniform
    glm::vec3 eyePosition = main_camera->position;
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

    // Set material property uniforms (shininess, kd, ks, object color) 
    int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(material_shininess, materialShininess);

    int material_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(material_kd, materialKd);

    int material_ks = glGetUniformLocation(shader->program, "material_ks");

    int slopeLocation = glGetUniformLocation(shader->program, "slope");
    glUniform1f(slopeLocation, abs(slope));

    glUniform1f(material_ks, materialKs);

    int object_color = glGetUniformLocation(shader->program, "object_color");
    glUniform3f(object_color, color.r, color.g, color.b);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = main_camera->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    if (texture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "textureObj"), 0);

        int textCoord = glGetUniformLocation(shader->program, "textCoord");
        int var = 10;
        if (strcmp(mesh->GetMeshID(), "square") == 0)
        {

            glUniform2f(textCoord, position.x / scale,
                position.z / (scale - var));
        }
        else
        {
            glUniform2f(textCoord, 0, 0);
        }
    }

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema3::OnInputUpdate(float deltaTime, int mods)
{
    float cameraSpeed = 10.0f;

    if (window->KeyHold(GLFW_KEY_W)) {
        // TODO(student): Translate the camera forward
        camera->MoveForward(cameraSpeed * deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        // TODO(student): Translate the camera to the left
        camera->TranslateRight(-cameraSpeed * deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        // TODO(student): Translate the camera backward
        camera->MoveForward(-cameraSpeed * deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        // TODO(student): Translate the camera to the right
        camera->TranslateRight(cameraSpeed * deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_Q)) {
        // TODO(student): Translate the camera downward
        camera->TranslateUpward(-cameraSpeed * deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_E)) {
        // TODO(student): Translate the camera upward
        camera->TranslateUpward(cameraSpeed * deltaTime);
    }
}


void Tema3::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_SPACE)
        if(gameOver)
          Init();

}


void Tema3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    float senX = 0.01f;
    float senY = 0.01f;

    if (!gameOver) {
        float mouseInX = (left - 2 - direction.y * 10) + ((float)mouseX / window->GetResolution().x)
            * (abs(left - 2 - direction.y * 10) + right + 2 + direction.y * 10);
        direction.x = mouseInX / 10;
        direction = glm::normalize(direction);
    }

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {

        if (window->GetSpecialKeyState() == 0) {
            camera->RotateFirstPerson_OX(-0.5 * senX * deltaY);
            camera->RotateFirstPerson_OY(-0.5 * senY * deltaX);
        }
    }
}


void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema3::OnWindowResize(int width, int height)
{
}
