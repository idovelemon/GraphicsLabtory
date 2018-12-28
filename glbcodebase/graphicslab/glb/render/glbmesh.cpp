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
#include "util/glbmeshreader.h"
#include "util/glbutil.h"
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
static MgrImp* s_MgrImp = nullptr;

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
    int32_t AddMesh(const char* fileName);
    int32_t AddInstanceMesh(const char* meshFile, int32_t maxInstance);
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
, m_Imp(nullptr) {
}

TriangleMesh::~TriangleMesh() {
    GLB_SAFE_DELETE(m_Imp);
}

TriangleMesh* TriangleMesh::Create(int32_t triangle_num, float* vertices, float* tex_coords, float* lightMapTexCoordBuf, float* normals, float* tangets, float* binormals) {
    TriangleMesh* triangle_mesh = nullptr;

    TriangleMesh::Imp* imp = TriangleMesh::Imp::Create(triangle_num, vertices, tex_coords, lightMapTexCoordBuf, normals, tangets, binormals);
    if (imp != nullptr) {
        triangle_mesh = new TriangleMesh;
        triangle_mesh->m_Imp = imp;
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return triangle_mesh;
}

void TriangleMesh::SetId(int32_t id) {
    if (m_Imp != nullptr) {
        m_Imp->SetId(id);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

int32_t TriangleMesh::GetId() {
    int32_t id = 0;

    if (m_Imp != nullptr) {
        id = m_Imp->GetId();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return id;
}

void TriangleMesh::SetName(std::string name) {
    if (m_Imp != nullptr) {
        m_Imp->SetName(name);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

std::string TriangleMesh::GetName() {
    std::string name;

    if (m_Imp != nullptr) {
        name = m_Imp->GetName();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return name;
}

void TriangleMesh::SetBoundBox(math::Vector min, math::Vector max) {
    if (m_Imp != nullptr) {
        m_Imp->SetBoundBox(min, max);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

math::Vector TriangleMesh::GetBoundBoxMin() {
    math::Vector result;

    if (m_Imp != nullptr) {
        result = m_Imp->GetBoundBoxMin();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

math::Vector TriangleMesh::GetBoundBoxMax() {
    math::Vector result;

    if (m_Imp != nullptr) {
        result = m_Imp->GetBoundBoxMax();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

VertexLayout TriangleMesh::GetVertexLayout() {
    VertexLayout layout;

    if (m_Imp != nullptr) {
        layout = m_Imp->GetVertexLayout();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return layout;
}

int32_t TriangleMesh::GetVertexNum() {
    int32_t vn = 0;

    if (m_Imp != nullptr) {
        vn = m_Imp->GetVertexNum();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return vn;
}

VertexBuffer* TriangleMesh::GetVertexBuffer() {
    VertexBuffer* buf = nullptr;

    if (m_Imp != nullptr) {
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
, m_Imp(nullptr) {
}

InstanceTriangleMesh::~InstanceTriangleMesh() {
    GLB_SAFE_DELETE(m_Imp);
}

InstanceTriangleMesh* InstanceTriangleMesh::Create(int32_t maxInstance, int32_t triangle_num, float* vertices, float* tex_coords, float* lightMapTexCoordBuf, float* normals, float* tangets, float* binormals) {
    InstanceTriangleMesh* triangle_mesh = nullptr;

    InstanceTriangleMesh::Imp* imp = InstanceTriangleMesh::Imp::Create(maxInstance, triangle_num, vertices, tex_coords, lightMapTexCoordBuf, normals, tangets, binormals);
    if (imp != nullptr) {
        triangle_mesh = new InstanceTriangleMesh;
        triangle_mesh->m_Imp = imp;
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return triangle_mesh;
}

void InstanceTriangleMesh::SetId(int32_t id) {
    if (m_Imp != nullptr) {
        m_Imp->SetId(id);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

int32_t InstanceTriangleMesh::GetId() {
    int32_t id = 0;

    if (m_Imp != nullptr) {
        id = m_Imp->GetId();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return id;
}

void InstanceTriangleMesh::SetName(std::string name) {
    if (m_Imp != nullptr) {
        m_Imp->SetName(name);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

std::string InstanceTriangleMesh::GetName() {
    std::string name;

    if (m_Imp != nullptr) {
        name = m_Imp->GetName();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return name;
}

void InstanceTriangleMesh::SetBoundBox(math::Vector min, math::Vector max) {
    if (m_Imp != nullptr) {
        m_Imp->SetBoundBox(min, max);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

math::Vector InstanceTriangleMesh::GetBoundBoxMin() {
    math::Vector result;

    if (m_Imp != nullptr) {
        result = m_Imp->GetBoundBoxMin();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

math::Vector InstanceTriangleMesh::GetBoundBoxMax() {
    math::Vector result;

    if (m_Imp != nullptr) {
        result = m_Imp->GetBoundBoxMax();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

VertexLayout InstanceTriangleMesh::GetVertexLayout() {
    VertexLayout layout;

    if (m_Imp != nullptr) {
        layout = m_Imp->GetVertexLayout();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return layout;
}

int32_t InstanceTriangleMesh::GetVertexNum() {
    int32_t vn = 0;

    if (m_Imp != nullptr) {
        vn = m_Imp->GetVertexNum();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return vn;
}

VertexBuffer* InstanceTriangleMesh::GetVertexBuffer() {
    VertexBuffer* buf = nullptr;

    if (m_Imp != nullptr) {
        buf = m_Imp->GetVertexBuffer();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return buf;
}

void InstanceTriangleMesh::UpdateInstanceBuffer(void* data) {
    if (m_Imp != nullptr) {
        m_Imp->UpdateInstanceBuffer(data);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

//-----------------------------------------------------------------------------------
// DynamicTriangleMesh DEFINITION
//-----------------------------------------------------------------------------------

DynamicTriangleMesh::DynamicTriangleMesh()
: m_Imp(nullptr) {
}

DynamicTriangleMesh::~DynamicTriangleMesh() {
    GLB_SAFE_DELETE(m_Imp);
}

DynamicTriangleMesh* DynamicTriangleMesh::Create(int32_t maxTriangleNum) {
    DynamicTriangleMesh* mesh = nullptr;

    DynamicTriangleMesh::Imp* imp = DynamicTriangleMesh::Imp::Create(maxTriangleNum);
    if (imp != nullptr) {
        mesh = new DynamicTriangleMesh;
        if (mesh != nullptr) {
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

    if (m_Imp != nullptr) {
        layout = m_Imp->GetVertexLayout();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return layout;
}

shader::Descriptor DynamicTriangleMesh::GetShaderDesc() {
    shader::Descriptor desc;

    if (m_Imp != nullptr) {
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
    VertexBuffer* buf = nullptr;

    if (m_Imp != nullptr) {
        buf = m_Imp->GetVertexBuffer();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return buf;
}

void DynamicTriangleMesh::AddRect(math::Vector lt, math::Vector rb, math::Vector color) {
    if (m_Imp != nullptr) {
        m_Imp->AddRect(lt, rb, color);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void DynamicTriangleMesh::AddRect(math::Vector v0, math::Vector c0, math::Vector uv0,
                                math::Vector v1, math::Vector c1, math::Vector uv1,
                                math::Vector v2, math::Vector c2, math::Vector uv2,
                                math::Vector v3, math::Vector c3, math::Vector uv3) {
    if (m_Imp != nullptr) {
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
: m_Imp(nullptr) {
}

DebugMesh::~DebugMesh() {
    GLB_SAFE_DELETE(m_Imp);
}

DebugMesh* DebugMesh::Create(int32_t max_lines) {
    DebugMesh* mesh = nullptr;

    DebugMesh::Imp* imp = DebugMesh::Imp::Create(max_lines);
    if (imp != nullptr) {
        mesh = new DebugMesh;
        if (mesh != nullptr) {
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
    if (m_Imp != nullptr) {
        m_Imp->AddLine(start, end, color);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void DebugMesh::ClearAllLines() {
    if (m_Imp != nullptr) {
        m_Imp->ClearAllLines();
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

VertexLayout DebugMesh::GetVertexLayout() {
    VertexLayout layout;

    if (m_Imp != nullptr) {
        layout = m_Imp->GetVertexLayout();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return layout;
}

shader::Descriptor DebugMesh::GetShaderDesc() {
    shader::Descriptor desc;

    if (m_Imp != nullptr) {
        desc = m_Imp->GetShaderDesc();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return desc;
}

int32_t DebugMesh::GetVertexNum() {
    int32_t num = 0;

    if (m_Imp != nullptr) {
        num = m_Imp->GetVertexNum();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return num;
}

VertexBuffer* DebugMesh::GetVertexBuffer() {
    VertexBuffer* buf = nullptr;

    if (m_Imp != nullptr) {
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
: m_Imp(nullptr) {
}

ScreenMesh::~ScreenMesh() {
    GLB_SAFE_DELETE(m_Imp);
}

ScreenMesh* ScreenMesh::Create(int32_t width, int32_t height) {
    ScreenMesh* mesh = nullptr;

    ScreenMesh::Imp* imp = ScreenMesh::Imp::Create(width, height);
    if (imp != nullptr) {
        mesh = new ScreenMesh;
        if (mesh != nullptr) {
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

    if (m_Imp != nullptr) {
        layout = m_Imp->GetVertexLayout();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return layout;
}

int32_t ScreenMesh::GetVertexNum() {
    int32_t vn = 0;

    if (m_Imp != nullptr) {
        vn = m_Imp->GetVertexNum();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return vn;
}

VertexBuffer* ScreenMesh::GetVertexBuffer() {
    VertexBuffer* buf = nullptr;

    if (m_Imp != nullptr) {
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
: m_Imp(nullptr) {
}

FontMesh::~FontMesh() {
    GLB_SAFE_DELETE(m_Imp);
}

FontMesh* FontMesh::Create(int32_t maxCharacter) {
    FontMesh* mesh = nullptr;

    FontMesh::Imp* imp = FontMesh::Imp::Create(maxCharacter);
    if (imp != nullptr) {
        mesh = new FontMesh;
        if (mesh != nullptr) {
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
    if (m_Imp != nullptr) {
        m_Imp->ClearAllChars();
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

VertexLayout FontMesh::GetVertexLayout() {
    VertexLayout layout;

    if (m_Imp != nullptr) {
        layout = m_Imp->GetVertexLayout();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return layout;
}

int32_t FontMesh::GetVertexNum() {
    int32_t vn = 0;

    if (m_Imp != nullptr) {
        vn = m_Imp->GetVertexNum();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return vn;
}

VertexBuffer* FontMesh::GetVertexBuffer() {
    VertexBuffer* buf = nullptr;

    if (m_Imp != nullptr) {
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

    if (mesh != nullptr) {
        result = m_MeshDataBase.size();
        m_MeshDataBase.push_back(mesh);
        mesh->SetId(result);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

int32_t MgrImp::AddMesh(const char* fileName) {
    int32_t result = -1;

    if (fileName != nullptr) {
        render::mesh::TriangleMesh* mesh = nullptr;

        float* vertexBuf = nullptr;
        float* texBuf = nullptr;
        float* lightMapTexBuf = nullptr;
        float* normalBuf = nullptr;
        float* tangentBuf = nullptr;
        float* binormalBuf = nullptr;
        math::Vector boundBoxMin(0.0f, 0.0f, 0.0f);
        math::Vector boundBoxMax(0.0f, 0.0f, 0.0f);
        std::string dir = util::path_get_dir(fileName);
        int32_t num_triangles = util::MeshReader::ExtractModelData(
            fileName,
            boundBoxMin,
            boundBoxMax,
            &vertexBuf,
            &texBuf,
            &lightMapTexBuf,
            &normalBuf,
            &tangentBuf,
            &binormalBuf
            );
        if (num_triangles > 0) {
            mesh = render::mesh::TriangleMesh::Create(num_triangles, vertexBuf, texBuf, lightMapTexBuf, normalBuf, tangentBuf, binormalBuf);
            mesh->SetBoundBox(boundBoxMin, boundBoxMax);
            result = render::mesh::Mgr::AddMesh(mesh);

            util::MeshReader::RelaseBuf(&vertexBuf, &texBuf, &normalBuf, &tangentBuf, &binormalBuf);
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

int32_t MgrImp::AddInstanceMesh(const char* meshFile, int32_t maxInstance) {
    int32_t result = -1;

    if (meshFile != nullptr) {
        render::mesh::InstanceTriangleMesh* mesh = nullptr;

        float* vertexBuf = nullptr;
        float* texBuf = nullptr;
        float* lightMapTexBuf = nullptr;
        float* normalBuf = nullptr;
        float* tangentBuf = nullptr;
        float* binormalBuf = nullptr;
        math::Vector boundBoxMin(0.0f, 0.0f, 0.0f);
        math::Vector boundBoxMax(0.0f, 0.0f, 0.0f);
        std::string dir = util::path_get_dir(meshFile);
        int32_t num_triangles = util::MeshReader::ExtractModelData(
            meshFile,
            boundBoxMin,
            boundBoxMax,
            &vertexBuf,
            &texBuf,
            &lightMapTexBuf,
            &normalBuf,
            &tangentBuf,
            &binormalBuf
            );
        if (num_triangles > 0) {
            mesh = render::mesh::InstanceTriangleMesh::Create(maxInstance, num_triangles, vertexBuf, texBuf, lightMapTexBuf, normalBuf, tangentBuf, binormalBuf);
            mesh->SetBoundBox(boundBoxMin, boundBoxMax);
            result = render::mesh::Mgr::AddMesh(mesh);

            util::MeshReader::RelaseBuf(&vertexBuf, &texBuf, &normalBuf, &tangentBuf, &binormalBuf);
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

MeshBase* MgrImp::GetMeshById(int32_t id) {
    MeshBase* mesh = nullptr;

    if (0 <= id && id < static_cast<int32_t>(m_MeshDataBase.size())) {
        mesh = m_MeshDataBase[id];
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return mesh;
}

MeshBase* MgrImp::GetMeshByName(std::string mesh_name) {
    MeshBase* mesh = nullptr;

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
    if (s_MgrImp == nullptr) {
        s_MgrImp = new MgrImp;
        if (s_MgrImp != nullptr) {
            s_MgrImp->Initialize();
        } else {
            GLB_SAFE_ASSERT(false);
        }
    }
}

void Mgr::Destroy() {
    if (s_MgrImp != nullptr) {
        s_MgrImp->Destroy();
        GLB_SAFE_DELETE(s_MgrImp);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

int32_t Mgr::AddMesh(const char* meshFile) {
    int32_t result = -1;

    if (s_MgrImp != nullptr) {
        result = s_MgrImp->AddMesh(meshFile);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

int32_t Mgr::AddMesh(MeshBase* mesh) {
    int32_t result = -1;

    if (s_MgrImp != nullptr) {
        result = s_MgrImp->AddMesh(mesh);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

int32_t Mgr::AddInstanceMesh(const char* meshFile, int32_t maxInstance) {
    int32_t result = -1;

    if (s_MgrImp != nullptr) {
        result = s_MgrImp->AddInstanceMesh(meshFile, maxInstance);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

MeshBase* Mgr::GetMeshById(int32_t id) {
    MeshBase* mesh = nullptr;

    if (s_MgrImp != nullptr) {
        mesh = s_MgrImp->GetMeshById(id);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return mesh;
}

MeshBase* Mgr::GetMeshByName(std::string mesh_name) {
    MeshBase* mesh = nullptr;
    
    if (s_MgrImp != nullptr) {
        mesh = s_MgrImp->GetMeshByName(mesh_name);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return mesh;
}

};  // namespace mesh

};  // namespace render

};  // namespace glb
