//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 03 / 31
// Brief: Some util method
//----------------------------------------------------------------------
#ifndef GLB_GLBUTIL_H_
#define GLB_GLBUTIL_H_

#include <string>

namespace glb {

namespace util {
// Path
std::string path_get_dir(const char* path);
std::string path_get_name(const char* path);
std::string path_get_file_type(const char* path);

};  // namespace util

};  // namespace glb

#endif GLB_GLBUTIL_H_