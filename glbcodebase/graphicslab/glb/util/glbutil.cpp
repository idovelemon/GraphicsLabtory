//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 03 / 31
// Brief: Some util method
//----------------------------------------------------------------------
#include "glbutil.h"

#include <stdint.h>

namespace glb {

namespace util {

// Path
std::string path_get_dir(const char* path) {
    std::string dir("");

    if (path != NULL) {
        std::string path_s(path);
        int32_t offset0 = path_s.find_last_of("/");
        int32_t offset1 = path_s.find_last_of("\\");
        if (offset0 >= 0) {
            dir = path_s.substr(0, offset0 + 1);
        }
        if (offset1 >= 0) {
            dir = path_s.substr(0, offset1 + 1);
        }
    }

    return dir;
}

std::string path_get_name(const char* path) {
    std::string name("");

    if (path != NULL) {
        std::string path_s(path);
        int32_t offset0 = path_s.find_last_of("/");
        int32_t offset1 = path_s.find_last_of("\\");
        if (offset0 >= 0) {
            name = path_s.substr(offset0 + 2, path_s.length());
        }
        if (offset1 >= 0) {
            name = path_s.substr(offset1 + 1, path_s.length());
        }
    }

    return name;
}

};

};