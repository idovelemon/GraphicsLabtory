//-----------------------------------------------------------------------
// Declaration: Copyright (c), i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/03/13
// Brief: Implement the texture with opengl
//-----------------------------------------------------------------------
#ifdef GLB_PLATFORM_OPENGL

#include "glbtextureimp.h"

#include <stdint.h>

#include <vector>

#include <GL\glew.h>

#include "render/glbtexture.h"

#include "util/glbmacro.h"
#include "util/glbtexturereader.h"
#include "util/glbtexturewriter.h"

namespace glb {

namespace render {

namespace texture {

//-----------------------------------------------------------------------------------
// CONSTANT VALUE
//-----------------------------------------------------------------------------------
struct {
    int32_t pixel_format;
    int32_t internel_format;
    int32_t data_format;
    int32_t data_type;
    int32_t bpp;
} kGLPixelFormatTbl[] = {
    {util::TPFT_R8G8B8, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE, 3},
    {util::TPFT_A8R8G8B8, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, 4},
    {util::TPFT_R8G8B8A8, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, 4},
    {util::TPFT_R16G16, GL_RG16, GL_RG, GL_UNSIGNED_SHORT, 4},
    {util::TPFT_G16R16, GL_RG16, GL_RG, GL_UNSIGNED_SHORT, 4},
    {util::TPFT_R16G16B16F, GL_RGB16F, GL_RGB, GL_FLOAT, 3},
    {util::TPFT_R16G16B16A16F, GL_RGBA16, GL_RGBA, GL_UNSIGNED_SHORT, 8},
    {util::TPFT_R32G32B32A32F, GL_RGBA32F, GL_RGBA, GL_FLOAT, 16},
};
static_assert(GLB_ARRAY_SIZE(kGLPixelFormatTbl) == util::TPFT_UNKOWN, "");

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
, m_Format(FMT_R8G8B8A8)
, m_TexID(-1)
, m_Width(0)
, m_Height(0)
, m_Depth(0)
, m_BPP(0)
, m_TexObj(-1) {
    memset(m_TexName, 0, sizeof(m_TexName));
}

Texture::Imp::~Imp() {
    Destroy();
}

Texture::Imp* Texture::Imp::Create(const char* texture_name) {
    Texture::Imp* tex = NULL;

    if (texture_name != NULL) {
        int8_t* texture_data = NULL;
        int32_t texture_width = 0;
        int32_t texture_height = 0;
        int32_t texture_type = util::TT_UNKOWN;
        int32_t texture_pixel_format = util::TPFT_UNKOWN;
        if (util::TextureReader::ReadTexture(texture_name, &texture_data, texture_width, texture_height, texture_type, texture_pixel_format)) {
            GLuint tex_obj = 0;
            glGenTextures(1, &tex_obj);

            if (texture_type == util::TT_2D) {
                CreateGLTexture2D(tex_obj, texture_width, texture_height, texture_data, texture_pixel_format);
            } else if (texture_type == util::TT_3D) {
                glBindTexture(GL_TEXTURE_3D, tex_obj);
                glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, texture_width, texture_width, texture_width, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
                glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  // TODO: Enable mipmap if needed
                glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glGenerateMipmap(GL_TEXTURE_3D);
                glBindTexture(GL_TEXTURE_3D, 0);
            } else if (texture_type == util::TT_CUBE) {
                CreateGLTextureCube(tex_obj, texture_width, texture_height, texture_data, texture_pixel_format);
            }

            util::TextureReader::ReleaseData(&texture_data);

            tex = new Texture::Imp();
            tex->m_TexObj = tex_obj;
            memcpy(tex->m_TexName, texture_name, strlen(texture_name));
            tex->m_TexName[strlen(texture_name)] = 0;

            if (texture_type == util::TT_2D) {
                tex->m_Type = TEX_2D;
                tex->m_Width = texture_width;
                tex->m_Height = texture_height;
                tex->m_Depth = 0;
                SetTexturePixelFormat(tex, texture_pixel_format);
            } else if (texture_type == util::TT_3D) {
                tex->m_Type = TEX_3D;
                tex->m_Width = texture_width;
                tex->m_Height = texture_width;
                tex->m_Depth = texture_width;
            } else if (texture_type == util::TT_CUBE) {
                tex->m_Type = TEX_CUBE;
                tex->m_Width = texture_width;
                tex->m_Height = texture_height;
                tex->m_Depth = 0;
                SetTexturePixelFormat(tex, texture_pixel_format);
            } else {
                GLB_SAFE_ASSERT(false);
            }
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return tex;
}

Texture::Imp* Texture::Imp::CreatePrefilterCubeMap(const char* fileName) {
    Texture::Imp* tex = NULL;

    if (fileName != NULL) {
        int8_t* texture_data = NULL;
        int32_t texture_width = 0;
        int32_t texture_height = 0;
        int32_t texture_type = util::TT_UNKOWN;
        int32_t texture_pixel_format = util::TPFT_UNKOWN;
        if (util::TextureReader::ReadTexture(fileName, &texture_data, texture_width, texture_height, texture_type, texture_pixel_format)) {
            GLuint tex_obj = 0;
            glGenTextures(1, &tex_obj);
            glBindTexture(GL_TEXTURE_CUBE_MAP, tex_obj);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP);

            int32_t miplevels = log(max(texture_width, texture_height)) / log(2) + 1;
            int32_t glFaces[] = {
                GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
                GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
            };

            int32_t index = 0;
            for (int32_t i = 0; i < miplevels; i++) {
                for(int32_t j = 0; j < 6; j++) {
                    int32_t width = texture_width * pow(2, -i);
                    int32_t height = texture_height * pow(2, -i);
                    glTexImage2D(glFaces[j], i, GL_RGBA16, width, height, 0, GL_RGBA, GL_UNSIGNED_SHORT, texture_data + index);

                    int32_t sizeBytes = sizeof(int16_t) * 4 * width * height;
                    index = index + sizeBytes;
                }
            }

            util::TextureReader::ReleaseData(&texture_data);

            tex = new Texture::Imp();
            tex->m_TexObj = tex_obj;
            memcpy(tex->m_TexName, fileName, strlen(fileName));
            tex->m_TexName[strlen(fileName)] = 0;
            tex->m_Type = TEX_CUBE;
            tex->m_Width = texture_width;
            tex->m_Height = texture_height;
            tex->m_Depth = 0;
            SetTexturePixelFormat(tex, texture_pixel_format);
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return tex;
}

Texture::Imp* Texture::Imp::CreatePrefilterTableMap(const char* fileName) {
    Texture::Imp* tex = NULL;

    if (fileName != NULL) {
        int8_t* texture_data = NULL;
        int32_t texture_width = 0;
        int32_t texture_height = 0;
        int32_t texture_type = util::TT_UNKOWN;
        int32_t texture_pixel_format = util::TPFT_UNKOWN;
        if (util::TextureReader::ReadTexture(fileName, &texture_data, texture_width, texture_height, texture_type, texture_pixel_format)) {
            GLuint tex_obj = 0;
            glGenTextures(1, &tex_obj);
            glBindTexture(GL_TEXTURE_2D, tex_obj);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, texture_width, texture_height, 0, GL_RGBA, GL_FLOAT, texture_data);

            util::TextureReader::ReleaseData(&texture_data);

            tex = new Texture::Imp();
            tex->m_TexObj = tex_obj;
            memcpy(tex->m_TexName, fileName, strlen(fileName));
            tex->m_TexName[strlen(fileName)] = 0;
            tex->m_Type = TEX_2D;
            tex->m_Width = texture_width;
            tex->m_Height = texture_height;
            tex->m_Depth = 0;
            SetTexturePixelFormat(tex, texture_pixel_format);
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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glGenerateMipmap(GL_TEXTURE_2D);

        tex = new Texture::Imp();
        if (tex != NULL) {
            tex->m_Type = TEX_2D;
            static char default_name[] = "DefaultEmpty";
            memcpy(tex->m_TexName, default_name, sizeof(default_name));
            tex->m_TexObj = tex_id;
            tex->m_Width = width;
            tex->m_Height = height;
            tex->m_Format = FMT_R8G8B8A8;
            tex->m_BPP = 4;
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return tex;
}

Texture::Imp* Texture::Imp::CreateFloat32Texture(int32_t width, int32_t height) {
    Texture::Imp* tex = NULL;

    if (width > 0 && height > 0) {
        int32_t tex_id = 0;
        glGenTextures(1, reinterpret_cast<GLuint*>(&tex_id));
        glBindTexture(GL_TEXTURE_2D, tex_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
        glGenerateMipmap(GL_TEXTURE_2D);

        tex = new Texture::Imp();
        if (tex != NULL) {
            tex->m_Type = TEX_2D;
            static char default_name[] = "DefaultEmpty";
            memcpy(tex->m_TexName, default_name, sizeof(default_name));
            tex->m_TexObj = tex_id;
            tex->m_Width = width;
            tex->m_Height = height;
            tex->m_Format = FMT_R32G32B32A32F;
            tex->m_BPP = 16;
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return tex;
}

Texture::Imp* Texture::Imp::CreateFloat32DepthTexture(int32_t width, int32_t height) {
    Texture::Imp* tex = NULL;

    if (width > 0 && height > 0) {
        int32_t tex_id = 0;
        glGenTextures(1, reinterpret_cast<GLuint*>(&tex_id));
        glBindTexture(GL_TEXTURE_2D, tex_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  // TODO: Enable mipmap if needed
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
            tex->m_Width = width;
            tex->m_Format = FMT_DEPTH32F;
            tex->m_BPP = 4;
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return tex;
}

Texture::Imp* Texture::Imp::CreateFloat32CubeTexture(int32_t width, int32_t height) {
    Texture::Imp* tex = NULL;

    // Warning: The cube map's 6 texture must be square and have the same size
    if (width > 0 && height > 0 && width == height) {
        int32_t tex_id = 0;
        glGenTextures(1, reinterpret_cast<GLuint*>(&tex_id));
        glBindTexture(GL_TEXTURE_CUBE_MAP, tex_id);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP);

        for(int32_t i = 0; i < 6; i++) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
        }

        tex = new Texture::Imp;
        if (tex != NULL) {
            tex->m_Type = TEX_CUBE;
            tex->m_Depth = 0;
            tex->m_Height = height;
            static char default_name[] = "DefaultEmpty";
            memcpy(tex->m_TexName, default_name, sizeof(default_name));
            tex->m_TexObj = tex_id;
            tex->m_Width = width;
            tex->m_Format = FMT_R32G32B32A32F;
            tex->m_BPP = 16;
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return tex;
}

Texture::Imp* Texture::Imp::CreateFloat16CubeTexture(int32_t width, int32_t height) {
    Texture::Imp* tex = NULL;

    // Warning: The cube map's 6 texture must be square and have the same size
    if (width > 0 && height > 0 && width == height) {
        int32_t tex_id = 0;
        glGenTextures(1, reinterpret_cast<GLuint*>(&tex_id));
        glBindTexture(GL_TEXTURE_CUBE_MAP, tex_id);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

        for(int32_t i = 0; i < 6; i++) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA16, width, height, 0, GL_RGBA, GL_UNSIGNED_SHORT, NULL);
        }
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

        tex = new Texture::Imp;
        if (tex != NULL) {
            tex->m_Type = TEX_CUBE;
            tex->m_Depth = 0;
            tex->m_Height = height;
            static char default_name[] = "DefaultEmpty";
            memcpy(tex->m_TexName, default_name, sizeof(default_name));
            tex->m_TexObj = tex_id;
            tex->m_Width = width;
            tex->m_Format = FMT_R16G16B16A16F;
            tex->m_BPP = 8;
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return tex;
}

Texture::Imp* Texture::Imp::CreateFloat323DTexture(int32_t width, int32_t height, int32_t depth) {
    Texture::Imp* tex = NULL;

    if (width > 0 && height > 0 && depth > 0) {
        int32_t tex_id = 0;
        glGenTextures(1, reinterpret_cast<GLuint*>(&tex_id));
        glBindTexture(GL_TEXTURE_3D, tex_id);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

        glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, width, height, depth, 0, GL_RGBA, GL_FLOAT, NULL);
        glGenerateMipmap(GL_TEXTURE_3D);

        tex = new Texture::Imp;
        if (tex != NULL) {
            tex->m_Type = TEX_3D;
            tex->m_Depth = depth;
            tex->m_Height = height;
            static char default_name[] = "DefaultEmpty";
            memcpy(tex->m_TexName, default_name, sizeof(default_name));
            tex->m_TexObj = tex_id;
            tex->m_Width = width;
            tex->m_Format = FMT_R32G32B32A32F;
            tex->m_BPP = 16;
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return tex;
}

void Texture::Imp::CreateGLTexture2D(int32_t tex_obj, int32_t width, int32_t height, int8_t* texture_data, int32_t texture_pixel_format) {
    for (int32_t i = 0; i < GLB_ARRAY_SIZE(kGLPixelFormatTbl); i++) {
        if (kGLPixelFormatTbl[i].pixel_format == texture_pixel_format) {
            glBindTexture(GL_TEXTURE_2D, tex_obj);
            if (kGLPixelFormatTbl[i].data_type == GL_FLOAT) {
                float* data = reinterpret_cast<float*>(texture_data);
                glTexImage2D(GL_TEXTURE_2D, 0, kGLPixelFormatTbl[i].internel_format, width, height, 0, kGLPixelFormatTbl[i].data_format, kGLPixelFormatTbl[i].data_type, data);
            } else {
                glTexImage2D(GL_TEXTURE_2D, 0, kGLPixelFormatTbl[i].internel_format, width, height, 0, kGLPixelFormatTbl[i].data_format, kGLPixelFormatTbl[i].data_type, texture_data);
            }
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  // TODO: Enable mipmap if needed
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glBindTexture(GL_TEXTURE_2D, 0);
            break;
        }
    }
}

void Texture::Imp::CreateGLTextureCube(int32_t tex_obj, int32_t width, int32_t height, int8_t* texture_data, int32_t texture_pixel_format) {
    // Warning: The cube map's 6 texture must be square and have the same size
    if (width > 0 && height > 0 && width == height) {
        glBindTexture(GL_TEXTURE_CUBE_MAP, tex_obj);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  // TODO: Enable mipmap if needed
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP);

        for (int32_t i = 0; i < GLB_ARRAY_SIZE(kGLPixelFormatTbl); i++) {
            if (kGLPixelFormatTbl[i].pixel_format == texture_pixel_format) {
                int32_t gl_faces[] = {
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                    GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,  // TODO: Just flip y, i do not know why
                    GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
                    GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
                };
                for(int32_t j = 0; j < 6; j++) {
                    glTexImage2D(gl_faces[j], 0, kGLPixelFormatTbl[i].internel_format, width, height, 0, kGLPixelFormatTbl[i].data_format, kGLPixelFormatTbl[i].data_type, texture_data + j * height * width * kGLPixelFormatTbl[i].bpp);
                }
                break;
            }
        }

        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Texture::Imp::SetTexturePixelFormat(Texture::Imp* texture, int32_t texture_pixel_format) {
    if (texture != NULL) {
        struct {
            int32_t pixel_format_from_file;
            int32_t pixel_format_from_lib;
            int32_t bytes_per_pixel;
        } pixel_format[] = {
            {util::TPFT_R8G8B8, FMT_R8G8B8, 3},
            {util::TPFT_A8R8G8B8, FMT_R8G8B8A8, 4},
            {util::TPFT_R8G8B8A8, FMT_R8G8B8A8, 4},
            {util::TPFT_R16G16, FMT_R16G16, 4},
            {util::TPFT_G16R16, FMT_R16G16, 4},
            {util::TPFT_R16G16B16F, FMT_R16G16B16F, 3},
            {util::TPFT_R16G16B16A16F, FMT_R16G16B16A16F, 8},
            {util::TPFT_R32G32B32A32F, FMT_R32G32B32A32F, 16},
        };
        static_assert(GLB_ARRAY_SIZE(pixel_format) == util::TPFT_UNKOWN, "");

        for (int32_t i = 0; i < GLB_ARRAY_SIZE(pixel_format); i++) {
            if (pixel_format[i].pixel_format_from_file == texture_pixel_format) {
                texture->m_Format = pixel_format[i].pixel_format_from_lib;
                texture->m_BPP = pixel_format[i].bytes_per_pixel;
                break;
            }
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Texture::Imp::Destroy() {
    if (m_TexID != -1) {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, reinterpret_cast<const GLuint*>(&m_TexObj));
        m_TexObj = 0;
        m_TexID = -1;
    }
}

void Texture::Imp::UpdateTextureData(const void* pixels, int32_t miplevel) {
    switch (m_Type) {
    case TEX_2D:
        switch (m_Format) {
        case FMT_R8G8B8A8:
            glBindTexture(GL_TEXTURE_2D, m_TexObj);
            glTexSubImage2D(GL_TEXTURE_2D, miplevel, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
            glBindTexture(GL_TEXTURE_2D, 0);
            break;

        case FMT_R32G32B32A32F:
            glBindTexture(GL_TEXTURE_2D, m_TexObj);
            glTexSubImage2D(GL_TEXTURE_2D, miplevel, 0, 0, m_Width, m_Height, GL_RGBA, GL_FLOAT, pixels);
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

void Texture::Imp::GetTextureData(void* pixel, int32_t miplevel) {
    switch (m_Type) {
    case TEX_2D:
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, m_TexObj);
        if (m_Format == FMT_R8G8B8A8) {
            glGetTexImage(GL_TEXTURE_2D, miplevel, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
        } else if (m_Format == FMT_R32G32B32A32F) {
            glGetTexImage(GL_TEXTURE_2D, miplevel, GL_RGBA, GL_FLOAT, pixel);
        } else {
            GLB_SAFE_ASSERT(false);
        }
        break;

    case TEX_CUBE:
        glEnable(GL_TEXTURE_CUBE_MAP);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_TexObj);
        if (m_Format == FMT_R16G16B16A16F) {
            glGetTexImage(GL_TEXTURE_CUBE_MAP, miplevel, GL_RGBA, GL_UNSIGNED_SHORT, pixel);
        } else {
            GLB_SAFE_ASSERT(false);
        }
        break;

    default:
        GLB_SAFE_ASSERT(false);  // Only support 2D texture now
        break;
    }
}

void Texture::Imp::GenerateMipmap() {
    switch (m_Type) {
    case TEX_2D:
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, m_TexObj);
        glGenerateMipmap(GL_TEXTURE_2D);
        break;

    case TEX_CUBE:
        glEnable(GL_TEXTURE_CUBE_MAP);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_TexObj);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        break;

    default:
        GLB_SAFE_ASSERT(false);  // Only support 2D texture now
        break;
    }
}

void Texture::Imp::Save(const char* fileName, util::TEXTURE_FILE_TYPE fileType) {
    if (m_Type == TEX_2D) {
        int8_t* pixel = new int8_t[m_Width * m_Height * m_BPP];

        util::TEXTURE_PIXEL_FORMAT_TYPE pixel_format = util::TPFT_UNKOWN;
        if (m_Format == FMT_R8G8B8A8) {
            pixel_format = util::TPFT_R8G8B8A8;
        } else {
            // TODO: do not support now
            GLB_SAFE_ASSERT(false);
        }

        GetTextureData(pixel, 0);

        // util::TextureWriter::Write(file_name, pixel, m_Width, m_Height, pixel_format, m_BPP, file_type);
    } else if (m_Type == TEX_CUBE) {

    } else {
        // TODO: Only save 2D texture now
        GLB_SAFE_ASSERT(false);
    }
}

void Texture::Imp::SavePrefilterCubeMap(const char* fileName) {
    if (m_Type == TEX_CUBE && m_BPP == 8 && m_Format == FMT_R16G16B16A16F) {
        // Calculate mipmap level
        int32_t miplevels = log(max(m_Width, m_Height)) / log(2) + 1;
        int32_t sizeBytes = 0;
        for (int32_t i = 0; i < miplevels; i++) {
            sizeBytes = sizeBytes + sizeof(int16_t) * 4 * (m_Width * pow(2, -i)) * (m_Height * pow(2, -i)) * 6;
        }

        int8_t* pixel = new int8_t[sizeBytes];

        glEnable(GL_TEXTURE_CUBE_MAP);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_TexObj);

        int32_t index = 0;
        for (int32_t i = 0; i < miplevels; i++) {
            sizeBytes = sizeof(int16_t) * 4 * (m_Width * pow(2, -i)) * (m_Height * pow(2, -i));
            int8_t* data = new int8_t[sizeBytes];

            // +X
            glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X, i, GL_RGBA, GL_UNSIGNED_SHORT, data);
            memcpy(pixel + index, data, sizeBytes);
            index = index + sizeBytes;

            // -X
            glGetTexImage(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, i, GL_RGBA, GL_UNSIGNED_SHORT, data);
            memcpy(pixel + index, data, sizeBytes);
            index = index + sizeBytes;

            // +Y
            glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, i, GL_RGBA, GL_UNSIGNED_SHORT, data);
            memcpy(pixel + index, data, sizeBytes);
            index = index + sizeBytes;

            // -Y
            glGetTexImage(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, i, GL_RGBA, GL_UNSIGNED_SHORT, data);
            memcpy(pixel + index, data, sizeBytes);
            index = index + sizeBytes;

            // +Z
            glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, i, GL_RGBA, GL_UNSIGNED_SHORT, data);
            memcpy(pixel + index, data, sizeBytes);
            index = index + sizeBytes;

            // -Z
            glGetTexImage(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, i, GL_RGBA, GL_UNSIGNED_SHORT, data);
            memcpy(pixel + index, data, sizeBytes);
            index = index + sizeBytes;

            delete[] data;
            data = NULL;
        }

        util::TextureWriter::Write(fileName, pixel, m_Width, m_Height, -1, -1, util::TFT_PFC);

        delete[] pixel;
        pixel = NULL;

    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Texture::Imp::SavePrefilterTableMap(const char* fileName) {
    if (m_Type == TEX_2D && m_BPP == 16 && m_Format == FMT_R32G32B32A32F) {
        int8_t* pixel = new int8_t[sizeof(int32_t) * 4 * m_Width * m_Height];
        GetTextureData(pixel, 0);

        util::TextureWriter::Write(fileName, pixel, m_Width, m_Height, -1, -1, util::TFT_PFT);

        delete[] pixel;
        pixel = NULL;

    } else {
        GLB_SAFE_ASSERT(false);
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

int32_t Texture::Imp::GetType() {
    return m_Type;
}

int32_t Texture::Imp::GetBPP() {
    return m_BPP;
}

const char* Texture::Imp::GetName() {
    return m_TexName;
}

int32_t Texture::Imp::GetNativeTex() {
    return m_TexObj;
}

};  // namespace texture

};  // namespace render

};  // namespace glb

#endif  // GLB_PLATFORM_OPENGL