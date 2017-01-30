//-------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 09 / 04
// Brief: Define some structures that used to hold the glb data
//-------------------------------------------------------
#include "stdafx.h"
#include "glbcomm.h"

#include <assert.h>

#include <map>

namespace glb {

//-----------------------------------------------------------
// Define
//-----------------------------------------------------------
class GlbMaterialMgrImp;
static GlbMaterialMgrImp* s_MaterilMgrImp = NULL;

//-----------------------------------------------------------
// @class
//-----------------------------------------------------------

//-----------------------------------------------------------
// @class: GlbMaterialMgrImp
//-----------------------------------------------------------
class GlbMaterialMgrImp {
public:
    GlbMaterialMgrImp();
    virtual ~GlbMaterialMgrImp();

public:
    void Initialize();
    void Terminate();
    void AddMaterial(GlbMaterial mat);
    void ChangeMaterial(GlbMaterial mat, int32_t id);
    int32_t GetMaterialCount();
    GlbMaterial* GetMaterial(int32_t id);

private:
    std::map<int32_t, GlbMaterial> m_MaterialDataBase;
};


//-----------------------------------------------------------
// @implementation
//-----------------------------------------------------------

//-----------------------------------------------------------
// @implementation: GlbMaterialMgrImp
//-----------------------------------------------------------
GlbMaterialMgrImp::GlbMaterialMgrImp() {
    m_MaterialDataBase.clear();
}

GlbMaterialMgrImp::~GlbMaterialMgrImp() {
    Terminate();
}

void GlbMaterialMgrImp::Initialize() {
}

void GlbMaterialMgrImp::Terminate() {
    m_MaterialDataBase.clear();
}

void GlbMaterialMgrImp::AddMaterial(GlbMaterial mat) {
    m_MaterialDataBase.insert(std::pair<int32_t, GlbMaterial>(mat.id, mat));
}

void GlbMaterialMgrImp::ChangeMaterial(GlbMaterial mat, int32_t id) {
    std::map<int32_t, GlbMaterial>::iterator it = m_MaterialDataBase.find(id);
    if (it != m_MaterialDataBase.end()) {
        it->second.ambient_x = mat.ambient_x;
        it->second.ambient_y = mat.ambient_y;
        it->second.ambient_z = mat.ambient_z;
        it->second.diffuse_x = mat.diffuse_x;
        it->second.diffuse_y = mat.diffuse_y;
        it->second.diffuse_z = mat.diffuse_z;
        it->second.specular_x = mat.specular_x;
        it->second.specular_y = mat.specular_y;
        it->second.specular_z = mat.specular_z;
        it->second.emission_x = mat.emission_x;
        it->second.emission_y = mat.emission_y;
        it->second.emission_z = mat.emission_z;
        it->second.pow = mat.pow;
    }
}

int32_t GlbMaterialMgrImp::GetMaterialCount() {
    return m_MaterialDataBase.size();
}

GlbMaterial* GlbMaterialMgrImp::GetMaterial(int32_t id) {
    GlbMaterial* result = NULL;
    std::map<int32_t, GlbMaterial>::iterator it = m_MaterialDataBase.find(id);
    if (it != m_MaterialDataBase.end()) {
        result = &it->second;
    }

    return result;
}

//-----------------------------------------------------------
// @implementation GlbMaterialMgr
//-----------------------------------------------------------
void GlbMaterialMgr::Initialize() {
    if (s_MaterilMgrImp == NULL) {
        s_MaterilMgrImp = new GlbMaterialMgrImp;
        if (s_MaterilMgrImp != NULL) {
            s_MaterilMgrImp->Initialize();
        }
    }
}

void GlbMaterialMgr::Terminate() {
    if (s_MaterilMgrImp != NULL) {
        s_MaterilMgrImp->Terminate();
        delete s_MaterilMgrImp;
        s_MaterilMgrImp = NULL;
    }
}

void GlbMaterialMgr::AddMaterial(GlbMaterial mat) {
    if (s_MaterilMgrImp != NULL) {
        s_MaterilMgrImp->AddMaterial(mat);
    } else {
        assert(false);
    }
}

void GlbMaterialMgr::ChangeMaterial(GlbMaterial mat, int32_t id) {
    if (s_MaterilMgrImp != NULL) {
        s_MaterilMgrImp->ChangeMaterial(mat, id);
    } else {
        assert(false);
    }
}

int32_t GlbMaterialMgr::GetMaterialCount() {
    int32_t result = 0;

    if (s_MaterilMgrImp != NULL) {
        result = s_MaterilMgrImp->GetMaterialCount();
    } else {
        assert(false);
    }

    return result;
}

GlbMaterial* GlbMaterialMgr::GetMaterial(int32_t id) {
    GlbMaterial* result = NULL;

    if (s_MaterilMgrImp != NULL) {
        result = s_MaterilMgrImp->GetMaterial(id);
    } else {
        assert(false);
    }

    return result;
}
};
