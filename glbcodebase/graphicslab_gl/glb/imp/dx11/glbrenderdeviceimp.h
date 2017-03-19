//-----------------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 03 / 18
// Brief: Implement renderdevice with dx11
//-----------------------------------------------------------------------------------
#ifdef GLB_PLATFORM_DX11

#ifndef GLB_GLBRENDERDEVICE_IMP_H_
#define GLB_GLBRENDERDEVICE_IMP_H_

#include "glbrenderdevice.h"

class ID3D11Device;
class ID3D11DeviceContext;
class IDXGISwapChain;
class ID3D11RenderTargetView;
class ID3D11Texture2D;
class ID3D11DepthStencilView;

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
    void SetUniform3f(int32_t location, const Vector& v);
    void SetUniformMatrix(int32_t location, Matrix& m);
    void SetUniformSampler(int32_t location, int32_t slot);

    // Render State
    void SetDepthTestEnable(bool enable);

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
    void SetClearColor(float r, float g, float b);
    void SetClearDepth(float depth);
    void Clear(int32_t clear_flag);
    void Draw(PrimitiveType type, int32_t first, int32_t size);
    void SwapBuffer();

protected:
    void SetupRenderState();

    void SetupCullFace();
    void SetupAlphaBlend();
    void SetupVertexLayout();
    void SetupTexture();

private:
    // D3D device
    ID3D11Device*           m_D3DDevice;
    ID3D11DeviceContext*    m_D3DContext;
    IDXGISwapChain*         m_SwapChain;
    ID3D11RenderTargetView* m_RenderTargetView;
    ID3D11Texture2D*        m_DepthStencilTex;
    ID3D11DepthStencilView* m_DepthStencilView;

    // Texture
    TextureUnit             m_Texture[TS_MAX];

    // Vertex Buffer
    VertexLayout            m_VertexLayout;

    // Shader
    ShaderLayout            m_ShaderLayout;

    // Render State
    bool                    m_EnableDepthTest;
    bool                    m_EnableAlphaBlend;
    bool                    m_EnableCullFace;
    CullMode                m_CullMode;
    AlphaBlendFunc          m_SrcBlendFunc;
    AlphaBlendFunc          m_DstBlendFunc;

    // Draw
    float                   m_ClearR;
    float                   m_ClearG;
    float                   m_ClearB;
    float                   m_ClearDepth;
};

};  // namespace render

};  // namespace glb

#endif  // GLB_GLBRENDERDEVICE_IMP_H_

#endif  // GLB_PLATFORM_DX11