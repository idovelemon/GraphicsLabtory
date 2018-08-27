//-----------------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 03 / 18
// Brief: Implement renderdevice with opengl
//-----------------------------------------------------------------------------------
#ifdef GLB_PLATFORM_OPENGL

#ifndef GLB_GLBRENDERDEVICE_IMP_H_
#define GLB_GLBRENDERDEVICE_IMP_H_


#include "render/glbrenderdevice.h"

#include <GL\glew.h>
#include <GL\GL.h>

namespace glb {

namespace render {

//-----------------------------------------------------------------------------------
// CONSTANT VALUE
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// TYPE DECLARATION
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// CLASS DECLARATION
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// DeviceImp DECLARATION
//---------------------------------------------------------------------------------
struct TextureUnit {
    int32_t tex_obj;
    int32_t tex_unit;
};

//-----------------------------------------------------------------------------------
// CLASS DECLARATION
//----------------------------------------------------------------------------------
class DeviceImp {
public:
    DeviceImp();
    virtual ~DeviceImp();

public:
    void Initialize();
    void Destroy();

    // Vertex Buffer
    void SetVertexBuffer(mesh::VertexBuffer* buf);
    void SetVertexLayout(VertexLayout vlayout);

    // Texture
    void SetTexture(int32_t slot, texture::Texture* tex, int32_t tex_unit);
    void ClearTexture();

    // Shader
    void SetShader(shader::Program* program);
    void SetShaderLayout(ShaderLayout layout);
    void SetUniform1i(int32_t location, int32_t v);
    void SetUniform1f(int32_t location, float v);
    void SetUniform3f(int32_t location, const math::Vector& v);
    void SetUniform4f(int32_t location, const math::Vector& v);
    void SetUniformMatrix(int32_t location, math::Matrix& m);
    void SetUniformSampler2D(int32_t location, int32_t slot);
    void SetUniformSampler3D(int32_t location, int32_t slot);
    void SetUniformSamplerCube(int32_t location, int32_t slot);

    // Render State
    void SetDepthTestEnable(bool enable);
    void SetDepthWriteEnable(bool enable);

    void SetAlphaBlendEnable(bool enable);
    void SetAlphaBlendFunc(AlphaBlendFactor factor, AlphaBlendFunc func);

    void SetCullFaceEnable(bool enable);
    void SetCullFaceMode(CullMode mode);

    // Render Target
    void SetRenderTarget(RenderTarget* rt);

    // Viewport
    void SetViewport(int32_t x, int32_t y, int32_t width, int32_t height);

    // Draw
    void SetDrawColorBuffer(DrawColorBuffer buffer);
    void SetDrawMultiColorBuffer(DrawColorBuffer* buffers, int32_t num);
    void SetClearColor(float r, float g, float b, float a);
    void SetClearDepth(float depth);
    void Clear(int32_t clear_flag);
    void Draw(PrimitiveType type, int32_t first, int32_t size);
    void DrawInstance(PrimitiveType type, int32_t first, int32_t size, int32_t instance);
    void SetupVSync(bool enable);
    void SwapBuffer();

protected:
    void SetupRenderState();
    GLenum GetPrimitiveTypeGL(PrimitiveType type);
    GLenum GetAlphaBlendFuncGL(AlphaBlendFunc func);
    GLenum GetDrawColorBufferGL(DrawColorBuffer buffer);

    void SetupCullFace();
    void SetupAlphaBlend();
    void SetupVertexLayout();
    void SetupInstanceLayout();
    void SetupTexture();

private:
    // OS-specific
    HDC                 m_WindowDC;
    HGLRC               m_OpenGLContext;

    // Vertex Buffer
    int32_t             m_VertexArrayObject;
    int32_t             m_VertexBufferObject;
    int32_t             m_InstanceBufferObject;
    VertexLayout        m_VertexLayout;

    // Texture
    TextureUnit         m_Texture[TS_MAX];

    // Shader
    ShaderLayout        m_ShaderLayout;

    // Render State
    bool                m_EnableDepthTest;
    bool                m_EnableDepthWrite;
    bool                m_EnableAlphaBlend;
    bool                m_EnableCullFace;
    CullMode            m_CullMode;
    AlphaBlendFunc      m_SrcBlendFunc;
    AlphaBlendFunc      m_DstBlendFunc;

    // Draw
    float               m_ClearR;
    float               m_ClearG;
    float               m_ClearB;
    float               m_ClearA;
    float               m_ClearDepth;
};

};  // namespace render

};  // namespace glb

#endif  // GLB_GLBRENDERDEVICE_IMP_H_

#endif  // GLB_PLATFORM_OPENGL