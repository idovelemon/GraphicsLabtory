//-----------------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 06 / 07
// Brief: A mesh contains all the vertices that form a model
//-----------------------------------------------------------------------------------
#ifndef GLB_GLBMESH_H_
#define GLB_GLBMESH_H_

#include <stdint.h>

#include <string>

#include "glbshader.h"
#include "glbstructs.h"
#include "glbrenderdevice.h"

namespace glb {

namespace mesh {

//-----------------------------------------------------------------------------------
// CONSTANT VALUE
//-----------------------------------------------------------------------------------
static const int32_t kDefaultMaxLines = 1000;

//-----------------------------------------------------------------------------------
// TYPE DECLARATION
//-----------------------------------------------------------------------------------

//------------------------------------------------------
// @TODO(i_dovelemon):Attribute index must be continuous,
// so define enumurate do not work in any situations.
//------------------------------------------------------
enum VertexAttributeIndex{
    VAI_POSITION,
    VAI_TEXCOORD,
    VAI_NORMAL,
};

//-----------------------------------------------------------------------------------
// CLASS DECLARATION
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// TriangleMesh DECLARATION
//----------------------------------------------------------------------------------
class TriangleMesh {
public:
    virtual ~TriangleMesh();

    //----------------------------------------------------------------------------------------------------
    // @brief: Create the triangle meshes from buffers
    // @param: triangle_num The number of the triangles
    // @param: vertex_buf The buffer of the vertex. Each triangle has three vertices
    // and each vertex has three float value, so this buffer's size must be [3 * triangle_num * 3 * sizeof(float)] bytes
    // @param: coord_buf The buffer of the texture coordinate. This is an optional parameter.
    // Each vertex has one texture coordinate with it and each texture coordinate has two float value,
    // so this buffer's size must be [3 * triangle_num * 2 * sizeof(float)] bytes
    // @param: normal_buf The buffer of the vertex normal. This is an optional parameter.
    // Each vertex has one normal and each normal has three float value,
    // so this buffer's size must be [3 * triangle_num * 3 * sizeof(float)] bytes
    //----------------------------------------------------------------------------------------------------
    static TriangleMesh* Create(int32_t triangle_num, float* vertex_buf, float* coord_buf = 0, float* normal_buf = 0);

public:
    void SetId(int32_t id);
    int32_t GetId();

    void SetName(std::string name);
    std::string GetName();

    uint32_t GetVAO();
    uint32_t GetVBO();
    VertexLayout GetVertexLayout();
    int32_t GetVertexNum();

protected:
    TriangleMesh();

private:
    int32_t      m_ID;
    std::string  m_Name;
    int32_t      m_VertexNum;
    int32_t      m_TriangleNum;
    int32_t      m_BufSizeInBytes;
    uint32_t     m_VAO;
    uint32_t     m_VBO;
    VertexLayout m_VertexLayout;
};

//-----------------------------------------------------------------------------------
// DebugMesh DECLARATION
//----------------------------------------------------------------------------------
class DebugMesh {
public:
    virtual ~DebugMesh();
    static DebugMesh* Create(int32_t max_lines = kDefaultMaxLines);

public:
    void AddLine(Vector start, Vector end, Vector color);
    void ClearAllLines();
    int32_t GetVAO();
    int32_t GetVBO();
    VertexLayout GetVertexLayout();
    shader::Descriptor GetShaderDesc();
    int32_t GetVertexNum();

protected:
    DebugMesh();

private:
    uint32_t            m_VertexArrayObject;
    uint32_t            m_VertexBufferObject;
    VertexLayout        m_VertexLayout;
    shader::Descriptor  m_ShaderDesc;
    int32_t             m_VertexNum;
    int32_t             m_CurLineNum;
    int32_t             m_MaxLineNum;
};

//-----------------------------------------------------------------------------------
// ScreenMesh DECLARATION
//----------------------------------------------------------------------------------
class ScreenMesh {
public:
    virtual ~ScreenMesh();
    static ScreenMesh* Create(int32_t width, int32_t height);

public:
    int32_t GetVAO();
    int32_t GetVBO();
    VertexLayout GetVertexLayout();
    int32_t GetVertexNum();

protected:
    ScreenMesh();

private:
    uint32_t            m_VertexArrayObject;
    uint32_t            m_VertexBufferObject;
    VertexLayout        m_VertexLayout;
    int32_t             m_VertexNum;
};

//-----------------------------------------------------------------------------------
// Mgr DECLARATION
//----------------------------------------------------------------------------------
class Mgr {
public:
    static void Initialize();
    static void Destroy();
    static int32_t AddMesh(TriangleMesh* mesh);
    static TriangleMesh* GetMeshById(int32_t mesh_id);
    static TriangleMesh* GetMeshByName(std::string mesh_name);
};

};  // namespace mesh

};  // namespace glb

#endif  // GLB_GLBMESH_H_
