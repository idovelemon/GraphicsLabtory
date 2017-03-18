//--------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 10 / 26
// Brief: Render device is used to render primitives
//--------------------------------------------------------------------
#include "glbrenderdevice.h"

#include "imp/gl/glbrenderdeviceimp.h"

namespace glb {

namespace render {

//-----------------------------------------------------------------------------------
// CONSTANT VALUE
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// TYPE DECLARATION
//-----------------------------------------------------------------------------------
static DeviceImp* s_DeviceImp = NULL;

//-----------------------------------------------------------------------------------
// Device DEFINITION
//-----------------------------------------------------------------------------------
void Device::Initialize() {
    if (s_DeviceImp == NULL) {
        s_DeviceImp = new DeviceImp;
        if (s_DeviceImp != NULL) {
            s_DeviceImp->Initialize();
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Device::Destroy() {
    if (s_DeviceImp != NULL) {
        s_DeviceImp->Destroy();
        GLB_SAFE_DELETE(s_DeviceImp);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Device::SetVertexBuffer(mesh::VertexBuffer* buf) {
    if (s_DeviceImp != NULL) {
        s_DeviceImp->SetVertexBuffer(buf);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Device::SetVertexLayout(VertexLayout layout) {
    if (s_DeviceImp != NULL) {
        s_DeviceImp->SetVertexLayout(layout);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Device::SetTexture(int32_t slot, texture::Texture* tex, int32_t tex_unit) {
    if (s_DeviceImp != NULL) {
        s_DeviceImp->SetTexture(slot, tex, tex_unit);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Device::ClearTexture() {
    if (s_DeviceImp != NULL) {
        s_DeviceImp->ClearTexture();
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Device::SetShader(shader::Program* program) {
    if (s_DeviceImp != NULL) {
        s_DeviceImp->SetShader(program);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Device::SetShaderLayout(ShaderLayout layout) {
    if (s_DeviceImp != NULL) {
        s_DeviceImp->SetShaderLayout(layout);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Device::SetUniform1i(int32_t location, int32_t v) {
    if (s_DeviceImp != NULL) {
        s_DeviceImp->SetUniform1i(location, v);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Device::SetUniform1f(int32_t location, float v) {
    if (s_DeviceImp != NULL) {
        s_DeviceImp->SetUniform1f(location, v);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Device::SetUniform3f(int32_t location, const Vector& v) {
    if (s_DeviceImp != NULL) {
        s_DeviceImp->SetUniform3f(location, v);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Device::SetUniformMatrix(int32_t location, const Matrix& m) {
    if (s_DeviceImp != NULL) {
        s_DeviceImp->SetUniformMatrix(location, const_cast<Matrix&>(m));
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Device::SetUniformSampler(int32_t location, int32_t sampler) {
    if (s_DeviceImp != NULL) {
        s_DeviceImp->SetUniformSampler(location, sampler);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Device::SetDepthTestEnable(bool enable) {
    if (s_DeviceImp != NULL) {
        s_DeviceImp->SetDepthTestEnable(enable);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Device::SetAlphaBlendEnable(bool enable) {
    if (s_DeviceImp != NULL) {
        s_DeviceImp->SetAlphaBlendEnable(enable);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Device::SetAlphaBlendFunc(AlphaBlendFactor factor, AlphaBlendFunc func) {
    if (s_DeviceImp != NULL) {
        s_DeviceImp->SetAlphaBlendFunc(factor, func);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Device::SetCullFaceEnable(bool enable) {
    if (s_DeviceImp != NULL) {
        s_DeviceImp->SetCullFaceEnable(enable);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Device::SetCullFaceMode(CullMode mode) {
    if (s_DeviceImp != NULL) {
        s_DeviceImp->SetCullFaceMode(mode);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Device::SetRenderTarget(RenderTarget* rt) {
    if (s_DeviceImp != NULL) {
        s_DeviceImp->SetRenderTarget(rt);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Device::SetViewport(int32_t x, int32_t y, int32_t width, int32_t height) {
    if (s_DeviceImp != NULL) {
        s_DeviceImp->SetViewport(x, y, width, height);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Device::SetDrawColorBuffer(DrawColorBuffer buffer) {
    if (s_DeviceImp != NULL) {
        s_DeviceImp->SetDrawColorBuffer(buffer);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Device::SetClearColor(float r, float g, float b) {
    if (s_DeviceImp != NULL) {
        s_DeviceImp->SetClearColor(r, g, b);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Device::SetClearDepth(float depth) {
    if (s_DeviceImp != NULL) {
        s_DeviceImp->SetClearDepth(depth);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Device::Clear(int32_t flag) {
    if (s_DeviceImp != NULL) {
        s_DeviceImp->Clear(flag);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Device::Draw(PrimitiveType type, int32_t first, int32_t count) {
    if (s_DeviceImp != NULL) {
        s_DeviceImp->Draw(type, first, count);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Device::SwapBuffer() {
    if (s_DeviceImp != NULL) {
        s_DeviceImp->SwapBuffer();
    } else {
        GLB_SAFE_ASSERT(false);
    }
}
};  // namespace render

};  // namespace glb
