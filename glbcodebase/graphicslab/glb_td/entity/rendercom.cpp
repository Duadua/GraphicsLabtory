//----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/05/26
// Brief: Render controls how to render a entity
//----------------------------------------------------------
#include "rendercom.h"

#include "scene/glbscene.h"
#include "scene/glbobject.h"

#include "transformcom.h"

namespace entity {

RenderCom::RenderCom(Entity* owner, const char* name)
: Component(CT_RENDER, owner)
, m_SceneObjID(-1) {
    m_SceneObjID = glb::scene::Scene::AddObject(name);
}

RenderCom::~RenderCom() {
    glb::scene::Scene::DestroyObject(m_SceneObjID);
    m_SceneObjID = -1;
}

void RenderCom::SetDepthTestEnable(bool enable) {
    glb::scene::Object* obj = glb::scene::Scene::GetObjectById(m_SceneObjID);
    if (obj != NULL) {
        obj->SetDepthTestEnable(enable);
    }
}

void RenderCom::SetDrawEnable(bool enable) {
    glb::scene::Object* obj = glb::scene::Scene::GetObjectById(m_SceneObjID);
    if (obj != NULL) {
        obj->SetDrawEnable(enable);
    }
}

void RenderCom::Render(TransformCom* transform) {
    if (transform != NULL) {
        glb::scene::Object* obj = glb::scene::Scene::GetObjectById(m_SceneObjID);
        if (obj != NULL) {
            glb::math::Vector pos = transform->GetPos();
            glb::math::Vector rotate = transform->GetRotate();
            glb::math::Vector scale = transform->GetScale();
            obj->SetPos(pos);
            obj->SetRotation(rotate);
            obj->SetScale(scale);
        }
    }
}

};  // namespace entity