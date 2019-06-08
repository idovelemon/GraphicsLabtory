//-------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 11 / 23
// Brief: Print and Record log
//-------------------------------------------------------------------
#include "glblog.h"

#include <stdint.h>
#include <stdio.h>

#include <string>

#include "glbmacro.h"

namespace glb {

namespace util {

namespace log {

static FILE* s_LogFile = nullptr;
static const char s_LogFileName[] = {"debug_log.txt"};
static const int32_t kLogMaxChar = 4096;

void Initialize() {
    fopen_s(&s_LogFile, s_LogFileName, "w+");
    GLB_SAFE_ASSERT(s_LogFile != nullptr);
}

void Destroy() {
    if (s_LogFile) {
        fclose(s_LogFile);
        s_LogFile = nullptr;
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void LogPrint(const char* fmt, ...) {
    char log[kLogMaxChar];
    memset(log, 0, sizeof(log));

    va_list va;
    va_start(va, fmt);
    vsprintf_s(log, kLogMaxChar, fmt, va);
    va_end(va);

    if (s_LogFile) {
        fwrite(log, sizeof(char), strlen(log), s_LogFile);
        fflush(s_LogFile);
    }

#if _DEBUG
    OutputDebugStringA(log);
#endif
}

};  // namespace log

};  // namespace util

};  // namespace glb