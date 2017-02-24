//-----------------------------------------------------------------------
// Declaration: Copyright (c), i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016/06/20
// Brief: Define the texture manager
//-----------------------------------------------------------------------
#include "glbtexture.h"

#include <stdint.h>

#include <vector>

#include <GL\glew.h>
#include <glut.h>

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
class MgrImp;
static MgrImp* s_MgrImp = NULL;

//-----------------------------------------------------------------------------------
// CLASS DECLARATION
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// MgrImp DECLARATION
//----------------------------------------------------------------------------------
class MgrImp {
 public:
     MgrImp();
     virtual ~MgrImp();

     void Initialize();
     void Destroy();

     int32_t LoadTexture(const char* texture_name);
     int32_t AddTexture(Texture* tex);
     Texture* GetTextureById(int32_t id);

 private:
     std::vector<Texture*> m_TexDataBase;
};

//-----------------------------------------------------------------------------------
// CLASS DEFINITION
//-----------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Texture DEFINITION
//--------------------------------------------------------------------------------------
Texture::Texture()
: m_Type(TEX_NONE)
, m_Format(FMT_RGBA)
, m_TexID(-1)
, m_Width(0)
, m_Height(0)
, m_Depth(0)
, m_TexObj(-1) {
    memset(m_TexName, 0, sizeof(m_TexName));
}

Texture::~Texture() {
    Destroy();
}

Texture* Texture::Create(const char* texture_name, int32_t type) {
    Texture* tex = NULL;

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

            tex = new Texture();
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

Texture* Texture::Create(int32_t width, int32_t height) {
    Texture* tex = NULL;

    if (width > 0 && height > 0) {
        int32_t tex_id = 0;
        glGenTextures(1, reinterpret_cast<GLuint*>(&tex_id));
        glBindTexture(GL_TEXTURE_2D, tex_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glGenerateMipmap(GL_TEXTURE_2D);

        tex = new Texture();
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

Texture* Texture::CreateFloat16Texture(int32_t width, int32_t height) {
    Texture* tex = NULL;

    if (width > 0 && height > 0) {
        int32_t tex_id = 0;
        glGenTextures(1, reinterpret_cast<GLuint*>(&tex_id));
        glBindTexture(GL_TEXTURE_2D, tex_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
        glGenerateMipmap(GL_TEXTURE_2D);

        tex = new Texture();
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

Texture* Texture::CreateFloat16DepthTexture(int32_t width, int32_t height) {
    Texture* tex = NULL;

    if (width > 0 && height > 0) {
        int32_t tex_id = 0;
        glGenTextures(1, reinterpret_cast<GLuint*>(&tex_id));
        glBindTexture(GL_TEXTURE_2D, tex_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        tex = new Texture;
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

void Texture::Destroy() {
    if (m_TexID) {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, reinterpret_cast<const GLuint*>(&m_TexObj));
        m_TexObj = 0;
    }
}

void Texture::UpdateTexture(const void* pixels) {
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

int32_t Texture::GetID() {
    return m_TexID;
}

void Texture::SetID(int32_t tex_id) {
    m_TexID = tex_id;
}

int32_t Texture::GetWidth() {
    return m_Width;
}

int32_t Texture::GetHeight() {
    return m_Height;
}

const char* Texture::GetName() {
    return m_TexName;
}

int32_t Texture::GetTexObj() {
    return m_TexObj;
}

//--------------------------------------------------------------------------------------
// MgrImp DEFINITION
//---------------------------------------------------------------------------------------
MgrImp::MgrImp() {
    m_TexDataBase.clear();
}

MgrImp::~MgrImp() {
    Destroy();
}

void MgrImp::Initialize() {
}

void MgrImp::Destroy() {
    for (size_t i = 0; i < m_TexDataBase.size(); i++) {
        m_TexDataBase[i]->Destroy();
        delete m_TexDataBase[i];
    }

    m_TexDataBase.clear();
}

int32_t MgrImp::LoadTexture(const char* texture_name) {
    int32_t result = -1;

    if (texture_name) {
        // Check if already loaded
        bool is_loaded = false;
        for (size_t i = 0; i < m_TexDataBase.size(); i++) {
            if (!strcmp(texture_name, m_TexDataBase[i]->GetName())) {
                is_loaded = true;
                result = i;
                break;
            }
        }

        // Load the texture
        if (is_loaded == false) {
            Texture* new_tex = Texture::Create(texture_name);
            if (new_tex) {
                result = m_TexDataBase.size();
                m_TexDataBase.push_back(new_tex);
                new_tex->SetID(result);
            } else {
                GLB_SAFE_ASSERT(false);
            }
        }
    }

    return result;
}

int32_t MgrImp::AddTexture(Texture* tex) {
    int32_t result = -1;

    if (tex != NULL) {
        result = m_TexDataBase.size();
        m_TexDataBase.push_back(tex);
        tex->SetID(result);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

Texture* MgrImp::GetTextureById(int32_t id) {
    Texture* result = NULL;

    if (0 <= id && id < static_cast<int32_t>(m_TexDataBase.size())) {
        result = m_TexDataBase[id];
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

//--------------------------------------------------------------------------------------
// MgrImp DEFINITION
//---------------------------------------------------------------------------------------
void Mgr::Initialize() {
    if (s_MgrImp == NULL) {
        s_MgrImp = new MgrImp();
        GLB_SAFE_ASSERT(s_MgrImp != NULL);
        s_MgrImp->Initialize();
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Mgr::Destroy() {
    if (s_MgrImp != NULL) {
        s_MgrImp->Destroy();
        GLB_SAFE_DELETE(s_MgrImp);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

int32_t Mgr::LoadTexture(const char* texture_name) {
    int32_t result = -1;
    
    if (s_MgrImp != NULL) {
        result = s_MgrImp->LoadTexture(texture_name);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

int32_t Mgr::AddTexture(Texture* tex) {
    int32_t result = -1;

    if (s_MgrImp != NULL) {
        result = s_MgrImp->AddTexture(tex);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

Texture* Mgr::GetTextureById(int32_t id) {
    Texture* result = NULL;

    if (s_MgrImp != NULL) {
        result = s_MgrImp->GetTextureById(id);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

};  // namespace texture

};  // namespace glb
