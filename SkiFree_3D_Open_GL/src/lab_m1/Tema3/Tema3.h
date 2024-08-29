#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema3/Lab_Camera_T3.h"

#include <chrono>


namespace m1
{
    class Tema3 : public gfxc::SimpleScene
    {
    public:
        Tema3();
        ~Tema3();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color = glm::vec3(1), Texture2D* texture1 = NULL);
        void finalGame();
        void renderRocks();
        void renderTrees();
        void renderGifts();
        void renderLight();
        void prepareObject(int& type, float& x, float& y, float& z);
        void prepareRender();
        void decisionCollision(int type, std::pair<glm::vec3, int> pos, int& i);
        void renderPlayer(float var);
        void deleteObjectsScene(int& i, int type);
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        std::vector<glm::vec3> pointLightPos;
        float left, right, bottom, top, zNear, zFar;
        std::unordered_map<std::string, Texture2D*> mapTextures;
        implemented::CameraT3* camera;
        float speed;
        bool gameOver;
        float scale;
        std::vector<glm::vec3> lightPos;
        glm::vec3 lightDirec;
        float slope;
        glm::vec3 position;
        unsigned int materialShininess;
        float materialKd;
        implemented::CameraT3* main_camera;
        float materialKs;
        std::vector<glm::vec3> lightDir;
        glm::mat4 projectionMatrix;
        float fov;
        std::vector<glm::vec3> pointLightCol;
        implemented::CameraT3* second_camera;
        int counter;
        glm::vec3 direction;
        float angle;
        std::vector<std::pair<glm::vec3, int>> renderObjects;
        std::chrono::high_resolution_clock::time_point begin;
        std::chrono::high_resolution_clock::time_point end;
        int score;
    };
}   // namespace m1
