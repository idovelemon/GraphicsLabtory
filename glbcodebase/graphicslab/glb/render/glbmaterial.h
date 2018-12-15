//------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 06 / 26
// Brief: Material manager manage all the light materials
//------------------------------------------------------------------------
#ifndef GLB_GLBMATERIAL_H_
#define GLB_GLBMATERIAL_H_

#include <memory.h>
#include <stdint.h>
#include <string.h>

#include <map>
#include <vector>

#include "math/glbmatrix.h"
#include "math/glbvector.h"

#include "glbstructs.h"

namespace glb {

namespace render {

namespace material {

//-----------------------------------------------------------------------------------
// CONSTANT VALUE
//-----------------------------------------------------------------------------------
static const int32_t kMaxTexUnits = 8;
static const int32_t kMaxParameterNameLength = 128;
static const int32_t kMaterialFileNameMaxLength = 128;
static const int32_t kMaxPassNameLength = 64;

//-----------------------------------------------------------------------------------
// TYPE DECLARATION
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// CLASS DECLARATION
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// Material DECLARATION
//----------------------------------------------------------------------------------
class Material {
public:
    enum {
        PARAMETER_TYPE_INTERNAL = 0,  // Internal parameter is setted by GLB itself, user can not set it directly
        PARAMETER_TYPE_USER,          // User parameter is setted by user
    };

    struct ParameterEntry {
        int32_t type;
        int32_t format;
        char name[kMaxParameterNameLength];

        int32_t intValue;
        float   floatValue;
        math::Vector vecValue;
        math::Matrix matValue;

        ParameterEntry()
            : type(PARAMETER_TYPE_USER)
            , format(PARAMETER_FORMAT_INT)
            , intValue(-1)
            , floatValue(0.0f)
            , vecValue(0.0f, 0.0f, 0.0f)
            , matValue()
        {}
    };

public:
    static Material* Create(const char* materialName);

public:
    Material(int32_t shaderID);
    virtual~ Material();

    virtual void SetMaterialID(int32_t id);
    virtual int32_t GetMaterialID() const;

    virtual int32_t GetShaderID() const;

    virtual const char* GetMaterialName() const;

    virtual void SetFloatParameterByName(const char* name, float value);
    virtual float GetFloatParameterByName(const char* name) const;

    virtual void SetIntParameterByName(const char* name, int32_t value);
    virtual int32_t GetIntParameterByName(const char* name) const;

    virtual void SetVectorParameterByName(const char* name, math::Vector value);
    virtual math::Vector GetVectorParameterByName(const char* name) const;

    virtual void SetTextureParameterByName(const char* name, int32_t textureID);
    virtual int32_t GetTextureParameterByName(const char* name) const;

    virtual void SetMatrixParameterByName(const char* name, math::Matrix value);
    virtual math::Matrix GetMatrixParameterByName(const char* name) const;

    virtual ParameterEntry& GetParameterByName(const char* name);
    virtual std::vector<ParameterEntry>& GetAllParameters();

    // Do not call it directly, it used by GLB
    virtual void SetInternalFloatParameterByName(const char* name, float value);
    virtual void SetInternalIntParameterByName(const char* name, int32_t value);
    virtual void SetInternalVectorParameterByName(const char* name, math::Vector value);
    virtual void SetInternalTextureParameterByName(const char* name, int32_t textureID);
    virtual void SetInternalMatrixParameterByName(const char* name, math::Matrix value);

protected:
    void CollectParameter();

protected:
    int32_t                             m_ID;
    int32_t                             m_ShaderID;
    char                                m_MaterialName[kMaterialFileNameMaxLength];
    std::vector<ParameterEntry>         m_Parameters;
};

//-----------------------------------------------------------------------------------
// MaterialGroup DECLARATION
// note: MaterialGroup store all material that will be used in all pass rendering
//----------------------------------------------------------------------------------
class MaterialGroup {
public:
    struct Entry {
        char name[kMaxPassNameLength];
        int32_t materialID;
    };

public:
    static MaterialGroup Create(const char* groupFile);

public:
    MaterialGroup();
    MaterialGroup(const MaterialGroup& group);
    virtual ~MaterialGroup();

    void AddPassMaterial(const char* passName, int32_t materialID);
    int32_t GetPassMaterial(const char* passName);
    std::vector<Entry> GetAllPassMaterial();

protected:
    std::vector<Entry>      m_AllPassMaterial;
};

//-----------------------------------------------------------------------------------
// Mgr DECLARATION
//----------------------------------------------------------------------------------
class Mgr {
 public:
     static void Initialize();
     static void Destroy();

     //-------------------------------------------------------------------
     // @brief: Add the material to manager
     // @param: materialName The material's file name
     // @return: If successfully, return the id of the material.
     // Otherwise, return -1.
     //-------------------------------------------------------------------
     static int32_t AddMaterial(const char* materialName);
     static MaterialGroup AddMaterialGroup(const char* materialGroupName);

     //-------------------------------------------------------------------
     // @brief: Get the material from manager
     // @param: id The id of the material
     // @return: If succesffuly, return the material.
     // Otherwise, return a material whose id is 0.
     //-------------------------------------------------------------------
     static Material* GetMaterial(int32_t id);

     //-------------------------------------------------------------------
     // @brief: Get the total material number
     //-------------------------------------------------------------------
     static int32_t GetMaterialCount();
};

};  // namespace material

};  // namespace render

};  // namespace glb

#endif  // GLB_GLBMATERIAL_H_