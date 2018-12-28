//-----------------------------------------------------------------------
// Declaration: Copyright (c), i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/03/19
// Brief: Implement the shader with dx11
//-----------------------------------------------------------------------
#ifdef GLB_PLATFORM_DX11

#include "glbshaderimp.h"

#include <memory.h>
#include <stdio.h>
#include <string.h>

#include "glblog.h"
#include "glbmacro.h"
#include "glbuniform.h"
#include "glbvector.h"

namespace glb {

namespace shader {

//-----------------------------------------------------------------------------------
// CONSTANT VALUE
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// TYPE DECLARATION
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// CLASS DECLARATION
//-----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// HELP METHOD
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// CLASS DEFINITION
//-----------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// VertexShader::Imp DEFINITION
//--------------------------------------------------------------------------------------
VertexShader::Imp::Imp() {
}

VertexShader::Imp::~Imp() {
}

VertexShader::Imp* VertexShader::Imp::Create(const char* vertex_shader_name) {
    VertexShader::Imp* result = nullptr;

    return result;
}

VertexShader::Imp* VertexShader::Imp::Create(std::vector<std::string> enable_macros, const char* uber_shader_file_name) {
    VertexShader::Imp* result = nullptr;

    return result;
}

uint32_t VertexShader::Imp::GetHandle() const {
    return 0;
}

//--------------------------------------------------------------------------------------
// FragmentShader::Imp DEFINITION
//--------------------------------------------------------------------------------------
FragmentShader::Imp::Imp() {
}

FragmentShader::Imp::~Imp() {
}

FragmentShader::Imp* FragmentShader::Imp::Create(const char* fragment_shader_name) {
    FragmentShader::Imp* result = nullptr;

    return result;
}

FragmentShader::Imp* FragmentShader::Imp::Create(std::vector<std::string> enable_macros, const char* uber_shader_file_name) {
    FragmentShader::Imp* result = nullptr;

    return result;
}

uint32_t FragmentShader::Imp::GetHandle() const {
    return 0;
}

//-----------------------------------------------------------------------------------
// Program::Imp DEFINITION
//-----------------------------------------------------------------------------------
Program::Imp::Imp()
: m_ID(-1)
, m_VertexShader(0)
, m_FragmentShader(0)
, m_ShaderLayout()
, m_ShaderDescptor() {
    memset(&m_ShaderLayout, 0, sizeof(m_ShaderLayout));
}

Program::Imp::~Imp() {
    if (m_VertexShader != nullptr) {
        GLB_SAFE_DELETE(m_VertexShader);
    }

    if (m_FragmentShader != nullptr) {
        GLB_SAFE_DELETE(m_FragmentShader);
    }
}

Program::Imp* Program::Imp::Create(const char* vertex_shader_file, const char* fragment_shader_file) {
    Program::Imp* shader_program = nullptr;
    VertexShader* vertex_shader = VertexShader::Create(vertex_shader_file);
    FragmentShader* fragment_shader = FragmentShader::Create(fragment_shader_file);

    return shader_program;
}

Program::Imp* Program::Imp::Create(Descriptor desc) {
    Program::Imp* shader_program = nullptr;

    std::vector<std::string> enable_macros;
    enable_macros.push_back("#version 330 core\n");
    for (int32_t i = 0; i < Descriptor::kFlagBitNum; i++) {
        if (desc.GetFlag(i)) {
            enable_macros.push_back(std::string(kEnableMacros[i]));
        }
    }

    VertexShader* vs = VertexShader::Create(enable_macros, "..\\glb\\shader\\uber.vs");
    FragmentShader* fs = FragmentShader::Create(enable_macros, "..\\glb\\shader\\uber.ps");

    return shader_program;
}

void Program::Imp::SetID(int32_t shader_id) {
    m_ID = shader_id;
}

ShaderLayout Program::Imp::GetShaderLayout() {
    return m_ShaderLayout;
}

Descriptor Program::Imp::GetShaderDescriptor() {
    return m_ShaderDescptor;
}

void* Program::Imp::GetNativeShader() {
    return nullptr;
}

std::vector<uniform::UniformEntry>& Program::Imp::GetUniforms() {
    return m_Uniforms;
}

};  // namespace shader

};  // namespace glb

#endif  // GLB_PLATFORM_DX11