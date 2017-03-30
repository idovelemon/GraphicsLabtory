//-----------------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 03 / 18
// Brief: Implement render target with opengl
//-----------------------------------------------------------------------------------
#ifdef GLB_PLATFORM_OPENGL

#ifndef GLB_GLBRENDERTARGET_IMP_H_
#define GLB_GLBRENDERTARGET_IMP_H_

#include "glbrendertarget.h"

namespace glb {

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
    void AttachDepthTexture(texture::Texture* depth_tex);
    void AttachColorTexture(render::DrawColorBuffer index, texture::Texture* color_tex);
    void AttachCubeTexture(render::DrawColorBuffer* index, texture::Texture* cube_tex);

    void EnableDrawColorBuffer(render::DrawColorBuffer index);
    void DisableDrawColorBuffer(render::DrawColorBuffer index);
    void DisableAllDrawColorBuffers();

    void* GetNativeRenderTarget();

private:
    int32_t m_FBO;
    int32_t m_DepthRBO;
    bool m_bDrawColorBuffers[kMaxDrawColorBuffers];
};

};  // namespace glb

#endif  // GLB_GLBRENDERTARGET_IMP_H_

#endif  // GLB_PLATFORM_OPENGL