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
VertexBuffer::VertexBuffer()
: m_VAO(0)
, m_VBO(0) {
    glGenVertexArrays(1, reinterpret_cast<GLuint*>(&m_VAO));
    glGenBuffers(1, reinterpret_cast<GLuint*>(&m_VBO));
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
}

int32_t VertexBuffer::GetVAO() {
    return m_VAO;
}

int32_t VertexBuffer::GetVBO() {
    return m_VBO;
}

//--------------------------------------------------------------------------------------
// VertexBuffer DEFINITION
//--------------------------------------------------------------------------------------
TriangleMesh::Imp::Imp()
: m_ID(-1)
, m_Name()
, m_VertexNum(0)
, m_TriangleNum(0)
, m_BufSizeInBytes(0)
, m_VertexBuffer(NULL)
, m_VertexLayout()  {
    memset(&m_VertexLayout, 0, sizeof(m_VertexLayout));
}

TriangleMesh::Imp::~Imp() {
    GLB_SAFE_DELETE(m_VertexBuffer);
}

TriangleMesh::Imp* TriangleMesh::Imp::Create(int32_t triangle_num, float* vertices, float* tex_coords, float* lightMapTexCoordBuf, float* normals, float* tangets, float* binormals) {
    TriangleMesh::Imp* triangle_mesh = NULL;

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

        int32_t lightMapTexCoordsBufSize = 0;
        if (lightMapTexCoordBuf) {
            lightMapTexCoordsBufSize = 3 * triangle_num * 2 * sizeof(float);
            layout.layouts[layout.count].attriType = VA_LIGHT_MAP_TEXCOORD;
            layout.layouts[layout.count].size = tex_coords_buf_size + vertices_buf_size;
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
        glBufferData(GL_ARRAY_BUFFER, buf_size, NULL, GL_STATIC_DRAW);

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
        if (triangle_mesh != NULL) {
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
// DebugMesh::Imp DEFINITION
//-----------------------------------------------------------------------------------
DebugMesh::Imp::Imp()
: m_VertexBuffer(NULL)
, m_VertexLayout()
, m_ShaderDesc()
, m_VertexNum(0)
, m_CurLineNum(0) {
}

DebugMesh::Imp::~Imp() {
    GLB_SAFE_DELETE(m_VertexBuffer);
}

DebugMesh::Imp* DebugMesh::Imp::Create(int32_t max_lines) {
    DebugMesh::Imp* mesh = NULL;

    // This mesh only has line primitive with position and color
    int32_t buf_size = (sizeof(float) * (3 + 3)) * max_lines * 2;

    // Create vertex array object
    VertexBuffer* vbuf = new VertexBuffer;
    uint32_t vao = vbuf->GetVAO();
    uint32_t vbo = vbuf->GetVBO();
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, buf_size, NULL, GL_DYNAMIC_DRAW);

    if (vao != 0 && vbo != 0) {
        mesh = new DebugMesh::Imp;
        if (mesh != NULL) {
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
: m_VertexBuffer(NULL)
, m_VertexLayout()
, m_VertexNum(0) {
}

ScreenMesh::Imp::~Imp() {
    GLB_SAFE_DELETE(m_VertexBuffer);
}

ScreenMesh::Imp* ScreenMesh::Imp::Create(int32_t width, int32_t height) {
    ScreenMesh::Imp* mesh = NULL;

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
        glBufferData(GL_ARRAY_BUFFER, kScreenMeshBufSize * sizeof(float), NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, kScreenMeshBufSize * sizeof(float), buffer);

        if (vao != 0 && vbo != 0) {
            mesh = new ScreenMesh::Imp();
            if (mesh != NULL) {
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
};  // namespace mesh

};  // namespace render

};  // namespace glb

#endif  // GLB_PLATFORM_OPENGL