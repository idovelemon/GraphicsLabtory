//----------------------------------------------------------------------------
// Declaration: Copyright (c), i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016/06/20
// Brief: Read the texture data from file.
// The supprted format has showed below:
// .bmp -- 24bit
//----------------------------------------------------------------------------
#ifndef GLB_GLBTEXTUREREADER_H_
#define GLB_GLBTEXTUREREADER_H_

#include <stdint.h>

namespace glb {

namespace util {

class TextureReader {
 public:
     //--------------------------------------------------------------------------------------------
     // @brief: Read the texture data from the files
     // @param: texture_file_name The file name of the texture
     // @param: texture_data The pointer of the memory to hold the texture data
     // @param: texture_width The with of the texture
     // @param: texture_height The height of the texture
     // @param: pixel_format The pixel format of the texture
     // @return: If failed, return 0.
     //--------------------------------------------------------------------------------------------
     static int32_t ReadTexture(const char* texture_file_name, int8_t** texture_data, int32_t& texture_width, int32_t& texture_height, int32_t& pixel_format);

     //--------------------------------------------------------------------------------------------
     // @brief: After you read the texture, call this method to release the texture buffer
     // @param: texture_data The pointer to the texture buffer
     //--------------------------------------------------------------------------------------------
     static void ReleaseData(int8_t** texture_data);
};

};  // namespace util

};  // namespace glb

#endif  // GLB_GLBTEXTUREREADER_H_