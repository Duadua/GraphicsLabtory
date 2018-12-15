//-----------------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 06 / 14
// Brief: Define a object
//-----------------------------------------------------------------------------------
#ifndef GLB_GLBOBJECT_H_
#define GLB_GLBOBJECT_H_

#include <stdint.h>

#include <map>
#include <vector>

#include "glbmodel.h"

#include "math/glbmatrix.h"
#include "math/glbvector.h"

#include "render/glbrenderdevice.h"
#include "render/glbshader.h"

namespace glb {

namespace scene {

class InstanceRenderObject;
class InstanceObject;

//---------------------------------------------------------------------------------------------

class Object {
public:
    enum {
        OBJECT_TYPE_NORMAL = 0,
        OBJECT_TYPE_DECAL,
        OBJECT_TYPE_INSTANCE,
        OBJECT_TYPE_INSTANCE_RENDER,
    };

public:
    Object();
    virtual~ Object();

    //--------------------------------------------------------------------------------
    // @brief: Create the object according the model file
    // @param: file_name The supported model file's name
    // @param: pos The position of the object
    // @param: scale The scale of the object
    // @param: rotation The rotation of the object
    // @return: If failed, return NULL
    //--------------------------------------------------------------------------------
    static Object* Create(const char* file_name, math::Vector pos = math::Vector(0.0f, 0.0f, 0.0f),
        math::Vector scale = math::Vector(1.0f, 1.0f, 1.0f), math::Vector rotation = math::Vector(0.0f, 0.0f, 0.0f));
    static Object* Create(const char* meshFile, const char* materialGroupFile, math::Vector pos = math::Vector(0.0f, 0.0f, 0.0f),
        math::Vector scale = math::Vector(1.0f, 1.0f, 1.0f), math::Vector rotation = math::Vector(0.0f, 0.0f, 0.0f));
    static Object* Create(scene::Model* model, math::Vector pos = math::Vector(0.0f, 0.0f, 0.0f),
        math::Vector scale = math::Vector(1.0f, 1.0f, 1.0f), math::Vector rotation = math::Vector(0.0f, 0.0f, 0.0f));

public:
    virtual int32_t GetObjectType() const;

    virtual void SetObjectId(int32_t id);
    virtual int32_t GetObjectId() const;

    virtual void SetDead(bool dead);
    virtual bool IsDead() const;

    virtual math::Vector GetBoundBoxMax();
    virtual math::Vector GetBoundBoxMin();

    virtual math::Vector GetPos();
    virtual math::Vector GetScale();
    virtual math::Matrix GetWorldMatrix() const;
    virtual void SetWorldMatrix(math::Matrix worldMatrix);

    virtual Model* GetModel();
    virtual render::shader::Descriptor GetShaderDesc();

    virtual void SetDrawEnable(bool enable);
    virtual bool IsDrawEnable();

    virtual void SetDepthTestEnable(bool enable);
    virtual bool IsDepthTestEnable();

    virtual void SetAlphaBlendEnable(bool enable);
    virtual bool IsAlphaBlendEnable();
    virtual void SetAlphaBlendFunc(render::AlphaBlendFactor factor, render::AlphaBlendFunc func);
    virtual render::AlphaBlendFunc GetAlphaBlendFunc(render::AlphaBlendFactor factor);

    virtual void SetCullFaceEnable(bool enable);
    virtual bool IsCullFaceEnable();
    virtual void SetCullFaceMode(render::CullMode mode);
    virtual render::CullMode GetCullFaceMode();

    virtual void SetTexWithId(int32_t slot, int32_t tex_id);
    virtual int32_t GetTexId(int32_t slot);

    virtual void Update();

protected:
    render::shader::Descriptor CalculateShaderDesc();

protected:
    int32_t                         m_ObjectType;
    int32_t                         m_ObjectId;
    bool                            m_IsDead;
    Model*                          m_Model;
    math::Matrix                    m_WorldMatrix;
    render::shader::Descriptor      m_ShaderDesc;

    bool                            m_EnableDraw;
    bool                            m_EnableCullFace;
    bool                            m_EnableDepthTest;
    bool                            m_EnableAlphaBlend;
    render::CullMode                m_CullMode;
    render::AlphaBlendFunc          m_SrcBlendFunc;
    render::AlphaBlendFunc          m_DstBlendFunc;
};

//---------------------------------------------------------------------------------------------

class DecalObject : public Object {
public:
    DecalObject();
    virtual ~DecalObject();

    static DecalObject* Create(const char* decalObjectFile, math::Vector pos = math::Vector(0.0f, 0.0f, 0.0f),
        math::Vector scale = math::Vector(1.0f, 1.0f, 1.0f), math::Vector rotation = math::Vector(0.0f, 0.0f, 0.0f));
};

//---------------------------------------------------------------------------------------------

class InstanceRenderObject : public Object {
public:
    InstanceRenderObject();
    virtual ~InstanceRenderObject();

    static InstanceRenderObject* Create(const char* objFileName, int32_t maxInstance = 256);

public:
    virtual void Update();
    void AddInstanceObject(InstanceObject* obj);
    void RemoveInstanceObject(InstanceObject* obj);
    void RemoveAllInstanceObject();
    int32_t GetCurInstanceNum() const;

protected:
    int32_t     m_MaxInstanceNum;
    int32_t     m_CurInstanceNum;
    typedef std::map<int32_t, InstanceObject*> InstanceObjectMap;
    InstanceObjectMap m_InstanceObjects;
};

//---------------------------------------------------------------------------------------------

class InstanceObject : public Object {
public:
    InstanceObject();
    virtual ~InstanceObject();

    static InstanceObject* Create(InstanceRenderObject* instanceRenderObject, math::Vector pos, math::Vector scale, math::Vector rotate);

public:
    virtual math::Vector GetBoundBoxMax();
    virtual math::Vector GetBoundBoxMin();

    virtual Model* GetModel();

    virtual void Update();
    virtual void ClearInstanceRenderObject();

private:
    InstanceRenderObject*       m_InstanceRenderObject;
};

};  // namespace scene

};  // namespace glb

#endif  GLB_GLBOBJECT_H_