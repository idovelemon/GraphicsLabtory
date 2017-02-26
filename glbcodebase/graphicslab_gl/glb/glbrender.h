//------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 10 / 28
// Brief: Draw the entire scene
//------------------------------------------------------------------
#ifndef GLB_GLBRENDER_H_
#define GLB_GLBRENDER_H_

#include "glbmatrix.h"

namespace glb {

namespace render {

class Render {
public:
    enum {
        NLU = 0,  // Near-Left-Up
        NLD,      // Near-Left-Down
        NRU,      // Near-Right-Up
        NRD,      // Near-Right-Down
        FLU,      // Far-Left-Up
        FLD,      // Far-Left-Down
        FRU,      // Far-Right-Up
        FRD,      // Far-Right-Down
    };

    enum {
        PRIMARY_PERS = 0,
        SECONDARY_PERS,
        MAX_PERS,
    };

public:
    static void Initialize(int32_t width, int32_t height);
    static void Destroy();
    static void Draw();

    // Perspective
    static void SetPerspective(int32_t type, float fov, float aspect, float znear, float zfar);
    static Matrix GetPerspective(int32_t type);
    static void SetCurPerspectiveType(int32_t type);
    static int32_t GetCurPerspectiveType();
    static Vector GetFrustumPointInView(int32_t index);
    static float GetFarClip();
    static float GetScreenWidth();
    static float GetScreenHeight();
    static int32_t GetRandomRotateTex();

    // HDR
    static float GetHDRAverageLum();
    static int32_t GetHDRSceneTex();
    static float GetBloomWidth();
    static float GetBloomHeight();
    static void SetExposureLevel(float level);
    static float GetExposureLevel();
    static void SetLightAdaption(float adaption);
    static float GetLightAdaption();

    // Debug draw
    static void AddLine(Vector start, Vector end, Vector color);
};

};  // namespace render

};  // namespace glb

#endif  // GLB_GLBRENDER_H_