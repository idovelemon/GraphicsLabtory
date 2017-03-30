//-----------------------------------------------------------------------
// Declaration: Copyright (c), i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/03/19
// Brief: Implement the texture with dx11
//-----------------------------------------------------------------------
#ifdef GLB_PLATFORM_DX11

#include "glbtextureimp.h"

#include "glbtexture.h"

#include <stdint.h>

#include <vector>

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
, m_Depth(0) {
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
            TextureReader::ReleaseData(&texture_data);

            tex = new Texture::Imp();
            tex->m_Type = type;
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
        tex = new Texture::Imp();
        if (tex != NULL) {
            tex->m_Type = TEX_2D;
            static char default_name[] = "DefaultEmpty";
            memcpy(tex->m_TexName, default_name, sizeof(default_name));
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
        tex = new Texture::Imp();
        if (tex != NULL) {
            tex->m_Type = TEX_2D;
            static char default_name[] = "DefaultEmpty";
            memcpy(tex->m_TexName, default_name, sizeof(default_name));
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
        tex = new Texture::Imp;
        if (tex != NULL) {
            tex->m_Type = TEX_2D;
            tex->m_Depth = 0;
            tex->m_Height = height;
            static char default_name[] = "DefaultEmpty";
            memcpy(tex->m_TexName, default_name, sizeof(default_name));
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
        m_TexID = -1;
    }
}

void Texture::Imp::UpdateTextureData(const void* pixels, int32_t miplevel) {
    switch (m_Type) {
    case TEX_2D:
        switch (m_Format) {
        case FMT_RGBA:
            break;

        case FMT_RGBA16F:
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

void Texture::Imp::GetTextureData(void* pixel, int32_t miplevel) {
}

void Texture::Imp::GenerateMipmap() {
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
    return 0;
}

};  // namespace texture

};  // namespace glb

#endif  // GLB_PLATFORM_DX11