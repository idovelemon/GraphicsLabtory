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

namespace render {

namespace mesh {

//-----------------------------------------------------------------------------------
// CONSTANT VALUE
//-----------------------------------------------------------------------------------
static const int32_t kDefaultMaxLines = 1000;
static const int32_t kDefaultMaxCharacter = 1000;

//-----------------------------------------------------------------------------------
// TYPE DECLARATION
//-----------------------------------------------------------------------------------
class VertexBuffer;

//-----------------------------------------------------------------------------------
// CLASS DECLARATION
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// MeshBase DECLARATION
//----------------------------------------------------------------------------------
class MeshBase {
public:
    virtual ~MeshBase() {}
    MeshBase() {}

public:
    virtual void SetId(int32_t id) = 0;
    virtual int32_t GetId() = 0;

    virtual void SetName(std::string name) = 0;
    virtual std::string GetName() = 0;

    virtual VertexLayout GetVertexLayout() = 0;
    virtual int32_t GetVertexNum() = 0;

    virtual VertexBuffer* GetVertexBuffer() = 0;
};

//-----------------------------------------------------------------------------------
// TriangleMesh DECLARATION
//----------------------------------------------------------------------------------
class TriangleMesh : public MeshBase {
public:
    virtual ~TriangleMesh();

    //----------------------------------------------------------------------------------------------------
    // @brief: Create the triangle meshes from buffers
    // @param: triangle_num The number of the triangles
    // @param: vertex_buf The buffer of the vertex. Each triangle has three vertices
    // and each vertex has three float value, so this buffer's size must be [3 * triangle_num * 3 * sizeof(float)] bytes
    // @param: coord_buf The buffer of the texture coordinate. This is an optional parameter.
    // Each vertex has one texture coordinate with it and each texture coordinate has two float value,
    // @param: lightMapTexCoordBuf The buffer of the light map texture coordinate. This is an optional parameter.
    // Each vertex has one light map texture coordinate with it and each light map texture coordinate has two float value,
    // so this buffer's size must be [3 * triangle_num * 2 * sizeof(float)] bytes
    // @param: normal_buf The buffer of the vertex normal. This is an optional parameter.
    // Each vertex has one normal and each normal has three float value,
    // so this buffer's size must be [3 * triangle_num * 3 * sizeof(float)] bytes
    // @param: tanget_buf The buffer of the vertex tanget. This is an optional parameter
    // Each vertex has one tanget and each tanget has three float value,
    // so this buffer's size must be [3 * triangle_num * 3 * sizeof(float)] bytes
    // @param: binormal_buf The buffer of the vertex binormal. This is an optional parameter
    // Each vertex has one binormal and each binormal has three float value,
    // so this buffer's size must be [3 * triangle_num * 3 * sizeof(float)] bytes
    //----------------------------------------------------------------------------------------------------
    static TriangleMesh* Create(int32_t triangle_num, float* vertex_buf, float* coord_buf = 0, float* lightMapTexCoordBuf = 0, float* normal_buf = 0, float* tanget_buf = 0, float* binormal_buf = 0);

public:
    virtual void SetId(int32_t id);
    virtual int32_t GetId();

    virtual void SetName(std::string name);
    virtual std::string GetName();

    virtual VertexLayout GetVertexLayout();
    virtual int32_t GetVertexNum();

    virtual VertexBuffer* GetVertexBuffer();

protected:
    TriangleMesh();

private:
    class Imp;
    Imp*         m_Imp;
};

//-----------------------------------------------------------------------------------
// InstanceTriangleMesh DECLARATION
//----------------------------------------------------------------------------------

class InstanceTriangleMesh : public MeshBase {
public:
    virtual ~InstanceTriangleMesh();
    static InstanceTriangleMesh* Create(int32_t maxInstance, int32_t triangle_num, float* vertex_buf, float* coord_buf = 0, float* lightMapTexCoordBuf = 0, float* normal_buf = 0, float* tanget_buf = 0, float* binormal_buf = 0);

protected:
    InstanceTriangleMesh();

public:
    virtual void SetId(int32_t id);
    virtual int32_t GetId();

    virtual void SetName(std::string name);
    virtual std::string GetName();

    virtual VertexLayout GetVertexLayout();
    virtual int32_t GetVertexNum();

    virtual VertexBuffer* GetVertexBuffer();

    void UpdateInstanceBuffer(void* data);

private:
    class Imp;
    Imp*    m_Imp;
};

//-----------------------------------------------------------------------------------
// DynamicTriangleMesh DECLARATION
//----------------------------------------------------------------------------------

class DynamicTriangleMesh {
public:
    virtual ~DynamicTriangleMesh();
    static DynamicTriangleMesh* Create(int32_t maxTriangleNum);

protected:
    DynamicTriangleMesh();

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
    class Imp;
    Imp*    m_Imp;
};

//-----------------------------------------------------------------------------------
// DebugMesh DECLARATION
//----------------------------------------------------------------------------------
class DebugMesh {
public:
    virtual ~DebugMesh();
    static DebugMesh* Create(int32_t max_lines = kDefaultMaxLines);

public:
    void AddLine(math::Vector start, math::Vector end, math::Vector color);
    void ClearAllLines();
    VertexLayout GetVertexLayout();
    shader::Descriptor GetShaderDesc();
    int32_t GetVertexNum();
    VertexBuffer* GetVertexBuffer();

protected:
    DebugMesh();

private:
    class Imp;
    Imp*                m_Imp;
};

//-----------------------------------------------------------------------------------
// ScreenMesh DECLARATION
//----------------------------------------------------------------------------------
class ScreenMesh {
public:
    virtual ~ScreenMesh();
    static ScreenMesh* Create(int32_t width = 1, int32_t height = 1);

public:
    VertexLayout GetVertexLayout();
    int32_t GetVertexNum();
    VertexBuffer* GetVertexBuffer();

protected:
    ScreenMesh();

private:
    class Imp;
    Imp*                m_Imp;
};

//-----------------------------------------------------------------------------------
// FontMesh DECLARATION
//----------------------------------------------------------------------------------
class FontMesh {
public:
    virtual ~FontMesh();
    static FontMesh* Create(int32_t maxFontNum = kDefaultMaxCharacter);

public:
    void AddChar(math::Vector ltUV, math::Vector rbUV, math::Vector pos, math::Vector size, math::Vector color);
    void ClearAllChars();
    VertexLayout GetVertexLayout();
    int32_t GetVertexNum();
    VertexBuffer* GetVertexBuffer();

protected:
    FontMesh();

private:
    class Imp;
    Imp*            m_Imp;
};


//-----------------------------------------------------------------------------------
// Mgr DECLARATION
//----------------------------------------------------------------------------------
class Mgr {
public:
    static void Initialize();
    static void Destroy();
    static int32_t AddMesh(MeshBase* mesh);
    static MeshBase* GetMeshById(int32_t mesh_id);
    static MeshBase* GetMeshByName(std::string mesh_name);
};

};  // namespace mesh

};  // namespace render

};  // namespace glb

#endif  // GLB_GLBMESH_H_
