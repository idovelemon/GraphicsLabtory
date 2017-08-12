//----------------------------------------------------------------------------
// Declaration: Copyright (c), i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/08/07
// Brief: DDS file format description
//----------------------------------------------------------------------------
#ifndef GLB_GLBDDSFORMAT_H_
#define GLB_GLBDDSFORMAT_H_

#include <stdint.h>

namespace glb {

namespace util {

//--------------------------------------------------------------------------------

struct DDSPixelFormat {
    int32_t size;
    int32_t flag;
    int32_t four_cc;
    int32_t rgb_bit_count;
    uint32_t red_bit_mask;
    uint32_t green_bit_mask;
    uint32_t blue_bit_mask;
    uint32_t alpha_bit_mask;
};

//--------------------------------------------------------------------------------

struct DDSCaps {
    int32_t caps_1;
    int32_t caps_2;
    int32_t reserved[2];
};

//--------------------------------------------------------------------------------

struct DDSSurfaceDesc {
    int32_t size;
    int32_t flag;
    int32_t height;
    int32_t width;
    int32_t pitch_or_linear_size;
    int32_t depth;
    int32_t mip_map_count;
    int32_t reserved[11];
    DDSPixelFormat format;
    DDSCaps caps;
    int32_t reserved_2;
};

//--------------------------------------------------------------------------------

struct DDSFile {
    int32_t magic_num;
    DDSSurfaceDesc desc;
    int8_t*     main_data;
    int8_t*     attach_data;
};

//--------------------------------------------------------------------------------

// DDSPixelFormat.flag
#define DDS_PF_RGB (0x40)

// DDSCaps.caps1
#define DDS_CAPS_TEXTURE (0x1000)

// DDSSurfaceDesc.flag
#define DDS_SD_CAPS (0x1)
#define DDS_SD_HEIGHT (0x2)
#define DDS_SD_WIDTH (0x4)
#define DDS_SD_PITCH (0x8)
#define DDS_SD_PIXELFORMAT (0x1000)

};  // namespace util

};  // namespace glb

#endif  // _GLB_GLBDDSFORMAT_H_