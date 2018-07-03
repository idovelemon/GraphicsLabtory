//----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/05/26
// Brief: Render controls how to render a entity
//----------------------------------------------------------
#ifndef ENTITY_RENDERCOM_H_
#define ENTITY_RENDERCOM_H_

#include "component.h"

#include <stdint.h>

#include <map>

#include "math/glbvector.h"

namespace entity {

//----------------------------------------------------------------
// Pre Declaration

class Entity;
class TransformCom;

//----------------------------------------------------------------
// Type Declaration

class RenderCom : public Component {
public:
    RenderCom(Entity* owner, const char* name, glb::math::Vector pos, glb::math::Vector rot, glb::math::Vector scale, bool enableInstance, int32_t maxInstanceNum);
    virtual ~RenderCom();

public:
    virtual void SetDepthTestEnable(bool enable);
    virtual void SetDrawEnable(bool enable);
    virtual glb::math::Vector GetBoundBoxMax();
    virtual glb::math::Vector GetBoundBoxMin();
    virtual void Render(TransformCom* transform);

protected:
    int32_t     m_SceneObjID;

    typedef std::map<std::string, int32_t> InstanceMap;
    static InstanceMap s_InstanceMap;
};

};  // namespace entity

#endif  // ENTITY_RENDERCOM_H_