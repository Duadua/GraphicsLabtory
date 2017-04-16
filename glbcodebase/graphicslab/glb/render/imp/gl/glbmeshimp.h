//-----------------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 03 / 14
// Brief: Implement mesh with opengl
//-----------------------------------------------------------------------------------
#ifdef GLB_PLATFORM_OPENGL

#ifndef GLB_GLBMESH_IMP_H_
#define GLB_GLBMESH_IMP_H_

#include "render/glbmesh.h"

namespace glb {

namespace render {

namespace mesh {

//-----------------------------------------------------------------------------------
// CONSTANT VALUE
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// TYPE DECLARATION
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// CLASS DECLARATION
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// VertexBuffer DECLARATION
//----------------------------------------------------------------------------------
class VertexBuffer {
public:
    VertexBuffer();
    virtual~ VertexBuffer();

public:
    int32_t GetVAO();
    int32_t GetVBO();

protected:
    int32_t             m_VAO;
    int32_t             m_VBO;
};

//-----------------------------------------------------------------------------------
// TriangleMesh::Imp DECLARATION
//----------------------------------------------------------------------------------
class TriangleMesh::Imp {
public:
    virtual ~Imp();
    static TriangleMesh::Imp* Create(int32_t triangle_num, float* vertex_buf, float* coord_buf = 0, float* normal_buf = 0, float* tanget_buf = 0, float* binormal_buf = 0);

public:
    void SetId(int32_t id);
    int32_t GetId();

    void SetName(std::string name);
    std::string GetName();

    VertexLayout GetVertexLayout();
    int32_t GetVertexNum();

    VertexBuffer* GetVertexBuffer();

protected:
    Imp();

protected:
    int32_t         m_ID;
    std::string     m_Name;
    int32_t         m_VertexNum;
    int32_t         m_TriangleNum;
    int32_t         m_BufSizeInBytes;
    VertexBuffer*   m_VertexBuffer;
    VertexLayout    m_VertexLayout;
};

//-----------------------------------------------------------------------------------
// DebugMesh::Imp DECLARATION
//----------------------------------------------------------------------------------
class DebugMesh::Imp {
public:
    virtual ~Imp();
    static DebugMesh::Imp* Create(int32_t max_lines = kDefaultMaxLines);

public:
    void AddLine(math::Vector start, math::Vector end, math::Vector color);
    void ClearAllLines();
    VertexLayout GetVertexLayout();
    shader::Descriptor GetShaderDesc();
    int32_t GetVertexNum();
    VertexBuffer* GetVertexBuffer();

protected:
    Imp();

private:
    VertexBuffer*       m_VertexBuffer;
    VertexLayout        m_VertexLayout;
    shader::Descriptor  m_ShaderDesc;
    int32_t             m_VertexNum;
    int32_t             m_CurLineNum;
    int32_t             m_MaxLineNum;
};

//-----------------------------------------------------------------------------------
// ScreenMesh::Imp DECLARATION
//----------------------------------------------------------------------------------
class ScreenMesh::Imp {
public:
    virtual ~Imp();
    static ScreenMesh::Imp* Create(int32_t width, int32_t height);

public:
    VertexLayout GetVertexLayout();
    int32_t GetVertexNum();
    VertexBuffer* GetVertexBuffer();

protected:
    Imp();

private:
    VertexBuffer*       m_VertexBuffer;
    VertexLayout        m_VertexLayout;
    int32_t             m_VertexNum;
};

};  // namespace mesh

};  // namespace render

};  // namespace glb

#endif  // GLB_GLBMESH_IMP_H_

#endif  // GLB_PLATFORM_OPENGL