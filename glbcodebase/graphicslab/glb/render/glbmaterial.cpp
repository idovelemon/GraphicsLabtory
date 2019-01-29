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
#include "util/glbutil.h"

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
    virtual ~MgrImp();

public:
    void Initialize();
    void Destroy();

    int32_t AddMaterial(const char* name);
    int32_t AddMaterial(Material* material);
    Material* GetMaterial(int32_t id);
    int32_t GetMaterialCount();

protected:
    int32_t GetMaterialByName(const char* name);

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
PassMaterial::PassMaterial()
: m_ShaderID(-1) {
    memset(m_PassName, 0, sizeof(m_PassName));
    m_Parameters.clear();
}

PassMaterial::~PassMaterial() {
}

void PassMaterial::SetShaderID(int32_t shaderID) {
    m_ShaderID = shaderID;
}

int32_t PassMaterial::GetShaderID() const {
    return m_ShaderID;
}

void PassMaterial::SetPassName(const char* name) {
    if (name) {
        size_t len = strlen(name);
        len = min(len, kMaxPassNameLength - 1);
        memcpy(m_PassName, name, len);
        m_PassName[len] = '\0';
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

const char* PassMaterial::GetPassName() const {
    return static_cast<const char*>(m_PassName);
}

void PassMaterial::SetFloatParameterByName(const char* name, float value) {
    for (auto& Entry : m_Parameters) {
        if (!strcmp(Entry.name, name) && Entry.type == PARAMETER_TYPE_USER) {
            Entry.floatValue = value;
            break;
        }
    }
}

float PassMaterial::GetFloatParameterByName(const char* name) const {
    float result = 0.0f;

    for (auto& Entry : m_Parameters) {
        if (!strcmp(Entry.name, name)) {
            result = Entry.floatValue;
            break;
        }
    }

    return result;
}

void PassMaterial::SetIntParameterByName(const char* name, int32_t value) {
    for (auto& Entry : m_Parameters) {
        if (!strcmp(Entry.name, name) && Entry.type == PARAMETER_TYPE_USER) {
            Entry.intValue = value;
            break;
        }
    }
}

int32_t PassMaterial::GetIntParameterByName(const char* name) const {
    int32_t result = 0;

    for (auto& Entry : m_Parameters) {
        if (!strcmp(Entry.name, name)) {
            result = Entry.intValue;
            break;
        }
    }

    return result;
}

void PassMaterial::SetVectorParameterByName(const char* name, math::Vector value) {
    for (auto& Entry : m_Parameters) {
        if (!strcmp(Entry.name, name) && Entry.type == PARAMETER_TYPE_USER) {
            Entry.vecValue = value;
            break;
        }
    }
}

math::Vector PassMaterial::GetVectorParameterByName(const char* name) const {
    math::Vector result(0.0f, 0.0f, 0.0f);

    for (auto& Entry : m_Parameters) {
        if (!strcmp(Entry.name, name)) {
            result = Entry.vecValue;
            break;
        }
    }

    return result;
}

void PassMaterial::SetTextureParameterByName(const char* name, int32_t textureID) {
    for (auto& Entry : m_Parameters) {
        if (!strcmp(Entry.name, name) && Entry.type == PARAMETER_TYPE_USER) {
            Entry.intValue = textureID;
            break;
        }
    }
}

int32_t PassMaterial::GetTextureParameterByName(const char* name) const {
    int32_t result = 0;

    for (auto& Entry : m_Parameters) {
        if (!strcmp(Entry.name, name)) {
            result = Entry.intValue;
            break;
        }
    }

    return result;
}

void PassMaterial::SetMatrixParameterByName(const char* name, math::Matrix value) {
    for (auto& Entry : m_Parameters) {
        if (!strcmp(Entry.name, name) && Entry.type == PARAMETER_TYPE_USER) {
            Entry.matValue = value;
            break;
        }
    }
}

math::Matrix PassMaterial::GetMatrixParameterByName(const char* name) const {
    math::Matrix result = math::Matrix::CreateIdentityMatrix();

    for (auto& Entry : m_Parameters) {
        if (!strcmp(Entry.name, name)) {
            result = Entry.matValue;
            break;
        }
    }

    return result;
}

PassMaterial::ParameterEntry& PassMaterial::GetParameterByName(const char* name) {
    static ParameterEntry emptyParameter;

    for (auto& Entry : m_Parameters) {
        if (!strcmp(name, Entry.name)) {
            return Entry;
        }
    }

    return emptyParameter;
}

std::vector<PassMaterial::ParameterEntry>& PassMaterial::GetAllParameters() {
    return m_Parameters;
}

void PassMaterial::SetInternalFloatParameterByName(const char* name, float value) {
    for (auto& Entry : m_Parameters) {
        if (!strcmp(Entry.name, name) && Entry.type == PARAMETER_TYPE_INTERNAL) {
            Entry.floatValue = value;
            break;
        }
    }
}

void PassMaterial::SetInternalIntParameterByName(const char* name, int32_t value) {
    for (auto& Entry : m_Parameters) {
        if (!strcmp(Entry.name, name) && Entry.type == PARAMETER_TYPE_INTERNAL) {
            Entry.intValue = value;
            break;
        }
    }
}

void PassMaterial::SetInternalVectorParameterByName(const char* name, math::Vector value) {
    for (auto& Entry : m_Parameters) {
        if (!strcmp(Entry.name, name) && Entry.type == PARAMETER_TYPE_INTERNAL) {
            Entry.vecValue = value;
            break;
        }
    }
}

void PassMaterial::SetInternalTextureParameterByName(const char* name, int32_t textureID) {
    for (auto& Entry : m_Parameters) {
        if (!strcmp(Entry.name, name) && Entry.type == PARAMETER_TYPE_INTERNAL) {
            Entry.intValue = textureID;
            break;
        }
    }
}

void PassMaterial::SetInternalMatrixParameterByName(const char* name, math::Matrix value) {
    for (auto& Entry : m_Parameters) {
        if (!strcmp(Entry.name, name) && Entry.type == PARAMETER_TYPE_INTERNAL) {
            Entry.matValue = value;
            break;
        }
    }
}

void PassMaterial::CollectParameter() {
    shader::Program* shader = shader::Mgr::GetShader(m_ShaderID);
    if (shader) {
        // Clear
        m_Parameters.clear();

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
                || !strcmp(entry.name, "glb_unif_EmissionTex")
                || !strcmp(entry.name, "glb_unif_DiffusePFCTex")
                || !strcmp(entry.name, "glb_unif_SpecularPFCTex")
                || !strcmp(entry.name, "glb_unif_SpecularPFCLOD")) {
                entry.type = PARAMETER_TYPE_USER;
            }
            m_Parameters.push_back(entry);
        }
    }
}

//-----------------------------------------------------------------------------------
// Material DEFINITION
//----------------------------------------------------------------------------------
Material* Material::Create(const char* materialFile) {
    Material* result = new Material;

    std::ifstream input;
    input.open(materialFile);

    if (!input.fail()) {
        std::string materialDir = util::path_get_dir(materialFile);

        PassMaterial* newPassMaterial = nullptr;

        while (!input.eof()) {
            char buffer[kMaterialKeywordMaxLength];
            input >> buffer;

            // beginpass passname
            if (!strcmp(buffer, "beginpass")) {
                newPassMaterial = new PassMaterial;

                // Pass Name
                input >> newPassMaterial->m_PassName;

            } else if (!strcmp(buffer, "endpass")) {
                // Save pass material
                result->m_AllPassMaterial.push_back(newPassMaterial);
                newPassMaterial = nullptr;
            } else if (!strcmp(buffer, "shader")) {
                char vertexShaderFile[kShaderFileNameMaxLength];
                char fragmentShaderFile[kShaderFileNameMaxLength];
                input >> vertexShaderFile >> fragmentShaderFile;

                std::string vertexShaderPath = materialDir.empty() ? vertexShaderFile : (materialDir + vertexShaderFile);  // Assuming they are in same directory
                std::string fragmentShaderPath = materialDir.empty() ? fragmentShaderFile : (materialDir + fragmentShaderFile);  // Assuming they are in same directory

                newPassMaterial->m_ShaderID = shader::Mgr::AddUberShader(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
                newPassMaterial->CollectParameter();
            } else if (!strcmp(buffer, "passparameter")) {
                // Parameter Name
                input >> buffer;
                PassMaterial::ParameterEntry& entry = newPassMaterial->GetParameterByName(buffer);

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
                    std::string destTexturePath = materialDir.empty() ? buffer : (materialDir + std::string(buffer));
                    entry.intValue = texture::Mgr::LoadTexture(destTexturePath.c_str());  // Assuming they are in same directory
                } else if (entry.format == PARAMETER_FORMAT_TEXTURE_3D) {
                    input >> buffer;
                    std::string destTexturePath = materialDir.empty() ? buffer : (materialDir + std::string(buffer));
                    entry.intValue = texture::Mgr::LoadTexture(destTexturePath.c_str());  // Assuming they are in same directory
                } else if (entry.format == PARAMETER_FORMAT_TEXTURE_CUBE) {
                    input >> buffer;

                    std::string destTexturePath = materialDir.empty() ? buffer : (materialDir + std::string(buffer));
                    if (!strcmp(util::path_get_file_type(buffer).c_str(), ".pfc")) {
                        entry.intValue = texture::Mgr::LoadPFCTexture(destTexturePath.c_str());  // Assuming they are in same directory
                    } else {
                        entry.intValue = texture::Mgr::LoadTexture(destTexturePath.c_str());  // Assuming they are in same directory
                    }
                } else {
                    GLB_SAFE_ASSERT(false);
                }
            } else if (!strcmp(buffer, "materialparameter")) {
                // Parameter Name
                input >> buffer;
                if (!strcmp(buffer, "castshadow")) {
                    int32_t enable = 0;
                    input >> enable;
                    if (enable == 0) {
                        result->m_EnableCastShadow = false;
                    } else {
                        result->m_EnableCastShadow = true;
                    }
                } else if (!strcmp(buffer, "receiveshadow")) {
                    int32_t enable = 0;
                    input >> enable;
                    if (enable == 0) {
                        result->m_EnableReceiveShadow = false;
                    } else {
                        result->m_EnableReceiveShadow = true;
                    }
                }
            } else {
                std::string msg = materialFile;
                msg += " invalid keyword [";
                msg += buffer;
                msg += "]";
                GLB_SAFE_ASSERT_LOG(false, msg.c_str());
            }
        }
    } else {
        GLB_SAFE_ASSERT_LOG(false, materialFile);
    }

    input.close();

    return result;
}

Material::Material()
: m_ID(-1)
, m_EnableCastShadow(false)
, m_EnableReceiveShadow(false) {
    memset(m_MaterialName, 0, sizeof(m_MaterialName));
    m_AllPassMaterial.clear();
}

Material::~Material() {
    for (auto passMaterial : m_AllPassMaterial) {
        GLB_SAFE_DELETE(passMaterial);
    }

    m_AllPassMaterial.clear();
}

void Material::SetMaterialID(int32_t id) {
    m_ID = id;
}

int32_t Material::GetMaterialID() const {
    return m_ID;
}

void Material::SetMaterialName(const char* name) {
    if (name) {
        size_t len = strlen(name);
        len = min(len, kMaterialFileNameMaxLength - 1);
        memcpy(m_MaterialName, name, len);
        m_MaterialName[len] = '\0';
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

const char* Material::GetMaterialName() const {
    return m_MaterialName;
}

void Material::AddPassMaterial(PassMaterial* passMaterial) {
    if (passMaterial) {
        m_AllPassMaterial.push_back(passMaterial);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

PassMaterial* Material::GetPassMaterial(const char* passName) {
    PassMaterial* result = nullptr;

    for (auto& material : m_AllPassMaterial) {
        if (!strcmp(material->m_PassName, passName)) {
            result = material;
            break;
        }
    }

    return result;
}

std::vector<PassMaterial*>& Material::GetAllPassMaterial() {
    return m_AllPassMaterial;
}

bool Material::IsCastShadowEnable() const {
    return m_EnableCastShadow;
}

bool Material::IsReceiveShadowEnable() const {
    return m_EnableReceiveShadow;
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
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return m_IDGen;
}

int32_t MgrImp::AddMaterial(Material* material) {
    if (material) {
        m_IDGen++;
        material->SetMaterialID(m_IDGen);
        m_MaterialDataBase.insert(std::make_pair(m_IDGen, material));
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return m_IDGen;
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

int32_t MgrImp::GetMaterialByName(const char* name) {
    int32_t result = -1;

    for (auto& material : m_MaterialDataBase) {
        if (material.second) {
            if (!strcmp(name, material.second->GetMaterialName())) {
                result = material.first;
                break;
            }
        }
    }

    return result;
}

//-----------------------------------------------------------------------------------
// Mgr DEFINITION
//-----------------------------------------------------------------------------------
void Mgr::Initialize() {
    if (s_MgrImp == nullptr) {
        s_MgrImp = new MgrImp();
        GLB_SAFE_ASSERT(s_MgrImp != nullptr);
        s_MgrImp->Initialize();
    } else {
        GLB_SAFE_ASSERT(false);
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

int32_t Mgr::AddMaterial(const char* materialName) {
    int32_t result = -1;

    if (s_MgrImp != nullptr) {
        result = s_MgrImp->AddMaterial(materialName);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

int32_t Mgr::AddMaterial(Material* material) {
    int32_t result = -1;

    if (s_MgrImp != nullptr) {
        result = s_MgrImp->AddMaterial(material);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

Material* Mgr::GetMaterial(int32_t id) {
    Material* result = nullptr;

    if (s_MgrImp != nullptr) {
        result = s_MgrImp->GetMaterial(id);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

int32_t Mgr::GetMaterialCount() {
    int32_t result = 0;

    if (s_MgrImp != nullptr) {
        result = s_MgrImp->GetMaterialCount();
    }

    return result;
}

};  // namespace material

};  // namespace render

};  // namespace glb
