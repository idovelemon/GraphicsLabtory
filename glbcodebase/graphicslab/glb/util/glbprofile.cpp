//--------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemeon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 11 / 29
// Brief: Profile
//--------------------------------------------------------------
#include "glbprofile.h"

namespace glb {

namespace util {

// ProfileTime
ProfileTime::ProfileTime()
: m_BeginTime()
, m_EndTime() {
}

ProfileTime::~ProfileTime() {
}

void ProfileTime::BeginProfile() {
    QueryPerformanceCounter(&m_BeginTime);
}

void ProfileTime::EndProfile() {
    QueryPerformanceCounter(&m_EndTime);
}

float ProfileTime::GetProfileTimeInMs() {
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);

    return (m_EndTime.QuadPart - m_BeginTime.QuadPart) * 1.0f / frequency.QuadPart * 1000.0f;
}

};  // namespace util

};  // namespace glb