//-----------------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 03 / 14
// Brief: Implement mesh with opengl
//-----------------------------------------------------------------------------------
#ifdef GLB_PLATFORM_OPENGL

#include "glbmeshimp.h"

#include <GL\glew.h>
#include <GL\GL.h>

namespace glb {

namespace render {

namespace mesh {

//-----------------------------------------------------------------------------------
// CONSTANT VALUE
//-----------------------------------------------------------------------------------
const int32_t kR8G8B8A8 = 4;
static const int32_t kScreenMeshBufSize = ((3 + 2)) * 6;

//-----------------------------------------------------------------------------------
// TYPE DECLARATION
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// CLASS DECLARATION
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// CLASS DEFINITION
//-----------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// VertexBuffer DEFINITION
//--------------------------------------------------------------------------------------
VertexBuffer::VertexBuffer(bool needInstance)
: m_VAO(0)
, m_VBO(0)
, m_IBO(0) {
    glGenVertexArrays(1, reinterpret_cast<GLuint*>(&m_VAO));
    glGenBuffers(1, reinterpret_cast<GLuint*>(&m_VBO));
    if (needInstance) {
        glGenBuffers(1, reinterpret_cast<GLuint*>(&m_IBO));
    }
}

VertexBuffer::~VertexBuffer() {
    if (m_VAO != 0) {
        glBindVertexArray(0);
        glDeleteVertexArrays(1, reinterpret_cast<GLuint*>(&m_VAO));
        m_VAO = 0;
    }

    if (m_VBO != 0) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, reinterpret_cast<GLuint*>(&m_VBO));
        m_VBO = 0;
    }

    if (m_IBO != 0) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, reinterpret_cast<GLuint*>(&m_IBO));
        m_IBO = 0;
    }
}

int32_t VertexBuffer::GetVAO() {
    return m_VAO;
}

int32_t VertexBuffer::GetVBO() {
    return m_VBO;
}

int32_t VertexBuffer::GetIBO() {
    return m_IBO;
}

//--------------------------------------------------------------------------------------
// VertexBuffer DEFINITION
//--------------------------------------------------------------------------------------

TriangleMesh::Imp::Imp()
: m_ID(-1)
, m_Name()
, m_BoundBoxMax(0.0f, 0.0f, 0.0f)
, m_BoundBoxMin(0.0f, 0.0f, 0.0f)
, m_VertexNum(0)
, m_TriangleNum(0)
, m_BufSizeInBytes(0)
, m_VertexBuffer(nullptr)
, m_VertexLayout()  {
    memset(&m_VertexLayout, 0, sizeof(m_VertexLayout));
}

TriangleMesh::Imp::~Imp() {
    GLB_SAFE_DELETE(m_VertexBuffer);
}

TriangleMesh::Imp* TriangleMesh::Imp::Create(int32_t triangle_num, float* vertices, float* tex_coords, float* lightMapTexCoordBuf, float* normals, float* tangets, float* binormals) {
    TriangleMesh::Imp* triangle_mesh = nullptr;

    if (triangle_num > 0 && vertices != nullptr) {
        // Calculate buffer size in bytes
        VertexLayout layout;
        memset(&layout, 0, sizeof(layout));

        int32_t vertices_buf_size = 3 * triangle_num * 3 * sizeof(float);
        layout.layouts[layout.count].attriType = VA_POS;
        layout.layouts[layout.count].size = vertices_buf_size;
        layout.layouts[layout.count].offset = 0;
        layout.count++;

        int32_t tex_coords_buf_size = 0;
        if (tex_coords) {
            tex_coords_buf_size = 3 * triangle_num * 2 * sizeof(float);
            layout.layouts[layout.count].attriType = VA_TEXCOORD;
            layout.layouts[layout.count].size = tex_coords_buf_size;
            layout.layouts[layout.count].offset = vertices_buf_size;
            layout.count++;
        }

        int32_t lightMapTexCoordsBufSize = 0;
        if (lightMapTexCoordBuf) {
            lightMapTexCoordsBufSize = 3 * triangle_num * 2 * sizeof(float);
            layout.layouts[layout.count].attriType = VA_LIGHT_MAP_TEXCOORD;
            layout.layouts[layout.count].size = lightMapTexCoordsBufSize;
            layout.layouts[layout.count].offset = tex_coords_buf_size + vertices_buf_size;
            layout.count++;
        }

        int32_t normal_buf_size = 0;
        if (normals) {
            normal_buf_size = 3 * triangle_num * 3 * sizeof(float);
            layout.layouts[layout.count].attriType = VA_NORMAL;
            layout.layouts[layout.count].size = normal_buf_size;
            layout.layouts[layout.count].offset = vertices_buf_size + tex_coords_buf_size + lightMapTexCoordsBufSize;
            layout.count++;
        }

        int32_t tanget_buf_size = 0;
        if (tangets) {
            tanget_buf_size = 3 * triangle_num * 3 * sizeof(float);
            layout.layouts[layout.count].attriType = VA_TANGENT;
            layout.layouts[layout.count].size = tanget_buf_size;
            layout.layouts[layout.count].offset = vertices_buf_size + tex_coords_buf_size + lightMapTexCoordsBufSize + normal_buf_size;
            layout.count++;
        }

        int32_t binormal_buf_size = 0;
        if (binormals) {
            binormal_buf_size = 3 * triangle_num * 3 * sizeof(float);
            layout.layouts[layout.count].attriType = VA_BINORMAL;
            layout.layouts[layout.count].size = binormal_buf_size;
            layout.layouts[layout.count].offset = vertices_buf_size + tex_coords_buf_size + lightMapTexCoordsBufSize + normal_buf_size + tanget_buf_size;
            layout.count++;
        }

        int32_t buf_size = vertices_buf_size + tex_coords_buf_size + lightMapTexCoordsBufSize + normal_buf_size + tanget_buf_size + binormal_buf_size;

        // Create vertex array object
        VertexBuffer* vbuf = new VertexBuffer;
        uint32_t vao = vbuf->GetVAO();

        // Create vertex buffer object
        uint32_t vbo = vbuf->GetVBO();
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, buf_size, nullptr, GL_STATIC_DRAW);

        // Update vertex data
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices_buf_size, vertices);

        // Update texture data
        if (tex_coords) {
            glBufferSubData(GL_ARRAY_BUFFER, vertices_buf_size, tex_coords_buf_size, tex_coords);
        }

        // Update light map texture data
        if (lightMapTexCoordBuf) {
            glBufferSubData(GL_ARRAY_BUFFER, vertices_buf_size + tex_coords_buf_size, lightMapTexCoordsBufSize, lightMapTexCoordBuf);
        }

        // Update normal data
        if (normals) {
            glBufferSubData(GL_ARRAY_BUFFER, vertices_buf_size + tex_coords_buf_size + lightMapTexCoordsBufSize, normal_buf_size, normals);
        }

        // Update tanget data
        if (tangets) {
            glBufferSubData(GL_ARRAY_BUFFER, vertices_buf_size + tex_coords_buf_size + lightMapTexCoordsBufSize + normal_buf_size
                , tanget_buf_size, tangets);
        }

        // Update binormal data
        if (binormals) {
            glBufferSubData(GL_ARRAY_BUFFER, vertices_buf_size + tex_coords_buf_size + lightMapTexCoordsBufSize + normal_buf_size + tanget_buf_size
                , binormal_buf_size, binormals);
        }

        // Create TriangleMesh and save value
        triangle_mesh = new TriangleMesh::Imp();
        if (triangle_mesh != nullptr) {
            triangle_mesh->m_TriangleNum = triangle_num;
            triangle_mesh->m_VertexNum = 3 * triangle_num;
            triangle_mesh->m_BufSizeInBytes = buf_size;
            triangle_mesh->m_VertexBuffer = vbuf;
            memcpy(&triangle_mesh->m_VertexLayout, &layout, sizeof(layout));
        } else {
            GLB_SAFE_ASSERT(false);
        }

        // Un-binding
        glBindVertexArray(0);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return triangle_mesh;
}

void TriangleMesh::Imp::SetId(int32_t id) {
    m_ID = id;
}

int32_t TriangleMesh::Imp::GetId() {
    return m_ID;
}

void TriangleMesh::Imp::SetName(std::string name) {
    m_Name = name;
}

std::string TriangleMesh::Imp::GetName() {
    return m_Name;
}

void TriangleMesh::Imp::SetBoundBox(math::Vector min, math::Vector max) {
    m_BoundBoxMin = min;
    m_BoundBoxMax = max;
}

math::Vector TriangleMesh::Imp::GetBoundBoxMin() {
    return m_BoundBoxMin;
}

math::Vector TriangleMesh::Imp::GetBoundBoxMax() {
    return m_BoundBoxMax;
}

VertexLayout TriangleMesh::Imp::GetVertexLayout() {
    return m_VertexLayout;
}

int32_t TriangleMesh::Imp::GetVertexNum() {
    return m_VertexNum;
}

VertexBuffer* TriangleMesh::Imp::GetVertexBuffer() {
    return m_VertexBuffer;
}

//-----------------------------------------------------------------------------------
// InstanceTriangleMesh::Imp DEFINITION
//-----------------------------------------------------------------------------------

InstanceTriangleMesh::Imp::Imp()
: m_ID(-1)
, m_Name()
, m_BoundBoxMax(0.0f, 0.0f, 0.0f)
, m_BoundBoxMin(0.0f, 0.0f, 0.0f)
, m_VertexNum(0)
, m_TriangleNum(0)
, m_BufSizeInBytes(0)
, m_InstanceBufSizeInBytes(0)
, m_VertexBuffer(nullptr)
, m_VertexLayout() {
    memset(&m_VertexLayout, 0, sizeof(m_VertexLayout));
}

InstanceTriangleMesh::Imp::~Imp() {
    GLB_SAFE_DELETE(m_VertexBuffer);
}

InstanceTriangleMesh::Imp* InstanceTriangleMesh::Imp::Create(int32_t maxInstance, int32_t triangle_num, float* vertices, float* tex_coords, float* lightMapTexCoordBuf, float* normals, float* tangets, float* binormals) {
    InstanceTriangleMesh::Imp* triangle_mesh = nullptr;

    if (triangle_num > 0 && vertices != nullptr) {
        // Calculate buffer size in bytes
        VertexLayout layout;
        memset(&layout, 0, sizeof(layout));

        // TODO: Instance buffer's layout stored at the same place as vertex buffer's layout.
        // It should split into two different layout in future.

        // Instance buffer
        int32_t instanceBufSize = 0;
        int32_t instanceWorldMatrixBufSize = maxInstance * 16 * sizeof(float);
        layout.layouts[layout.count].attriType = VA_WORLDMATRIX;
        layout.layouts[layout.count].size = instanceWorldMatrixBufSize;
        layout.layouts[layout.count].offset = 0;
        layout.count++;

        int32_t instanceTransInvWorldMatrixBufSize = maxInstance * 16 * sizeof(float);
        layout.layouts[layout.count].attriType = VA_TRANSINVWORLDMATRIX;
        layout.layouts[layout.count].size = instanceTransInvWorldMatrixBufSize;
        layout.layouts[layout.count].offset = instanceWorldMatrixBufSize;
        layout.count++;

        instanceBufSize = instanceWorldMatrixBufSize + instanceTransInvWorldMatrixBufSize;

        // Vertex buffer
        int32_t vertices_buf_size = 3 * triangle_num * 3 * sizeof(float);
        layout.layouts[layout.count].attriType = VA_POS;
        layout.layouts[layout.count].size = vertices_buf_size;
        layout.layouts[layout.count].offset = 0;
        layout.count++;

        int32_t tex_coords_buf_size = 0;
        if (tex_coords) {
            tex_coords_buf_size = 3 * triangle_num * 2 * sizeof(float);
            layout.layouts[layout.count].attriType = VA_TEXCOORD;
            layout.layouts[layout.count].size = tex_coords_buf_size;
            layout.layouts[layout.count].offset = vertices_buf_size;
            layout.count++;
        }

        int32_t lightMapTexCoordsBufSize = 0;
        if (lightMapTexCoordBuf) {
            lightMapTexCoordsBufSize = 3 * triangle_num * 2 * sizeof(float);
            layout.layouts[layout.count].attriType = VA_LIGHT_MAP_TEXCOORD;
            layout.layouts[layout.count].size = lightMapTexCoordsBufSize;
            layout.layouts[layout.count].offset = tex_coords_buf_size + vertices_buf_size;
            layout.count++;
        }

        int32_t normal_buf_size = 0;
        if (normals) {
            normal_buf_size = 3 * triangle_num * 3 * sizeof(float);
            layout.layouts[layout.count].attriType = VA_NORMAL;
            layout.layouts[layout.count].size = normal_buf_size;
            layout.layouts[layout.count].offset = vertices_buf_size + tex_coords_buf_size + lightMapTexCoordsBufSize;
            layout.count++;
        }

        int32_t tanget_buf_size = 0;
        if (tangets) {
            tanget_buf_size = 3 * triangle_num * 3 * sizeof(float);
            layout.layouts[layout.count].attriType = VA_TANGENT;
            layout.layouts[layout.count].size = tanget_buf_size;
            layout.layouts[layout.count].offset = vertices_buf_size + tex_coords_buf_size + lightMapTexCoordsBufSize + normal_buf_size;
            layout.count++;
        }

        int32_t binormal_buf_size = 0;
        if (binormals) {
            binormal_buf_size = 3 * triangle_num * 3 * sizeof(float);
            layout.layouts[layout.count].attriType = VA_BINORMAL;
            layout.layouts[layout.count].size = binormal_buf_size;
            layout.layouts[layout.count].offset = vertices_buf_size + tex_coords_buf_size + lightMapTexCoordsBufSize + normal_buf_size + tanget_buf_size;
            layout.count++;
        }

        int32_t buf_size = vertices_buf_size + tex_coords_buf_size + lightMapTexCoordsBufSize + normal_buf_size + tanget_buf_size + binormal_buf_size;

        // Create vertex array object
        VertexBuffer* vbuf = new VertexBuffer(true);
        uint32_t vao = vbuf->GetVAO();

        // Create instance buffer object
        uint32_t ibo = vbuf->GetIBO();
        glBindBuffer(GL_ARRAY_BUFFER, ibo);
        glBufferData(GL_ARRAY_BUFFER, instanceBufSize, nullptr, GL_DYNAMIC_DRAW);

        // Create vertex buffer object
        uint32_t vbo = vbuf->GetVBO();
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, buf_size, nullptr, GL_STATIC_DRAW);

        // Update vertex data
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices_buf_size, vertices);

        // Update texture data
        if (tex_coords) {
            glBufferSubData(GL_ARRAY_BUFFER, vertices_buf_size, tex_coords_buf_size, tex_coords);
        }

        // Update light map texture data
        if (lightMapTexCoordBuf) {
            glBufferSubData(GL_ARRAY_BUFFER, vertices_buf_size + tex_coords_buf_size, lightMapTexCoordsBufSize, lightMapTexCoordBuf);
        }

        // Update normal data
        if (normals) {
            glBufferSubData(GL_ARRAY_BUFFER, vertices_buf_size + tex_coords_buf_size + lightMapTexCoordsBufSize, normal_buf_size, normals);
        }

        // Update tanget data
        if (tangets) {
            glBufferSubData(GL_ARRAY_BUFFER, vertices_buf_size + tex_coords_buf_size + lightMapTexCoordsBufSize + normal_buf_size
                , tanget_buf_size, tangets);
        }

        // Update binormal data
        if (binormals) {
            glBufferSubData(GL_ARRAY_BUFFER, vertices_buf_size + tex_coords_buf_size + lightMapTexCoordsBufSize + normal_buf_size + tanget_buf_size
                , binormal_buf_size, binormals);
        }

        // Create TriangleMesh and save value
        triangle_mesh = new InstanceTriangleMesh::Imp();
        if (triangle_mesh != nullptr) {
            triangle_mesh->m_TriangleNum = triangle_num;
            triangle_mesh->m_VertexNum = 3 * triangle_num;
            triangle_mesh->m_BufSizeInBytes = buf_size;
            triangle_mesh->m_InstanceBufSizeInBytes = instanceBufSize;
            triangle_mesh->m_VertexBuffer = vbuf;
            memcpy(&triangle_mesh->m_VertexLayout, &layout, sizeof(layout));
        } else {
            GLB_SAFE_ASSERT(false);
        }

        // Un-binding
        glBindVertexArray(0);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return triangle_mesh;
}

void InstanceTriangleMesh::Imp::SetId(int32_t id) {
    m_ID = id;
}

int32_t InstanceTriangleMesh::Imp::GetId() {
    return m_ID;
}

void InstanceTriangleMesh::Imp::SetName(std::string name) {
    m_Name = name;
}

std::string InstanceTriangleMesh::Imp::GetName() {
    return m_Name;
}

void InstanceTriangleMesh::Imp::SetBoundBox(math::Vector min, math::Vector max) {
    m_BoundBoxMin = min;
    m_BoundBoxMax = max;
}

math::Vector InstanceTriangleMesh::Imp::GetBoundBoxMin() {
    return m_BoundBoxMin;
}

math::Vector InstanceTriangleMesh::Imp::GetBoundBoxMax() {
    return m_BoundBoxMax;
}

VertexLayout InstanceTriangleMesh::Imp::GetVertexLayout() {
    return m_VertexLayout;
}

int32_t InstanceTriangleMesh::Imp::GetVertexNum() {
    return m_VertexNum;
}

VertexBuffer* InstanceTriangleMesh::Imp::GetVertexBuffer() {
    return m_VertexBuffer;
}

void InstanceTriangleMesh::Imp::UpdateInstanceBuffer(void* buf) {
    glBindVertexArray(m_VertexBuffer->GetVAO());
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer->GetIBO());

    glBufferSubData(GL_ARRAY_BUFFER, 0, m_InstanceBufSizeInBytes, buf);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

//-----------------------------------------------------------------------------------
// DynamicTriangleMesh::Imp DEFINITION
//-----------------------------------------------------------------------------------

DynamicTriangleMesh::Imp::Imp()
: m_VertexBuffer(nullptr)
, m_VertexLayout()
, m_ShaderDesc()
, m_CurrentTriangleNum(0)
, m_MaxTriangleNum(0) {
}

DynamicTriangleMesh::Imp::~Imp() {
}

DynamicTriangleMesh::Imp* DynamicTriangleMesh::Imp::Create(int32_t maxTriangleNum) {
    DynamicTriangleMesh::Imp* mesh = nullptr;

    // This mesh only has line primitive with position, color and uv
    int32_t buf_size = (sizeof(float) * (3 + 4 + 2)) * maxTriangleNum * 3;

    // Create vertex array object
    VertexBuffer* vbuf = new VertexBuffer;
    uint32_t vao = vbuf->GetVAO();
    uint32_t vbo = vbuf->GetVBO();
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, buf_size, nullptr, GL_DYNAMIC_DRAW);

    if (vao != 0 && vbo != 0) {
        mesh = new DynamicTriangleMesh::Imp;
        if (mesh != nullptr) {
            mesh->m_VertexBuffer = vbuf;
            mesh->m_VertexNum = maxTriangleNum * 2;
            mesh->m_MaxTriangleNum = maxTriangleNum;
            mesh->m_VertexLayout.count = 3;

            // Pos attribute layout
            mesh->m_VertexLayout.layouts[0].attriType = VA_POS;
            mesh->m_VertexLayout.layouts[0].offset = 0;
            mesh->m_VertexLayout.layouts[0].size = 0;

            // Color attribute layout
            mesh->m_VertexLayout.layouts[1].attriType = VA_COLOR;
            mesh->m_VertexLayout.layouts[1].offset = sizeof(float) * 3 * 3 * maxTriangleNum;
            mesh->m_VertexLayout.layouts[1].size = 0;

            // UV attribute layout
            mesh->m_VertexLayout.layouts[2].attriType = VA_TEXCOORD;
            mesh->m_VertexLayout.layouts[2].offset = sizeof(float) * 3 * 3 * maxTriangleNum + sizeof(float) * 4 * 3 * maxTriangleNum;
            mesh->m_VertexLayout.layouts[2].size = 0;

            // Shader desc
            mesh->m_ShaderDesc.SetFlag(shader::GLB_COLOR_IN_VERTEX, true);
            mesh->m_ShaderDesc.SetFlag(shader::GLB_TEXCOORD_IN_VERTEX, true);
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return mesh;
}

VertexLayout DynamicTriangleMesh::Imp::GetVertexLayout() {
    return m_VertexLayout;
}

shader::Descriptor DynamicTriangleMesh::Imp::GetShaderDesc() {
    return m_ShaderDesc;
}

int32_t DynamicTriangleMesh::Imp::GetVertexNum() {
    return m_CurrentTriangleNum * 3;
}

VertexBuffer* DynamicTriangleMesh::Imp::GetVertexBuffer() {
    return m_VertexBuffer;
}

void DynamicTriangleMesh::Imp::AddRect(math::Vector lt, math::Vector rb, math::Vector color) {
    AddRect(lt, color, math::Vector(0.0f, 0.0f, 0.0f)
        , math::Vector(rb.x, lt.y, rb.z), color, math::Vector(0.0f, 0.0f, 0.0f)
        , rb, color, math::Vector(0.0f, 0.0f, 0.0f)
        , math::Vector(lt.x, rb.y, lt.z), color, math::Vector(0.0f, 0.0f, 0.0f));
}

void DynamicTriangleMesh::Imp::AddRect(math::Vector v0, math::Vector c0, math::Vector uv0,
                                    math::Vector v1, math::Vector c1, math::Vector uv1,
                                    math::Vector v2, math::Vector c2, math::Vector uv2,
                                    math::Vector v3, math::Vector c3, math::Vector uv3) {
    if (m_CurrentTriangleNum < m_MaxTriangleNum) {
        GLfloat vertexBuf[18];
        vertexBuf[0] = v0.x;
        vertexBuf[1] = v0.y;
        vertexBuf[2] = v0.z;
        vertexBuf[3] = v1.x;
        vertexBuf[4] = v1.y;
        vertexBuf[5] = v1.z;
        vertexBuf[6] = v2.x;
        vertexBuf[7] = v2.y;
        vertexBuf[8] = v2.z;
        vertexBuf[9] = v0.x;
        vertexBuf[10] = v0.y;
        vertexBuf[11] = v0.z;
        vertexBuf[12] = v2.x;
        vertexBuf[13] = v2.y;
        vertexBuf[14] = v2.z;
        vertexBuf[15] = v3.x;
        vertexBuf[16] = v3.y;
        vertexBuf[17] = v3.z;

        GLfloat colorBuf[24];
        colorBuf[0] = c0.x;
        colorBuf[1] = c0.y;
        colorBuf[2] = c0.z;
        colorBuf[3] = c0.w;
        colorBuf[4] = c1.x;
        colorBuf[5] = c1.y;
        colorBuf[6] = c1.z;
        colorBuf[7] = c1.w;
        colorBuf[8] = c2.x;
        colorBuf[9] = c2.y;
        colorBuf[10] = c2.z;
        colorBuf[11] = c2.w;
        colorBuf[12] = c0.x;
        colorBuf[13] = c0.y;
        colorBuf[14] = c0.z;
        colorBuf[15] = c0.w;
        colorBuf[16] = c2.x;
        colorBuf[17] = c2.y;
        colorBuf[18] = c2.z;
        colorBuf[19] = c2.w;
        colorBuf[20] = c3.x;
        colorBuf[21] = c3.y;
        colorBuf[22] = c3.z;
        colorBuf[23] = c3.w;

        GLfloat uvBuf[12];
        uvBuf[0] = uv0.x;
        uvBuf[1] = uv0.y;
        uvBuf[2] = uv1.x;
        uvBuf[3] = uv1.y;
        uvBuf[4] = uv2.x;
        uvBuf[5] = uv2.y;
        uvBuf[6] = uv0.x;
        uvBuf[7] = uv0.y;
        uvBuf[8] = uv2.x;
        uvBuf[9] = uv2.y;
        uvBuf[10] = uv3.x;
        uvBuf[11] = uv3.y;

        GLuint va = m_VertexBuffer->GetVAO();
        GLuint vb = m_VertexBuffer->GetVBO();
        glBindVertexArray(va);
        glBindBuffer(GL_ARRAY_BUFFER, vb);
        glBufferSubData(GL_ARRAY_BUFFER, m_CurrentTriangleNum * 3 * 3 * sizeof(float), sizeof(vertexBuf), vertexBuf);
        glBufferSubData(GL_ARRAY_BUFFER, m_MaxTriangleNum * 3 * 3 * sizeof(float) + m_CurrentTriangleNum * 3 * 4 * sizeof(float), sizeof(colorBuf), colorBuf);
        glBufferSubData(GL_ARRAY_BUFFER, m_MaxTriangleNum * 3 * 3 * sizeof(float) + m_MaxTriangleNum * 3 * 4 * sizeof(float) + m_CurrentTriangleNum * 3 * 2 * sizeof(float), sizeof(uvBuf), uvBuf);

        m_CurrentTriangleNum = m_CurrentTriangleNum + 2;
    }
}

void DynamicTriangleMesh::Imp::Clear() {
    m_CurrentTriangleNum = 0;
}

//-----------------------------------------------------------------------------------
// DebugMesh::Imp DEFINITION
//-----------------------------------------------------------------------------------
DebugMesh::Imp::Imp()
: m_VertexBuffer(nullptr)
, m_VertexLayout()
, m_ShaderDesc()
, m_VertexNum(0)
, m_CurLineNum(0) {
}

DebugMesh::Imp::~Imp() {
    GLB_SAFE_DELETE(m_VertexBuffer);
}

DebugMesh::Imp* DebugMesh::Imp::Create(int32_t max_lines) {
    DebugMesh::Imp* mesh = nullptr;

    // This mesh only has line primitive with position and color
    int32_t buf_size = (sizeof(float) * (3 + 3)) * max_lines * 2;

    // Create vertex array object
    VertexBuffer* vbuf = new VertexBuffer;
    uint32_t vao = vbuf->GetVAO();
    uint32_t vbo = vbuf->GetVBO();
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, buf_size, nullptr, GL_DYNAMIC_DRAW);

    if (vao != 0 && vbo != 0) {
        mesh = new DebugMesh::Imp;
        if (mesh != nullptr) {
            mesh->m_VertexBuffer = vbuf;
            mesh->m_VertexNum = max_lines * 2;
            mesh->m_MaxLineNum = max_lines;
            mesh->m_VertexLayout.count = 2;

            // Pos attribute layout
            mesh->m_VertexLayout.layouts[0].attriType = VA_POS;
            mesh->m_VertexLayout.layouts[0].offset = 0;
            mesh->m_VertexLayout.layouts[0].size = 0;

            // Color attribute layout
            mesh->m_VertexLayout.layouts[1].attriType = VA_COLOR;
            mesh->m_VertexLayout.layouts[1].offset = sizeof(float) * 3 * 2 * max_lines;
            mesh->m_VertexLayout.layouts[1].size = 0;

            // Shader desc
            mesh->m_ShaderDesc.SetFlag(shader::GLB_COLOR_IN_VERTEX, true);
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return mesh;
}

void DebugMesh::Imp::AddLine(math::Vector start, math::Vector end, math::Vector color) {
    if (m_CurLineNum < m_MaxLineNum) {
        GLfloat vertex_buf[6];
        vertex_buf[0] = start.x;
        vertex_buf[1] = start.y;
        vertex_buf[2] = start.z;
        vertex_buf[3] = end.x;
        vertex_buf[4] = end.y;
        vertex_buf[5] = end.z;

        GLfloat color_buf[6];
        color_buf[0] = color.x;
        color_buf[1] = color.y;
        color_buf[2] = color.z;
        color_buf[3] = color.x;
        color_buf[4] = color.y;
        color_buf[5] = color.z;

        GLuint va = m_VertexBuffer->GetVAO();
        GLuint vb = m_VertexBuffer->GetVBO();
        glBindVertexArray(va);
        glBindBuffer(GL_ARRAY_BUFFER, vb);
        glBufferSubData(GL_ARRAY_BUFFER, m_CurLineNum * 2 * 3 * sizeof(float), sizeof(vertex_buf), vertex_buf);
        glBufferSubData(GL_ARRAY_BUFFER, m_MaxLineNum * 2 * 3 * sizeof(float) + m_CurLineNum * 2 * 3 * sizeof(float), sizeof(color_buf), color_buf);

        m_CurLineNum++;
    }
}

void DebugMesh::Imp::ClearAllLines() {
    m_CurLineNum = 0;
}

VertexLayout DebugMesh::Imp::GetVertexLayout() {
    return m_VertexLayout;
}

shader::Descriptor DebugMesh::Imp::GetShaderDesc() {
    return m_ShaderDesc;
}

int32_t DebugMesh::Imp::GetVertexNum() {
    return m_CurLineNum * 2;
}

VertexBuffer* DebugMesh::Imp::GetVertexBuffer() {
    return m_VertexBuffer;
}

//-----------------------------------------------------------------------------------
// ScreenMesh::Imp DEFINITION
//-----------------------------------------------------------------------------------
ScreenMesh::Imp::Imp()
: m_VertexBuffer(nullptr)
, m_VertexLayout()
, m_VertexNum(0) {
}

ScreenMesh::Imp::~Imp() {
    GLB_SAFE_DELETE(m_VertexBuffer);
}

ScreenMesh::Imp* ScreenMesh::Imp::Create(int32_t width, int32_t height) {
    ScreenMesh::Imp* mesh = nullptr;

    if (0 < width && 0 < height) {
        // This mesh only has position and texture coordinate
        float buffer[kScreenMeshBufSize];
        memset(buffer, 0, sizeof(buffer));

        // Build mesh data
        float half_width = 1.0f;
        float half_height = 1.0f;
        float depth = 0.0f;

        // Triangle data
        buffer[0] = -half_width;
        buffer[1] = half_height;
        buffer[2] = depth;
        buffer[3] = half_width;
        buffer[4] = -half_height;
        buffer[5] = depth;
        buffer[6] = half_width;
        buffer[7] = half_height;
        buffer[8] = depth;
        buffer[9] = -half_width;
        buffer[10] = half_height;
        buffer[11] = depth;
        buffer[12] = -half_width;
        buffer[13] = -half_height;
        buffer[14] = depth;
        buffer[15] = half_width;
        buffer[16] = -half_height;
        buffer[17] = depth;

        // Texture coordinate
        buffer[18] = 0.0f;
        buffer[19] = 1.0f;
        buffer[20] = 1.0f;
        buffer[21] = 0.0f;
        buffer[22] = 1.0f;
        buffer[23] = 1.0f;
        buffer[24] = 0.0f;
        buffer[25] = 1.0f;
        buffer[26] = 0.0f;
        buffer[27] = 0.0f;
        buffer[28] = 1.0f;
        buffer[29] = 0.0f;

        // Create vertex array object
        VertexBuffer* vbuf = new VertexBuffer;
        uint32_t vao = vbuf->GetVAO();
        uint32_t vbo = vbuf->GetVBO();
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, kScreenMeshBufSize * sizeof(float), nullptr, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, kScreenMeshBufSize * sizeof(float), buffer);

        if (vao != 0 && vbo != 0) {
            mesh = new ScreenMesh::Imp();
            if (mesh != nullptr) {
                mesh->m_VertexBuffer = vbuf;
                mesh->m_VertexNum = 6;

                mesh->m_VertexLayout.count = 2;

                // Pos attribute layout
                mesh->m_VertexLayout.layouts[0].attriType = VA_POS;
                mesh->m_VertexLayout.layouts[0].offset = 0;
                mesh->m_VertexLayout.layouts[0].size = 0;

                // Color attribute layout
                mesh->m_VertexLayout.layouts[1].attriType = VA_TEXCOORD;
                mesh->m_VertexLayout.layouts[1].offset = sizeof(float) * 3 * 6;
                mesh->m_VertexLayout.layouts[1].size = 0;
            } else {
                GLB_SAFE_ASSERT(false);
            }
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return mesh;
}

VertexLayout ScreenMesh::Imp::GetVertexLayout() {
    return m_VertexLayout;
}

int32_t ScreenMesh::Imp::GetVertexNum() {
    return m_VertexNum;
}

VertexBuffer* ScreenMesh::Imp::GetVertexBuffer() {
    return m_VertexBuffer;
}

//-----------------------------------------------------------------------------------
// FontMesh::Imp DEFINITION
//-----------------------------------------------------------------------------------
FontMesh::Imp::Imp()
: m_VertexBuffer(nullptr)
, m_VertexLayout()
, m_ShaderDesc()
, m_VertexNum(0) {
}

FontMesh::Imp::~Imp() {
    GLB_SAFE_DELETE(m_VertexBuffer);
}

FontMesh::Imp* FontMesh::Imp::Create(int32_t maxCharacter) {
    FontMesh::Imp* mesh = nullptr;

    // This mesh only has line primitive with position, color and uv
    int32_t buf_size = (sizeof(float) * (3 + 4 + 2)) * maxCharacter * 6;

    // Create vertex array object
    VertexBuffer* vbuf = new VertexBuffer;
    uint32_t vao = vbuf->GetVAO();
    uint32_t vbo = vbuf->GetVBO();
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, buf_size, nullptr, GL_DYNAMIC_DRAW);

    if (vao != 0 && vbo != 0) {
        mesh = new FontMesh::Imp;
        if (mesh != nullptr) {
            mesh->m_VertexBuffer = vbuf;
            mesh->m_VertexNum = 0;
            mesh->m_CurCharacterNum = 0;
            mesh->m_MaxCharacterNum = maxCharacter;
            mesh->m_VertexLayout.count = 3;

            // Pos attribute layout
            mesh->m_VertexLayout.layouts[0].attriType = VA_POS;
            mesh->m_VertexLayout.layouts[0].offset = 0;
            mesh->m_VertexLayout.layouts[0].size = 0;

            // Color attribute layout
            mesh->m_VertexLayout.layouts[1].attriType = VA_COLOR;
            mesh->m_VertexLayout.layouts[1].offset = sizeof(float) * 3 * 6 * maxCharacter;
            mesh->m_VertexLayout.layouts[1].size = 0;

            // UV attribute layout
            mesh->m_VertexLayout.layouts[2].attriType = VA_TEXCOORD;
            mesh->m_VertexLayout.layouts[2].offset = sizeof(float) * (3 + 4) * 6 * maxCharacter;
            mesh->m_VertexLayout.layouts[2].size = 0;

            // Shader desc
            mesh->m_ShaderDesc.SetFlag(shader::GLB_COLOR_IN_VERTEX, true);
            mesh->m_ShaderDesc.SetFlag(shader::GLB_TEXCOORD_IN_VERTEX, true);
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return mesh;
}

void FontMesh::Imp::AddChar(math::Vector ltUV, math::Vector rbUV, math::Vector pos, math::Vector size, math::Vector color) {
    if (m_CurCharacterNum < m_MaxCharacterNum) {
        GLfloat vertex_buf[18];
        vertex_buf[0] = pos.x;
        vertex_buf[1] = pos.y;
        vertex_buf[2] = 0.0f;
        vertex_buf[3] = pos.x + size.x;
        vertex_buf[4] = pos.y;
        vertex_buf[5] = 0.0f;
        vertex_buf[6] = pos.x + size.x;
        vertex_buf[7] = pos.y - size.y;
        vertex_buf[8] = 0.0f;
        vertex_buf[9] = pos.x;
        vertex_buf[10] = pos.y;
        vertex_buf[11] = 0.0f;
        vertex_buf[12] = pos.x + size.x;
        vertex_buf[13] = pos.y - size.y;
        vertex_buf[14] = 0.0f;
        vertex_buf[15] = pos.x;
        vertex_buf[16] = pos.y - size.y;
        vertex_buf[17] = 0.0f;

        GLfloat color_buf[24];
        color_buf[0] = color.x;
        color_buf[1] = color.y;
        color_buf[2] = color.z;
        color_buf[3] = color.w;
        color_buf[4] = color.x;
        color_buf[5] = color.y;
        color_buf[6] = color.z;
        color_buf[7] = color.w;
        color_buf[8] = color.x;
        color_buf[9] = color.y;
        color_buf[10] = color.z;
        color_buf[11] = color.w;
        color_buf[12] = color.x;
        color_buf[13] = color.y;
        color_buf[14] = color.z;
        color_buf[15] = color.w;
        color_buf[16] = color.x;
        color_buf[17] = color.y;
        color_buf[18] = color.z;
        color_buf[19] = color.w;
        color_buf[20] = color.x;
        color_buf[21] = color.y;
        color_buf[22] = color.z;
        color_buf[23] = color.w;

        GLfloat uvBuf[12];
        uvBuf[0] = ltUV.x;
        uvBuf[1] = ltUV.y;
        uvBuf[2] = rbUV.x;
        uvBuf[3] = ltUV.y;
        uvBuf[4] = rbUV.x;
        uvBuf[5] = rbUV.y;
        uvBuf[6] = ltUV.x;
        uvBuf[7] = ltUV.y;
        uvBuf[8] = rbUV.x;
        uvBuf[9] = rbUV.y;
        uvBuf[10] = ltUV.x;
        uvBuf[11] = rbUV.y;

        GLuint va = m_VertexBuffer->GetVAO();
        GLuint vb = m_VertexBuffer->GetVBO();
        glBindVertexArray(va);
        glBindBuffer(GL_ARRAY_BUFFER, vb);
        glBufferSubData(GL_ARRAY_BUFFER, m_CurCharacterNum * 6 * 3 * sizeof(float), sizeof(vertex_buf), vertex_buf);
        glBufferSubData(GL_ARRAY_BUFFER, m_MaxCharacterNum * 6 * 3 * sizeof(float) + m_CurCharacterNum * 6 * 4 * sizeof(float), sizeof(color_buf), color_buf);
        glBufferSubData(GL_ARRAY_BUFFER, m_MaxCharacterNum * 6 * 3 * sizeof(float) + m_MaxCharacterNum * 6 * 4 * sizeof(float) + m_CurCharacterNum * 6 * 2 * sizeof(float), sizeof(uvBuf), uvBuf);

        m_CurCharacterNum++;
    }
}

void FontMesh::Imp::ClearAllChars() {
    m_CurCharacterNum = 0;
}

VertexLayout FontMesh::Imp::GetVertexLayout() {
    return m_VertexLayout;
}

int32_t FontMesh::Imp::GetVertexNum() {
    return m_CurCharacterNum * 6;
}

VertexBuffer* FontMesh::Imp::GetVertexBuffer() {
    return m_VertexBuffer;
}

};  // namespace mesh

};  // namespace render

};  // namespace glb

#endif  // GLB_PLATFORM_OPENGL