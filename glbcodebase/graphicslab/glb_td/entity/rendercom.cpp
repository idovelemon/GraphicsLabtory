//----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/05/26
// Brief: Render controls how to render a entity
//----------------------------------------------------------
#include "rendercom.h"

#include "scene/glbscene.h"
#include "scene/glbobject.h"

#include "transformcom.h"

namespace entity {

RenderCom::InstanceMap RenderCom::s_InstanceMap;

RenderCom::RenderCom(Entity* owner, const char* name, glb::math::Vector pos, glb::math::Vector rot, glb::math::Vector scale, bool enableInstance, int32_t maxInstanceNum)
: Component(CT_RENDER, owner)
, m_SceneObjID(-1) {
    if (enableInstance) {
        InstanceMap::iterator it = s_InstanceMap.find(name);
        if (it != s_InstanceMap.end()) {
            m_SceneObjID = glb::scene::Scene::AddInstanceObject(it->second);
        } else {
            int32_t instanceRenderID = glb::scene::Scene::AddInstanceRenderObject(name, maxInstanceNum);
            m_SceneObjID = glb::scene::Scene::AddInstanceObject(instanceRenderID);
            s_InstanceMap.insert(std::pair<std::string, int32_t>(name, instanceRenderID));

            glb::scene::Scene::GetObjectById(instanceRenderID)->SetCullFaceEnable(true);
            glb::scene::Scene::GetObjectById(instanceRenderID)->SetCullFaceMode(glb::render::CULL_BACK);
            glb::scene::Scene::GetObjectById(instanceRenderID)->SetDepthTestEnable(true);
        }
    } else {
        m_SceneObjID = glb::scene::Scene::AddObject(name);
    }

    glb::scene::Object* obj = glb::scene::Scene::GetObjectById(m_SceneObjID);
    if (obj != NULL) {
        obj->SetPos(pos);
        obj->SetRotation(rot);
        obj->SetScale(scale);
        obj->Update();

        obj->SetCullFaceEnable(true);
        obj->SetCullFaceMode(glb::render::CULL_BACK);
        obj->SetDepthTestEnable(true);
    }
}

RenderCom::~RenderCom() {
    glb::scene::Scene::DestroyObject(m_SceneObjID);
    m_SceneObjID = -1;
}

void RenderCom::SetDepthTestEnable(bool enable) {
    glb::scene::Object* obj = glb::scene::Scene::GetObjectById(m_SceneObjID);
    if (obj != NULL) {
        obj->SetDepthTestEnable(enable);
    }
}

void RenderCom::SetDrawEnable(bool enable) {
    glb::scene::Object* obj = glb::scene::Scene::GetObjectById(m_SceneObjID);
    if (obj != NULL) {
        obj->SetDrawEnable(enable);
    }
}

glb::math::Vector RenderCom::GetBoundBoxMax() {
    glb::math::Vector result(0.0f, 0.0f, 0.0f);
    glb::scene::Object* obj = glb::scene::Scene::GetObjectById(m_SceneObjID);
    if (obj != NULL) {
        result = obj->GetBoundBoxMax();
    }

    return result;
}

glb::math::Vector RenderCom::GetBoundBoxMin() {
    glb::math::Vector result(0.0f, 0.0f, 0.0f);
    glb::scene::Object* obj = glb::scene::Scene::GetObjectById(m_SceneObjID);
    if (obj != NULL) {
        result = obj->GetBoundBoxMin();
    }

    return result;
}

void RenderCom::Render(TransformCom* transform) {
    if (transform != NULL) {
        glb::scene::Object* obj = glb::scene::Scene::GetObjectById(m_SceneObjID);
        if (obj != NULL) {
            glb::math::Vector pos = transform->GetPosWorld();
            glb::math::Vector rotate = transform->GetRotateWorld();
            glb::math::Vector scale = transform->GetScale();
            obj->SetPos(pos);
            obj->SetRotation(rotate);
            obj->SetScale(scale);
        }
    }
}

};  // namespace entity