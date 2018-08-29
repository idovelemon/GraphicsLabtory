//-------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 09 / 26
// Brief: Scene hold all the object that need to be draw
//-------------------------------------------------------
#include "glbscene.h"

#include <vector>

#include "render/glbrender.h"

#include "scene/glbcamera.h"
#include "scene/glbobject.h"

#include "util/glbmacro.h"

namespace glb {

namespace scene {

//-----------------------------------------------------------------------------------
// TYPE DECLARATION
//-----------------------------------------------------------------------------------
class SceneImp;
static SceneImp* s_SceneImp = NULL;

static const int32_t kObjectMaxNum = 10000;
//-----------------------------------------------------------------------------------
// CLASS DECLARATION
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// SceneImp DECLARATION
//----------------------------------------------------------------------------------
class SceneImp {
public:
    SceneImp();
    virtual~SceneImp();

public:
    void Initialize();
    void Destroy();
    void Update();

    // Space partioning
    math::Vector GetSceneBoundBoxMax();
    math::Vector GetSceneBoundBoxMin();

    // Object
    int32_t AddObject(const char* objectFile);
    int32_t AddDecalObject(const char* decalObjectFile);
    int32_t AddInstanceRenderObject(const char* objectFile, int32_t maxInstance);
    int32_t AddInstanceObject(int32_t instanceRenderObject, math::Vector pos, math::Vector scale, math::Vector rotate);
    int32_t AddObject(scene::Model* model);
    int32_t AddSkyObject(const char* objectFile);
    Object* GetObjectById(int32_t id);
    Object* GetSkyObject();
    void GetAllObjects(std::vector<Object*>& objs);
    void DestroyObject(int32_t id);

    // Light
    void SetLight(Light lit, int32_t id);
    Light GetLight(int32_t id);

    // Camera
    void SetCamera(int32_t type, CameraBase* cam);
    CameraBase* GetCamera(int32_t type);
    void SetCurCamera(int32_t type);
    int32_t GetCurCameraType();
    CameraBase* GetCurCamera();

    // Debug Draw
    void AddBoundBox(math::Vector color);

protected:
    int32_t FindEmptyID();

private:
    int32_t              m_CurCameraType;
    std::vector<Object*> m_ObjectDataBase;
    Light                m_Lights[kMaxLight];
    CameraBase*          m_Camera[MAX_CAM];
    Object*              m_SkyObject;
    math::Vector         m_BoundBoxMax;
    math::Vector         m_BoundBoxMin;
};

//-----------------------------------------------------------------------------------
// CLASS DEFINITION
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// SceneImp DEFINITION
//-----------------------------------------------------------------------------------
SceneImp::SceneImp()
: m_CurCameraType(PRIMIAY_CAM)
, m_ObjectDataBase()
, m_SkyObject(NULL)
, m_BoundBoxMax(0.0f, 0.0f, 0.0f)
, m_BoundBoxMin(0.0f, 0.0f, 0.0f) {
    memset(m_Camera, 0, sizeof(m_Camera));
    m_ObjectDataBase.resize(10000, NULL);
}

SceneImp::~SceneImp() {
    Destroy();
}

void SceneImp::Initialize() {
}

void SceneImp::Destroy() {
    for (int32_t i = 0; i < static_cast<int32_t>(m_ObjectDataBase.size()); i++) {
        GLB_SAFE_DELETE(m_ObjectDataBase[i]);
    }
    m_ObjectDataBase.clear();

    for (int32_t i = 0; i < MAX_CAM; i++) {
        GLB_SAFE_DELETE(m_Camera[i]);
    }

    GLB_SAFE_DELETE(m_SkyObject);
}

void SceneImp::Update() {
    for (int32_t i = 0; i < MAX_CAM; i++) {
        if (m_Camera[i] != NULL) {
            m_Camera[i]->Update(0.0f);
        } 
    }

    if (m_SkyObject != NULL && m_Camera[PRIMIAY_CAM] != NULL) {
        m_SkyObject->SetPos(m_Camera[PRIMIAY_CAM]->GetPos());
        m_SkyObject->Update();
    }

    // Remove dead objects
    for (int32_t i = 0; i < static_cast<int32_t>(m_ObjectDataBase.size()); i++) {
        if (m_ObjectDataBase[i] != NULL) {
            if (m_ObjectDataBase[i]->IsDead()) {
                GLB_SAFE_DELETE(m_ObjectDataBase[i]);
            }
        }
    }

    // Update
    m_BoundBoxMax = math::Vector(0.0f, 0.0f, 0.0f);
    m_BoundBoxMin = math::Vector(FLT_MAX, FLT_MAX, FLT_MAX);
    for (int32_t i = 0; i < static_cast<int32_t>(m_ObjectDataBase.size()); i++) {
        if (m_ObjectDataBase[i] != NULL) {
            m_ObjectDataBase[i]->Update();
            math::Vector max = m_ObjectDataBase[i]->GetBoundBoxMax();
            math::Vector min = m_ObjectDataBase[i]->GetBoundBoxMin();
            if (max.x > m_BoundBoxMax.x) {
                m_BoundBoxMax.x = max.x;
            }
            if (max.y > m_BoundBoxMax.y) {
                m_BoundBoxMax.y = max.y;
            }
            if (max.z > m_BoundBoxMax.z) {
                m_BoundBoxMax.z = max.z;
            }
            if (min.x < m_BoundBoxMin.x) {
                m_BoundBoxMin.x = min.x;
            }
            if (min.y < m_BoundBoxMin.y) {
                m_BoundBoxMin.y = min.y;
            }
            if (min.z < m_BoundBoxMin.z) {
                m_BoundBoxMin.z = min.z;
            }
        }
    }
}

math::Vector SceneImp::GetSceneBoundBoxMax() {
    return m_BoundBoxMax;
}

math::Vector SceneImp::GetSceneBoundBoxMin() {
    return m_BoundBoxMin;
}

int32_t SceneImp::AddObject(const char* object_file) {
    int32_t id = -1;

    if (object_file != NULL) {
        Object* obj = Object::Create(object_file);
        if (obj != NULL) {
            id = FindEmptyID();
            if (id != -1) {
                m_ObjectDataBase[id] = obj;
                obj->SetObjectId(id);
            } else {
                GLB_SAFE_ASSERT(false);
            }
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return id;
}

int32_t SceneImp::AddDecalObject(const char* decalObjectFile) {
    int32_t id = -1;

    if (decalObjectFile != NULL) {
        Object* obj = DecalObject::Create(decalObjectFile);
        if (obj != NULL) {
            id = FindEmptyID();
            if (id != -1) {
                m_ObjectDataBase[id] = obj;
                obj->SetObjectId(id);
            } else {
                GLB_SAFE_ASSERT(false);
            }
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return id;
}

int32_t SceneImp::AddInstanceRenderObject(const char* objectFile, int32_t maxInstance) {
    int32_t id = -1;

    if (objectFile != NULL) {
        Object* obj = InstanceRenderObject::Create(objectFile, maxInstance);
        if (obj != NULL) {
            id = FindEmptyID();
            if (id != -1) {
                m_ObjectDataBase[id] = obj;
                obj->SetObjectId(id);
            } else {
                GLB_SAFE_ASSERT(false);
            }
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return id;
}

int32_t SceneImp::AddInstanceObject(int32_t instanceRenderObjectId, math::Vector pos, math::Vector scale, math::Vector rotate) {
    int32_t id = -1;

    if (0 <= instanceRenderObjectId && instanceRenderObjectId < m_ObjectDataBase.size()) {
        InstanceRenderObject* instanceRenderObject = reinterpret_cast<InstanceRenderObject*>(m_ObjectDataBase[instanceRenderObjectId]);
        InstanceObject* obj = InstanceObject::Create(instanceRenderObject, pos, scale, rotate);
        if (obj != NULL) {
            id = FindEmptyID();
            if (id != -1) {
                m_ObjectDataBase[id] = obj;
                obj->SetObjectId(id);
                instanceRenderObject->AddInstanceObject(obj);
            } else {
                GLB_SAFE_ASSERT(false);
            }
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return id;
}

int32_t SceneImp::AddObject(scene::Model* model) {
    int32_t id = -1;

    if (model != NULL) {
        Object* obj = Object::Create(model);
        if (obj != NULL) {
            id = FindEmptyID();
            if (id != -1) {
                m_ObjectDataBase[id] = obj;
            } else {
                GLB_SAFE_ASSERT(false);
            }
        } else {
            GLB_SAFE_ASSERT(false);
        }
    }

    return id;
}

int32_t SceneImp::AddSkyObject(const char* object_file) {
    int32_t id = -1;

    if (object_file != NULL) {
        Object* obj = Object::Create(object_file);
        if (obj != NULL) {
            id = 0;
            m_SkyObject = obj;

            // Setup sky object
            m_SkyObject->SetCullFaceEnable(false);
            m_SkyObject->SetDepthTestEnable(false);
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return id;
}

Object* SceneImp::GetObjectById(int32_t id) {
    Object* obj = NULL;

    if (0 <= id && id < static_cast<int32_t>(m_ObjectDataBase.size())) {
        obj = m_ObjectDataBase[id];
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return obj;
}

Object* SceneImp::GetSkyObject() {
    return m_SkyObject;
}

void SceneImp::GetAllObjects(std::vector<Object*>& objs) {
    for (int32_t i = 0; i < static_cast<int32_t>(m_ObjectDataBase.size()); i++) {
        if (m_ObjectDataBase[i] != NULL && !m_ObjectDataBase[i]->IsDead()) {
            objs.push_back(m_ObjectDataBase[i]);
        }
    }
}

void SceneImp::DestroyObject(int32_t object_id) {
    if (0 <= object_id && object_id < static_cast<int32_t>(m_ObjectDataBase.size())) {
        if (m_ObjectDataBase[object_id] != NULL) {
            m_ObjectDataBase[object_id]->SetDead(true);
        }
    }
}

void SceneImp::SetLight(Light lit, int32_t id) {
    if (0 <= id && id < kMaxLight) {
        m_Lights[id] = lit;
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

Light SceneImp::GetLight(int32_t id) {
    Light lit(UNKNOWN_LIGHT);

    if (0 <= id && id < kMaxLight) {
        lit = m_Lights[id];
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return lit;
}

void SceneImp::SetCamera(int32_t type, CameraBase* cam) {
    if (0 <= type && type < MAX_CAM) {
        m_Camera[type] = cam;
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

CameraBase* SceneImp::GetCamera(int32_t type) {
    CameraBase* cam = NULL;

    if (0 <= type && type < MAX_CAM) {
        cam = m_Camera[type];
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return cam;
}

void SceneImp::SetCurCamera(int32_t type) {
    m_CurCameraType = type;
}

int32_t SceneImp::GetCurCameraType() {
    return m_CurCameraType;
}

CameraBase* SceneImp::GetCurCamera() {
    return m_Camera[m_CurCameraType];
}

void SceneImp::AddBoundBox(math::Vector color) {
    math::Vector view_space_points[8];
    view_space_points[render::Render::NLU] = math::Vector(m_BoundBoxMin.x, m_BoundBoxMax.y, m_BoundBoxMax.z);
    view_space_points[render::Render::NLD] = math::Vector(m_BoundBoxMin.x, m_BoundBoxMin.y, m_BoundBoxMax.z);
    view_space_points[render::Render::NRU] = math::Vector(m_BoundBoxMax.x, m_BoundBoxMax.y, m_BoundBoxMax.z);
    view_space_points[render::Render::NRD] = math::Vector(m_BoundBoxMax.x, m_BoundBoxMin.y, m_BoundBoxMax.z);
    view_space_points[render::Render::FLU] = math::Vector(m_BoundBoxMin.x, m_BoundBoxMax.y, m_BoundBoxMin.z);
    view_space_points[render::Render::FLD] = math::Vector(m_BoundBoxMin.x, m_BoundBoxMin.y, m_BoundBoxMin.z);
    view_space_points[render::Render::FRU] = math::Vector(m_BoundBoxMax.x, m_BoundBoxMax.y, m_BoundBoxMin.z);
    view_space_points[render::Render::FRD] = math::Vector(m_BoundBoxMax.x, m_BoundBoxMin.y, m_BoundBoxMin.z);

    render::Render::AddLine(view_space_points[render::Render::NLU], view_space_points[render::Render::NRU], color);
    render::Render::AddLine(view_space_points[render::Render::NRU], view_space_points[render::Render::NRD], color);
    render::Render::AddLine(view_space_points[render::Render::NRD], view_space_points[render::Render::NLD], color);
    render::Render::AddLine(view_space_points[render::Render::NLD], view_space_points[render::Render::NLU], color);
    render::Render::AddLine(view_space_points[render::Render::FLU], view_space_points[render::Render::FRU], color);
    render::Render::AddLine(view_space_points[render::Render::FRU], view_space_points[render::Render::FRD], color);
    render::Render::AddLine(view_space_points[render::Render::FRD], view_space_points[render::Render::FLD], color);
    render::Render::AddLine(view_space_points[render::Render::FLD], view_space_points[render::Render::FLU], color);
    render::Render::AddLine(view_space_points[render::Render::NLD], view_space_points[render::Render::FLD], color);
    render::Render::AddLine(view_space_points[render::Render::NLU], view_space_points[render::Render::FLU], color);
    render::Render::AddLine(view_space_points[render::Render::NRU], view_space_points[render::Render::FRU], color);
    render::Render::AddLine(view_space_points[render::Render::NRD], view_space_points[render::Render::FRD], color);
}

int32_t SceneImp::FindEmptyID() {
    int32_t id = -1;

    for (int32_t i = 0; i < static_cast<int32_t>(m_ObjectDataBase.size()); i++) {
        if (m_ObjectDataBase[i] == NULL) {
            id  = i;
            break;
        }
    }

    return id;
}
//-----------------------------------------------------------------------------------
// Scene DEFINITION
//-----------------------------------------------------------------------------------
void Scene::Initialize() {
    if (s_SceneImp == NULL) {
        s_SceneImp = new SceneImp;
        if (s_SceneImp != NULL) {
            s_SceneImp->Initialize();
        } else {
            GLB_SAFE_ASSERT(false);
        }
    }
}

void Scene::Destroy() {
    if (s_SceneImp != NULL) {
        s_SceneImp->Destroy();
        GLB_SAFE_DELETE(s_SceneImp);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Scene::Update() {
    if (s_SceneImp != NULL) {
        s_SceneImp->Update();
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

math::Vector Scene::GetSceneBoundBoxMax() {
    math::Vector result(0.0f, 0.0f, 0.0f);

    if (s_SceneImp != NULL) {
        result = s_SceneImp->GetSceneBoundBoxMax();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

math::Vector Scene::GetSceneBoundBoxMin() {
    math::Vector result(0.0f, 0.0f, 0.0f);

    if (s_SceneImp != NULL) {
        result = s_SceneImp->GetSceneBoundBoxMin();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

int32_t Scene::AddObject(const char* objectFile) {
    int32_t result = -1;

    if (s_SceneImp != NULL) {
        result = s_SceneImp->AddObject(objectFile);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

int32_t Scene::AddInstanceRenderObject(const char* objectFile, int32_t maxInstance) {
    int32_t result = -1;

    if (s_SceneImp != NULL) {
        result = s_SceneImp->AddInstanceRenderObject(objectFile, maxInstance);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

int32_t Scene::AddInstanceObject(int32_t instanceRenderObject, math::Vector pos, math::Vector scale, math::Vector rotate) {
    int32_t result = -1;

    if (s_SceneImp != NULL) {
        result = s_SceneImp->AddInstanceObject(instanceRenderObject, pos, scale, rotate);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

int32_t Scene::AddDecalObject(const char* decalObjectFile) {
    int32_t result = -1;

    if (s_SceneImp != NULL) {
        result = s_SceneImp->AddDecalObject(decalObjectFile);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

int32_t Scene::AddObject(scene::Model* model) {
    int32_t result = -1;

    if (s_SceneImp != NULL) {
        result = s_SceneImp->AddObject(model);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

int32_t Scene::AddSkyObject(const char* objectFile) {
    int32_t result = -1;

    if (s_SceneImp != NULL) {
        result = s_SceneImp->AddSkyObject(objectFile);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

Object* Scene::GetObjectById(int32_t id) {
    Object* result = NULL;

    if (s_SceneImp != NULL) {
        result = s_SceneImp->GetObjectById(id);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

Object* Scene::GetSkyObject() {
    Object* result = NULL;

    if (s_SceneImp != NULL) {
        result = s_SceneImp->GetSkyObject();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

void Scene::GetAllObjects(std::vector<Object*>& objs) {
    if (s_SceneImp != NULL) {
        s_SceneImp->GetAllObjects(objs);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Scene::DestroyObject(int32_t objectId) {
    if (s_SceneImp != NULL) {
        s_SceneImp->DestroyObject(objectId);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Scene::SetLight(Light lit, int32_t id) {
    if (s_SceneImp != NULL) {
        s_SceneImp->SetLight(lit, id);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

Light Scene::GetLight(int32_t id) {
    Light lit(PARALLEL_LIGHT);

    if (s_SceneImp != NULL) {
        lit = s_SceneImp->GetLight(id);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return lit;
}

void Scene::SetCamera(int32_t type, CameraBase* cam) {
    if (s_SceneImp != NULL) {
        s_SceneImp->SetCamera(type, cam);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

CameraBase* Scene::GetCamera(int32_t type) {
    CameraBase* cam = NULL;

    if (s_SceneImp != NULL) {
        cam = s_SceneImp->GetCamera(type);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return cam;
}

void Scene::SetCurCamera(int32_t type) {
    if (s_SceneImp != NULL) {
        s_SceneImp->SetCurCamera(type);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

int32_t Scene::GetCurCameraType() {
    int32_t result = PRIMIAY_CAM;

    if (s_SceneImp != NULL) {
        result = s_SceneImp->GetCurCameraType();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

CameraBase* Scene::GetCurCamera() {
    CameraBase* cam = NULL;

    if (s_SceneImp != NULL) {
        cam = s_SceneImp->GetCurCamera();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return cam;
}

void Scene::AddBoundBox(math::Vector color) {
    if (s_SceneImp != NULL) {
        s_SceneImp->AddBoundBox(color);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

};  // namespace scene

};  // namespace glb