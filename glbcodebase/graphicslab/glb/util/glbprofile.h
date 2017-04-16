//--------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemeon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 11 / 29
// Brief: Profile
//--------------------------------------------------------------
#ifndef GLB_GLBPROFILE_H_
#define GLB_GLBPROFILE_H_

#include <stdint.h>
#include <windows.h>

namespace glb {

namespace util {

//--------------------------------------------------------------
// @brief£º Used to profile how much time to take on a piece of code
//--------------------------------------------------------------
class ProfileTime {
public:
    ProfileTime();
    virtual ~ProfileTime();

    void BeginProfile();
    void EndProfile();
    float GetProfileTimeInMs();

private:
    LARGE_INTEGER   m_BeginTime;
    LARGE_INTEGER   m_EndTime;
};

};  // namespace util

};  // namespace glb

#endif  // GLB_GLBPROFILE_H_
