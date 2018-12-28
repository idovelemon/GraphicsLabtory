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
    VertexBuffer(bool needInstance = false);
    virtual~ VertexBuffer();

public:
    int32_t GetVAO();
    int32_t GetVBO();
    int32_t GetIBO();

protected:
    int32_t             m_VAO;
    int32_t             m_VBO;
    int32_t             m_IBO;  // Instance Buffer Object
};

//-----------------------------------------------------------------------------------
// TriangleMesh::Imp DECLARATION
//----------------------------------------------------------------------------------

class TriangleMesh::Imp {
public:
    virtual ~Imp();
    static TriangleMesh::Imp* Create(int32_t triangle_num, float* vertex_buf, float* coord_buf = 0, float* lightMapTexCoordBuf = 0, float* normal_buf = 0, float* tanget_buf = 0, float* binormal_buf = 0);

public:
    void SetId(int32_t id);
    int32_t GetId();

    void SetName(std::string name);
    std::string GetName();

    void SetBoundBox(math::Vector min, math::Vector max);
    math::Vector GetBoundBoxMin();
    math::Vector GetBoundBoxMax();

    VertexLayout GetVertexLayout();
    int32_t GetVertexNum();

    VertexBuffer* GetVertexBuffer();

protected:
    Imp();

protected:
    int32_t         m_ID;
    std::string     m_Name;
    math::Vector    m_BoundBoxMax;
    math::Vector    m_BoundBoxMin;
    int32_t         m_VertexNum;
    int32_t         m_TriangleNum;
    int32_t         m_BufSizeInBytes;
    VertexBuffer*   m_VertexBuffer;
    VertexLayout    m_VertexLayout;
};

//-----------------------------------------------------------------------------------
// InstanceTriangleMesh::Imp DECLARATION
//----------------------------------------------------------------------------------

class InstanceTriangleMesh::Imp {
public:
    virtual ~Imp();
    static InstanceTriangleMesh::Imp* Create(int32_t maxInstance, int32_t triangle_num, float* vertex_buf, float* coord_buf = 0, float* lightMapTexCoordBuf = 0, float* normal_buf = 0, float* tanget_buf = 0, float* binormal_buf = 0);

public:
    void SetId(int32_t id);
    int32_t GetId();

    void SetName(std::string name);
    std::string GetName();

    void SetBoundBox(math::Vector min, math::Vector max);
    math::Vector GetBoundBoxMin();
    math::Vector GetBoundBoxMax();

    VertexLayout GetVertexLayout();
    int32_t GetVertexNum();

    VertexBuffer* GetVertexBuffer();
    void UpdateInstanceBuffer(void* buf);

protected:
    Imp();

protected:
    int32_t         m_ID;
    std::string     m_Name;
    math::Vector    m_BoundBoxMax;
    math::Vector    m_BoundBoxMin;
    int32_t         m_VertexNum;
    int32_t         m_TriangleNum;
    int32_t         m_BufSizeInBytes;
    int32_t         m_InstanceBufSizeInBytes;
    VertexBuffer*   m_VertexBuffer;
    VertexLayout    m_VertexLayout;
};

//-----------------------------------------------------------------------------------
// DynamicTriangleMesh::Imp DECLARATION
//----------------------------------------------------------------------------------

class DynamicTriangleMesh::Imp {
public:
    virtual ~Imp();
    static Imp* Create(int32_t maxTriangleNum);

protected:
    Imp();

public:
    VertexLayout GetVertexLayout();
    shader::Descriptor GetShaderDesc();
    int32_t GetVertexNum();
    VertexBuffer* GetVertexBuffer();

    void AddRect(math::Vector lt, math::Vector rb, math::Vector color);
    void AddRect(math::Vector v0, math::Vector c0, math::Vector uv0,
                 math::Vector v1, math::Vector c1, math::Vector uv1,
                 math::Vector v2, math::Vector c2, math::Vector uv2,
                 math::Vector v3, math::Vector c3, math::Vector uv3);
    void Clear();

private:
    VertexBuffer*       m_VertexBuffer;
    VertexLayout        m_VertexLayout;
    shader::Descriptor  m_ShaderDesc;
    int32_t             m_VertexNum;
    int32_t             m_MaxTriangleNum;
    int32_t             m_CurrentTriangleNum;
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

//-----------------------------------------------------------------------------------
// FontMesh::Imp DECLARATION
//----------------------------------------------------------------------------------
class FontMesh::Imp {
public:
    virtual ~Imp();
    static FontMesh::Imp* Create(int32_t maxFontNum = kDefaultMaxCharacter);

public:
    void AddChar(math::Vector ltUV, math::Vector rbUV, math::Vector pos, math::Vector size, math::Vector color);
    void ClearAllChars();
    VertexLayout GetVertexLayout();
    int32_t GetVertexNum();
    VertexBuffer* GetVertexBuffer();

protected:
    Imp();

private:
    VertexBuffer*       m_VertexBuffer;
    VertexLayout        m_VertexLayout;
    shader::Descriptor  m_ShaderDesc;
    int32_t             m_VertexNum;
    int32_t             m_CurCharacterNum;
    int32_t             m_MaxCharacterNum;
};

};  // namespace mesh

};  // namespace render

};  // namespace glb

#endif  // GLB_GLBMESH_IMP_H_

#endif  // GLB_PLATFORM_OPENGL