//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018 / 08 / 16
// Brief: Log
//----------------------------------------------------------------------

#include <stdio.h>
#include <varargs.h>
#include <Windows.h>

namespace tdlog {

void Print(const char* format, ...) {
    char buffer[512];
    va_list args; 
    va_start(args, format);
    _vsnprintf(buffer, sizeof(buffer), format, args); 
    va_end(args);

    printf(buffer);
    OutputDebugStringA(buffer);
}

}  // namespace log