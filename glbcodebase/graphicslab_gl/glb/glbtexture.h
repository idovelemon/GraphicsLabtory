//-----------------------------------------------------------------------
// Declaration: Copyright (c), i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016/06/20
// Brief: Define the texture manager
//-----------------------------------------------------------------------
#ifndef GLB_GLBTEXTURE_H_
#define GLB_GLBTEXTURE_H_

#include <stdint.h>

namespace glb {

namespace texture {

//-----------------------------------------------------------------------------------
// CONSTANT VALUE
//-----------------------------------------------------------------------------------
const int32_t kMaxTexNameLen = 256;

//-----------------------------------------------------------------------------------
// TYPE DECLARATION
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// CLASS DECLARATION
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// Texture DECLARATION
//----------------------------------------------------------------------------------
class Texture {
public:
    enum Type {
        TEX_NONE = 0,
        TEX_2D,
        TEX_3D
    };

    enum Format {
        FMT_RGBA = 0,
        FMT_RGBA16F,
        FMT_DEPTH16F,
    };

public:
    Texture();
    virtual ~Texture();

    static Texture* Create(const char* texture_name, int32_t type = TEX_2D);
    static Texture* Create(int32_t width, int32_t height);
    static Texture* CreateFloat16Texture(int32_t width, int32_t height);
    static Texture* CreateFloat16DepthTexture(int32_t width, int32_t height);

 public:
     int32_t GetID();
     void SetID(int32_t id);
     int32_t GetWidth();
     int32_t GetHeight();
     const char* GetName();
     int32_t GetTexObj();

     void Destroy();
     void UpdateTexture(const void* pixel);

 private:
     int32_t m_Type;                    // The type of the texture
     int32_t m_Format;                  // The format of the texture
     int32_t m_TexID;                   // The id of this texture
     int32_t m_Width;                   // The width of the texture
     int32_t m_Height;                  // The height of the texture
     int32_t m_Depth;                   // The depth of the texture
     char    m_TexName[kMaxTexNameLen]; // The name of the texture
     int32_t m_TexObj;                  // The texture object in opengl
};

//-----------------------------------------------------------------------------------
// Mgr DECLARATION
//----------------------------------------------------------------------------------
class Mgr {
 public:
     static void Initialize();
     static void Destroy();

     //------------------------------------------------------------------------
     // @return: If failed, return -1. Otherwise, return the ID of the texture
     //------------------------------------------------------------------------
     static int32_t LoadTexture(const char* texture_name);

     //------------------------------------------------------------------------
     // @return: If failed, return -1. Otherwise, return the ID of the texture
     //------------------------------------------------------------------------
     static int32_t AddTexture(Texture* tex);

     static Texture* GetTextureById(int32_t tex_id);
};

};  // namespace texture

};  // namespace glb

#endif  // GLB_GLBTEXTURE_H_