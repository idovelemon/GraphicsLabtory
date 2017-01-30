//-----------------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 06 / 14
// Brief: Define a object
//-----------------------------------------------------------------------------------
#ifndef GLB_GLBOBJECT_H_
#define GLB_GLBOBJECT_H_

#include <stdint.h>

#include <vector>

#include "glbmatrix.h"
#include "glbmodel.h"
#include "glbrenderdevice.h"
#include "glbshader.h"
#include "glbvector.h"

namespace glb {

class TriangleMesh;

class Object {
public:
    Object();
    virtual~ Object();

    //--------------------------------------------------------------------------------
    // @brief: Create the object according the model file
    // @param: file_name The supported model file's name
    // @param: pos The position of the object
    // @param: scale The scale of the object
    // @param: rotation The rotation of the object
    // @return: If failed, return NULL
    //--------------------------------------------------------------------------------
    static Object* Create(const char* file_name, Vector pos = Vector(0.0f, 0.0f, 0.0f),
        Vector scale = Vector(1.0f, 1.0f, 1.0f), Vector rotation = Vector(0.0f, 0.0f, 0.0f));

public:
    void SetPos(Vector pos);
    Vector GetPos() const;

    void SetScale(Vector scale);
    Vector GetScale() const;

    void SetRotation(Vector rotation);
    Vector GetRotation() const;

    Vector GetBoundBoxMax();
    Vector GetBoundBoxMin();

    Matrix GetWorldMatrix() const;

    Model* GetModel();
    shader::Descriptor GetShaderDesc();
    int32_t GetShaderProgramID();

    void SetDepthTestEnable(bool enable);
    bool IsDepthTestEnable();

    void SetAlphaBlendEnable(bool enable);
    bool IsAlphaBlendEnable();
    void SetAlphaBlendFunc(render::AlphaBlendFactor factor, render::AlphaBlendFunc func);
    render::AlphaBlendFunc GetAlphaBlendFunc(render::AlphaBlendFactor factor);

    void SetCullFaceEnable(bool enable);
    bool IsCullFaceEnable();
    void SetCullFaceMode(render::CullMode mode);
    render::CullMode GetCullFaceMode();

    virtual void Update();

private:
    shader::Descriptor CalculateShaderDesc();

private:
    Model* m_Model;
    Vector m_Pos;
    Vector m_Scale;
    Vector m_Rotation;
    Matrix m_WorldMatrix;
    shader::Descriptor m_ShaderDesc;
    int32_t m_ShaderProgramID;

    bool   m_EnableCullFace;
    bool   m_EnableDepthTest;
    bool   m_EnableAlphaBlend;
    render::CullMode   m_CullMode;
    render::AlphaBlendFunc m_SrcBlendFunc;
    render::AlphaBlendFunc m_DstBlendFunc;
};

};  // namespace glb

#endif  GLB_GLBOBJECT_H_