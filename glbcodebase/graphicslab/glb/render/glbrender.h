//------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 10 / 28
// Brief: Draw the entire scene
//------------------------------------------------------------------
#ifndef GLB_GLBRENDER_H_
#define GLB_GLBRENDER_H_

#include "math/glbbv.h"
#include "math/glbmatrix.h"

namespace glb {

namespace scene {
class Object;
};

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
        MAX_FRUSTUM,
    };

    enum {
        PRIMARY_PERS = 0,
        SECONDARY_PERS,
        MAX_PERS,
    };

public:
    static void Initialize(int32_t width, int32_t height);
    static void InitializeAfterUserApp();
    static void Destroy();
    static void Draw();

    // Texture
    static int32_t GetBRDFMap();
    static void SetSkyLightCubeMap(const char* diffuseSkyCubeMap, const char* specularSkyCubeMap, int32_t specularPFCLOD);
    static int32_t GetDiffuseSkyLightCubeMap();
    static int32_t GetSpecularSkyLightCubeMap();
    static int32_t GetSpecularSkyPFCLOD();

    // Perspective
    static void SetPerspective(int32_t type, float fov, float aspect, float znear, float zfar);
    static math::Matrix GetPerspective(int32_t type);
    static void SetCurPerspectiveType(int32_t type);
    static int32_t GetCurPerspectiveType();
    static math::Vector GetFrustumPointInView(int32_t index);
    static float GetFarClip();
    static int32_t GetScreenWidth();
    static int32_t GetScreenHeight();

    // PSSM
    static math::Matrix GetShadowMapMatrix(int32_t index);
    static int GetCurShadowMapIndex();
    static float GetShadowSplitValue(int32_t index);
    static int32_t GetShadowMap(int32_t index);

    // Decal
    static math::Matrix GetDecalViewMatrix();
    static math::Matrix GetDecalProjMatrix();
    static int32_t GetDecalTexture();

    // HDR
    static void SetHighLightBase(float base);
    static float GetHighLightBase();
    static void SetBloomWeights(float w0, float w1, float w2, float w3);

    // Debug draw
    static void AddLine(math::Vector start, math::Vector end, math::Vector color);
    static void AddMenuMesh(math::Vector lt, math::Vector rb, math::Vector color);
    static void AddBoundBox(math::AABB bv, math::Vector color);
    static void AddText(const char* text, math::Vector pos, math::Vector color, float scale);
};

};  // namespace render

};  // namespace glb

#endif  // GLB_GLBRENDER_H_