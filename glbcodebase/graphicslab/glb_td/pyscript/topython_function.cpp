//------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 05 / 23
// Brief: Interface that will be used in python file
//------------------------------------------------------------------
#include "topython_function.h"

#include <stdio.h>

//-----------------------------------------------------------------
// Test
void TestPrintHelloWorld() {
    printf("HelloWorld!\n");
}

//-----------------------------------------------------------------
// Debug
void DebugPrint(const char* s) {
    printf(s);
}