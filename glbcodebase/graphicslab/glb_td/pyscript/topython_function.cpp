//------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 05 / 23
// Brief: Interface that will be used in python file
//------------------------------------------------------------------
#include "topython_function.h"

#include <stdio.h>
#include <stdint.h>

#include "scene/glbobject.h"
#include "scene/glbscene.h"
#include "util/glbmacro.h"

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

//-----------------------------------------------------------------
// Object
int ObjectAddObject(const char* name) {
    int object = -1;

    if (name != NULL) {
        object = glb::scene::Scene::AddObject(name);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return object;
}

void ObjectSetPos(int id, float x, float y, float z) {
    glb::scene::Object* obj = glb::scene::Scene::GetObjectById(id);

    if (obj != NULL) {
        obj->SetPos(glb::math::Vector(x, y, z));
    } else {
        GLB_SAFE_ASSERT(false);
    }
}