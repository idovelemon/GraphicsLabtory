//----------------------------------------------------------------------------
// Declaration: Copyright (c), i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/08/08
// Brief: Write texture data to file
//----------------------------------------------------------------------------
#ifndef GLB_GLBTEXTUREWRITER_H_
#define GLB_GLBTEXTUREWRITER_H_

#include <stdint.h>

#include "glbtextureinfo.h"

namespace glb {

namespace util {

class TextureWriter {
public:
    static bool Write(const char* file_name, int8_t* data, int32_t width, int32_t height, int32_t pixel_format = TPFT_R8G8B8, int32_t bit_count = 24, int32_t format = TFT_DDS);
};

};  // namespace util

};  // namespace glb

#endif  // GLB_GLBTEXTUREWRITER_H_