//-----------------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 03 / 18
// Brief: Implement render target with opengl
//-----------------------------------------------------------------------------------
#ifdef GLB_PLATFORM_OPENGL

#ifndef GLB_GLBRENDERTARGET_IMP_H_
#define GLB_GLBRENDERTARGET_IMP_H_

#include "render/glbrendertarget.h"

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
// RenderTarget::Imp DECLARATION
//----------------------------------------------------------------------------------
class RenderTarget::Imp {
public:
    Imp();
    virtual ~Imp();

    static RenderTarget::Imp* Create(int32_t width, int32_t height);

public:
    void AttachDepthTextureMS(texture::Texture* depthTex);
    void AttachDepthTexture(texture::Texture* depth_tex);
    void AttachColorTextureMS(render::DrawColorBuffer index, texture::Texture* colorTex);
    void AttachColorTexture(render::DrawColorBuffer index, texture::Texture* color_tex, int32_t level);
    void AttachCubeTexture(render::DrawColorBuffer* index, texture::Texture* cube_tex, int32_t level);
    void Attach3DColorTexture(render::DrawColorBuffer index, texture::Texture* color_tex, int32_t layer, int32_t level);

    void EnableDrawColorBuffer(render::DrawColorBuffer index);
    void DisableDrawColorBuffer(render::DrawColorBuffer index);
    void DisableAllDrawColorBuffers();

    void* GetNativeRenderTarget();

    int32_t GetWidth() const;
    int32_t GetHeight() const;

private:
    int32_t m_FBO;
    int32_t m_DepthRBO;
    int32_t m_Width;
    int32_t m_Height;
    bool m_bDrawColorBuffers[kMaxDrawColorBuffers];
};

};  // namespace render

};  // namespace glb

#endif  // GLB_GLBRENDERTARGET_IMP_H_

#endif  // GLB_PLATFORM_OPENGL