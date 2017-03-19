//-----------------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 03 / 19
// Brief: Implement render target with dx11
//-----------------------------------------------------------------------------------
#ifdef GLB_PLATFORM_DX11

#include "glbrendertargetimp.h"

#include "glbtexture.h"

namespace glb {

//-----------------------------------------------------------------------------------
// CONSTANT VALUE
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// TYPE DECLARATION
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// CLASS DECLARATION
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// CLASS DEFINITION
//-----------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// RenderTarget::Imp DEFINITION
//--------------------------------------------------------------------------------------
RenderTarget::Imp::Imp() {
    memset(m_bDrawColorBuffers, 0, sizeof(m_bDrawColorBuffers));
}

RenderTarget::Imp::~Imp() {
}

RenderTarget::Imp* RenderTarget::Imp::Create(int32_t width, int32_t height) {
    RenderTarget::Imp* target = new RenderTarget::Imp();
    return target;
}

void RenderTarget::Imp::AttachDepthTexture(texture::Texture* depth_tex) {
}

void RenderTarget::Imp::AttachColorTexture(render::DrawColorBuffer index, texture::Texture* color_tex) {
}

void RenderTarget::Imp::EnableDrawColorBuffer(render::DrawColorBuffer index) {
    if (render::COLORBUF_COLOR_ATTACHMENT0 <= index && index <= render::COLORBUF_COLOR_ATTACHMENT7) {
        m_bDrawColorBuffers[index - render::COLORBUF_COLOR_ATTACHMENT0] = true;
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void RenderTarget::Imp::DisableDrawColorBuffer(render::DrawColorBuffer index) {
    if (render::COLORBUF_COLOR_ATTACHMENT0 <= index && index <= render::COLORBUF_COLOR_ATTACHMENT7) {
        m_bDrawColorBuffers[index - render::COLORBUF_COLOR_ATTACHMENT0] = false;
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void RenderTarget::Imp::DisableAllDrawColorBuffers() {
    for (int32_t i = 0; i < kMaxDrawColorBuffers; i++) {
        m_bDrawColorBuffers[i] = false;
    }
}

void* RenderTarget::Imp::GetNativeRenderTarget() {
    return NULL;
}

};  // namespace glb

#endif  // GLB_PLATFORM_DX11