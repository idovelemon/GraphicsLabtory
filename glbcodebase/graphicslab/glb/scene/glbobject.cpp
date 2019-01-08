//-----------------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 06 / 14
// Brief: Define a object
//-----------------------------------------------------------------------------------
#include "glbobject.h"

#include <fstream>

#include "render/glbmaterial.h"
#include "util/glbmacro.h"

namespace glb {

namespace scene {

//---------------------------------------------------------------------------------------------

Object::Object()
: m_ObjectType(OBJECT_TYPE_NORMAL)
, m_IsDead(false)
, m_ObjectId(-1)
, m_Mesh(nullptr)
, m_Material(nullptr)
, m_WorldMatrix()
, m_EnableDraw(true)
, m_EnableCullFace(false)
, m_EnableDepthTest(false)
, m_EnableAlphaBlend(false)
, m_CullMode(render::CULL_NONE) {
}

Object::~Object() {
    m_Mesh = nullptr;
    m_Material = nullptr;
}

Object* Object::Create(const char* fileName, math::Vector pos, math::Vector scale, math::Vector rotation) {
    Object* obj = nullptr;
    
    if (fileName) {
        render::mesh::MeshBase* mesh = render::mesh::Mgr::GetMeshByName(fileName);
        if (mesh == nullptr) {
            mesh = render::mesh::Mgr::GetMeshById(render::mesh::Mgr::AddMesh(fileName));
        }

        if (mesh) {
            obj = new Object();
            if (obj) {
                obj->m_Mesh = mesh;
                obj->m_WorldMatrix.MakeIdentityMatrix();
            } else {
                GLB_SAFE_ASSERT(false);
            }
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return obj;
}

Object* Object::Create(const char* meshFile, const char* materialGroupFile, math::Vector pos, math::Vector scale, math::Vector rotation) {
    Object* obj = nullptr;

    render::mesh::MeshBase* mesh = render::mesh::Mgr::GetMeshByName(meshFile);
    if (mesh == nullptr) {
        mesh = render::mesh::Mgr::GetMeshById(render::mesh::Mgr::AddMesh(meshFile));
    }

    render::material::Material* material;
    if (materialGroupFile) {
        material = render::material::Mgr::GetMaterial(render::material::Mgr::AddMaterial(materialGroupFile));
    } else {
        GLB_SAFE_ASSERT(false);
    }

    if (mesh) {
        obj = new Object();
        if (obj) {
            obj->m_Mesh = mesh;
            obj->m_Material = material;
            obj->m_WorldMatrix.MakeIdentityMatrix();
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return obj;
}

Object* Object::Create(render::mesh::MeshBase* mesh, math::Vector pos, math::Vector scale, math::Vector rotation) {
    Object* obj = nullptr;

    if (mesh) {
        obj = new Object();
        if (obj) {
            obj->m_Mesh = mesh;
            obj->m_WorldMatrix.MakeIdentityMatrix();
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return obj;
}

int32_t Object::GetObjectType() const {
    return m_ObjectType;
}

void Object::SetObjectId(int32_t id) {
    m_ObjectId = id;
}

int32_t Object::GetObjectId() const {
    return m_ObjectId;
}

void Object::SetDead(bool dead) {
    m_IsDead = dead;
}

bool Object::IsDead() const {
    return m_IsDead;
}

render::mesh::MeshBase* Object::GetMesh() {
    return m_Mesh;
}

math::Vector Object::GetBoundBoxMax() {
    math::Vector boundboxMax = m_Mesh->GetBoundBoxMax();
    boundboxMax = boundboxMax * m_WorldMatrix.GetScale();
    boundboxMax += m_WorldMatrix.GetTranslation();
    return boundboxMax;
}

math::Vector Object::GetBoundBoxMin() {
    math::Vector boundboxMin = m_Mesh->GetBoundBoxMin();
    boundboxMin = boundboxMin * m_WorldMatrix.GetScale();
    boundboxMin += m_WorldMatrix.GetTranslation();
    return boundboxMin;
}

math::Vector Object::GetPos() {
    return m_WorldMatrix.GetTranslation();
}

math::Vector Object::GetScale() {
    return m_WorldMatrix.GetScale();
}

math::Matrix Object::GetWorldMatrix() const {
    return m_WorldMatrix;
}

void Object::SetWorldMatrix(math::Matrix worldMatrix) {
    m_WorldMatrix = worldMatrix;
}

render::material::Material* Object::GetMaterial() {
    return m_Material;
}

void Object::SetDrawEnable(bool enable) {
    m_EnableDraw = enable;
}

bool Object::IsDrawEnable() {
    return m_EnableDraw;
}

void Object::SetAlphaBlendEnable(bool enable) {
    m_EnableAlphaBlend = enable;
}

bool Object::IsAlphaBlendEnable() {
    return m_EnableAlphaBlend;
}

void Object::SetAlphaBlendFunc(render::AlphaBlendFactor factor, render::AlphaBlendFunc func) {
    if (factor == render::FACTOR_SRC) {
        m_SrcBlendFunc = func;
    } else if (factor == render::FACTOR_DST) {
        m_DstBlendFunc = func;
    }
}

render::AlphaBlendFunc Object::GetAlphaBlendFunc(render::AlphaBlendFactor factor) {
    render::AlphaBlendFunc func = render::FUNC_ZERO;

    if (factor == render::FACTOR_SRC) {
        func = m_SrcBlendFunc;
    } else if (factor == render::FACTOR_DST) {
        func = m_DstBlendFunc;
    }

    return func;
}

void Object::SetCullFaceEnable(bool enable) {
    m_EnableCullFace = enable;
}

bool Object::IsCullFaceEnable() {
    return m_EnableCullFace;
}

void Object::SetDepthTestEnable(bool enable) {
    m_EnableDepthTest = enable;
}

bool Object::IsDepthTestEnable() {
    return m_EnableDepthTest;
}

void Object::SetCullFaceMode(render::CullMode cull_mode) {
    m_CullMode = cull_mode;
}

render::CullMode Object::GetCullFaceMode() {
    return m_CullMode;
}

void Object::Update() {
}

//---------------------------------------------------------------------------------------------

DecalObject::DecalObject() {
    m_ObjectType = OBJECT_TYPE_DECAL;
}

DecalObject::~DecalObject() {
}

DecalObject* DecalObject::Create(const char* decalObjectFile, math::Vector pos, math::Vector scale, math::Vector rotation) {
    DecalObject* obj = nullptr;
    
    if (decalObjectFile) {
        render::mesh::MeshBase* mesh = render::mesh::Mgr::GetMeshByName(decalObjectFile);
        if (mesh == nullptr) {
            mesh = render::mesh::Mgr::GetMeshById(render::mesh::Mgr::AddMesh(decalObjectFile));
        }

        if (mesh) {
            obj = new DecalObject();
            if (obj) {
                obj->m_Mesh = mesh;
                obj->m_WorldMatrix.MakeIdentityMatrix();
            } else {
                GLB_SAFE_ASSERT(false);
            }
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return obj;
}

DecalObject* DecalObject::Create(const char* meshFile, const char* materialFile, math::Vector pos, math::Vector scale, math::Vector rotation) {
    DecalObject* obj = nullptr;
    
    if (meshFile && materialFile) {
        render::mesh::MeshBase* mesh = render::mesh::Mgr::GetMeshByName(meshFile);
        if (mesh == nullptr) {
            mesh = render::mesh::Mgr::GetMeshById(render::mesh::Mgr::AddMesh(meshFile));
        }

        render::material::Material* material = render::material::Material::Create(materialFile);

        if (mesh && material->GetPassMaterial(render::kDecalPassName) != nullptr) {
            obj = new DecalObject();
            if (obj) {
                obj->m_Mesh = mesh;
                obj->m_Material = material;
                obj->m_WorldMatrix.MakeIdentityMatrix();
            } else {
                GLB_SAFE_ASSERT(false);
            }
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return obj;
}

//---------------------------------------------------------------------------------------------

InstanceRenderObject::InstanceRenderObject()
: m_MaxInstanceNum(0)
, m_CurInstanceNum(0)
, m_MatrixBuf(nullptr) {
    m_ObjectType = OBJECT_TYPE_INSTANCE_RENDER;
}

InstanceRenderObject::~InstanceRenderObject() {
    RemoveAllInstanceObject();
    GLB_SAFE_DELETE(m_MatrixBuf);
}

InstanceRenderObject* InstanceRenderObject::Create(const char* meshFile, int32_t maxInstance) {
    InstanceRenderObject* obj = nullptr;

    if (meshFile != nullptr) {
        render::mesh::MeshBase* mesh = render::mesh::Mgr::GetMeshByName(meshFile);
        if (mesh == nullptr) {
            mesh = render::mesh::Mgr::GetMeshById(render::mesh::Mgr::AddInstanceMesh(meshFile, maxInstance));
        }

        if (mesh) {
            obj = new InstanceRenderObject();
            obj->m_Mesh = mesh;
            obj->m_MaxInstanceNum = maxInstance;
            obj->m_MatrixBuf = new float[maxInstance * 16 * 2];  // WorldMatrix and TransInvWorldMatrix
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return obj;
}

InstanceRenderObject* InstanceRenderObject::Create(const char* meshFile, const char* materialFile, int32_t maxInstance) {
    InstanceRenderObject* obj = nullptr;

    if (meshFile != nullptr && materialFile != nullptr) {
        render::mesh::MeshBase* mesh = render::mesh::Mgr::GetMeshByName(meshFile);
        if (mesh == nullptr) {
            mesh = render::mesh::Mgr::GetMeshById(render::mesh::Mgr::AddInstanceMesh(meshFile, maxInstance));
        }

        render::material::Material* material = render::material::Material::Create(materialFile);

        if (mesh) {
            obj = new InstanceRenderObject();
            obj->m_Mesh = mesh;
            obj->m_Material = material;
            obj->m_MaxInstanceNum = maxInstance;
            obj->m_MatrixBuf = new float[maxInstance * 16 * 2];  // WorldMatrix and TransInvWorldMatrix
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return obj;
}

void InstanceRenderObject::Update() {
    InstanceObjectMap::iterator it = m_InstanceObjects.begin();

    std::vector<math::Matrix> matV;
    for (int32_t i = 0; i < m_CurInstanceNum; i++) {
        // Build world transform matrix
        math::Matrix worldMatrix;
        worldMatrix.MakeIdentityMatrix();
        worldMatrix = it->second->GetWorldMatrix();

        matV.push_back(worldMatrix);
        it++;
    }

    // Update world matrix attribute
    for (std::vector<math::Matrix>::size_type i = 0; i < matV.size(); i++) {
        // OpenGL store matrix in transpose, so we must transpose the result matrix and upload to opengl server
        math::Matrix world = matV[i];
        world.Transpose();
        memcpy(m_MatrixBuf + i * 16, world.m_Matrix.v, sizeof(world.m_Matrix.v));

        math::Matrix transInvM = matV[i];
        transInvM.Inverse();

        memcpy(m_MatrixBuf + (m_MaxInstanceNum + i) * 16, transInvM.m_Matrix.v, sizeof(transInvM.m_Matrix.v));
    }

    render::mesh::InstanceTriangleMesh* mesh = reinterpret_cast<render::mesh::InstanceTriangleMesh*>(m_Mesh);
    if (mesh) {
        mesh->UpdateInstanceBuffer(m_MatrixBuf);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void InstanceRenderObject::AddInstanceObject(InstanceObject* obj) {
    if (obj != nullptr) {
        if (m_CurInstanceNum < m_MaxInstanceNum) {
            m_InstanceObjects.insert(std::pair<int32_t, InstanceObject*>(obj->GetObjectId(), obj));
            m_CurInstanceNum++;
        } else {
            GLB_SAFE_ASSERT(false);  // Too much geometry instance, try to increase the max instance number
        }
    }
}

void InstanceRenderObject::RemoveInstanceObject(InstanceObject* obj) {
    if (obj != nullptr) {
        InstanceObjectMap::iterator it = m_InstanceObjects.find(obj->GetObjectId());
        if (it != m_InstanceObjects.end()) {
            obj->ClearInstanceRenderObject();
            m_InstanceObjects.erase(it);
            m_CurInstanceNum--;
        } else {
            GLB_SAFE_ASSERT(false);
        }
    }
}

void InstanceRenderObject::RemoveAllInstanceObject() {
    InstanceObjectMap::iterator it = m_InstanceObjects.begin();

    for (; it != m_InstanceObjects.end(); ++it) {
        if (it->second != nullptr) {
            it->second->ClearInstanceRenderObject();
            it->second = nullptr;
        }
    }

    m_InstanceObjects.clear();
    m_CurInstanceNum = 0;
}

int32_t InstanceRenderObject::GetCurInstanceNum() const {
    return m_CurInstanceNum;
}

//---------------------------------------------------------------------------------------------

InstanceObject::InstanceObject()
: m_InstanceRenderObject(nullptr) {
    m_ObjectType = OBJECT_TYPE_INSTANCE;
}

InstanceObject::~InstanceObject() {
    if (m_InstanceRenderObject) {
        m_InstanceRenderObject->RemoveInstanceObject(this);
    }
    m_InstanceRenderObject = nullptr;
}

InstanceObject* InstanceObject::Create(InstanceRenderObject* instanceRenderObject, math::Vector pos, math::Vector scale, math::Vector rotate) {
    InstanceObject* obj = nullptr;

    if (instanceRenderObject != nullptr) {
        obj = new InstanceObject();
        obj->m_InstanceRenderObject = instanceRenderObject;
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return obj;
}

math::Vector InstanceObject::GetBoundBoxMax() {
    math::Vector boxMax(0.0f, 0.0f, 0.0f);

    if (m_InstanceRenderObject) {
        boxMax = m_InstanceRenderObject->GetMesh()->GetBoundBoxMax();
        boxMax = boxMax + m_WorldMatrix.GetTranslation();
    }

    return boxMax;
}

math::Vector InstanceObject::GetBoundBoxMin() {
    math::Vector boxMin(0.0f, 0.0f, 0.0f);

    if (m_InstanceRenderObject) {
        boxMin = m_InstanceRenderObject->GetMesh()->GetBoundBoxMin();
        boxMin = boxMin + m_WorldMatrix.GetTranslation();
    }

    return boxMin;
}

render::material::Material* InstanceObject::GetMaterial() {
    render::material::Material* material = nullptr;

    if (m_InstanceRenderObject) {
        material = m_InstanceRenderObject->GetMaterial();
    }

    return material;
}

void InstanceObject::Update() {
    Object::Update();
}

void InstanceObject::ClearInstanceRenderObject() {
    m_InstanceRenderObject = nullptr;
}

};  // namespace scene

};  // namespace glb
