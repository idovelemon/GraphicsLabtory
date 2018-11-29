//------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018 / 11 / 11
// Brief: Manage all font
//------------------------------------------------------------------------

#ifndef GLB_GLBFONT_H_
#define GLB_GLBFONT_H_

#include <stdint.h>

namespace glb {

namespace render {

namespace font {

//-----------------------------------------------------------------------------------
// Mgr DECLARATION
//----------------------------------------------------------------------------------
class Mgr {
public:
    static void Initialize(const char* fontImage, const char* fontList);
    static void Update();
    static void Destroy();

    static void GetCharacter(const char c, float& ltX, float& ltY, float& rbX, float& rbY, float& ndcSizeX, float& ndcSizeY);
    static void GetTextSize(const char* text, float& ndcSizeX, float& ndcSizeY);
    static int32_t GetFontTexture();
};

};  // namespace font

};  // namespace render

};  // namespace glb

#endif
