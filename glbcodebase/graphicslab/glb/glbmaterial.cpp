//------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 06 / 26
// Brief: Material manager manage all the light materials
//------------------------------------------------------------------------
#include "glbmaterial.h"

#include <map>

#include "glbmacro.h"

namespace glb {

namespace material {

//-----------------------------------------------------------------------------------
// CONSTANT VALUE
//----------------------------------------------------------------------------------

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
     virtual ~MgrImp();

 public:
     void Initialize();
     void Destroy();

     int32_t AddMaterial(Material mat);
     void ChangeMaterial(Material mat, int32_t id);
     const Material& GetMaterial(int32_t id);
     int32_t GetMaterialCount();

 protected:
     std::map<int32_t, Material> m_MaterialDataBase;
     int32_t                     m_IDGen;
};

//-----------------------------------------------------------------------------------
// CLASS DEFINITION
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// MgrImp DEFINITION
//-----------------------------------------------------------------------------------
MgrImp::MgrImp()
: m_IDGen(0) {
    m_MaterialDataBase.clear();
}

MgrImp::~MgrImp() {
}

void MgrImp::Initialize() {
}

void MgrImp::Destroy() {
    m_MaterialDataBase.clear();
    m_IDGen = 0;
}

int32_t MgrImp::AddMaterial(Material mat) {
    m_IDGen++;
    mat.id = m_IDGen;
    m_MaterialDataBase.insert(std::make_pair(mat.id, mat));

    return mat.id;
}

void MgrImp::ChangeMaterial(Material mat, int32_t id) {
    std::map<int32_t, Material>::iterator it = m_MaterialDataBase.find(id);
    if (it != m_MaterialDataBase.end()) {
        it->second.ambient = mat.ambient;
        it->second.diffuse = mat.diffuse;
        it->second.specular = mat.specular;
        it->second.emission = mat.emission;
        it->second.specular_pow = mat.specular_pow;
    }
}

const Material& MgrImp::GetMaterial(int32_t id) {
    static Material null_material;

    std::map<int32_t, Material>::iterator it = m_MaterialDataBase.find(id);
    if (it != m_MaterialDataBase.end()) {
        return it->second;
    } else {
        return null_material;
    }
}

int32_t MgrImp::GetMaterialCount() {
    return m_MaterialDataBase.size();
}

//-----------------------------------------------------------------------------------
// Mgr DEFINITION
//-----------------------------------------------------------------------------------
void Mgr::Initialize() {
    if (s_MgrImp == NULL) {
        s_MgrImp = new MgrImp();
        GLB_SAFE_ASSERT(s_MgrImp != NULL);
        s_MgrImp->Initialize();
    } else {
        GLB_SAFE_ASSERT(false);
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

int32_t Mgr::AddMaterial(Material mat) {
    int32_t result = 0;

    if (s_MgrImp != NULL) {
        result = s_MgrImp->AddMaterial(mat);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

void Mgr::ChangeMaterial(Material mat, int32_t id) {
    if (s_MgrImp != NULL) {
        s_MgrImp->ChangeMaterial(mat, id);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

const Material& Mgr::GetMaterial(int32_t id) {
    static Material null_material;

    if (s_MgrImp != NULL) {
        return s_MgrImp->GetMaterial(id);
    } else {
        return null_material;
    }
}

int32_t Mgr::GetMaterialCount() {
    int32_t result = 0;

    if (s_MgrImp != NULL) {
        result = s_MgrImp->GetMaterialCount();
    }

    return result;
}

};  // namespace material

};  // namespace glb
