//-----------------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 03 / 18
// Brief: Implement renderdevice with dx11
//-----------------------------------------------------------------------------------
#ifdef GLB_PLATFORM_DX11

#include "glbrenderdeviceimp.h"

#include <D3Dx11.h>

#include "glbapplication.h"

namespace glb {

namespace render {

//-----------------------------------------------------------------------------------
// CLASS DEFINITION
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// DeviceImp DEFINITION
//-----------------------------------------------------------------------------------
DeviceImp::DeviceImp()
: m_D3DDevice(NULL)
, m_D3DContext(NULL)
, m_SwapChain(NULL)
, m_RenderTargetView(NULL)
, m_DepthStencilTex(NULL)
, m_DepthStencilView(NULL)
, m_VertexLayout()
, m_ShaderLayout()
, m_EnableDepthTest(false)
, m_EnableAlphaBlend(false)
, m_EnableCullFace(false)
, m_CullMode(CULL_NONE)
, m_SrcBlendFunc(FUNC_ZERO)
, m_DstBlendFunc(FUNC_ZERO)
, m_ClearR(0.0f)
, m_ClearG(0.0f)
, m_ClearB(0.0f)
, m_ClearDepth(0.0f) {
    memset(m_Texture, 0, sizeof(m_Texture));
}

DeviceImp::~DeviceImp() {
    Destroy();
}

void DeviceImp::Initialize() {
    // Create d3d11 device
    UINT create_device_flag = 0;
#ifdef _DEBUG
    create_device_flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL feature_level;
    HRESULT hr = D3D11CreateDevice(
        NULL,  // Default adapter
        D3D_DRIVER_TYPE_HARDWARE,  // Hardware accelerate
        0,  // No software device
        create_device_flag,
        NULL, 0,  // Default feature level array
        D3D11_SDK_VERSION,  // DX11
        &m_D3DDevice,
        &feature_level,
        &m_D3DContext
        );
    GLB_SAFE_ASSERT(!FAILED(hr));
    GLB_SAFE_ASSERT(feature_level == D3D_FEATURE_LEVEL_11_0);

    // Create swap chain
    IDXGIDevice* dxgi_device = NULL;
    hr = m_D3DDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgi_device);
    GLB_SAFE_ASSERT(!FAILED(hr));
    IDXGIAdapter* dxgi_adapter = NULL;
    hr = dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgi_adapter);
    GLB_SAFE_ASSERT(!FAILED(hr));
    IDXGIFactory* dxgi_factory = NULL;
    hr = dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgi_factory);
    GLB_SAFE_ASSERT(!FAILED(hr));

    DXGI_SWAP_CHAIN_DESC sd;
    sd.BufferDesc.Width = app::Application::GetWindowWidth();
    sd.BufferDesc.Height = app::Application::GetWindowHeight();
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 1;
    sd.OutputWindow = app::Application::GetWindowHandle();
    sd.Windowed = true;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = 0;
    hr = dxgi_factory->CreateSwapChain(m_D3DDevice, &sd, &m_SwapChain);
    GLB_SAFE_ASSERT(!FAILED(hr));
    dxgi_device->Release();
    dxgi_device = NULL;
    dxgi_adapter->Release();
    dxgi_adapter = NULL;
    dxgi_factory->Release();
    dxgi_factory = NULL;

    // Create render target view
    ID3D11Texture2D* back_buf_tex = NULL;
    hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buf_tex));
    GLB_SAFE_ASSERT(!FAILED(hr));
    hr = m_D3DDevice->CreateRenderTargetView(back_buf_tex, NULL, &m_RenderTargetView);
    GLB_SAFE_ASSERT(!FAILED(hr));
    back_buf_tex->Release();
    back_buf_tex = NULL;

    // Create depth/stencil view
    D3D11_TEXTURE2D_DESC desc;
    desc.Width = app::Application::GetWindowWidth();
    desc.Height = app::Application::GetWindowHeight();
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    hr = m_D3DDevice->CreateTexture2D(&desc, NULL, &m_DepthStencilTex);
    GLB_SAFE_ASSERT(!FAILED(hr));
    hr = m_D3DDevice->CreateDepthStencilView(m_DepthStencilTex, NULL, &m_DepthStencilView);
    GLB_SAFE_ASSERT(!FAILED(hr));

    // Bind render target and depth/stencil view to Output Merge stage at default
    m_D3DContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);
}

void DeviceImp::Destroy() {
    GLB_SAFE_RELEASE(m_DepthStencilView);
    GLB_SAFE_RELEASE(m_DepthStencilTex);
    GLB_SAFE_RELEASE(m_RenderTargetView);
    GLB_SAFE_RELEASE(m_SwapChain);
    GLB_SAFE_RELEASE(m_D3DContext);
    GLB_SAFE_RELEASE(m_D3DDevice);
}

// Vertex Buffer
void DeviceImp::SetVertexBuffer(mesh::VertexBuffer* buf) {
}

void DeviceImp::SetVertexLayout(VertexLayout layout) {
    m_VertexLayout = layout;
}

// Texture
void DeviceImp::SetTexture(int32_t slot, texture::Texture* tex, int32_t tex_unit) {
}

void DeviceImp::ClearTexture() {
}

// Shader
void DeviceImp::SetShader(shader::Program* program) {
}

void DeviceImp::SetShaderLayout(ShaderLayout layout) {
    m_ShaderLayout = layout;
}

void DeviceImp::SetUniform1i(int32_t location, int32_t v) {
}

void DeviceImp::SetUniform1f(int32_t location, float v) {
}

void DeviceImp::SetUniform3f(int32_t location, const Vector& v) {
}

void DeviceImp::SetUniformMatrix(int32_t location, Matrix& m) {
}

void DeviceImp::SetUniformSampler(int32_t location, int32_t slot) {
}

// Render State
void DeviceImp::SetDepthTestEnable(bool enable) {
    m_EnableDepthTest = enable;
}

void DeviceImp::SetAlphaBlendEnable(bool enable) {
    m_EnableAlphaBlend = enable;
}

void DeviceImp::SetAlphaBlendFunc(AlphaBlendFactor factor, AlphaBlendFunc func) {
    switch (factor) {
    case FACTOR_SRC:
        m_SrcBlendFunc = func;
        break;

    case FACTOR_DST:
        m_DstBlendFunc = func;
        break;

    default:
        GLB_SAFE_ASSERT(false);
        break;
    }
}

void DeviceImp::SetCullFaceEnable(bool enable) {
    m_EnableCullFace = enable;
}

void DeviceImp::SetCullFaceMode(CullMode mode) {
    m_CullMode = mode;
}

void DeviceImp::SetRenderTarget(RenderTarget* rt) {
}

void DeviceImp::SetViewport(int32_t x, int32_t y, int32_t width, int32_t height) {
}

// Draw command
void DeviceImp::SetDrawColorBuffer(DrawColorBuffer buffer) {
}

void DeviceImp::SetClearColor(float r, float g, float b) {
    m_ClearR = r;
    m_ClearG = g;
    m_ClearB = b;
}

void DeviceImp::SetClearDepth(float depth) {
    m_ClearDepth = depth;
}

void DeviceImp::Clear(int32_t flag) {
    uint32_t clear_flag_gl = 0;

    if (flag & CLEAR_COLOR) {
        float rgba[4] = {m_ClearR, m_ClearG, m_ClearB, 1.0f};
        if (m_D3DContext) {
            m_D3DContext->ClearRenderTargetView(m_RenderTargetView, rgba);
        }
    }

    if (flag & CLEAR_DEPTH) {
    }
}

void DeviceImp::Draw(PrimitiveType type, int32_t first, int32_t size) {
}

void DeviceImp::SwapBuffer() {
    if (m_SwapChain) {
        m_SwapChain->Present(0, 0);
    }
}

// Internel Helper
void DeviceImp::SetupRenderState() {
    // Depth
    if (m_EnableDepthTest) {
    } else {
    }

    // Alpha
    if (m_EnableAlphaBlend) {
        SetupAlphaBlend();
    } else {
    }

    // Cull
    if (m_EnableCullFace) {
        SetupCullFace();
    } else {
    }

    // Vertex layout
    SetupVertexLayout();

    // Texture
    SetupTexture();
}

void DeviceImp::SetupCullFace() {
    switch (m_CullMode) {
    case CULL_FRONT:
        break;

    case CULL_BACK:
        break;

    case CULL_NONE:
        break;

    default:
        GLB_SAFE_ASSERT(false);
        break;
    }
}

void DeviceImp::SetupAlphaBlend() {
}

void DeviceImp::SetupVertexLayout() {
    for (int32_t i = 0; i < m_VertexLayout.count; i++) {
        switch (m_VertexLayout.layouts[i].attriType) {
        case VA_POS:
            {
                int32_t location = -1;
                for (int32_t j = 0; j < m_ShaderLayout.count; j++) {
                    if (m_ShaderLayout.layouts[j].attriType == VA_POS) {
                        location = m_ShaderLayout.layouts[j].location;
                        break;
                    }
                }

                // Must have position
                if (location != -1) {
                } else {
                    GLB_SAFE_ASSERT(false);
                }
            }
            break;

        case VA_COLOR:
            {
                int32_t location = -1;
                for (int32_t j = 0; j < m_ShaderLayout.count; j++) {
                    if (m_ShaderLayout.layouts[j].attriType == VA_COLOR) {
                        location = m_ShaderLayout.layouts[j].location;
                        break;
                    }
                }

                // Must have position
                if (location != -1) {
                } else {
                    GLB_SAFE_ASSERT(false);
                }
            }
            break;

        case VA_TEXCOORD:
            {
                int32_t location = -1;
                for (int32_t j = 0; j < m_ShaderLayout.count; j++) {
                    if (m_ShaderLayout.layouts[j].attriType == VA_TEXCOORD) {
                        location = m_ShaderLayout.layouts[j].location;
                        break;
                    }
                }

                // Texture Coordinate is optional
                if (location != -1) {
                }
            }
            break;

        case VA_NORMAL:
            {
                int32_t location = -1;
                for (int32_t j = 0; j < m_ShaderLayout.count; j++) {
                    if (m_ShaderLayout.layouts[j].attriType == VA_NORMAL) {
                        location = m_ShaderLayout.layouts[j].location;
                        break;
                    }
                }

                // Normal is optional
                if (location != -1) {
                }
            }
            break;

        case VA_TANGENT:
            {
                int32_t location = -1;
                for (int32_t j = 0; j < m_ShaderLayout.count; j++) {
                    if (m_ShaderLayout.layouts[j].attriType == VA_TANGENT) {
                        location = m_ShaderLayout.layouts[j].location;
                        break;
                    }
                }

                // Tangent is optional
                if (location != -1) {
                }
            }
            break;

        case VA_BINORMAL:
            {
                int32_t location = -1;
                for (int32_t j = 0; j < m_ShaderLayout.count; j++) {
                    if (m_ShaderLayout.layouts[j].attriType == VA_BINORMAL) {
                        location = m_ShaderLayout.layouts[j].location;
                        break;
                    }
                }

                // Binormal is optional
                if (location != -1) {
                }
            }
            break;

        case VA_UNKNOWN:
        default:
            GLB_SAFE_ASSERT(false);
            break;
        }
    }
}

void DeviceImp::SetupTexture() {
}

};  // namespace render

};  // namespace glb

#endif  // GLB_PLATFORM_DX11