//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 03 / 31
// Brief: Some util method
//----------------------------------------------------------------------
#include "glbutil.h"

#include <stdint.h>

#include <vector>

namespace glb {

namespace util {

// User error string
static std::vector<std::string> sUserErrorMsg;

// Path
std::string path_get_dir(const char* path) {
    std::string dir("");

    if (path != nullptr) {
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

    if (path != nullptr) {
        std::string path_s(path);
        int32_t offset0 = path_s.find_last_of("/");
        int32_t offset1 = path_s.find_last_of("\\");
        if (offset0 >= 0) {
            name = path_s.substr(offset0 + 1, path_s.length());
        }
        if (offset1 >= 0) {
            name = path_s.substr(offset1 + 1, path_s.length());
        }

        if (offset0 < 0 && offset1 < 0) {
            name = path;
        }
    }

    return name;
}

std::string path_get_file_type(const char* path) {
    std::string type("");

    if (path != nullptr) {
        std::string path_s(path);
        int32_t offset = path_s.find_last_of('.');
        if (offset >= 0) {
            type = path_s.substr(offset, path_s.length());
        }
    }

    return type;
}

// User Error
void user_error_pushmsg(const char* msg) {
    sUserErrorMsg.insert(sUserErrorMsg.begin(), std::string(msg));
}

std::string user_error_popmsg() {
    std::string result = "";

    if (!sUserErrorMsg.empty()) {
        result = sUserErrorMsg.back();
        sUserErrorMsg.pop_back();
    }

    return result;
}

};

};