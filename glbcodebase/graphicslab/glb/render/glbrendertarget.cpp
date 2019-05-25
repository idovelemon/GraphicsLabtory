//----------------------------------------------------------------
// Declaration: Copyright (c), i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 06 / 27
// Brief: Render target
//----------------------------------------------------------------
#include "glbrendertarget.h"

#include "imp/dx11/glbrendertargetimp.h"
#include "imp/gl/glbrendertargetimp.h"

#include "util/glbmacro.h"

namespace glb {

namespace render {

RenderTarget::RenderTarget()
: m_Imp(nullptr) {
}

RenderTarget::~RenderTarget() {
    GLB_SAFE_DELETE(m_Imp);
}

RenderTarget* RenderTarget::Create(int32_t width, int32_t height) {
    RenderTarget* target = nullptr;
    RenderTarget::Imp* imp = RenderTarget::Imp::Create(width, height);
    if (imp != nullptr) {
        target = new RenderTarget();
        if (target != nullptr) {
            target->m_Imp = imp;
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return target;
}

RenderTarget* RenderTarget::DefaultRenderTarget() {
    return nullptr;
}

void RenderTarget::AttachDepthTextureMS(texture::Texture* depthTex) {
    if (m_Imp != nullptr) {
        m_Imp->AttachDepthTextureMS(depthTex);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void RenderTarget::AttachDepthTexture(texture::Texture* depth_tex) {
    if (m_Imp != nullptr) {
        m_Imp->AttachDepthTexture(depth_tex);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void RenderTarget::AttachColorTextureMS(render::DrawColorBuffer index, texture::Texture* colorTex) {
    if (m_Imp != nullptr) {
        m_Imp->AttachColorTextureMS(index, colorTex);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void RenderTarget::AttachColorTexture(render::DrawColorBuffer index, texture::Texture* color_tex, int32_t level) {
    if (m_Imp != nullptr) {
        m_Imp->AttachColorTexture(index, color_tex, level);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void RenderTarget::AttachCubeTexture(render::DrawColorBuffer* index, texture::Texture* cube_tex, int32_t level) {
    if (m_Imp != nullptr) {
        m_Imp->AttachCubeTexture(index, cube_tex, level);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void RenderTarget::Attach3DColorTexture(render::DrawColorBuffer index, texture::Texture* color_tex_3d, int32_t layer, int32_t level) {
    if (m_Imp != nullptr) {
        m_Imp->Attach3DColorTexture(index, color_tex_3d, layer, level);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void RenderTarget::EnableDrawColorBuffer(render::DrawColorBuffer index) {
    if (m_Imp != nullptr) {
        m_Imp->EnableDrawColorBuffer(index);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void RenderTarget::DisableDrawColorBuffer(render::DrawColorBuffer index) {
    if (m_Imp != nullptr) {
        m_Imp->DisableDrawColorBuffer(index);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void RenderTarget::DisableAllDrawColorBuffers() {
    if (m_Imp != nullptr) {
        m_Imp->DisableAllDrawColorBuffers();
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void* RenderTarget::GetNativeRenderTarget() {
    void* rt = nullptr;

    if (m_Imp != nullptr) {
        rt = m_Imp->GetNativeRenderTarget();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return rt;
}

int32_t RenderTarget::GetWidth() const {
    int32_t result = 0;

    if (m_Imp != nullptr) {
        result = m_Imp->GetWidth();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

int32_t RenderTarget::GetHeight() const {
    int32_t result = 0;

    if (m_Imp != nullptr) {
        result = m_Imp->GetHeight();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

};  // namespace render

};  // namespace glb