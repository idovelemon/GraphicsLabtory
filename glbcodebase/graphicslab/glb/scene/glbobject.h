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

#include "glbmodel.h"

#include "math/glbmatrix.h"
#include "math/glbvector.h"

#include "render/glbrenderdevice.h"
#include "render/glbshader.h"

namespace glb {

class TriangleMesh;

namespace scene {

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
    static Object* Create(const char* file_name, math::Vector pos = math::Vector(0.0f, 0.0f, 0.0f),
        math::Vector scale = math::Vector(1.0f, 1.0f, 1.0f), math::Vector rotation = math::Vector(0.0f, 0.0f, 0.0f));
    static Object* Create(scene::Model* model, math::Vector pos = math::Vector(0.0f, 0.0f, 0.0f),
        math::Vector scale = math::Vector(1.0f, 1.0f, 1.0f), math::Vector rotation = math::Vector(0.0f, 0.0f, 0.0f));

public:
    void SetDead(bool dead);
    bool IsDead() const;

    void SetPos(math::Vector pos);
    math::Vector GetPos() const;

    void SetScale(math::Vector scale);
    math::Vector GetScale() const;

    void SetRotation(math::Vector rotation);
    math::Vector GetRotation() const;

    math::Vector GetBoundBoxMax();
    math::Vector GetBoundBoxMin();

    math::Matrix GetWorldMatrix() const;

    Model* GetModel();
    render::shader::Descriptor GetShaderDesc();

    void SetDrawEnable(bool enable);
    bool IsDrawEnable();

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

    void SetTexWithId(int32_t slot, int32_t tex_id);
    int32_t GetTexId(int32_t slot);

    virtual void Update();

private:
    render::shader::Descriptor CalculateShaderDesc();

private:
    bool                            m_IsDead;
    Model*                          m_Model;
    math::Vector                    m_Pos;
    math::Vector                    m_Scale;
    math::Vector                    m_Rotation;
    math::Matrix                    m_WorldMatrix;
    render::shader::Descriptor      m_ShaderDesc;

    bool                            m_EnableDraw;
    bool                            m_EnableCullFace;
    bool                            m_EnableDepthTest;
    bool                            m_EnableAlphaBlend;
    render::CullMode                m_CullMode;
    render::AlphaBlendFunc          m_SrcBlendFunc;
    render::AlphaBlendFunc          m_DstBlendFunc;
};

};  // namespace scene

};  // namespace glb

#endif  GLB_GLBOBJECT_H_