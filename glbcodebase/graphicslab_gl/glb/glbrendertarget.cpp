//----------------------------------------------------------------
// Declaration: Copyright (c), i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 06 / 27
// Brief: Render target
//----------------------------------------------------------------
#include "glbrendertarget.h"

#include <stdio.h>

#include <GL\glew.h>
#include <GL\GL.h>

#include "glbmacro.h"
#include "glbtexture.h"

namespace glb {

static const int32_t kMinColorBufferIndex = 0;
static const int32_t kMaxColorBufferIndex = 8;

RenderTarget::RenderTarget()
: m_FBO(0)
, m_DepthRBO(0) {
    memset(m_bDrawColorBuffers, 0, sizeof(m_bDrawColorBuffers));
}

RenderTarget::~RenderTarget() {
    if (m_DepthRBO != 0) {
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glDeleteRenderbuffers(1, reinterpret_cast<const GLuint*>(&m_DepthRBO));
        m_DepthRBO = 0;
    }

    if (m_FBO != 0) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteFramebuffers(1, reinterpret_cast<const GLuint*>(&m_FBO));
        m_FBO = 0;
    }
}

RenderTarget* RenderTarget::Create(int32_t width, int32_t height) {
    // Create frame buffer object
    int32_t fbo = 0;
    glGenFramebuffers(1, reinterpret_cast<GLuint*>(&fbo));
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    
    //------------------------------------------------------------------------
    // Must attach a depth buffer. If do not, the image won't have the effect of depth
    //------------------------------------------------------------------------

    // Create depth render buffer object
    int32_t depth_rbo = 0;
    glGenRenderbuffers(1, reinterpret_cast<GLuint*>(&depth_rbo));
    glBindRenderbuffer(GL_RENDERBUFFER, depth_rbo);

    // Allocate memory for depth render buffer object
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

    // Attach to frame buffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_rbo);

    // Check the status
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    char err_msg[64];
    sprintf(err_msg, "FBO not ready Status:0x%X", status);
    GLB_SAFE_ASSERT(status == GL_FRAMEBUFFER_COMPLETE);

    RenderTarget* target = new RenderTarget();
    if (target != NULL) {
        target->m_FBO = fbo;
        target->m_DepthRBO = depth_rbo;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return target;
}

void RenderTarget::AttachDepthTexture(texture::Texture* depth_tex) {
    if (depth_tex != NULL) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
        glBindTexture(GL_TEXTURE_2D, depth_tex->GetTexObj());
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_tex->GetTexObj(), 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void RenderTarget::AttachColorTexture(render::DrawColorBuffer index, texture::Texture* color_tex) {
    if (render::COLORBUF_COLOR_ATTACHMENT0 <= index && index <= render::COLORBUF_COLOR_ATTACHMENT7
        && color_tex != NULL) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
        glBindTexture(GL_TEXTURE_2D, color_tex->GetTexObj());
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index - render::COLORBUF_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_tex->GetTexObj(), 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void RenderTarget::EnableDrawColorBuffer(render::DrawColorBuffer index) {
    if (render::COLORBUF_COLOR_ATTACHMENT0 <= index && index <= render::COLORBUF_COLOR_ATTACHMENT7) {
        m_bDrawColorBuffers[index - render::COLORBUF_COLOR_ATTACHMENT0] = true;
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void RenderTarget::DisableDrawColorBuffer(render::DrawColorBuffer index) {
    if (render::COLORBUF_COLOR_ATTACHMENT0 <= index && index <= render::COLORBUF_COLOR_ATTACHMENT7) {
        m_bDrawColorBuffers[index - render::COLORBUF_COLOR_ATTACHMENT0] = false;
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void RenderTarget::DisableAllDrawColorBuffers() {
    for (int32_t i = 0; i < kMaxDrawColorBuffers; i++) {
        m_bDrawColorBuffers[i] = false;
    }
}

int32_t RenderTarget::GetRenderTargetObj() {
    return m_FBO;
}

};  // namespace glb