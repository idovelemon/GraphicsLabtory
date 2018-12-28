//--------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016/06/19
// Brief: Define a interface to analysis the model file
// The format that supported by this interfact has showed below:
// .obj -- Exported by 3DMax2009
//--------------------------------------------------------------------
#ifndef GLB_GLBMODELFILE_H_
#define GLB_GLBMODELFILE_H_

#include <memory.h>
#include <stdint.h>
#include <stdio.h>

#include "math/glbvector.h"

#include "render/glbtexture.h"

namespace glb {

namespace util {

class MeshReader {
public:
    //----------------------------------------------------------------------------------------------
    // @brief: Extract the vertex and texture coordinate data from the model file
    // @param: meshFile The name of the mesh's file
    // @param: boundMin The bound box min
    // @param: boundMax The bound box max
    // @param: vertexBuf A pointer to recieve the vertex data. This can not be nullptr.
    // @param: texcoordBuf A pointer to recieve the texture coordinate data. This is an optional parameter
    // @param: lightMapTexCoordBuf A pointer to receive the light map texture coordinate data. This is an optional parameter
    // @param: normalBuf A pointer to recieve the normal data. This is an optional parameter
    // @param: tangentBuf A pointer to recieve the tangent data. This is an optional parameter
    // @param: binormalBuf A pointter to recieve the binormal data. This is an optional parameter
    // @return: If successfully, return the number of triangles. Otherwise retur 0.
    //----------------------------------------------------------------------------------------------
    static int32_t ExtractModelData(
        const char* meshFile, 
        math::Vector& boundMin,
        math::Vector& boundMax,
        float** vertexBuf,
        float** texcoordBuf = nullptr,
        float** lightMapTexCoordBuf = nullptr,
        float** normalBuf = nullptr,
        float** tangentBuf = nullptr,
        float** binormalBuf = nullptr
        );

    //----------------------------------------------------------------------------------------------
    // @brief: Release the buffer memory that returned by ExtractModelData method
    // @param: vertexBuf The buffer pointer hold the vertex data
    // @param: texcoordBuf The buffer pointer hold the texcoord data
    // @param: lightMapTexCoordBuf The buffer pointer hold the light map texture coordinate data
    // @param: normal_buf The buffer pointer hold the normal data
    // @param: tangent_buf The buffer pointer hold the tangent data
    // @param: binormal_buf The buffer pointer hold the binormal data
    //----------------------------------------------------------------------------------------------
    static void RelaseBuf(float** vertexBuf, float** texcoordBuf = nullptr, float** lightMapTexCoordBuf = nullptr, float** normalBuf = nullptr
        , float** tangentBuf = nullptr, float** binormalBuf = nullptr);
};

};  // namespace scene

};  // namespace glb

#endif  // GLB_GLB_MODELFILE_H_
