//-------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 09 / 26
// Brief: Scene hold all the object that need to be draw
//-------------------------------------------------------
#ifndef GLB_GLBSCENE_H_
#define GLB_GLBSCENE_H_

#include <stdint.h>

#include <vector>

#include "glblight.h"
#include "../math/glbmath.h"

namespace glb {

namespace scene {

class CameraBase;
class Model;
class Object;

//-----------------------------------------------------------------------------------
// CONSTANT VALUE
//-----------------------------------------------------------------------------------
static const int32_t kMaxLight = 8;

//-----------------------------------------------------------------------------------
// TYPE DECLARATION
//-----------------------------------------------------------------------------------
enum {
    PRIMIAY_CAM = 0,
    SECONDARY_CAM,
    MAX_CAM,
};

//-----------------------------------------------------------------------------------
// CLASS DECLARATION
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// Scene DECLARATION
//----------------------------------------------------------------------------------
class Scene {
public:
    static void Initialize();
    static void Destroy();
    static void Update();

    // Space partioning
    static math::Vector GetSceneBoundBoxMax();
    static math::Vector GetSceneBoundBoxMin();

    // Object
    static int32_t AddObject(const char* objectFile);
    static int32_t AddInstanceRenderObject(const char* objectFile, int32_t maxInstance = 256);
    static int32_t AddInstanceObject(int32_t instanceRenderObject, math::Vector pos = math::Vector(0.0f, 0.0f, 0.0f), math::Vector scale = math::Vector(1.0f, 1.0f, 1.0f), math::Vector rotate = math::Vector(0.0f, 0.0f, 0.0f));
    static int32_t AddDecalObject(const char* decalObjectFile);
    static int32_t AddObject(Model* model);
    static int32_t AddSkyObject(const char* objectFile);
    static Object* GetObjectById(int32_t objectId);
    static Object* GetSkyObject();
    static void GetAllObjects(std::vector<Object*>& objs);
    static void DestroyObject(int32_t objectId);

    // Light
    static void SetLight(Light light, int32_t id);
    static Light GetLight(int32_t id);

    // Camera
    static void SetCamera(int32_t type, CameraBase* camera);
    static CameraBase* GetCamera(int32_t type);
    static void SetCurCamera(int32_t type);
    static int32_t GetCurCameraType();
    static CameraBase* GetCurCamera();
    static math::Matrix GetViewMatrix(int32_t type);

    // Debug Draw
    static void AddBoundBox(math::Vector color);
};

};  // namespace scene

};  // namespace glb

#endif  // GLB_GLBSCENE_H_