//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 03 / 01
// Brief: Demostrate the Normal Mapping
//----------------------------------------------------------------------
#include "glb_normalmap.h"
#include "resource.h"

class ApplicationNormalMap : public glb::app::ApplicationBase {
public:
    ApplicationNormalMap() {}
    virtual~ApplicationNormalMap() {}

public:
    static glb::app::ApplicationBase* Create() {
        return new ApplicationNormalMap;
    }

public:
    bool Initialize() {
        // Camera
        scene::FreeCamera* cam = scene::FreeCamera::Create(math::Vector(0.0f, 0.0f, 160.0f), math::Vector(0.0f, 0.0f, 0.0f));
        scene::Scene::SetCamera(glb::scene::PRIMIAY_CAM, cam);

        // Light
        scene::Light ambientLight(scene::AMBIENT_LIGHT);
        ambientLight.color = math::Vector(0.5f, 0.5f, 0.5f);
        scene::Scene::SetLight(ambientLight, 0);

        scene::Light paralleLight(scene::PARALLEL_LIGHT);
        paralleLight.color = math::Vector(2.5f, 2.5f, 2.5f);
        paralleLight.dir = math::Vector(-1.0f, -1.0f, -1.0f);
        paralleLight.dir.Normalize();
        scene::Scene::SetLight(paralleLight, 1);

        // Perspective
        render::Render::SetPerspective(glb::render::Render::PRIMARY_PERS, 69.0f, 800 * 1.0f / 600, 0.1f, 500.0f);

        // HDR
        render::Render::SetExposureLevel(0.7f);
        render::Render::SetLightAdaption(0.04f);
        render::Render::SetHighLightBase(0.9f);

        // Objects
        int32_t cube = scene::Scene::AddObject("res/sphere.obj");
        scene::Scene::GetObjectById(cube)->SetCullFaceEnable(true);
        scene::Scene::GetObjectById(cube)->SetCullFaceMode(glb::render::CULL_BACK);
        scene::Scene::GetObjectById(cube)->SetDepthTestEnable(true);
        scene::Scene::GetObjectById(cube)->SetPos(math::Vector(0.0f, 0.0f, 0.0f));
        m_Cube = cube;

        return true;
    }

    void Update(float dt) {
        util::ProfileTime time;
        time.BeginProfile();

        // Update scene
        scene::Light lit = glb::scene::Scene::GetLight(1);
        float rotY = 1.0f;
        math::Matrix rot;
        rot.MakeRotateYMatrix(rotY);
        lit.dir = rot * lit.dir;
        //glb::scene::Scene::SetLight(lit, 1);

        // Randomly Rotate the sphere
        math::Vector rot_v = glb::scene::Scene::GetObjectById(m_Cube)->GetRotation();
        rot_v += math::Vector(0.5f, 0.5f, 0.5f);
        glb::scene::Scene::GetObjectById(m_Cube)->SetRotation(rot_v);

        glb::scene::Scene::Update();

        // Draw scene
        glb::render::Render::Draw();

        time.EndProfile();
        printf("%f\n", time.GetProfileTimeInMs());
    }

    void Destroy() {
    }

protected:
    int32_t m_Cube;
};

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR cmdLine, int nShowCmd) {
    glb::app::AppConfig config;
    memcpy(config.caption, L"glb_normalmap", sizeof(L"glb_normalmap"));
    config.screen_width = 800;
    config.screen_height = 600;
    config.shadow_map_width = 1024;
    config.shadow_map_height = 1024;
    config.icon = IDI_ICON1;
    if (!glb::app::Application::Initialize(ApplicationNormalMap::Create, hInstance, config)) {
        return 0;
    }

    glb::app::Application::Update();

    glb::app::Application::Destroy();

    return 0;
}