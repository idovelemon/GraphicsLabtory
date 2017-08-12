//-----------------------------------------------------------------------
// Declaration: Copyright (c), i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/03/13
// Brief: Implement the texture with opengl
//-----------------------------------------------------------------------
#ifdef GLB_PLATFORM_OPENGL

#ifndef GLB_GLBTEXTUREIMP_GL_
#define GLB_GLBTEXTUREIMP_GL_

#include "render/glbtexture.h"

#include <GL\glew.h>

namespace glb {

namespace render {

namespace texture {

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
// Texture::Imp DECLARATION
//---------------------------------------------------------------------------------
class Texture::Imp {
public:
    Imp();
    virtual ~Imp();

    static Imp* Create(const char* texture_name, int32_t type = TEX_2D);
    static Imp* Create(int32_t width, int32_t height);
    static Imp* CreateFloat32Texture(int32_t width, int32_t height);
    static Imp* CreateFloat32DepthTexture(int32_t width, int32_t height);
    static Imp* CreateFloat32CubeTexture(int32_t width, int32_t height);

protected:
    static void CreateGLTexture2D(int32_t tex_obj, int32_t width, int32_t height, int8_t* texture_data, int32_t texture_pixel_format);
    static void SetTexturePixelFormat(Texture::Imp* texture, int32_t texture_pixel_format);

public:
    int32_t GetID();
    void SetID(int32_t id);
    int32_t GetWidth();
    int32_t GetHeight();
    int32_t GetType();
    int32_t GetBPP();
    const char* GetName();
    int32_t GetNativeTex();

    void Destroy();
    void UpdateTextureData(const void* pixel, int32_t miplevel);
    void GetTextureData(void* pixel, int32_t miplevel);
    void GenerateMipmap();

    void Save(const char* file_name, util::TEXTURE_FILE_TYPE file_type);

private:
    int32_t m_Type;                    // The type of the texture
    int32_t m_Format;                  // The format of the texture
    int32_t m_TexID;                   // The id of this texture
    int32_t m_Width;                   // The width of the texture
    int32_t m_Height;                  // The height of the texture
    int32_t m_Depth;                   // The depth of the texture
    int32_t m_BPP;                     // The Bytes per pixel of the texture
    char    m_TexName[kMaxTexNameLen]; // The name of the texture
    int32_t m_TexObj;                  // The texture object in opengl
};

};  // namespace texture

};  // namespace render

};  // namespace glb

#endif  // GLB_GLBTEXTUREIMP_GL_

#endif  // GLB_PLATFORM_OPENGL