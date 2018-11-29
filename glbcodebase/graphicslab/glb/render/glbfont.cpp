//------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018 / 11 / 11
// Brief: Manage all font
//------------------------------------------------------------------------
#include "glbfont.h"

#include <map>
#include <fstream>

#include "glbapplication.h"
#include "glbtexture.h"
#include "util/glbmacro.h"

namespace glb {

namespace render {

namespace font {

//-----------------------------------------------------------------------------------
// CONSTANT VALUE
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// TYPE DECLARATION
//-----------------------------------------------------------------------------------
class MgrImp;
static MgrImp* s_MgrImp = NULL;

//-----------------------------------------------------------------------------------
// CLASS DECLARATION
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// MgrImp DECLARATION
//----------------------------------------------------------------------------------
class MgrImp {
public:
    MgrImp();
    virtual~ MgrImp();

    void Initialize(const char* fontImage, const char* fontList);
    void Update();
    void Destroy();

    void GetCharacter(const char c, float& ltX, float& ltY, float& rbX, float& rbY, float& ndcSizeX, float& ndcSizeY);
    void GetTextSize(const char* text, float& ndcSizeX, float& ndcSizeY);
    int32_t GetFontTexture();

protected:
    struct FontEntry {
        char c;
        int32_t ltX;
        int32_t ltY;
        int32_t rbX;
        int32_t rbY;

        FontEntry()
        : c(' ')
        , ltX(0)
        , ltY(0)
        , rbX(0)
        , rbY(0) {
        }
    };

    int32_t m_FontTexture;
    std::map<const char, FontEntry> m_FontTbl;
};

//-----------------------------------------------------------------------------------
// CLASS DEFINITION
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// MgrImp DEFINITION
//-----------------------------------------------------------------------------------
MgrImp::MgrImp()
: m_FontTexture(-1) {
    m_FontTbl.clear();
}

MgrImp::~MgrImp() {
    m_FontTexture = -1;
    m_FontTbl.clear();
}

void MgrImp::Initialize(const char* fontImage, const char* fontList) {
    m_FontTexture = texture::Mgr::LoadTexture(fontImage);

    std::ifstream input(fontList);

    if (!input.fail()) {
        while (!input.eof()) {
            char c = ' ';
            int32_t ltX = 0, ltY = 0, rbX = 0, rbY = 0;
            input >> c >> ltX >> ltY >> rbX >> rbY;

            FontEntry entry;
            entry.c = c;
            entry.ltX = ltX;
            entry.ltY = ltY;
            entry.rbX = rbX;
            entry.rbY = rbY;

            m_FontTbl[c] = entry;
        }
    }
}

void MgrImp::Update() {
}

void MgrImp::Destroy() {
}

void MgrImp::GetCharacter(const char c, float& ltU, float& ltV, float& rbU, float& rbV, float& ndcSizeX, float& ndcSizeY) {
    render::texture::Texture* fontTex = texture::Mgr::GetTextureById(m_FontTexture);
    int32_t width = fontTex->GetWidth();
    int32_t height = fontTex->GetHeight();
    int32_t screenWidth = app::Application::GetWindowWidth();
    int32_t screenHeight = app::Application::GetWindowHeight();

    char realC = c;
    if (c == ' ') {
        realC = ',';  // If come with space, using ','
    }

    int32_t ltX = m_FontTbl[realC].ltX;
    int32_t ltY = m_FontTbl[realC].ltY;
    int32_t rbX = m_FontTbl[realC].rbX;
    int32_t rbY = m_FontTbl[realC].rbY;

    // Calculate character original size in NDC
    ndcSizeX = ((rbX - ltX) * 1.0f / screenWidth) * 2.0f;
    ndcSizeY = ((rbY - ltY) * 1.0f / screenHeight) * 2.0f;

    // Inverse Y coordinate
    ltY = height - ltY;
    rbY = height - rbY;

    ltU = ltX * 1.0f / width;
    ltV = ltY * 1.0f / height;
    rbU = rbX * 1.0f / width;
    rbV = rbY * 1.0f / height;
}

void MgrImp::GetTextSize(const char* text, float& ndcSizeX, float& ndcSizeY) {
    ndcSizeX = 0.0f, ndcSizeY = 0.0f;

    if (text) {
        for (int32_t i = 0; i < strlen(text); i++) {
            float ltU = 0.0f, ltV = 0.0f, rbU = 0.0f, rbV = 0.0f, ndcSizeXPerChar = 0.0f, ndcSizeYPerChar = 0.0f;
            if (text[i] == ' ') {
                GetCharacter(',', ltU, ltV, rbU, rbV, ndcSizeXPerChar, ndcSizeYPerChar);
            } else {
                GetCharacter(text[i], ltU, ltV, rbU, rbV, ndcSizeXPerChar, ndcSizeYPerChar);
            }

            ndcSizeX = ndcSizeX + ndcSizeXPerChar;
            if (ndcSizeYPerChar > ndcSizeY) {
                ndcSizeY = ndcSizeYPerChar;
            }
        }
    }
}

int32_t MgrImp::GetFontTexture() {
    return m_FontTexture;
}

//-----------------------------------------------------------------------------------
// Mgr DEFINITION
//-----------------------------------------------------------------------------------
void Mgr::Initialize(const char* fontImage, const char* fontList) {
    if (s_MgrImp == NULL) {
        s_MgrImp = new MgrImp;
        if (s_MgrImp != NULL) {
            s_MgrImp->Initialize(fontImage, fontList);
        } else {
            GLB_SAFE_ASSERT(false);
        }
    }
}

void Mgr::Update() {
    if (s_MgrImp != NULL) {
        s_MgrImp->Update();
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

void Mgr::GetCharacter(const char c, float& ltX, float& ltY, float& rbX, float& rbY, float& ndcSizeX, float& ndcSizeY) {
    if (s_MgrImp != NULL) {
        s_MgrImp->GetCharacter(c, ltX, ltY, rbX, rbY, ndcSizeX, ndcSizeY);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Mgr::GetTextSize(const char* text, float& ndcSizeX, float& ndcSizeY) {
    if (s_MgrImp != NULL) {
        s_MgrImp->GetTextSize(text, ndcSizeX, ndcSizeY);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

int32_t Mgr::GetFontTexture() {
    int32_t result = -1;

    if (s_MgrImp != nullptr) {
        result = s_MgrImp->GetFontTexture();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

};  // namespace material

};  // namespace render

};  // namespace glb
