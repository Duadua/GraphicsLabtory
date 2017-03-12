//-----------------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 06 / 14
// Brief: Define a object
//-----------------------------------------------------------------------------------
#include "glbobject.h"

#include <fstream>

#include "glbmacro.h"
#include "glbmodel.h"

namespace glb {

Object::Object()
: m_Model(NULL)
, m_Pos(0.0f, 0.0f, 0.0f)
, m_Scale(0.0f, 0.0f, 0.0f)
, m_Rotation(0.0f, 0.0f, 0.0f)
, m_WorldMatrix()
, m_EnableCullFace(false)
, m_EnableDepthTest(false)
, m_EnableAlphaBlend(false)
, m_CullMode(render::CULL_NONE) {
}

Object::~Object() {
    m_Model = NULL;
}

Object* Object::Create(const char* file_name, Vector pos, Vector scale, Vector rotation) {
    Object* obj = NULL;
    
    if (file_name) {
        Model* model = ModelMgr::GetModelByName(file_name);
        if (model == NULL) {
            model = ModelMgr::AddModel(file_name);
        }

        if (model) {
            obj = new Object();
            if (obj) {
                obj->m_Model = model;
                obj->m_Pos = pos;
                obj->m_Scale = scale;
                obj->m_Rotation = rotation;
                obj->m_WorldMatrix.MakeIdentityMatrix();
                obj->m_ShaderDesc = obj->CalculateShaderDesc();
                obj->m_ShaderProgramID = shader::Mgr::GetShader(obj->m_ShaderDesc);
            } else {
                GLB_SAFE_ASSERT(false);
            }
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return obj;
}

void Object::SetPos(Vector pos) {
    m_Pos = pos;
}

Vector Object::GetPos() const {
    return m_Pos;
}

void Object::SetScale(Vector scale) {
    m_Scale = scale;
}

Vector Object::GetScale() const {
    return m_Scale;
}

void Object::SetRotation(Vector rotation) {
    m_Rotation = rotation;
}

Vector Object::GetRotation() const {
    return m_Rotation;
}

Vector Object::GetBoundBoxMax() {
    Vector boundbox_max = m_Model->GetBoundBoxMax();
    boundbox_max += m_Pos;
    return boundbox_max;
}

Vector Object::GetBoundBoxMin() {
    Vector boundbox_min = m_Model->GetBoundBoxMin();
    boundbox_min += m_Pos;
    return boundbox_min;
}

Matrix Object::GetWorldMatrix() const {
    return m_WorldMatrix;
}

Model* Object::GetModel() {
    return m_Model;
}

shader::Descriptor Object::GetShaderDesc() {
    return m_ShaderDesc;
}

int32_t Object::GetShaderProgramID() {
    return m_ShaderProgramID;
}

void Object::SetAlphaBlendEnable(bool enable) {
    m_EnableAlphaBlend = enable;
}

bool Object::IsAlphaBlendEnable() {
    return m_EnableAlphaBlend;
}

void Object::SetAlphaBlendFunc(render::AlphaBlendFactor factor, render::AlphaBlendFunc func) {
    if (factor == render::FACTOR_SRC) {
        m_SrcBlendFunc = func;
    } else if (factor == render::FACTOR_DST) {
        m_DstBlendFunc = func;
    }
}

render::AlphaBlendFunc Object::GetAlphaBlendFunc(render::AlphaBlendFactor factor) {
    render::AlphaBlendFunc func = render::FUNC_ZERO;

    if (factor == render::FACTOR_SRC) {
        func = m_SrcBlendFunc;
    } else if (factor == render::FACTOR_DST) {
        func = m_DstBlendFunc;
    }

    return func;
}

void Object::SetCullFaceEnable(bool enable) {
    m_EnableCullFace = enable;
}

bool Object::IsCullFaceEnable() {
    return m_EnableCullFace;
}

void Object::SetDepthTestEnable(bool enable) {
    m_EnableDepthTest = enable;
}

bool Object::IsDepthTestEnable() {
    return m_EnableDepthTest;
}

void Object::SetCullFaceMode(render::CullMode cull_mode) {
    m_CullMode = cull_mode;
}

render::CullMode Object::GetCullFaceMode() {
    return m_CullMode;
}

void Object::Update() {
    // Build world transform matrix
    m_WorldMatrix.MakeIdentityMatrix();
    m_WorldMatrix.Scale(m_Scale.x, m_Scale.y, m_Scale.z);

    // Build rotation matrix in zxy-order
    m_WorldMatrix.RotateZXY(m_Rotation.x, m_Rotation.y, m_Rotation.z);

    m_WorldMatrix.Translate(m_Pos.x, m_Pos.y, m_Pos.z);
}

shader::Descriptor Object::CalculateShaderDesc() {
    shader::Descriptor desc;

    if (m_Model->HasDiffuseTexture()) {
        desc.SetFlag(shader::GLB_TEXCOORD_IN_VERTEX, true);
    }

    if (m_Model->HasAlphaTexture()) {
        desc.SetFlag(shader::GLB_ENABLE_ALPHA_TEX, true);
    }

    if (m_Model->HasNormalTexture()) {
        desc.SetFlag(shader::GLB_ENABLE_NORMAL_TEX, true);
    }

    if (m_Model->HasNormal()) {
        desc.SetFlag(shader::GLB_NORMAL_IN_VERTEX, true);
    }

    if (m_Model->HasTangent()) {
        desc.SetFlag(shader::GLB_TANGENT_IN_VERTEX, true);
    }

    if (m_Model->HasBinormal()) {
        desc.SetFlag(shader::GLB_BINORMAL_IN_VERTEX, true);
    }

    if (m_Model->IsAcceptLight()) {
        desc.SetFlag(shader::GLB_ENABLE_LIGHTING, true);
        switch(scene::Scene::GetLight(0).type) {
        case light::PARALLEL_LIGHT:
            desc.SetFlag(shader::GLB_USE_PARALLEL_LIGHT, true);
            break;

        default:
            GLB_SAFE_ASSERT(false);
            break;
        }
    }

    if (m_Model->IsAcceptShadow()) {
        desc.SetFlag(shader::GLB_ENABLE_SHADOW, true);
    }

    if (m_Model->IsUseAO()) {
        desc.SetFlag(shader::GLB_ENABLE_AO, true);
    }

    return desc;
}
};  // namespace glb
