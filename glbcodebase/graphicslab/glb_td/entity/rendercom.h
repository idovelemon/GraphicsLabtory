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

namespace entity {

//----------------------------------------------------------------
// Pre Declaration

class TransformCom;

//----------------------------------------------------------------
// Type Declaration

class RenderCom : public Component {
public:
    RenderCom(const char* name);
    virtual ~RenderCom();

public:
    virtual void Render(TransformCom* transform);

protected:
    int32_t     m_SceneObjID;
};

};  // namespace entity

#endif  // ENTITY_RENDERCOM_H_