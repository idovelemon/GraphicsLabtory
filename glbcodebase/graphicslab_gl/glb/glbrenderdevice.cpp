//--------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 10 / 26
// Brief: Render device is used to render primitives
//--------------------------------------------------------------------
#include "glbrenderdevice.h"

#include <stdint.h>

#include <GL\glew.h>
#include <glut.h>
#include <GL\GL.h>

namespace glb {

namespace render {

//-----------------------------------------------------------------------------------
// CONSTANT VALUE
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// TYPE DECLARATION
//-----------------------------------------------------------------------------------
class DeviceImp;
static DeviceImp* s_DeviceImp = NULL;

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
    void SetVertexArray(int32_t va);
    void SetVertexBuffer(int32_t vb);
    void SetVertexLayout(VertexLayout vlayout);

    // Texture
    void SetTexture(int32_t slot, int32_t tex_obj, int32_t tex_unit);
    void ClearTexture();

    // Shader
    void SetShader(int32_t shader);
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
    void SetRenderTarget(int32_t rt);

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
    GLenum GetPrimitiveTypeGL(PrimitiveType type);
    GLenum GetAlphaBlendFuncGL(AlphaBlendFunc func);
    GLenum GetDrawColorBufferGL(DrawColorBuffer buffer);

    void SetupCullFace();
    void SetupAlphaBlend();
    void SetupVertexLayout();
    void SetupTexture();

private:
    // Vertex Buffer
    int32_t             m_VertexArrayObject;
    int32_t             m_VertexBufferObject;
    VertexLayout        m_VertexLayout;

    // Texture
    TextureUnit         m_Texture[TS_MAX];

    // Shader
    ShaderLayout        m_ShaderLayout;

    // Render State
    bool                m_EnableDepthTest;
    bool                m_EnableAlphaBlend;
    bool                m_EnableCullFace;
    CullMode            m_CullMode;
    AlphaBlendFunc      m_SrcBlendFunc;
    AlphaBlendFunc      m_DstBlendFunc;

    // Draw
    float               m_ClearR;
    float               m_ClearG;
    float               m_ClearB;
    float               m_ClearDepth;
};

//-----------------------------------------------------------------------------------
// CLASS DEFINITION
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// DeviceImp DEFINITION
//-----------------------------------------------------------------------------------
DeviceImp::DeviceImp()
: m_VertexArrayObject(-1)
, m_VertexBufferObject(-1)
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
}

void DeviceImp::Destroy() {
}


// Vertex Buffer
void DeviceImp::SetVertexArray(int32_t va) {
    m_VertexArrayObject = va;
}

void DeviceImp::SetVertexBuffer(int32_t vb) {
    m_VertexBufferObject = vb;
}

void DeviceImp::SetVertexLayout(VertexLayout layout) {
    m_VertexLayout = layout;
}

// Texture
void DeviceImp::SetTexture(int32_t slot, int32_t tex_obj, int32_t tex_unit) {
    if (0 <= slot && slot < TS_MAX) {
        m_Texture[slot].tex_obj = tex_obj;
        m_Texture[slot].tex_unit = tex_unit;
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void DeviceImp::ClearTexture() {
    for (int32_t i = 0; i < TS_MAX; i++) {
        m_Texture[i].tex_obj = -1;
        m_Texture[i].tex_unit = -1;
    }
}

// Shader
void DeviceImp::SetShader(int32_t shader) {
    if (shader > -1) {
        glUseProgram(shader);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void DeviceImp::SetShaderLayout(ShaderLayout layout) {
    m_ShaderLayout = layout;
}

void DeviceImp::SetUniform1i(int32_t location, int32_t v) {
    if (location > -1) {
        glUniform1i(location, v);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void DeviceImp::SetUniform1f(int32_t location, float v) {
    if (location > -1) {
        glUniform1f(location, v);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void DeviceImp::SetUniform3f(int32_t location, const Vector& v) {
    if (location > -1) {
        glUniform3f(location, v.x, v.y, v.z);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void DeviceImp::SetUniformMatrix(int32_t location, Matrix& m) {
    if (location > -1) {
        glUniformMatrix4fv(location, 1, true, m.GetData());
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void DeviceImp::SetUniformSampler(int32_t location, int32_t slot) {
    if (location > -1 && 0 <= slot && slot < TS_MAX) {
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0 + m_Texture[slot].tex_unit);
        glBindTexture(GL_TEXTURE_2D, m_Texture[slot].tex_obj);
        glUniform1i(location, m_Texture[slot].tex_unit);
    } else {
        GLB_SAFE_ASSERT(false);
    }
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

void DeviceImp::SetRenderTarget(int32_t rt) {
    if (rt > -1) {
        glBindFramebuffer(GL_FRAMEBUFFER, rt);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void DeviceImp::SetViewport(int32_t x, int32_t y, int32_t width, int32_t height) {
    glViewport(x, y, width, height);
}

// Draw command
void DeviceImp::SetDrawColorBuffer(DrawColorBuffer buffer) {
    GLenum draw_buf = GetDrawColorBufferGL(buffer);
    glDrawBuffer(draw_buf);
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
        clear_flag_gl |= GL_COLOR_BUFFER_BIT;
        glClearColor(m_ClearR, m_ClearG, m_ClearB, 1.0f);
    }

    if (flag & CLEAR_DEPTH) {
        clear_flag_gl |= GL_DEPTH_BUFFER_BIT;
        glClearDepth(m_ClearDepth);
    }

    glClear(clear_flag_gl);
}

void DeviceImp::Draw(PrimitiveType type, int32_t first, int32_t size) {
    glBindVertexArray(m_VertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObject);

    SetupRenderState();

    glDrawArrays(GetPrimitiveTypeGL(type), first, size);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void DeviceImp::SwapBuffer() {
    glutSwapBuffers();
}

// Internel Helper
void DeviceImp::SetupRenderState() {
    // Depth
    if (m_EnableDepthTest) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }

    // Alpha
    if (m_EnableAlphaBlend) {
        SetupAlphaBlend();
    } else {
        glDisable(GL_BLEND);
    }

    // Cull
    if (m_EnableCullFace) {
        SetupCullFace();
    } else {
        glDisable(GL_CULL_FACE);
    }

    // Vertex layout
    SetupVertexLayout();

    // Texture
    SetupTexture();
}

GLenum DeviceImp::GetPrimitiveTypeGL(PrimitiveType type) {
    GLenum mode = GL_TRIANGLES;

    switch (type) {
    case PT_TRIANGLES:
        mode = GL_TRIANGLES;
        break;

    case PT_LINES:
        mode = GL_LINES;
        break;

    default:
        GLB_SAFE_ASSERT(false);
        break;
    }

    return mode;
}

GLenum DeviceImp::GetAlphaBlendFuncGL(AlphaBlendFunc func) {
    GLenum gl_func;

    switch (func) {
    case FUNC_ZERO:
        gl_func = GL_ZERO;
        break;

    case FUNC_ONE:
        gl_func = GL_ONE;
        break;

    case FUNC_SRC_ALPHA:
        gl_func = GL_SRC_ALPHA;
        break;

    case FUNC_DST_ALPHA:
        gl_func = GL_DST_ALPHA;
        break;

    case FUNC_ONE_MINUS_SRC_ALPHA:
        gl_func = GL_ONE_MINUS_SRC_ALPHA;
        break;

    case FUNC_ONE_MINUS_DST_ALPHA:
        gl_func = GL_ONE_MINUS_DST_ALPHA;
        break;

    default:
        break;
    }

    return gl_func;
}

GLenum DeviceImp::GetDrawColorBufferGL(DrawColorBuffer buffer) {
    GLenum result = GL_NONE;

    switch (buffer) {
    case COLORBUF_NONE:
        result = GL_NONE;
        break;

    case COLORBUF_FRONT:
        result = GL_FRONT;
        break;

    case COLORBUF_BACK:
        result = GL_BACK;
        break;

    case COLORBUF_COLOR_ATTACHMENT0:
        result = GL_COLOR_ATTACHMENT0;
        break;

    case COLORBUF_COLOR_ATTACHMENT1:
        result = GL_COLOR_ATTACHMENT1;
        break;

    case COLORBUF_COLOR_ATTACHMENT2:
        result = GL_COLOR_ATTACHMENT2;
        break;

    case COLORBUF_COLOR_ATTACHMENT3:
        result = GL_COLOR_ATTACHMENT3;
        break;

    case COLORBUF_COLOR_ATTACHMENT4:
        result = GL_COLOR_ATTACHMENT4;
        break;

    case COLORBUF_COLOR_ATTACHMENT5:
        result = GL_COLOR_ATTACHMENT5;
        break;

    case COLORBUF_COLOR_ATTACHMENT6:
        result = GL_COLOR_ATTACHMENT6;
        break;

    case COLORBUF_COLOR_ATTACHMENT7:
        result = GL_COLOR_ATTACHMENT7;
        break;

    default:
        GLB_SAFE_ASSERT(false);
        break;
    }

    return result;
}

void DeviceImp::SetupCullFace() {
    glEnable(GL_CULL_FACE);
    switch (m_CullMode) {
    case CULL_FRONT:
        glCullFace(GL_FRONT);
        break;

    case CULL_BACK:
        glCullFace(GL_BACK);
        break;

    case CULL_NONE:
        glCullFace(GL_NONE);
        break;

    default:
        GLB_SAFE_ASSERT(false);
        break;
    }
}

void DeviceImp::SetupAlphaBlend() {
    glEnable(GL_BLEND);
    GLenum src_blend_func = GetAlphaBlendFuncGL(m_SrcBlendFunc);
    GLenum dst_blend_func = GetAlphaBlendFuncGL(m_DstBlendFunc);
    glBlendFunc(src_blend_func, dst_blend_func);
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
                    glEnableVertexAttribArray(location);
                    glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid*>(m_VertexLayout.layouts[i].offset));
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
                    glEnableVertexAttribArray(location);
                    glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid*>(m_VertexLayout.layouts[i].offset));
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
                    glEnableVertexAttribArray(location);
                    glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid*>(m_VertexLayout.layouts[i].offset));
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
                    glEnableVertexAttribArray(location);
                    glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid*>(m_VertexLayout.layouts[i].offset));
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

                // Normal is optional
                if (location != -1) {
                    glEnableVertexAttribArray(location);
                    glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid*>(m_VertexLayout.layouts[i].offset));
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
    //for (int32_t i = 0; i < TS_MAX; i++) {
    //    if (m_Texture[i] != -1) {
    //        glActiveTexture(GL_TEXTURE0 + i);
    //        glBindTexture(GL_TEXTURE_2D, m_Texture[i]);
    //    }
    //}
}

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

void Device::SetVertexArray(int32_t va) {
    if (s_DeviceImp != NULL) {
        s_DeviceImp->SetVertexArray(va);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Device::SetVertexBuffer(int32_t vb) {
    if (s_DeviceImp != NULL) {
        s_DeviceImp->SetVertexBuffer(vb);
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

void Device::SetTexture(int32_t slot, int32_t tex_obj, int32_t tex_unit) {
    if (s_DeviceImp != NULL) {
        s_DeviceImp->SetTexture(slot, tex_obj, tex_unit);
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

void Device::SetShader(int32_t shader) {
    if (s_DeviceImp != NULL) {
        s_DeviceImp->SetShader(shader);
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

void Device::SetRenderTarget(int32_t rt) {
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
