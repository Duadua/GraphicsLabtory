//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 05 / 23
// Brief: A tower defense game for my girl
//----------------------------------------------------------------------
#include "glb_td.h"
#include "resource.h"

#include "pyscript/pyscriptmgr.h"

class ApplicationTd : public glb::app::ApplicationBase {
public:
    ApplicationTd() {}
    virtual~ApplicationTd() {}

public:
    static glb::app::ApplicationBase* Create() {
        return new ApplicationTd;
    }

protected:

public:
    bool Initialize() {
        AllocConsole();
        freopen("CONOUT$", "w", stdout);

        // Camera
        scene::ModelCamera* cam = scene::ModelCamera::Create(math::Vector(0.0f, 2.3f, 1.5f), math::Vector(0.0f, 2.2f, 0.0f));
        glb::scene::Scene::SetCamera(glb::scene::PRIMIAY_CAM, cam);

        // Light
        scene::Light light(scene::PARALLEL_LIGHT);
        light.ambient = math::Vector(0.1f, 0.1f, 0.1f);
        light.diffuse = math::Vector(2.0f, 2.0f, 2.0f);
        light.specular = math::Vector(100.0f, 100.0f, 100.0f);
        light.dir = math::Vector(-1.0f, -1.0f, -1.0f);
        light.dir.Normalize();
        light.pow = 128.0f;
        glb::scene::Scene::SetLight(light, 0);

        // Perspective
        glb::render::Render::SetPerspective(glb::render::Render::PRIMARY_PERS, 69.0f, 800 * 1.0f / 600, 0.1f, 500.0f);

        // HDR
        glb::render::Render::SetExposureLevel(0.7f);
        glb::render::Render::SetLightAdaption(0.001f);
        glb::render::Render::SetHighLightBase(0.95f);

        return true;
    }

    void Update(float dt) {
        util::ProfileTime time;
        time.BeginProfile();

        // Update scene
        glb::scene::Scene::Update();

        // Draw scene
        glb::render::Render::Draw();

        time.EndProfile();
        char buf[128];
        sprintf_s(buf, "%f\n", time.GetProfileTimeInMs());
        //printf(buf);
    }

    void Destroy() {
        FreeConsole();
    }
};

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR cmdLine, int nShowCmd) {
    glb::app::AppConfig config;
    memcpy(config.caption, L"glb_td", sizeof(L"glb_td"));
    config.screen_width = 800;
    config.screen_height = 600;
    config.shadow_map_width = 2048;
    config.shadow_map_height = 2048;
    config.icon = IDI_ICON1;
    if (!glb::app::Application::Initialize(ApplicationTd::Create, hInstance, config)) {
        return 0;
    }

    glb::app::Application::Update();

    glb::app::Application::Destroy();

    return 0;
}