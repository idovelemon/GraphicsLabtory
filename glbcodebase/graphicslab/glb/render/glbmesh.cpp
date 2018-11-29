//-----------------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 06 / 07
// Brief: A mesh contains all the vertices that form a model
//-----------------------------------------------------------------------------------
#include "glbmesh.h"

#include <fstream>
#include <vector>

#include "glbshader.h"
#include "math/glbvector.h"
#include "util/glbmacro.h"
#include "imp/dx11/glbmeshimp.h"
#include "imp/gl/glbmeshimp.h"

namespace glb {

namespace render {

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
    int32_t AddMesh(MeshBase* mesh);
    MeshBase* GetMeshById(int32_t mesh_id);
    MeshBase* GetMeshByName(std::string mesh_name);

private:
    std::vector<MeshBase*> m_MeshDataBase;
};  // class MgrImp

//-----------------------------------------------------------------------------------
// CLASS DEFINITION
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// TriangleMesh DEFINITION
//-----------------------------------------------------------------------------------
TriangleMesh::TriangleMesh()
: MeshBase()
, m_Imp(NULL) {
}

TriangleMesh::~TriangleMesh() {
    GLB_SAFE_DELETE(m_Imp);
}

TriangleMesh* TriangleMesh::Create(int32_t triangle_num, float* vertices, float* tex_coords, float* lightMapTexCoordBuf, float* normals, float* tangets, float* binormals) {
    TriangleMesh* triangle_mesh = NULL;

    TriangleMesh::Imp* imp = TriangleMesh::Imp::Create(triangle_num, vertices, tex_coords, lightMapTexCoordBuf, normals, tangets, binormals);
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
// InstanceTriangleMesh DEFINITION
//-----------------------------------------------------------------------------------
InstanceTriangleMesh::InstanceTriangleMesh()
: MeshBase()
, m_Imp(NULL) {
}

InstanceTriangleMesh::~InstanceTriangleMesh() {
    GLB_SAFE_DELETE(m_Imp);
}

InstanceTriangleMesh* InstanceTriangleMesh::Create(int32_t maxInstance, int32_t triangle_num, float* vertices, float* tex_coords, float* lightMapTexCoordBuf, float* normals, float* tangets, float* binormals) {
    InstanceTriangleMesh* triangle_mesh = NULL;

    InstanceTriangleMesh::Imp* imp = InstanceTriangleMesh::Imp::Create(maxInstance, triangle_num, vertices, tex_coords, lightMapTexCoordBuf, normals, tangets, binormals);
    if (imp != NULL) {
        triangle_mesh = new InstanceTriangleMesh;
        triangle_mesh->m_Imp = imp;
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return triangle_mesh;
}

void InstanceTriangleMesh::SetId(int32_t id) {
    if (m_Imp != NULL) {
        m_Imp->SetId(id);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

int32_t InstanceTriangleMesh::GetId() {
    int32_t id = 0;

    if (m_Imp != NULL) {
        id = m_Imp->GetId();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return id;
}

void InstanceTriangleMesh::SetName(std::string name) {
    if (m_Imp != NULL) {
        m_Imp->SetName(name);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

std::string InstanceTriangleMesh::GetName() {
    std::string name;

    if (m_Imp != NULL) {
        name = m_Imp->GetName();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return name;
}

VertexLayout InstanceTriangleMesh::GetVertexLayout() {
    VertexLayout layout;

    if (m_Imp != NULL) {
        layout = m_Imp->GetVertexLayout();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return layout;
}

int32_t InstanceTriangleMesh::GetVertexNum() {
    int32_t vn = 0;

    if (m_Imp != NULL) {
        vn = m_Imp->GetVertexNum();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return vn;
}

VertexBuffer* InstanceTriangleMesh::GetVertexBuffer() {
    VertexBuffer* buf = NULL;

    if (m_Imp != NULL) {
        buf = m_Imp->GetVertexBuffer();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return buf;
}

void InstanceTriangleMesh::UpdateInstanceBuffer(void* data) {
    if (m_Imp != NULL) {
        m_Imp->UpdateInstanceBuffer(data);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

//-----------------------------------------------------------------------------------
// DynamicTriangleMesh DEFINITION
//-----------------------------------------------------------------------------------

DynamicTriangleMesh::DynamicTriangleMesh()
: m_Imp(NULL) {
}

DynamicTriangleMesh::~DynamicTriangleMesh() {
    GLB_SAFE_DELETE(m_Imp);
}

DynamicTriangleMesh* DynamicTriangleMesh::Create(int32_t maxTriangleNum) {
    DynamicTriangleMesh* mesh = NULL;

    DynamicTriangleMesh::Imp* imp = DynamicTriangleMesh::Imp::Create(maxTriangleNum);
    if (imp != NULL) {
        mesh = new DynamicTriangleMesh;
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

VertexLayout DynamicTriangleMesh::GetVertexLayout() {
    VertexLayout layout;

    if (m_Imp != NULL) {
        layout = m_Imp->GetVertexLayout();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return layout;
}

shader::Descriptor DynamicTriangleMesh::GetShaderDesc() {
    shader::Descriptor desc;

    if (m_Imp != NULL) {
        desc = m_Imp->GetShaderDesc();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return desc;
}

int32_t DynamicTriangleMesh::GetVertexNum() {
    int32_t result = 0;

    if (m_Imp) {
        result = m_Imp->GetVertexNum();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

VertexBuffer* DynamicTriangleMesh::GetVertexBuffer() {
    VertexBuffer* buf = NULL;

    if (m_Imp != NULL) {
        buf = m_Imp->GetVertexBuffer();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return buf;
}

void DynamicTriangleMesh::AddRect(math::Vector lt, math::Vector rb, math::Vector color) {
    if (m_Imp != NULL) {
        m_Imp->AddRect(lt, rb, color);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void DynamicTriangleMesh::AddRect(math::Vector v0, math::Vector c0, math::Vector uv0,
                                math::Vector v1, math::Vector c1, math::Vector uv1,
                                math::Vector v2, math::Vector c2, math::Vector uv2,
                                math::Vector v3, math::Vector c3, math::Vector uv3) {
    if (m_Imp != NULL) {
        m_Imp->AddRect(v0, c0, uv0, v1, c1, uv1, v2, c2, uv2, v3, c3, uv3);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void DynamicTriangleMesh::Clear() {
    if (m_Imp) {
        m_Imp->Clear();
    } else {
        GLB_SAFE_ASSERT(false);
    }
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

void DebugMesh::AddLine(math::Vector start, math::Vector end, math::Vector color) {
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
        num = m_Imp->GetVertexNum();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return num;
}

VertexBuffer* DebugMesh::GetVertexBuffer() {
    VertexBuffer* buf = NULL;

    if (m_Imp != NULL) {
        buf = m_Imp->GetVertexBuffer();
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
// FontMesh DEFINITION
//-----------------------------------------------------------------------------------
FontMesh::FontMesh()
: m_Imp(NULL) {
}

FontMesh::~FontMesh() {
    GLB_SAFE_DELETE(m_Imp);
}

FontMesh* FontMesh::Create(int32_t maxCharacter) {
    FontMesh* mesh = NULL;

    FontMesh::Imp* imp = FontMesh::Imp::Create(maxCharacter);
    if (imp != NULL) {
        mesh = new FontMesh;
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

void FontMesh::AddChar(math::Vector ltUV, math::Vector rbUV, math::Vector pos, math::Vector size, math::Vector color) {
    if (m_Imp != nullptr) {
        m_Imp->AddChar(ltUV, rbUV, pos, size, color);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void FontMesh::ClearAllChars() {
    if (m_Imp != NULL) {
        m_Imp->ClearAllChars();
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

VertexLayout FontMesh::GetVertexLayout() {
    VertexLayout layout;

    if (m_Imp != NULL) {
        layout = m_Imp->GetVertexLayout();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return layout;
}

int32_t FontMesh::GetVertexNum() {
    int32_t vn = 0;

    if (m_Imp != NULL) {
        vn = m_Imp->GetVertexNum();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return vn;
}

VertexBuffer* FontMesh::GetVertexBuffer() {
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

int32_t MgrImp::AddMesh(MeshBase* mesh) {
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

MeshBase* MgrImp::GetMeshById(int32_t id) {
    MeshBase* mesh = NULL;

    if (0 <= id && id < static_cast<int32_t>(m_MeshDataBase.size())) {
        mesh = m_MeshDataBase[id];
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return mesh;
}

MeshBase* MgrImp::GetMeshByName(std::string mesh_name) {
    MeshBase* mesh = NULL;

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

int32_t Mgr::AddMesh(MeshBase* mesh) {
    int32_t result = -1;

    if (s_MgrImp != NULL) {
        result = s_MgrImp->AddMesh(mesh);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

MeshBase* Mgr::GetMeshById(int32_t id) {
    MeshBase* mesh = NULL;

    if (s_MgrImp != NULL) {
        mesh = s_MgrImp->GetMeshById(id);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return mesh;
}

MeshBase* Mgr::GetMeshByName(std::string mesh_name) {
    MeshBase* mesh = NULL;
    
    if (s_MgrImp != NULL) {
        mesh = s_MgrImp->GetMeshByName(mesh_name);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return mesh;
}

};  // namespace mesh

};  // namespace render

};  // namespace glb
