//------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/05/25
// Brief: Script component update entity's AI
//------------------------------------------------------------
#ifndef ENTITY_SCRIPTCOM_H_
#define ENTITY_SCRIPTCOM_H_

#include "component.h"

#include <stdint.h>

namespace entity {

//-------------------------------------------------------
// Pre Declaration

class Entity;


//-------------------------------------------------------
// Type Declaration

class ScriptCom : public Component {
public:
    ScriptCom(Entity* owner, const char* file);
    virtual ~ScriptCom();

public:
    virtual void Update(float dt);

protected:
    static const int32_t kScriptFileNameMaxLen = 128;
    char    m_ScriptFile[kScriptFileNameMaxLen];
};

};  // namespace entity

#endif  // ENTITY_SCRIPTCOM_H_