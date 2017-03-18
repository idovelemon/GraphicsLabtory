//----------------------------------------------------------------
// Declaration: Copyright (c), i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 06 / 27
// Brief: Render target
//----------------------------------------------------------------
#ifndef GLB_GLBRENDERTARGET_H_
#define GLB_GLBRENDERTARGET_H_

#include <stdint.h>

#include <vector>

#include "glbrenderdevice.h"

namespace glb {

static const int32_t kMaxDrawColorBuffers = 8;

namespace texture {
class Texture;
};

class RenderTarget {
public:
    RenderTarget();
    virtual ~RenderTarget();

    static RenderTarget* Create(int32_t width, int32_t height);

public:
    void AttachDepthTexture(texture::Texture* depth_tex);
    void AttachColorTexture(render::DrawColorBuffer index, texture::Texture* color_tex);

    void EnableDrawColorBuffer(render::DrawColorBuffer index);
    void DisableDrawColorBuffer(render::DrawColorBuffer index);
    void DisableAllDrawColorBuffers();

    void* GetNativeRenderTarget();

private:
    class Imp;
    Imp*    m_Imp;
};

};  // namespace glb

#endif  // GLB_GLBRENDERTARGET_H_