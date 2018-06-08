//------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018 / 06 / 07
// Brief: Core routine for light studio
//------------------------------------------------------------------
#ifndef GLB_GLB_LIGHT_STUDIO_CORE_H_
#define GLB_GLB_LIGHT_STUDIO_CORE_H_

#include "glb.h"

class ApplicationCore : public glb::app::ApplicationBase {
public:
    ApplicationCore();
    virtual ~ApplicationCore();

public:
    static glb::app::ApplicationBase* Create();
    static ApplicationCore* GetInstance();

public:
    virtual bool Initialize();
    virtual void Update(float dt);
    virtual void Destroy();

    bool AddSceneMesh(const char* name);

protected:
    void UpdateCamera();
    void DrawScene();

private:
    static ApplicationCore* s_Instance;

    glb::scene::CameraBase* m_Camera;

    glb::scene::Model*              m_SceneMesh;

    glb::render::texture::Texture*  m_LightMap[3];
    int32_t                         m_LightMapWidth;
    int32_t                         m_LightMapHeight;

    glb::render::shader::UserProgram*   m_SceneProgram;
};

#endif  // GLB_GLB_LIGHT_STUDIO_CORE_H_