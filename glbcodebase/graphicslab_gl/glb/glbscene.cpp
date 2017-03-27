//-------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 09 / 26
// Brief: Scene hold all the object that need to be draw
//-------------------------------------------------------
#include "glbscene.h"

#include <vector>

#include "glbcamera.h"
#include "glbmacro.h"
#include "glbobject.h"
#include "glbrender.h"

namespace glb {

namespace scene {

//-----------------------------------------------------------------------------------
// TYPE DECLARATION
//-----------------------------------------------------------------------------------
class SceneImp;
static SceneImp* s_SceneImp = NULL;

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
    Vector GetSceneBoundBoxMax();
    Vector GetSceneBoundBoxMin();

    // Object
    int32_t AddObject(const char* object_file);
    int32_t AddSkyObject(const char* object_file);
    Object* GetObjectById(int32_t id);
    Object* GetSkyObject();
    void GetAllObjects(std::vector<Object*>& objs);

    // Light
    void SetLight(light::Light lit, int32_t id);
    light::Light GetLight(int32_t id);

    // Camera
    void SetCamera(int32_t type, camera::CameraBase* cam);
    camera::CameraBase* GetCamera(int32_t type);
    void SetCurCamera(int32_t type);
    int32_t GetCurCameraType();
    camera::CameraBase* GetCurCamera();

    // Debug Draw
    void AddBoundBox(Vector color);

private:
    int32_t              m_CurCameraType;
    std::vector<Object*> m_ObjectDataBase;
    light::Light         m_Lights[kMaxLight];
    camera::CameraBase*  m_Camera[MAX_CAM];
    Object*              m_SkyObject;
    Vector               m_BoundBoxMax;
    Vector               m_BoundBoxMin;
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

    m_BoundBoxMax = Vector(0.0f, 0.0f, 0.0f);
    m_BoundBoxMin = Vector(FLT_MAX, FLT_MAX, FLT_MAX);
    for (int32_t i = 0; i < static_cast<int32_t>(m_ObjectDataBase.size()); i++) {
        if (m_ObjectDataBase[i] != NULL) {
            m_ObjectDataBase[i]->Update();
            Vector max = m_ObjectDataBase[i]->GetBoundBoxMax();
            Vector min = m_ObjectDataBase[i]->GetBoundBoxMin();
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

Vector SceneImp::GetSceneBoundBoxMax() {
    return m_BoundBoxMax;
}

Vector SceneImp::GetSceneBoundBoxMin() {
    return m_BoundBoxMin;
}

int32_t SceneImp::AddObject(const char* object_file) {
    int32_t id = -1;

    if (object_file != NULL) {
        Object* obj = Object::Create(object_file);
        if (obj != NULL) {
            id = m_ObjectDataBase.size();
            m_ObjectDataBase.push_back(obj);
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
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
        objs.push_back(m_ObjectDataBase[i]);
    }
}

void SceneImp::SetLight(light::Light lit, int32_t id) {
    if (0 <= id && id < kMaxLight) {
        m_Lights[id] = lit;
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

light::Light SceneImp::GetLight(int32_t id) {
    light::Light lit(light::UNKNOWN_LIGHT);

    if (0 <= id && id < kMaxLight) {
        lit = m_Lights[id];
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return lit;
}

void SceneImp::SetCamera(int32_t type, camera::CameraBase* cam) {
    if (0 <= type && type < MAX_CAM) {
        m_Camera[type] = cam;
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

camera::CameraBase* SceneImp::GetCamera(int32_t type) {
    camera::CameraBase* cam = NULL;

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

camera::CameraBase* SceneImp::GetCurCamera() {
    return m_Camera[m_CurCameraType];
}

void SceneImp::AddBoundBox(Vector color) {
        Vector view_space_points[8];
        view_space_points[render::Render::NLU] = Vector(m_BoundBoxMin.x, m_BoundBoxMax.y, m_BoundBoxMax.z);
        view_space_points[render::Render::NLD] = Vector(m_BoundBoxMin.x, m_BoundBoxMin.y, m_BoundBoxMax.z);
        view_space_points[render::Render::NRU] = Vector(m_BoundBoxMax.x, m_BoundBoxMax.y, m_BoundBoxMax.z);
        view_space_points[render::Render::NRD] = Vector(m_BoundBoxMax.x, m_BoundBoxMin.y, m_BoundBoxMax.z);
        view_space_points[render::Render::FLU] = Vector(m_BoundBoxMin.x, m_BoundBoxMax.y, m_BoundBoxMin.z);
        view_space_points[render::Render::FLD] = Vector(m_BoundBoxMin.x, m_BoundBoxMin.y, m_BoundBoxMin.z);
        view_space_points[render::Render::FRU] = Vector(m_BoundBoxMax.x, m_BoundBoxMax.y, m_BoundBoxMin.z);
        view_space_points[render::Render::FRD] = Vector(m_BoundBoxMax.x, m_BoundBoxMin.y, m_BoundBoxMin.z);

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

Vector Scene::GetSceneBoundBoxMax() {
    Vector result(0.0f, 0.0f, 0.0f);

    if (s_SceneImp != NULL) {
        result = s_SceneImp->GetSceneBoundBoxMax();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

Vector Scene::GetSceneBoundBoxMin() {
    Vector result(0.0f, 0.0f, 0.0f);

    if (s_SceneImp != NULL) {
        result = s_SceneImp->GetSceneBoundBoxMin();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

int32_t Scene::AddObject(const char* object_file) {
    int32_t result = -1;

    if (s_SceneImp != NULL) {
        result = s_SceneImp->AddObject(object_file);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

int32_t Scene::AddSkyObject(const char* object_file) {
    int32_t result = -1;

    if (s_SceneImp != NULL) {
        result = s_SceneImp->AddSkyObject(object_file);
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

void Scene::SetLight(light::Light lit, int32_t id) {
    if (s_SceneImp != NULL) {
        s_SceneImp->SetLight(lit, id);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

light::Light Scene::GetLight(int32_t id) {
    light::Light lit(light::PARALLEL_LIGHT);

    if (s_SceneImp != NULL) {
        lit = s_SceneImp->GetLight(id);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return lit;
}

void Scene::SetCamera(int32_t type, camera::CameraBase* cam) {
    if (s_SceneImp != NULL) {
        s_SceneImp->SetCamera(type, cam);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

camera::CameraBase* Scene::GetCamera(int32_t type) {
    camera::CameraBase* cam = NULL;

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

camera::CameraBase* Scene::GetCurCamera() {
    camera::CameraBase* cam = NULL;

    if (s_SceneImp != NULL) {
        cam = s_SceneImp->GetCurCamera();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return cam;
}

void Scene::AddBoundBox(Vector color) {
    if (s_SceneImp != NULL) {
        s_SceneImp->AddBoundBox(color);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

};  // namespace scene

};  // namespace glb