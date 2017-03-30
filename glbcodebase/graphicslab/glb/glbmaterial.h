//------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 06 / 26
// Brief: Material manager manage all the light materials
//------------------------------------------------------------------------
#ifndef GLB_GLBMATERIAL_H_
#define GLB_GLBMATERIAL_H_

#include <memory.h>
#include <stdint.h>
#include <string.h>

#include "glbvector.h"

namespace glb {

namespace material {

//-----------------------------------------------------------------------------------
// CONSTANT VALUE
//-----------------------------------------------------------------------------------
const int32_t kMaxTexUnits = 8;

//-----------------------------------------------------------------------------------
// TYPE DECLARATION
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// CLASS DECLARATION
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// Material DECLARATION
//----------------------------------------------------------------------------------
class Material {
 public:
     int32_t id;
     glb::Vector ambient;
     glb::Vector diffuse;
     glb::Vector emission;
     glb::Vector specular;
     float specular_pow;
     char mat_name[128];

     Material()
     : id(0)
     , ambient(0.0f, 0.0f, 0.0f)
     , diffuse(0.0f, 0.0f, 0.0f)
     , emission(0.0f, 0.0f, 0.0f)
     , specular(0.0f, 0.0f, 0.0f)
     , specular_pow(0.0f) {
         memset(mat_name, 0, sizeof(mat_name));
         memcpy(mat_name, "default", sizeof("default"));
     }

     Material(const char* name)
     : id(0)
     , ambient(0.0f, 0.0f, 0.0f)
     , diffuse(0.0f, 0.0f, 0.0f)
     , emission(0.0f, 0.0f, 0.0f)
     , specular(0.0f, 0.0f, 0.0f)
     , specular_pow(0.0f) {
         memset(mat_name, 0, sizeof(mat_name));
         memcpy(mat_name, name, strlen(name));
     }
};

class Mgr {
 public:
     static void Initialize();
     static void Destroy();

     //-------------------------------------------------------------------
     // @brief: Add the material to manager
     // @param: mat The material
     // @return: If successfully, return the id of the material.
     // Otherwise, return 0.
     //-------------------------------------------------------------------
     static int32_t AddMaterial(Material mat);

     //-------------------------------------------------------------------
     // @brief: Change the specific material
     // @param: mat The new material attributes
     // @param: id The id of the material
     //-------------------------------------------------------------------
     static void ChangeMaterial(Material mat, int32_t id);

     //-------------------------------------------------------------------
     // @brief: Get the material from manager
     // @param: id The id of the material
     // @return: If succesffuly, return the material.
     // Otherwise, return a material whose id is 0.
     //-------------------------------------------------------------------
     static const Material& GetMaterial(int32_t id);

     //-------------------------------------------------------------------
     // @brief: Get the total material number
     //-------------------------------------------------------------------
     static int32_t GetMaterialCount();
};

};  // namespace material

};  // namespace glb

#endif  // GLB_GLBMATERIAL_H_