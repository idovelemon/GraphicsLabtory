//--------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/06/01
// Brief: Dynamic world hold all dynamic objects
//--------------------------------------------------------------------
#ifndef DYNAMIC_DYNAMICWORLD_H_
#define DYNAMIC_DYNAMICWORLD_H_

#include <stdint.h>
#include <vector>

namespace dynamic {

//--------------------------------------------------------------------
// Pre Declaration
class DynamicObject;

//--------------------------------------------------------------------
// Type Declaration

//--------------------------------------------------------------------
class DynamicWorld {
public:
    static void Initialize();
    static void Update();
    static void Destroy();

    static int32_t AddDynamicObject(DynamicObject* object);
    static void RemoveDynamicObject(int32_t id);
    static DynamicObject* GetDynamicObject(int32_t id);

    static void CheckCollision(int32_t id, std::vector<int32_t>& coll_ids);
};

};  // namespace dynamic

#endif  // DYNAMIC_DYNAMICWORLD_H_