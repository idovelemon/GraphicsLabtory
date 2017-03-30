//-----------------------------------------------------------------------
// Declaration: Copyright (c), i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016/06/20
// Brief: Define the texture manager
//-----------------------------------------------------------------------
#include "glbtexture.h"

#include <stdint.h>

#include <vector>

#include "glbmacro.h"
#include "glbtexturereader.h"
#include "imp/dx11/glbtextureimp.h"
#include "imp/gl/glbtextureimp.h"

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
: m_Imp(NULL) {
}

Texture::~Texture() {
    Destroy();
}

Texture* Texture::Create(const char* texture_name, int32_t type) {
    Texture* tex = NULL;
    Texture::Imp* imp = Texture::Imp::Create(texture_name, type);

    if (imp != NULL) {
        tex = new Texture;
        if (tex != NULL) {
            tex->m_Imp = imp;
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
    Texture::Imp* imp = Texture::Imp::Create(width, height);

    if (imp != NULL) {
        tex = new Texture;
        if (tex != NULL) {
            tex->m_Imp = imp;
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
    Texture::Imp* imp = Texture::Imp::CreateFloat16Texture(width, height);

    if (imp != NULL) {
        tex = new Texture;
        if (tex != NULL) {
            tex->m_Imp = imp;
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
    Texture::Imp* imp = Texture::Imp::CreateFloat16DepthTexture(width, height);

    if (imp != NULL) {
        tex = new Texture;
        if (tex != NULL) {
            tex->m_Imp = imp;
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return tex;
}

Texture* Texture::CreateFloat16CubeTexture(int32_t width, int32_t height) {
    Texture* tex = NULL;
    Texture::Imp* imp = Texture::Imp::CreateFloat16CubeTexture(width, height);

    if (imp != NULL) {
        tex = new Texture;
        if (tex != NULL) {
            tex->m_Imp = imp;
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return tex;
}

void Texture::Destroy() {
    if (m_Imp != NULL) {
        m_Imp->Destroy();
        GLB_SAFE_DELETE(m_Imp);
    }
}

void Texture::UpdateTextureData(const void* pixels, int32_t miplevel) {
    if (m_Imp != NULL) {
        m_Imp->UpdateTextureData(pixels, miplevel);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Texture::GetTextureData(void* pixels, int32_t miplevel) {
    if (m_Imp != NULL) {
        m_Imp->GetTextureData(pixels, miplevel);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Texture::GenerateMipmap() {
    if (m_Imp != NULL) {
        m_Imp->GenerateMipmap();
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

int32_t Texture::GetID() {
    int32_t id = -1;

    if (m_Imp != NULL) {
        id = m_Imp->GetID();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return id;
}

void Texture::SetID(int32_t tex_id) {
    if (m_Imp != NULL) {
        m_Imp->SetID(tex_id);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

int32_t Texture::GetWidth() {
    int32_t width = 0;

    if (m_Imp != NULL) {
        width = m_Imp->GetWidth();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return width;
}

int32_t Texture::GetHeight() {
    int32_t height = 0;

    if (m_Imp != NULL) {
        height = m_Imp->GetHeight();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return height;
}

const char* Texture::GetName() {
    if (m_Imp != NULL) {
        return m_Imp->GetName();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return NULL;
}

int32_t Texture::GetType() {
    int32_t result = TEX_2D;

    if (m_Imp != NULL) {
        result = m_Imp->GetType();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

void* Texture::GetNativeTex() {
    void* result = NULL;

    if (m_Imp != NULL) {
        result = reinterpret_cast<void*>(m_Imp->GetNativeTex());
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
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
        GLB_SAFE_DELETE(m_TexDataBase[i]);
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