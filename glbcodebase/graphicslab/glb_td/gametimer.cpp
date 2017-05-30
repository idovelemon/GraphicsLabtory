//-----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/05/28
// Brief: Game timer hold all the timer this game using
//-----------------------------------------------------------
#include "gametimer.h"

#include "util/glbmacro.h"

namespace td {

//---------------------------------------------------------------------
// Pre Declaration

class GameTimerImp;
static GameTimerImp* s_GameTimerImp = NULL;

//---------------------------------------------------------------------
// Type Declaration

//---------------------------------------------------------------------
class GameTimerImp {
public:
    GameTimerImp();
    virtual ~GameTimerImp();

public:
    void Initialize();
    void Update(float dt);
    void Destroy();
    float GetPrevGameTime() const;
    float GetCurGameTime() const;
    float GetFrameSpeed() const;

protected:
    float       m_PrevTime;
    float       m_CurTime;
    float       m_FrameSpeed;
};

//---------------------------------------------------------------------
// Type Definition

//---------------------------------------------------------------------
GameTimerImp::GameTimerImp()
: m_PrevTime(0.0f)
, m_CurTime(0.0f)
, m_FrameSpeed(0.0f) {
}

GameTimerImp::~GameTimerImp() {
}

void GameTimerImp::Initialize() {
}

void GameTimerImp::Update(float dt) {
    m_PrevTime = m_CurTime;
    m_CurTime += dt;
    m_FrameSpeed = dt;
}

void GameTimerImp::Destroy() {
}

float GameTimerImp::GetPrevGameTime() const {
    return m_PrevTime;
}

float GameTimerImp::GetCurGameTime() const {
    return m_CurTime;
}

float GameTimerImp::GetFrameSpeed() const {
    return m_FrameSpeed;
}

//---------------------------------------------------------------------
void GameTimer::Initialize() {
    if (s_GameTimerImp == NULL) {
        s_GameTimerImp = new GameTimerImp;
        if (s_GameTimerImp != NULL) {
            s_GameTimerImp->Initialize();
        }
    }
}

void GameTimer::Update(float dt) {
    if (s_GameTimerImp != NULL) {
        s_GameTimerImp->Update(dt);
    }
}

void GameTimer::Destroy() {
    if (s_GameTimerImp != NULL) {
        s_GameTimerImp->Destroy();
        GLB_SAFE_DELETE(s_GameTimerImp);
    }
}

float GameTimer::GetPrevGameTime() {
    float dt = 0.0f;

    if (s_GameTimerImp != NULL) {
        dt = s_GameTimerImp->GetPrevGameTime();
    }

    return dt;
}

float GameTimer::GetCurGameTime() {
    float dt = 0.0f;

    if (s_GameTimerImp != NULL) {
        dt = s_GameTimerImp->GetCurGameTime();
    }

    return dt;
}

float GameTimer::GetFrameSpeed() {
    float dt = 0.0f;

    if (s_GameTimerImp != NULL) {
        dt = s_GameTimerImp->GetFrameSpeed();
    }

    return dt;
}

};  // namespace td