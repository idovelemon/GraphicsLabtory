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

namespace log {

static FILE* s_LogFile = NULL;
static const char s_LogFileName[] = {"debug_log.txt"};
static const int32_t kLogMaxChar = 4096;

void Initialize() {
    s_LogFile = fopen(s_LogFileName, "w+");
    GLB_SAFE_ASSERT(s_LogFile != NULL);
}

void Destroy() {
    if (s_LogFile) {
        fclose(s_LogFile);
        s_LogFile = NULL;
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void LogPrint(const char* fmt, ...) {
    char log[kLogMaxChar];
    memset(log, 0, sizeof(log));

    va_list va;
    va_start(va, fmt);
    vsprintf(log, fmt, va);
    va_end(va);

    int32_t len = strlen(log);
    if (len < kLogMaxChar) {
        log[len] = '\n';
    } else {
        log[kLogMaxChar - 1] = '\n';
    }

    if (s_LogFile) {
        fwrite(log, sizeof(char), strlen(log), s_LogFile);
    }

    OutputDebugStringA(log);
}

};  // namespace log

};  // namespace glb