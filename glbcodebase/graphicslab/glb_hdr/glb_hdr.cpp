//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 01 / 14
// Brief: Demostrate the high dynamic range rendering
//----------------------------------------------------------------------
#include "glb_hdr.h"

class ApplicationHDR : public glb::app::ApplicationBase {
public:
    ApplicationHDR() {}
    virtual~ApplicationHDR() {}

public:
    static glb::app::ApplicationBase* Create() {
        return new ApplicationHDR;
    }

public:
    bool Initialize() {
        // Camera
        camera::FreeCamera* cam = camera::FreeCamera::Create(glb::Vector(0.0f, 0.0f, 200.0f), glb::Vector(0.0f, 0.0f, 0.0));
        glb::scene::Scene::SetCamera(glb::scene::PRIMIAY_CAM, cam);

        // Light
        light::Light light(light::PARALLEL_LIGHT);
        light.ambient = glb::Vector(0.2f, 0.2f, 0.2f);
        light.diffuse = glb::Vector(2.0f, 2.0f, 2.0f);
        light.specular = glb::Vector(10.0f, 10.0f, 10.0f);
        light.dir = glb::Vector(-1.0f, -1.0f, 1.0f);
        light.dir.Normalize();
        light.pow = 128.0f;
        glb::scene::Scene::SetLight(light, 0);

        // Perspective
        glb::render::Render::SetPerspective(glb::render::Render::PRIMARY_PERS, 69.0f, 800 * 1.0f / 600, 0.1f, 500.0f);

        // HDR
        glb::render::Render::SetExposureLevel(0.7f);
        glb::render::Render::SetLightAdaption(0.04f);
        glb::render::Render::SetHighLightBase(0.5f);

        //glb::scene::Scene::AddSkyObject("sky.obj");

        int32_t floor = glb::scene::Scene::AddObject("floor.obj");
        glb::scene::Scene::GetObjectById(floor)->SetCullFaceEnable(true);
        glb::scene::Scene::GetObjectById(floor)->SetCullFaceMode(glb::render::CULL_BACK);
        glb::scene::Scene::GetObjectById(floor)->SetDepthTestEnable(true);
        glb::scene::Scene::GetObjectById(floor)->SetPos(Vector(0.0, 0.0, 0.0));

        int32_t cube = glb::scene::Scene::AddObject("cube.obj");
        glb::scene::Scene::GetObjectById(cube)->SetCullFaceEnable(true);
        glb::scene::Scene::GetObjectById(cube)->SetCullFaceMode(glb::render::CULL_BACK);
        glb::scene::Scene::GetObjectById(cube)->SetDepthTestEnable(true);
        glb::scene::Scene::GetObjectById(cube)->SetPos(Vector(70.0f, 50.0f, 0.0f));

        cube = glb::scene::Scene::AddObject("cube.obj");
        glb::scene::Scene::GetObjectById(cube)->SetCullFaceEnable(true);
        glb::scene::Scene::GetObjectById(cube)->SetCullFaceMode(glb::render::CULL_BACK);
        glb::scene::Scene::GetObjectById(cube)->SetDepthTestEnable(true);
        glb::scene::Scene::GetObjectById(cube)->SetPos(Vector(-70.0f, 50.0f, 0.0f));

        return true;
    }

    void Update(float dt) {
        glb::profile::ProfileTime time;
        time.BeginProfile();

        // Update scene
        glb::light::Light lit = glb::scene::Scene::GetLight(0);
        float rotY = 1.0f;
        Matrix rot;
        rot.MakeRotateYMatrix(rotY);
        lit.dir = rot * lit.dir;
        //glb::scene::Scene::SetLight(lit, 0);
        glb::scene::Scene::Update();

        // Draw scene
        glb::render::Render::Draw();

        time.EndProfile();
        printf("%f\n", time.GetProfileTimeInMs());
    }

    void Destroy() {
    }
};

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR cmdLine, int nShowCmd) {
    glb::app::AppConfig config;
    memcpy(config.caption, L"glb_hdr", sizeof(L"glb_hdr"));
    config.screen_width = 800;
    config.screen_height = 600;
    config.shadow_map_width = 1024;
    config.shadow_map_height = 1024;
    if (!glb::app::Application::Initialize(ApplicationHDR::Create, hInstance, config)) {
        return 0;
    }

    glb::app::Application::Update();

    glb::app::Application::Destroy();

    return 0;
}