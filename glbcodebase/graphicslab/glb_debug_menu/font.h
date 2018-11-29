//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018 / 11 / 25
// Brief: Debug Menu Font System
//----------------------------------------------------------------------

#ifndef DEBUG_FONT_H_
#define DEBUG_FONT_H_

#include <stdint.h>

namespace debugmenu2 {

class FontMgr {
public:
    static void Initialize(const char* fontImage, const char* fontList);
    static void Update();
    static void Destroy();

    static void GetCharacter(const char c, float& ltX, float& ltY, float& rbX, float& rbY, float& ndcSizeX, float& ndcSizeY);
    static void GetTextSize(const char* text, float& ndcSizeX, float& ndcSizeY);
    static int32_t GetFontTexture();
};

};

#endif  // DEBUG_FONT_H_