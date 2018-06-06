//-----------------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 03 / 18
// Brief: Implement render target with opengl
//-----------------------------------------------------------------------------------
#ifdef GLB_PLATFORM_OPENGL

#include "glbrendertargetimp.h"

#include <GL\glew.h>
#include <GL\GL.h>

#include "render/glbtexture.h"

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
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// CLASS DEFINITION
//-----------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// RenderTarget::Imp DEFINITION
//--------------------------------------------------------------------------------------
RenderTarget::Imp::Imp()
: m_FBO(0)
, m_DepthRBO(0)
, m_Width(0)
, m_Height(0) {
    memset(m_bDrawColorBuffers, 0, sizeof(m_bDrawColorBuffers));
}

RenderTarget::Imp::~Imp() {
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

RenderTarget::Imp* RenderTarget::Imp::Create(int32_t width, int32_t height) {
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
    GLB_SAFE_ASSERT(status == GL_FRAMEBUFFER_COMPLETE);

    RenderTarget::Imp* target = new RenderTarget::Imp();
    if (target != NULL) {
        target->m_FBO = fbo;
        target->m_DepthRBO = depth_rbo;
        target->m_Width = width;
        target->m_Height = height;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return target;
}

void RenderTarget::Imp::AttachDepthTexture(texture::Texture* depth_tex) {
    if (depth_tex != NULL) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
        glBindTexture(GL_TEXTURE_2D, reinterpret_cast<GLuint>(depth_tex->GetNativeTex()));
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, reinterpret_cast<GLuint>(depth_tex->GetNativeTex()), 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void RenderTarget::Imp::AttachColorTexture(render::DrawColorBuffer index, texture::Texture* color_tex, int32_t level) {
    if (render::COLORBUF_COLOR_ATTACHMENT0 <= index && index <= render::COLORBUF_COLOR_ATTACHMENT7
        && color_tex != NULL) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
        glBindTexture(GL_TEXTURE_2D, reinterpret_cast<GLuint>(color_tex->GetNativeTex()));
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index - render::COLORBUF_COLOR_ATTACHMENT0, GL_TEXTURE_2D, reinterpret_cast<GLuint>(color_tex->GetNativeTex()), level);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void RenderTarget::Imp::AttachCubeTexture(render::DrawColorBuffer* index, texture::Texture* cube_tex, int32_t level) {
    if (cube_tex != NULL) {
        GLuint tex_obj = reinterpret_cast<GLuint>(cube_tex->GetNativeTex());
        if (cube_tex->GetType() == texture::Texture::TEX_CUBE) {
            glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
            glBindTexture(GL_TEXTURE_CUBE_MAP, tex_obj);
            GLint face[6] = {
                GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
                GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
                GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
            };
            for (int32_t i = 0; i < 6; i++) {
                if (render::COLORBUF_COLOR_ATTACHMENT0 <= index[i] && index[i] <= render::COLORBUF_COLOR_ATTACHMENT7) {
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index[i] - render::COLORBUF_COLOR_ATTACHMENT0
                    , face[i], tex_obj, level);
                } else {
                    GLB_SAFE_ASSERT(false);
                }
            }
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void RenderTarget::Imp::Attach3DColorTexture(render::DrawColorBuffer index, texture::Texture* color_tex, int32_t layer, int32_t level) {
    if (render::COLORBUF_COLOR_ATTACHMENT0 <= index && index <= render::COLORBUF_COLOR_ATTACHMENT7
        && color_tex != NULL) {
        GLuint tex_obj = reinterpret_cast<GLuint>(color_tex->GetNativeTex());
        if (color_tex->GetType() == texture::Texture::TEX_3D) {
            glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
            glBindTexture(GL_TEXTURE_3D, tex_obj);
            glFramebufferTexture3D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index - render::COLORBUF_COLOR_ATTACHMENT0, GL_TEXTURE_3D, tex_obj, level, layer);
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }
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
    return reinterpret_cast<void*>(m_FBO);
}

int32_t RenderTarget::Imp::GetWidth() const {
    return m_Width;
}

int32_t RenderTarget::Imp::GetHeight() const {
    return m_Height;
}

};  // namespace render

};  // namespace glb

#endif  // GLB_PLATFORM_OPENGL