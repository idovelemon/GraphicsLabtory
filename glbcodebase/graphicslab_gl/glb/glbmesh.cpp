//-----------------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 06 / 07
// Brief: A mesh contains all the vertices that form a model
//-----------------------------------------------------------------------------------
#include "glbmesh.h"

#include <stdio.h>

#include <fstream>
#include <vector>

#include <GL\glew.h>
#include <GL\GL.h>

#include "glbmacro.h"
#include "glbshader.h"
#include "glbvector.h"

namespace glb {

namespace mesh {

//-----------------------------------------------------------------------------------
// CONSTANT VALUE
//-----------------------------------------------------------------------------------
static const int32_t kMaxBytesPerLine = 256;
static const int32_t kScreenMeshBufSize = ((3 + 2)) * 6;

//-----------------------------------------------------------------------------------
// TYPE DECLARATION
//-----------------------------------------------------------------------------------
struct Face {
    int32_t vertex_index[3];
    int32_t texcoord_index[3];
};

class MgrImp;
static MgrImp* s_MgrImp = NULL;

//-----------------------------------------------------------------------------------
// CLASS DECLARATION
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// MgrImp DECLARATION
//----------------------------------------------------------------------------------
class MgrImp {
public:
    MgrImp();
    virtual~MgrImp();

public:
    void Initialize();
    void Destroy();
    int32_t AddMesh(TriangleMesh* mesh);
    TriangleMesh* GetMeshById(int32_t mesh_id);
    TriangleMesh* GetMeshByName(std::string mesh_name);

private:
    std::vector<TriangleMesh*> m_MeshDataBase;
};  // class TriangleMeshMgrImp

//-----------------------------------------------------------------------------------
// CLASS DEFINITION
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// TriangleMesh DEFINITION
//-----------------------------------------------------------------------------------
TriangleMesh::TriangleMesh()
: m_ID(-1)
, m_Name()
, m_VertexNum(0)
, m_TriangleNum(0)
, m_BufSizeInBytes(0)
, m_VAO(0)
, m_VBO(0)
, m_VertexLayout() {
    memset(&m_VertexLayout, 0, sizeof(m_VertexLayout));
}

TriangleMesh::~TriangleMesh() {
    if (m_VAO != 0) {
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &m_VAO);
        m_VAO = 0;
    }

    if (m_VBO != 0) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &m_VBO);
        m_VBO = 0;
    }
}

TriangleMesh* TriangleMesh::Create(int32_t triangle_num, float* vertices, float* tex_coords, float* normals) {
    TriangleMesh* triangle_mesh = NULL;

    if (triangle_num > 0 && vertices != NULL) {
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

        int32_t normal_buf_size = 0;
        if (normals) {
            normal_buf_size = 3 * triangle_num * 3 * sizeof(float);
            layout.layouts[layout.count].attriType = VA_NORMAL;
            layout.layouts[layout.count].size = normal_buf_size;
            layout.layouts[layout.count].offset = vertices_buf_size + tex_coords_buf_size;
            layout.count++;
        }

        int32_t buf_size = vertices_buf_size + tex_coords_buf_size + normal_buf_size;

        // Create vertex array object
        uint32_t vao = 0;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Create vertex buffer object
        uint32_t vbo = 0;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, buf_size, NULL, GL_STATIC_DRAW);

        // Update vertex data
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices_buf_size, vertices);

        // Update texture data
        if (tex_coords) {
            glBufferSubData(GL_ARRAY_BUFFER, vertices_buf_size, tex_coords_buf_size, tex_coords);
        }

        // Update normal data
        if (normals) {
            glBufferSubData(GL_ARRAY_BUFFER, vertices_buf_size + tex_coords_buf_size, normal_buf_size, normals);
        }

        // Create TriangleMesh and save value
        triangle_mesh = new TriangleMesh();
        if (triangle_mesh != NULL) {
            triangle_mesh->m_TriangleNum = triangle_num;
            triangle_mesh->m_VertexNum = 3 * triangle_num;
            triangle_mesh->m_BufSizeInBytes = buf_size;
            triangle_mesh->m_VAO = vao;
            triangle_mesh->m_VBO = vbo;
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

void TriangleMesh::SetId(int32_t id) {
    m_ID = id;
}

int32_t TriangleMesh::GetId() {
    return m_ID;
}

void TriangleMesh::SetName(std::string name) {
    m_Name = name;
}

std::string TriangleMesh::GetName() {
    return m_Name;
}

uint32_t TriangleMesh::GetVAO() {
    return m_VAO;
}

uint32_t TriangleMesh::GetVBO() {
    return m_VBO;
}

VertexLayout TriangleMesh::GetVertexLayout() {
    return m_VertexLayout;
}

int32_t TriangleMesh::GetVertexNum() {
    return m_VertexNum;
}

//-----------------------------------------------------------------------------------
// DebugMesh DEFINITION
//-----------------------------------------------------------------------------------
DebugMesh::DebugMesh()
: m_VertexArrayObject(0)
, m_VertexBufferObject(0)
, m_VertexLayout()
, m_ShaderDesc()
, m_VertexNum(0)
, m_CurLineNum(0) {
}

DebugMesh::~DebugMesh() {
    if (m_VertexArrayObject != 0) {
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &m_VertexArrayObject);
        m_VertexArrayObject = 0;
    }

    if (m_VertexBufferObject != 0) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &m_VertexBufferObject);
        m_VertexBufferObject = 0;
    }
}

DebugMesh* DebugMesh::Create(int32_t max_lines) {
    DebugMesh* mesh = NULL;

    // This mesh only has line primitive with position and color
    int32_t buf_size = (sizeof(float) * (3 + 3)) * max_lines * 2;

    // Create vertex array object
    uint32_t vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create vertex buffer object
    uint32_t vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, buf_size, NULL, GL_DYNAMIC_DRAW);

    if (vao != 0 && vbo != 0) {
        mesh = new DebugMesh;
        if (mesh != NULL) {
            mesh->m_VertexArrayObject = vao;
            mesh->m_VertexBufferObject = vbo;
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

void DebugMesh::AddLine(Vector start, Vector end, Vector color) {
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

        glBindVertexArray(m_VertexArrayObject);
        glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObject);
        glBufferSubData(GL_ARRAY_BUFFER, m_CurLineNum * 2 * 3 * sizeof(float), sizeof(vertex_buf), vertex_buf);
        glBufferSubData(GL_ARRAY_BUFFER, m_MaxLineNum * 2 * 3 * sizeof(float) + m_CurLineNum * 2 * 3 * sizeof(float), sizeof(color_buf), color_buf);

        m_CurLineNum++;
    }
}

void DebugMesh::ClearAllLines() {
    m_CurLineNum = 0;
}

int32_t DebugMesh::GetVAO() {
    return m_VertexArrayObject;
}

int32_t DebugMesh::GetVBO() {
    return m_VertexBufferObject;
}

VertexLayout DebugMesh::GetVertexLayout() {
    return m_VertexLayout;
}

shader::Descriptor DebugMesh::GetShaderDesc() {
    return m_ShaderDesc;
}

int32_t DebugMesh::GetVertexNum() {
    return m_CurLineNum * 2;
}

//-----------------------------------------------------------------------------------
// ScreenMesh DEFINITION
//-----------------------------------------------------------------------------------
ScreenMesh::ScreenMesh()
: m_VertexArrayObject(-1)
, m_VertexBufferObject(-1)
, m_VertexLayout()
, m_VertexNum(0) {
}

ScreenMesh::~ScreenMesh() {
    if (m_VertexArrayObject != 0) {
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &m_VertexArrayObject);
        m_VertexArrayObject = 0;
    }

    if (m_VertexBufferObject != 0) {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &m_VertexBufferObject);
        m_VertexBufferObject = 0;
    }
}

ScreenMesh* ScreenMesh::Create(int32_t width, int32_t height) {
    ScreenMesh* mesh = NULL;

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
        uint32_t vao = 0;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Create vertex buffer object
        uint32_t vbo = 0;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, kScreenMeshBufSize * sizeof(float), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, kScreenMeshBufSize * sizeof(float), buffer);

        if (vao != 0 && vbo != 0) {
            mesh = new ScreenMesh;
            if (mesh != NULL) {
                mesh->m_VertexArrayObject = vao;
                mesh->m_VertexBufferObject = vbo;
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

int32_t ScreenMesh::GetVAO() {
    return m_VertexArrayObject;
}

int32_t ScreenMesh::GetVBO() {
    return m_VertexBufferObject;
}

VertexLayout ScreenMesh::GetVertexLayout() {
    return m_VertexLayout;
}

int32_t ScreenMesh::GetVertexNum() {
    return m_VertexNum;
}

//-----------------------------------------------------------------------------------
// TriangleMeshMgrImp DEFINITION
//-----------------------------------------------------------------------------------
MgrImp::MgrImp()
:m_MeshDataBase() {
    m_MeshDataBase.clear();
}

MgrImp::~MgrImp() {
    Destroy();
}

void MgrImp::Initialize() {
}

void MgrImp::Destroy() {
    for (int32_t i = 0; i < static_cast<int32_t>(m_MeshDataBase.size()); i++) {
        GLB_SAFE_DELETE(m_MeshDataBase[i]);
    }

    m_MeshDataBase.clear();
}

int32_t MgrImp::AddMesh(TriangleMesh* mesh) {
    int32_t result = -1;

    if (mesh != NULL) {
        result = m_MeshDataBase.size();
        m_MeshDataBase.push_back(mesh);
        mesh->SetId(result);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

TriangleMesh* MgrImp::GetMeshById(int32_t id) {
    TriangleMesh* mesh = NULL;

    if (0 <= id && id < static_cast<int32_t>(m_MeshDataBase.size())) {
        mesh = m_MeshDataBase[id];
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return mesh;
}

TriangleMesh* MgrImp::GetMeshByName(std::string mesh_name) {
    TriangleMesh* mesh = NULL;

    for (int32_t i = 0; i < static_cast<int32_t>(m_MeshDataBase.size()); i++) {
        if (!strcmp(m_MeshDataBase[i]->GetName().c_str(), mesh_name.c_str())) {
            mesh = m_MeshDataBase[i];
            break;
        }
    }

    return mesh;
}

//-----------------------------------------------------------------------------------
// Mgr DEFINITION
//-----------------------------------------------------------------------------------
void Mgr::Initialize() {
    if (s_MgrImp == NULL) {
        s_MgrImp = new MgrImp;
        if (s_MgrImp != NULL) {
            s_MgrImp->Initialize();
        } else {
            GLB_SAFE_ASSERT(false);
        }
    }
}

void Mgr::Destroy() {
    if (s_MgrImp != NULL) {
        s_MgrImp->Destroy();
        GLB_SAFE_DELETE(s_MgrImp);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

int32_t Mgr::AddMesh(TriangleMesh* mesh) {
    int32_t result = -1;

    if (s_MgrImp != NULL) {
        result = s_MgrImp->AddMesh(mesh);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

TriangleMesh* Mgr::GetMeshById(int32_t id) {
    TriangleMesh* mesh = NULL;

    if (s_MgrImp != NULL) {
        mesh = s_MgrImp->GetMeshById(id);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return mesh;
}

TriangleMesh* Mgr::GetMeshByName(std::string mesh_name) {
    TriangleMesh* mesh = NULL;
    
    if (s_MgrImp != NULL) {
        mesh = s_MgrImp->GetMeshByName(mesh_name);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return mesh;
}

};  // namespace mesh

};  // namespace glb
