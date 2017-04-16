//-------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 11 / 23
// Brief: Print and Record log
//-------------------------------------------------------------------
#ifndef GLB_GLBLOG_H_
#define GLB_GLBLOG_H_

namespace glb {

namespace util {

namespace log {

void Initialize();
void Destroy();

void LogPrint(const char* fmt, ...);

};  // namespace log

};  // namespace util

};  // namespace glb

#endif // GLB_GLBLOG_H_
