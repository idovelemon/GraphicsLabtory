//-------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018/08/30
// Brief: Obstacle's updater
//-------------------------------------------------------------
#include "../entity.h"

#include <assert.h>

#include "../../gametimer.h"
#include "../../pyscript/pyscriptmgr.h"
#include "../datacom.h"
#include "../transformcom.h"
#include "updater.h"

#include "glb.h"

namespace entity {

void EntityObstacleSpikeUpdater(Entity* entity) {
    TransformCom* transform = reinterpret_cast<TransformCom*>(entity->GetComponent(CT_TRANSFORM));
    if (!transform) {
        assert(false);
        return;
    }

    DataCom* data = reinterpret_cast<DataCom*>(entity->GetComponent(CT_DATA));
    if (!data) {
        assert(false);
        return;
    }

    std::string state = data->GetDataString("State");
    float totalTime = data->GetDataFloat("TotalTime");
    float curTime = data->GetDataFloat("CurTime");
    float startPosY = data->GetDataFloat("StartY");
    float endPosY = data->GetDataFloat("EndY");
    float extraSpeed = data->GetDataFloat("ExtraSpeed");
    float extraSpeedTimer = data->GetDataFloat("ExtraSpeedTimer");

    glb::math::Vector pos = transform->GetPos();

    auto SinSmooth = [](float a, float b, float ratio) {
        return a + (b - a) * sin(3.14159f / 2.0f * ratio);
    };

    curTime = curTime + td::GameTimer::GetFrameSpeed() * extraSpeed;
    extraSpeedTimer = extraSpeedTimer - td::GameTimer::GetFrameSpeed();
    if (extraSpeedTimer <= 0.0f) {
        extraSpeedTimer = 0.0f;
        extraSpeed = 1.0f;
    }

    if (!state.compare("up")) {
        // In up stage
        pos.y = SinSmooth(startPosY, endPosY, curTime / totalTime);
        if (curTime >= totalTime) {
            curTime = 0.0f;
            data->SetDataFloat("TotalTime", pyscript::PyScriptMgr::GetValueF("SPIKE_DOWN_TIME"));
            data->SetDataString("State", "down");
        }
    } else if (!state.compare("down")) {
        // In down stage
        pos.y = SinSmooth(endPosY, startPosY, curTime / totalTime);
        if (curTime >= totalTime) {
            curTime = 0.0f;
            data->SetDataFloat("TotalTime", pyscript::PyScriptMgr::GetValueF("SPIKE_UP_TIME"));
            data->SetDataString("State", "up");
        }
    } else {
        assert(false);  // Unknown state
    }

    data->SetDataFloat("CurTime", curTime);
    data->SetDataFloat("ExtraSpeed", extraSpeed);
    data->SetDataFloat("ExtraSpeedTimer", extraSpeedTimer);

    transform->SetPos(pos);
}

void EntityObstaclePushRockUpdater(Entity* entity) {
    TransformCom* transform = reinterpret_cast<TransformCom*>(entity->GetComponent(CT_TRANSFORM));
    if (!transform) {
        assert(false);
        return;
    }

    DataCom* data = reinterpret_cast<DataCom*>(entity->GetComponent(CT_DATA));
    if (!data) {
        assert(false);
        return;
    }

    glb::math::Vector oriPos;
    std::string state = data->GetDataString("State");
    float totalTime = data->GetDataFloat("TotalTime");
    float curTime = data->GetDataFloat("CurTime");
    float startPosZ = data->GetDataFloat("StartZ");
    float endPosZ = data->GetDataFloat("EndZ");
    float extraSpeed = data->GetDataFloat("ExtraSpeed");
    float extraSpeedTimer = data->GetDataFloat("ExtraSpeedTimer");
    oriPos.x = data->GetDataFloat("OriX");
    oriPos.y = data->GetDataFloat("OriY");
    oriPos.z = data->GetDataFloat("OriZ");

    glb::math::Vector pos = transform->GetPos();
    glb::math::Vector rot = transform->GetRotate();
    glb::math::Matrix mat = glb::math::Matrix::CreateRotateXYZMatrix(rot.x, rot.y, rot.z);
    glb::math::Vector moveDir = mat * glb::math::Vector(0.0f, 0.0f, 1.0f);

    auto SinSmooth = [](float a, float b, float ratio) {
        return a + (b - a) * sin(3.14159f / 2.0f * ratio);
    };

    curTime = curTime + td::GameTimer::GetFrameSpeed() * extraSpeed;
    extraSpeedTimer = extraSpeedTimer - td::GameTimer::GetFrameSpeed();
    if (extraSpeedTimer <= 0.0f) {
        extraSpeedTimer = 0.0f;
        extraSpeed = 1.0f;
    }

    if (!state.compare("push")) {
        // In up stage
        pos = oriPos + moveDir * SinSmooth(startPosZ, endPosZ, curTime / totalTime);
        if (curTime >= totalTime) {
            curTime = 0.0f;
            data->SetDataFloat("TotalTime", pyscript::PyScriptMgr::GetValueF("PUSHROCK_PUSH_TIME"));
            data->SetDataString("State", "back");
        }
    } else if (!state.compare("back")) {
        // In down stage
        pos = oriPos + moveDir * SinSmooth(endPosZ, startPosZ, curTime / totalTime);
        if (curTime >= totalTime) {
            curTime = 0.0f;
            data->SetDataFloat("TotalTime", pyscript::PyScriptMgr::GetValueF("PUSHROCK_BACK_TIME"));
            data->SetDataString("State", "push");
        }
    } else {
        assert(false);  // Unknown state
    }

    data->SetDataFloat("CurTime", curTime);
    data->SetDataFloat("ExtraSpeed", extraSpeed);
    data->SetDataFloat("ExtraSpeedTimer", extraSpeedTimer);

    transform->SetPos(pos);
}

void EntityObstacleRotateRockUpdater(Entity* entity) {
    TransformCom* transform = reinterpret_cast<TransformCom*>(entity->GetComponent(CT_TRANSFORM));
    if (!transform) {
        assert(false);
        return;
    }

    DataCom* data = reinterpret_cast<DataCom*>(entity->GetComponent(CT_DATA));
    if (!data) {
        assert(false);
        return;
    }

    float extraSpeed = data->GetDataFloat("ExtraSpeed");

    glb::math::Vector rot = transform->GetRotate();
    rot.y = rot.y + pyscript::PyScriptMgr::GetValueF("ROTATEROCK_ROTATE_SPEED") * td::GameTimer::GetFrameSpeed() * extraSpeed;
    transform->SetRotate(rot);

    float extraSpeedTimer = data->GetDataFloat("ExtraSpeedTimer");
    extraSpeedTimer = extraSpeedTimer - td::GameTimer::GetFrameSpeed();
    if (extraSpeedTimer <= 0.0f) {
        extraSpeedTimer = 0.0f;
        extraSpeed = 1.0f;
    }

    data->SetDataFloat("ExtraSpeed", extraSpeed);
    data->SetDataFloat("ExtraSpeedTimer", extraSpeedTimer);
}

};  // namespace entity