//------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018 / 06 / 07
// Brief: Core routine for light studio
//------------------------------------------------------------------
#include "glb_lightstudio_core.h"

#include "GL/glew.h"

using namespace glb;

ApplicationCore* ApplicationCore::s_Instance = NULL;

ApplicationCore::ApplicationCore() {
    s_Instance = this;
}

ApplicationCore::~ApplicationCore() {
    s_Instance = NULL;
}

app::ApplicationBase* ApplicationCore::Create() {
    return new ApplicationCore();
}

ApplicationCore* ApplicationCore::GetInstance() {
    return s_Instance;
}

bool ApplicationCore::Initialize() {
    return true;
}

void ApplicationCore::Update(float dt) {
    util::ProfileTime time;
    time.BeginProfile();

    glb::Input::Update();

    // Reset render target
    render::Device::SetRenderTarget(render::RenderTarget::DefaultRenderTarget());

    // SwapBuffers
    render::Device::SwapBuffer();

    GLB_CHECK_GL_ERROR;

    time.EndProfile();
    // printf("%f\n", time.GetProfileTimeInMs());
}

void ApplicationCore::Destroy() {
}