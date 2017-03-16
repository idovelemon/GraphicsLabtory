//-----------------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 06 / 07
// Brief: A mesh contains all the vertices that form a model
//-----------------------------------------------------------------------------------
#include "glbmesh.h"

#include <fstream>
#include <vector>

#include "glbmacro.h"
#include "glbshader.h"
#include "glbvector.h"

#include "imp/gl/glbmeshimp.h"

namespace glb {

namespace mesh {

//-----------------------------------------------------------------------------------
// CONSTANT VALUE
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// TYPE DECLARATION
//-----------------------------------------------------------------------------------
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
};  // class MgrImp

//-----------------------------------------------------------------------------------
// CLASS DEFINITION
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// TriangleMesh DEFINITION
//-----------------------------------------------------------------------------------
TriangleMesh::TriangleMesh()
: m_Imp(NULL) {
}

TriangleMesh::~TriangleMesh() {
    GLB_SAFE_DELETE(m_Imp);
}

TriangleMesh* TriangleMesh::Create(int32_t triangle_num, float* vertices, float* tex_coords, float* normals, float* tangets, float* binormals) {
    TriangleMesh* triangle_mesh = NULL;

    TriangleMesh::Imp* imp = TriangleMesh::Imp::Create(triangle_num, vertices, tex_coords, normals, tangets, binormals);
    if (imp != NULL) {
        triangle_mesh = new TriangleMesh;
        triangle_mesh->m_Imp = imp;
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return triangle_mesh;
}

void TriangleMesh::SetId(int32_t id) {
    if (m_Imp != NULL) {
        m_Imp->SetId(id);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

int32_t TriangleMesh::GetId() {
    int32_t id = 0;

    if (m_Imp != NULL) {
        id = m_Imp->GetId();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return id;
}

void TriangleMesh::SetName(std::string name) {
    if (m_Imp != NULL) {
        m_Imp->SetName(name);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

std::string TriangleMesh::GetName() {
    std::string name;

    if (m_Imp != NULL) {
        name = m_Imp->GetName();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return name;
}

uint32_t TriangleMesh::GetVAO() {
    int32_t ao = -1;

    if (m_Imp != NULL) {
        ao = m_Imp->GetVAO();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return ao;
}

uint32_t TriangleMesh::GetVBO() {
    int32_t bo = -1;

    if (m_Imp != NULL) {
        bo = m_Imp->GetVBO();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return bo;
}

VertexLayout TriangleMesh::GetVertexLayout() {
    VertexLayout layout;

    if (m_Imp != NULL) {
        layout = m_Imp->GetVertexLayout();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return layout;
}

int32_t TriangleMesh::GetVertexNum() {
    int32_t vn = 0;

    if (m_Imp != NULL) {
        vn = m_Imp->GetVertexNum();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return vn;
}

VertexBuffer* TriangleMesh::GetVertexBuffer() {
    VertexBuffer* buf = NULL;

    if (m_Imp != NULL) {
        buf = m_Imp->GetVertexBuffer();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return buf;
}

//-----------------------------------------------------------------------------------
// DebugMesh DEFINITION
//-----------------------------------------------------------------------------------
DebugMesh::DebugMesh()
: m_Imp(NULL) {
}

DebugMesh::~DebugMesh() {
    GLB_SAFE_DELETE(m_Imp);
}

DebugMesh* DebugMesh::Create(int32_t max_lines) {
    DebugMesh* mesh = NULL;

    DebugMesh::Imp* imp = DebugMesh::Imp::Create(max_lines);
    if (imp != NULL) {
        mesh = new DebugMesh;
        if (mesh != NULL) {
            mesh->m_Imp = imp;
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return mesh;
}

void DebugMesh::AddLine(Vector start, Vector end, Vector color) {
    if (m_Imp != NULL) {
        m_Imp->AddLine(start, end, color);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void DebugMesh::ClearAllLines() {
    if (m_Imp != NULL) {
        m_Imp->ClearAllLines();
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

int32_t DebugMesh::GetVAO() {
    int32_t ao = -1;

    if (m_Imp != NULL) {
        ao = m_Imp->GetVAO();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return ao;
}

int32_t DebugMesh::GetVBO() {
    int32_t bo = -1;

    if (m_Imp != NULL) {
        bo = m_Imp->GetVBO();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return bo;
}

VertexLayout DebugMesh::GetVertexLayout() {
    VertexLayout layout;

    if (m_Imp != NULL) {
        layout = m_Imp->GetVertexLayout();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return layout;
}

shader::Descriptor DebugMesh::GetShaderDesc() {
    shader::Descriptor desc;

    if (m_Imp != NULL) {
        desc = m_Imp->GetShaderDesc();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return desc;
}

int32_t DebugMesh::GetVertexNum() {
    int32_t num = 0;

    if (m_Imp != NULL) {
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return num;
}

VertexBuffer* DebugMesh::GetVertexBuffer() {
    VertexBuffer* buf = NULL;

    if (m_Imp != NULL) {
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return buf;
}

//-----------------------------------------------------------------------------------
// ScreenMesh DEFINITION
//-----------------------------------------------------------------------------------
ScreenMesh::ScreenMesh()
: m_Imp(NULL) {
}

ScreenMesh::~ScreenMesh() {
    GLB_SAFE_DELETE(m_Imp);
}

ScreenMesh* ScreenMesh::Create(int32_t width, int32_t height) {
    ScreenMesh* mesh = NULL;

    ScreenMesh::Imp* imp = ScreenMesh::Imp::Create(width, height);
    if (imp != NULL) {
        mesh = new ScreenMesh;
        if (mesh != NULL) {
            mesh->m_Imp = imp;
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return mesh;
}

int32_t ScreenMesh::GetVAO() {
    int32_t ao = -1;

    if (m_Imp != NULL) {
        ao = m_Imp->GetVAO();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return ao;
}

int32_t ScreenMesh::GetVBO() {
    int32_t bo = -1;

    if (m_Imp != NULL) {
        bo = m_Imp->GetVBO();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return bo;
}

VertexLayout ScreenMesh::GetVertexLayout() {
    VertexLayout layout;

    if (m_Imp != NULL) {
        layout = m_Imp->GetVertexLayout();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return layout;
}

int32_t ScreenMesh::GetVertexNum() {
    int32_t vn = 0;

    if (m_Imp != NULL) {
        vn = m_Imp->GetVertexNum();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return vn;
}

VertexBuffer* ScreenMesh::GetVertexBuffer() {
    VertexBuffer* buf = NULL;

    if (m_Imp != NULL) {
        buf = m_Imp->GetVertexBuffer();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return buf;
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
