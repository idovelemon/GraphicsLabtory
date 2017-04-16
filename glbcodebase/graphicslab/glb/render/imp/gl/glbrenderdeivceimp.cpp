//-----------------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 03 / 18
// Brief: Implement renderdevice with opengl
//-----------------------------------------------------------------------------------
#ifdef GLB_PLATFORM_OPENGL

#include "glbrenderdeviceimp.h"

#include "glbapplication.h"

#include "util/glblog.h"

#include "render/imp/gl/glbmeshimp.h"
#include "render/imp/gl/glbrendertargetimp.h"
#include "render/imp/gl/glbtextureimp.h"

namespace glb {

namespace render {

//-----------------------------------------------------------------------------------
// CLASS DEFINITION
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// DeviceImp DEFINITION
//-----------------------------------------------------------------------------------
DeviceImp::DeviceImp()
: m_WindowDC(NULL)
, m_OpenGLContext(NULL)
, m_VertexArrayObject(-1)
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
    m_WindowDC = ::GetDC(app::Application::GetWindowHandle());

    // Set surface pixel format
    PIXELFORMATDESCRIPTOR pixel_desc;
    pixel_desc.nSize = sizeof(pixel_desc);
    pixel_desc.nVersion = 1;
    pixel_desc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pixel_desc.iPixelType = PFD_TYPE_RGBA;
    pixel_desc.cColorBits = 32;
    pixel_desc.cRedBits = 0;
    pixel_desc.cRedShift = 0;
    pixel_desc.cGreenBits = 0;
    pixel_desc.cGreenShift = 0;
    pixel_desc.cBlueBits = 0;
    pixel_desc.cBlueShift = 0;
    pixel_desc.cAlphaBits = 0;
    pixel_desc.cAlphaShift = 0;
    pixel_desc.cAccumBits = 0;
    pixel_desc.cAccumRedBits = 0;
    pixel_desc.cAccumGreenBits = 0;
    pixel_desc.cAccumBlueBits = 0;
    pixel_desc.cAccumAlphaBits = 0;
    pixel_desc.cDepthBits = 24;
    pixel_desc.cStencilBits = 8;
    pixel_desc.cAuxBuffers = 0;
    pixel_desc.iLayerType = PFD_MAIN_PLANE;
    pixel_desc.bReserved = 0;
    pixel_desc.dwLayerMask = 0;
    pixel_desc.dwVisibleMask = 0;
    pixel_desc.dwDamageMask = 0;

    int pixel_fmt = ChoosePixelFormat(m_WindowDC, &pixel_desc);
    if (pixel_fmt == 0) {
        return;
    }

    if (SetPixelFormat(m_WindowDC, pixel_fmt, &pixel_desc) == FALSE) {
        return;
    }

    int n = GetPixelFormat(m_WindowDC);
    DescribePixelFormat(m_WindowDC, n, sizeof(pixel_desc), &pixel_desc);

    // Create opengl render context
    m_OpenGLContext = wglCreateContext(m_WindowDC);
    if (m_OpenGLContext == NULL) {
        return;
    }
    wglMakeCurrent(m_WindowDC, m_OpenGLContext);

    // Initialize opengl extension
    glewInit();

    const GLubyte* version = glGetString(GL_VERSION);
    util::log::LogPrint("OpenGL Version: %s", reinterpret_cast<LPCSTR>(version));

    const GLubyte* vendor = glGetString(GL_VENDOR);
    util::log::LogPrint("OpenGL Vendor: %s", reinterpret_cast<LPCSTR>(vendor));

    const GLubyte* render = glGetString(GL_RENDERER);
    util::log::LogPrint("OpenGL Render: %s", reinterpret_cast<LPCSTR>(render));

    const GLubyte* glew_version = glewGetString(GLEW_VERSION);
    util::log::LogPrint("GLEW Version: %s", reinterpret_cast<LPCSTR>(glew_version));
}

void DeviceImp::Destroy() {
    wglMakeCurrent(m_WindowDC, NULL);
    wglDeleteContext(m_OpenGLContext);
    ReleaseDC(app::Application::GetWindowHandle(), m_WindowDC);
    m_WindowDC = NULL;
    m_OpenGLContext = NULL;
}

// Vertex Buffer
void DeviceImp::SetVertexBuffer(mesh::VertexBuffer* buf) {
    m_VertexArrayObject = buf->GetVAO();
    m_VertexBufferObject = buf->GetVBO();
}

void DeviceImp::SetVertexLayout(VertexLayout layout) {
    m_VertexLayout = layout;
}

// Texture
void DeviceImp::SetTexture(int32_t slot, texture::Texture* tex, int32_t tex_unit) {
    if (0 <= slot && slot < TS_MAX) {
        m_Texture[slot].tex_obj = reinterpret_cast<int32_t>(tex->GetNativeTex());
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
void DeviceImp::SetShader(shader::Program* program) {
    if (program != NULL) {
        int32_t shader = reinterpret_cast<int32_t>(program->GetNativeShader());
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

void DeviceImp::SetUniform3f(int32_t location, const math::Vector& v) {
    if (location > -1) {
        glUniform3f(location, v.x, v.y, v.z);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void DeviceImp::SetUniformMatrix(int32_t location, math::Matrix& m) {
    if (location > -1) {
        glUniformMatrix4fv(location, 1, true, m.GetData());
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void DeviceImp::SetUniformSampler2D(int32_t location, int32_t slot) {
    if (location > -1 && 0 <= slot && slot < TS_MAX) {
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0 + m_Texture[slot].tex_unit);
        glBindTexture(GL_TEXTURE_2D, m_Texture[slot].tex_obj);
        glUniform1i(location, m_Texture[slot].tex_unit);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void DeviceImp::SetUniformSamplerCube(int32_t location, int32_t slot) {
    if (location > -1 && 0 <= slot && slot < TS_MAX) {
        glEnable(GL_TEXTURE_CUBE_MAP);
        glActiveTexture(GL_TEXTURE0 + m_Texture[slot].tex_unit);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture[slot].tex_obj);
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

void DeviceImp::SetRenderTarget(RenderTarget* rt) {
    if (rt != NULL) {
        int32_t fbo = reinterpret_cast<int32_t>(rt->GetNativeRenderTarget());
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    } else {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
    glFlush();
    SwapBuffers(m_WindowDC);
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

                // Tangent is optional
                if (location != -1) {
                    glEnableVertexAttribArray(location);
                    glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid*>(m_VertexLayout.layouts[i].offset));
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

};  // namespace render

};  // namespace glb

#endif  // GLB_PLATFORM_OPENGL