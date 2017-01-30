//----------------------------------------------------------------------------
// Declaration: Copyright (c), i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016/06/20
// Brief: Read the texture data from file.
// The supprted format has showed below:
// .bmp -- 24bit
//----------------------------------------------------------------------------
#include "glbtexturereader.h"

#include <stdio.h>

#include <fstream>

#include <Windows.h>
#include <wingdi.h>

#include "glbmacro.h"

namespace glb {

//--------------------------------------------------------------------------------
// CONSTANT
//--------------------------------------------------------------------------------
const int32_t kMaxPostfixLen = 8;
const int32_t kBytesPerPixel = 4;
const int32_t kBmpRGBBit = 24;
const int32_t kBmpRGBABit = 32;

//--------------------------------------------------------------------------------
// DECLARATION
//--------------------------------------------------------------------------------
class TextureReaderBase {
 public:
     TextureReaderBase() {
     }

     virtual ~TextureReaderBase() {
     }

 public:
     virtual int32_t ReadTexture(const char* file_name, int8_t** texture_data, int32_t& tex_width, int32_t& tex_height) = 0;
};

class BmpTextureReader:public TextureReaderBase {
 public:
     BmpTextureReader();
     virtual ~BmpTextureReader();

 public:
     virtual int32_t ReadTexture(const char* file_name, int8_t** texture_data, int32_t& tex_width, int32_t& tex_height);
};

//--------------------------------------------------------------------------------
// DEFINITION
//--------------------------------------------------------------------------------
BmpTextureReader::BmpTextureReader() {
}

BmpTextureReader::~BmpTextureReader() {
}

int32_t BmpTextureReader::ReadTexture(const char* file_name, int8_t** texture_data, int32_t& tex_width, int32_t& tex_height) {
    int32_t result = 0;

    if (file_name != NULL && texture_data != NULL) {
        FILE* file = fopen(file_name, "rb");

        if (file) {
            BITMAPFILEHEADER file_header;
            fread(&file_header, sizeof(BITMAPFILEHEADER), 1, file);

            BITMAPINFOHEADER info_header;
            fread(&info_header, sizeof(BITMAPINFOHEADER), 1, file);

            (*texture_data) = new int8_t[info_header.biWidth * info_header.biHeight * kBytesPerPixel];
            int8_t* raw_data = new int8_t[info_header.biSizeImage];
            fread(raw_data, sizeof(int8_t), info_header.biSizeImage, file);

            if (info_header.biBitCount == kBmpRGBBit) {
                // Convert B8G8R8 to R8G8B8A8
                int32_t bpp_in_file = kBmpRGBBit / 8;
                for (int32_t i = 0; i < info_header.biHeight; i++) {
                    for (int32_t j = 0; j < info_header.biWidth; j++) {
                        int8_t b = raw_data[i * info_header.biWidth * bpp_in_file + j * bpp_in_file];
                        int8_t g = raw_data[i * info_header.biWidth * bpp_in_file + j * bpp_in_file + 1];
                        int8_t r = raw_data[i * info_header.biWidth * bpp_in_file + j * bpp_in_file + 2];
                        int8_t a = static_cast<int8_t>(0xff);
                        (*texture_data)[i * info_header.biWidth * kBytesPerPixel + j * kBytesPerPixel] = r;
                        (*texture_data)[i * info_header.biWidth * kBytesPerPixel + j * kBytesPerPixel + 1] = g;
                        (*texture_data)[i * info_header.biWidth * kBytesPerPixel + j * kBytesPerPixel + 2] = b;
                        (*texture_data)[i * info_header.biWidth * kBytesPerPixel + j * kBytesPerPixel + 3] = a;
                    }
                }
            } else if (info_header.biBitCount == kBmpRGBABit) {
                // Convert B8G8R8A8 to R8G8B8A8
                int32_t bpp_in_file = kBmpRGBABit / 8;
                for (int32_t i = 0; i < info_header.biHeight; i++) {
                    for (int32_t j = 0; j < info_header.biWidth; j++) {
                        int8_t b = raw_data[i * info_header.biWidth * bpp_in_file + j * bpp_in_file];
                        int8_t g = raw_data[i * info_header.biWidth * bpp_in_file + j * bpp_in_file + 1];
                        int8_t r = raw_data[i * info_header.biWidth * bpp_in_file + j * bpp_in_file + 2];
                        int8_t a = raw_data[i * info_header.biWidth * bpp_in_file + j * bpp_in_file + 3];
                        (*texture_data)[i * info_header.biWidth * kBytesPerPixel + j * kBytesPerPixel] = r;
                        (*texture_data)[i * info_header.biWidth * kBytesPerPixel + j * kBytesPerPixel + 1] = g;
                        (*texture_data)[i * info_header.biWidth * kBytesPerPixel + j * kBytesPerPixel + 2] = b;
                        (*texture_data)[i * info_header.biWidth * kBytesPerPixel + j * kBytesPerPixel + 3] = a;
                    }
                }
            }

            delete[] raw_data;
            raw_data = NULL;

            tex_width = info_header.biWidth;
            tex_height = info_header.biHeight;

            result = 1;

            fclose(file);
            file = NULL;

        } else {
            GLB_SAFE_ASSERT(false);
        }
    }

    return result;
}

int32_t TextureReader::ReadTexture(const char* file_name, int8_t** data, int32_t& tex_width, int32_t& tex_height) {
    int32_t result = 0;

    if (file_name != NULL) {
        // Get the postfix
        int32_t index = 0;
        int32_t file_name_len = strlen(file_name);
        for (index = 0; index < file_name_len; index++) {
            if (file_name[index] == '.') {
                break;
            }
        }

        char postfix[kMaxPostfixLen];
        memcpy(postfix, file_name + index, file_name_len - index);
        postfix[file_name_len - index] = 0;

        // Check format
        if (!strcmp(postfix, ".bmp")) {
            BmpTextureReader bmp_reader;
            result = bmp_reader.ReadTexture(file_name, data, tex_width, tex_height);
        } else {
            GLB_SAFE_ASSERT(false);
        }
    }

    return result;
}

void TextureReader::ReleaseData(int8_t** texture_data) {
    if (texture_data) {
        if (*texture_data) {
            delete[] *texture_data;
            *texture_data = NULL;
        }
    }
}

};  // namespace glb