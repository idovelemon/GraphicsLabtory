//-----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/05/28
// Brief: Game timer hold all the timer this game using
//-----------------------------------------------------------
#ifndef TD_GAMETIMER_H_
#define TD_GAMETIMER_H_

namespace td {

class GameTimer {
public:
    static void Initialize();
    static void Update(float dt);
    static void Destroy();
    static float GetPrevGameTime();
    static float GetCurGameTime();
    static float GetFrameSpeed();
};

};  // namespace td

#endif  // TD_GAMETIMER_H_