//-----------------------------------------------------------------------
// Declaration: Copyright (c), i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/03/13
// Brief: Implement the texture with opengl
//-----------------------------------------------------------------------
#ifdef GLB_PLATFORM_OPENGL

#include "glbtextureimp.h"

#include "glbtexture.h"

#include <stdint.h>

#include <vector>

#include <GL\glew.h>

#include "glbmacro.h"
#include "glbtexturereader.h"

namespace glb {

namespace texture {

//-----------------------------------------------------------------------------------
// CONSTANT VALUE
//-----------------------------------------------------------------------------------
const int32_t kR8G8B8A8 = 4;

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
// Texture::Imp DEFINITION
//--------------------------------------------------------------------------------------
Texture::Imp::Imp()
: m_Type(TEX_NONE)
, m_Format(FMT_RGBA)
, m_TexID(-1)
, m_Width(0)
, m_Height(0)
, m_Depth(0)
, m_TexObj(-1) {
    memset(m_TexName, 0, sizeof(m_TexName));
}

Texture::Imp::~Imp() {
    Destroy();
}

Texture::Imp* Texture::Imp::Create(const char* texture_name, int32_t type) {
    Texture::Imp* tex = NULL;

    if (texture_name != NULL) {
        int8_t* texture_data = NULL;
        int32_t texture_width = 0;
        int32_t texture_height = 0;
        if (TextureReader::ReadTexture(texture_name, &texture_data, texture_width, texture_height)) {
            GLuint tex_obj = 0;
            glGenTextures(1, &tex_obj);

            if (type == TEX_2D) {
                glBindTexture(GL_TEXTURE_2D, tex_obj);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
                glGenerateMipmap(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, 0);
            } else if (type == TEX_3D) {
                glBindTexture(GL_TEXTURE_3D, tex_obj);
                glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, texture_width, texture_width, texture_width, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
                glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glGenerateMipmap(GL_TEXTURE_3D);
                glBindTexture(GL_TEXTURE_3D, 0);
            }

            TextureReader::ReleaseData(&texture_data);

            tex = new Texture::Imp();
            tex->m_Type = type;
            tex->m_TexObj = tex_obj;
            memcpy(tex->m_TexName, texture_name, strlen(texture_name));
            tex->m_TexName[strlen(texture_name)] = 0;

            if (type == TEX_2D) {
                tex->m_Width = texture_width;
                tex->m_Height = texture_height;
                tex->m_Depth = 0;
                tex->m_Format = FMT_RGBA;
            }
            else if (type == TEX_3D) {
                tex->m_Width = texture_width;
                tex->m_Height = texture_width;
                tex->m_Depth = texture_width;
            }
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return tex;
}

Texture::Imp* Texture::Imp::Create(int32_t width, int32_t height) {
    Texture::Imp* tex = NULL;

    if (width > 0 && height > 0) {
        int32_t tex_id = 0;
        glGenTextures(1, reinterpret_cast<GLuint*>(&tex_id));
        glBindTexture(GL_TEXTURE_2D, tex_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glGenerateMipmap(GL_TEXTURE_2D);

        tex = new Texture::Imp();
        if (tex != NULL) {
            tex->m_Type = TEX_2D;
            static char default_name[] = "DefaultEmpty";
            memcpy(tex->m_TexName, default_name, sizeof(default_name));
            tex->m_TexObj = tex_id;
            tex->m_Width = width;
            tex->m_Height = height;
            tex->m_Format = FMT_RGBA;
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return tex;
}

Texture::Imp* Texture::Imp::CreateFloat16Texture(int32_t width, int32_t height) {
    Texture::Imp* tex = NULL;

    if (width > 0 && height > 0) {
        int32_t tex_id = 0;
        glGenTextures(1, reinterpret_cast<GLuint*>(&tex_id));
        glBindTexture(GL_TEXTURE_2D, tex_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
        glGenerateMipmap(GL_TEXTURE_2D);

        tex = new Texture::Imp();
        if (tex != NULL) {
            tex->m_Type = TEX_2D;
            static char default_name[] = "DefaultEmpty";
            memcpy(tex->m_TexName, default_name, sizeof(default_name));
            tex->m_TexObj = tex_id;
            tex->m_Width = width;
            tex->m_Height = height;
            tex->m_Format = FMT_RGBA16F;
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return tex;
}

Texture::Imp* Texture::Imp::CreateFloat16DepthTexture(int32_t width, int32_t height) {
    Texture::Imp* tex = NULL;

    if (width > 0 && height > 0) {
        int32_t tex_id = 0;
        glGenTextures(1, reinterpret_cast<GLuint*>(&tex_id));
        glBindTexture(GL_TEXTURE_2D, tex_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        tex = new Texture::Imp;
        if (tex != NULL) {
            tex->m_Type = TEX_2D;
            tex->m_Depth = 0;
            tex->m_Height = height;
            static char default_name[] = "DefaultEmpty";
            memcpy(tex->m_TexName, default_name, sizeof(default_name));
            tex->m_TexObj = tex_id;
            tex->m_Type = TEX_2D;
            tex->m_Width = width;
            tex->m_Format = FMT_DEPTH16F;
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return tex;
}

void Texture::Imp::Destroy() {
    if (m_TexID != -1) {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, reinterpret_cast<const GLuint*>(&m_TexObj));
        m_TexObj = 0;
        m_TexID = -1;
    }
}

void Texture::Imp::UpdateTexture(const void* pixels) {
    switch (m_Type) {
    case TEX_2D:
        switch (m_Format) {
        case FMT_RGBA:
            glBindTexture(GL_TEXTURE_2D, m_TexObj);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
            glBindTexture(GL_TEXTURE_2D, 0);
            break;

        case FMT_RGBA16F:
            glBindTexture(GL_TEXTURE_2D, m_TexObj);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_FLOAT, pixels);
            glBindTexture(GL_TEXTURE_2D, 0);
            break;
        default:
            GLB_SAFE_ASSERT(false);  // Only support RGBA now
            break;
        }
        break;

    default:
        GLB_SAFE_ASSERT(false);  // Only support 2D texture now
        break;
    }
}

int32_t Texture::Imp::GetID() {
    return m_TexID;
}

void Texture::Imp::SetID(int32_t tex_id) {
    m_TexID = tex_id;
}

int32_t Texture::Imp::GetWidth() {
    return m_Width;
}

int32_t Texture::Imp::GetHeight() {
    return m_Height;
}

const char* Texture::Imp::GetName() {
    return m_TexName;
}

int32_t Texture::Imp::GetNativeTex() {
    return m_TexObj;
}

};  // namespace texture

};  // namespace glb

#endif  // GLB_PLATFORM_OPENGL