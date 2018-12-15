//------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 06 / 26
// Brief: Material manager manage all the light materials
//------------------------------------------------------------------------
#include "glbmaterial.h"

#include <fstream>
#include <map>

#include "glbshader.h"
#include "glbtexture.h"

#include "util/glbmacro.h"

namespace glb {

namespace render {

namespace material {

//-----------------------------------------------------------------------------------
// CONSTANT VALUE
//----------------------------------------------------------------------------------
static const int32_t kShaderFileNameMaxLength = 128;
static const int32_t kMaterialKeywordMaxLength = 64;

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

     int32_t AddMaterial(const char* name);
     MaterialGroup AddMaterialGroup(const char* name);
     Material* GetMaterial(int32_t id);
     int32_t GetMaterialCount();

 protected:
     std::map<int32_t, Material*>   m_MaterialDataBase;
     int32_t                        m_IDGen;
};

//-----------------------------------------------------------------------------------
// CLASS DEFINITION
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// MgrImp DEFINITION
//-----------------------------------------------------------------------------------
Material* Material::Create(const char* materialName) {
    Material* result = nullptr;

    std::ifstream input;
    input.open(materialName);

    if (!input.fail()) {
        result = new Material(-1);

        while (!input.eof()) {
            char buffer[kMaterialKeywordMaxLength];
            input >> buffer;

            // "shader" must be in head line of file
            if (!strcmp(buffer, "shader")) {
                char vertexShaderFile[kShaderFileNameMaxLength];
                char fragmentShaderFile[kShaderFileNameMaxLength];
                input >> vertexShaderFile >> fragmentShaderFile;

                result->m_ShaderID = shader::Mgr::AddUberShader(vertexShaderFile, fragmentShaderFile);
                result->CollectParameter();
            } else if (!strcmp(buffer, "parameter")) {
                // Parameter Name
                input >> buffer;
                ParameterEntry& entry = result->GetParameterByName(buffer);

                if (entry.format == PARAMETER_FORMAT_FLOAT) {
                    input >> entry.floatValue;
                } else if (entry.format == PARAMETER_FORMAT_FLOAT3) {
                    input >> entry.vecValue.x >> entry.vecValue.y >> entry.vecValue.z;
                } else if (entry.format == PARAMETER_FORMAT_FLOAT4) {
                    input >> entry.vecValue.x >> entry.vecValue.y >> entry.vecValue.z >> entry.vecValue.w;
                } else if (entry.format == PARAMETER_FORMAT_INT) {
                    input >> entry.intValue;
                } else if (entry.format == PARAMETER_FORMAT_MATRIX) {
                    for (int32_t i = 0; i < 16; i++) {
                        input >> entry.matValue.m_Matrix.v[i];
                    }
                } else if (entry.format == PARAMETER_FORMAT_TEXTURE_2D) {
                    input >> buffer;
                    entry.intValue = texture::Mgr::LoadTexture(buffer);
                } else if (entry.format == PARAMETER_FORMAT_TEXTURE_3D) {
                    input >> buffer;
                    entry.intValue = texture::Mgr::LoadTexture(buffer);
                } else if (entry.format == PARAMETER_FORMAT_TEXTURE_CUBE) {
                    input >> buffer;
                    entry.intValue = texture::Mgr::LoadTexture(buffer);
                } else {
                    GLB_SAFE_ASSERT(false);
                }
            } else {
                std::string msg = materialName;
                msg += " invalid keyword [";
                msg += buffer;
                msg += "]";
                GLB_SAFE_ASSERT_LOG(false, msg.c_str());
            }
        }
    } else {
        GLB_SAFE_ASSERT_LOG(false, materialName);
    }

    input.close();

    return result;
}

Material::Material(int32_t shaderID)
: m_ID(-1)
, m_ShaderID(shaderID) {
    m_Parameters.clear();
}

Material::~Material() {
}

void Material::SetMaterialID(int32_t id) {
    m_ID = id;
}

int32_t Material::GetMaterialID() const {
    return m_ID;
}

void Material::SetFloatParameterByName(const char* name, float value) {
    for (auto& Entry : m_Parameters) {
        if (!strcmp(Entry.name, name) && Entry.type == PARAMETER_TYPE_USER) {
            Entry.floatValue = value;
            break;
        }
    }
}

float Material::GetFloatParameterByName(const char* name) const {
    float result = 0.0f;

    for (auto& Entry : m_Parameters) {
        if (!strcmp(Entry.name, name)) {
            result = Entry.floatValue;
            break;
        }
    }

    return result;
}

void Material::SetIntParameterByName(const char* name, int32_t value) {
    for (auto& Entry : m_Parameters) {
        if (!strcmp(Entry.name, name) && Entry.type == PARAMETER_TYPE_USER) {
            Entry.intValue = value;
            break;
        }
    }
}

int32_t Material::GetIntParameterByName(const char* name) const {
    int32_t result = 0.0f;

    for (auto& Entry : m_Parameters) {
        if (!strcmp(Entry.name, name)) {
            result = Entry.intValue;
            break;
        }
    }

    return result;
}

void Material::SetVectorParameterByName(const char* name, math::Vector value) {
    for (auto& Entry : m_Parameters) {
        if (!strcmp(Entry.name, name) && Entry.type == PARAMETER_TYPE_USER) {
            Entry.vecValue = value;
            break;
        }
    }
}

math::Vector Material::GetVectorParameterByName(const char* name) const {
    math::Vector result(0.0f, 0.0f, 0.0f);

    for (auto& Entry : m_Parameters) {
        if (!strcmp(Entry.name, name)) {
            result = Entry.vecValue;
            break;
        }
    }

    return result;
}

void Material::SetTextureParameterByName(const char* name, int32_t textureID) {
    for (auto& Entry : m_Parameters) {
        if (!strcmp(Entry.name, name) && Entry.type == PARAMETER_TYPE_USER) {
            Entry.intValue = textureID;
            break;
        }
    }
}

int32_t Material::GetTextureParameterByName(const char* name) const {
    int32_t result = 0.0f;

    for (auto& Entry : m_Parameters) {
        if (!strcmp(Entry.name, name)) {
            result = Entry.intValue;
            break;
        }
    }

    return result;
}

void Material::SetMatrixParameterByName(const char* name, math::Matrix value) {
    for (auto& Entry : m_Parameters) {
        if (!strcmp(Entry.name, name) && Entry.type == PARAMETER_TYPE_USER) {
            Entry.matValue = value;
            break;
        }
    }
}

math::Matrix Material::GetMatrixParameterByName(const char* name) const {
    math::Matrix result = math::Matrix::CreateIdentityMatrix();

    for (auto& Entry : m_Parameters) {
        if (!strcmp(Entry.name, name)) {
            result = Entry.matValue;
            break;
        }
    }

    return result;
}

Material::ParameterEntry& Material::GetParameterByName(const char* name) {
    static ParameterEntry emptyParameter;

    for (auto& Entry : m_Parameters) {
        if (!strcmp(name, Entry.name)) {
            return Entry;
        }
    }

    return emptyParameter;
}

std::vector<Material::ParameterEntry>& Material::GetAllParameters() {
    return m_Parameters;
}

void Material::SetInternalFloatParameterByName(const char* name, float value) {
    for (auto& Entry : m_Parameters) {
        if (!strcmp(Entry.name, name) && Entry.type == PARAMETER_TYPE_INTERNAL) {
            Entry.floatValue = value;
            break;
        }
    }
}

void Material::SetInternalIntParameterByName(const char* name, int32_t value) {
    for (auto& Entry : m_Parameters) {
        if (!strcmp(Entry.name, name) && Entry.type == PARAMETER_TYPE_INTERNAL) {
            Entry.intValue = value;
            break;
        }
    }
}

void Material::SetInternalVectorParameterByName(const char* name, math::Vector value) {
    for (auto& Entry : m_Parameters) {
        if (!strcmp(Entry.name, name) && Entry.type == PARAMETER_TYPE_INTERNAL) {
            Entry.vecValue = value;
            break;
        }
    }
}

void Material::SetInternalTextureParameterByName(const char* name, int32_t textureID) {
    for (auto& Entry : m_Parameters) {
        if (!strcmp(Entry.name, name) && Entry.type == PARAMETER_TYPE_INTERNAL) {
            Entry.intValue = textureID;
            break;
        }
    }
}

void Material::SetInternalMatrixParameterByName(const char* name, math::Matrix value) {
    for (auto& Entry : m_Parameters) {
        if (!strcmp(Entry.name, name) && Entry.type == PARAMETER_TYPE_INTERNAL) {
            Entry.matValue = value;
            break;
        }
    }
}

void Material::CollectParameter() {
    shader::Program* shader = shader::Mgr::GetShader(m_ShaderID);
    if (shader) {
        std::vector<ShaderParameter> parameters = shader->GetProgramParameter();
        for (auto& param : parameters) {
            ParameterEntry entry;
            entry.type = uniform::IsInternalParameter(param.name) ? PARAMETER_TYPE_INTERNAL : PARAMETER_TYPE_USER;
            entry.format = param.format;
            memcpy(entry.name, param.name, strlen(param.name));
            entry.name[strlen(param.name)] = '\0';

            // TEST
            if (!strcmp(entry.name, "glb_unif_AlbedoTex")
                || !strcmp(entry.name, "glb_unif_RoughnessTex")
                || !strcmp(entry.name, "glb_unif_MetallicTex")
                || !strcmp(entry.name, "glb_unif_NormalTex")
                || !strcmp(entry.name, "glb_unif_DiffusePFCTex")
                || !strcmp(entry.name, "glb_unif_SpecularPFCTex")) {
                entry.type = PARAMETER_TYPE_USER;
            }
            m_Parameters.push_back(entry);
        }
    }
}

//-----------------------------------------------------------------------------------
// MaterialGroup DEFINITION
// note: MaterialGroup store all material that will be used in all pass rendering
//----------------------------------------------------------------------------------
MaterialGroup MaterialGroup::Create(const char* groupFile) {
    MaterialGroup result;

    std::ifstream input;
    input.open(groupFile);

    if (!input.fail()) {
        while (!input.eof()) {
            char buffer[kMaterialKeywordMaxLength];
            input >> buffer;

            // pass passname materialname
            if (!strcmp(buffer, "pass")) {
                // Pass Name
                Entry entry;
                input >> entry.name;

                // Material Name
                input >> buffer;
                entry.materialID = Mgr::AddMaterial(buffer);

                result.m_AllPassMaterial.push_back(entry);
            } else {
                std::string msg = groupFile;
                msg += " invalid keyword [";
                msg += buffer;
                msg += "]";
                GLB_SAFE_ASSERT_LOG(false, msg.c_str());
            }
        }
    } else {
        GLB_SAFE_ASSERT_LOG(false, groupFile);
    }

    input.close();

    return result;
}

MaterialGroup::MaterialGroup() {
    m_AllPassMaterial.clear();
}

MaterialGroup::MaterialGroup(const MaterialGroup& group) {
    for (auto& entry : group.m_AllPassMaterial) {
        m_AllPassMaterial.push_back(entry);
    }
}

MaterialGroup::~MaterialGroup() {
}

void MaterialGroup::AddPassMaterial(const char* passName, int32_t materialID) {
    Entry entry;
    memcpy(entry.name, passName, strlen(passName));
    entry.name[strlen(passName)] = '\0';
    entry.materialID = materialID;
    m_AllPassMaterial.push_back(entry);
}

int32_t MaterialGroup::GetPassMaterial(const char* passName) {
    int32_t result = -1;

    for (auto& entry : m_AllPassMaterial) {
        if (!strcmp(entry.name, passName)) {
            result = entry.materialID;
            break;
        }
    }

    return result;
}

std::vector<MaterialGroup::Entry> MaterialGroup::GetAllPassMaterial() {
    return m_AllPassMaterial;
}

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

int32_t MgrImp::AddMaterial(const char* name) {
    Material* material = Material::Create(name);

    if (material) {
        m_IDGen++;
        material->SetMaterialID(m_IDGen);
        m_MaterialDataBase.insert(std::make_pair(m_IDGen, material));
    }

    return m_IDGen;
}

MaterialGroup MgrImp::AddMaterialGroup(const char* name) {
    return MaterialGroup::Create(name);
}

Material* MgrImp::GetMaterial(int32_t id) {
    Material* result = nullptr;

    std::map<int32_t, Material*>::iterator it = m_MaterialDataBase.find(id);
    if (it != m_MaterialDataBase.end()) {
        result = it->second;
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
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

int32_t Mgr::AddMaterial(const char* materialName) {
    int32_t result = -1;

    if (s_MgrImp != nullptr) {
        result = s_MgrImp->AddMaterial(materialName);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

MaterialGroup Mgr::AddMaterialGroup(const char* materialGroupName) {
    MaterialGroup result;

    if (s_MgrImp != nullptr) {
        result = s_MgrImp->AddMaterialGroup(materialGroupName);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

Material* Mgr::GetMaterial(int32_t id) {
    Material* result = nullptr;

    if (s_MgrImp != NULL) {
        result = s_MgrImp->GetMaterial(id);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

int32_t Mgr::GetMaterialCount() {
    int32_t result = 0;

    if (s_MgrImp != NULL) {
        result = s_MgrImp->GetMaterialCount();
    }

    return result;
}

};  // namespace material

};  // namespace render

};  // namespace glb
