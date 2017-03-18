//-----------------------------------------------------------------------
// Declaration: Copyright (c), i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/03/18
// Brief: Implement the shader with opengl
//-----------------------------------------------------------------------
#ifndef GLB_GLBSHADER_IMP_H_
#define GLB_GLBSHADER_IMP_H_

#ifdef GLB_PLATFORM_OPENGL

#include "glbshader.h"

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
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// VertexShader::Imp DECLARATION
//----------------------------------------------------------------------------------
class VertexShader::Imp {
public:
    virtual ~Imp();
    static VertexShader::Imp* Create(const char* vertex_shader_name);
    static VertexShader::Imp* Create(std::vector<std::string> enable_macros, const char* uber_shader_file_name);

public:
    uint32_t GetHandle() const;

protected:
    Imp();

private:
    uint32_t m_VertexShader;
};

//-----------------------------------------------------------------------------------
// FragmentShader::Imp DECLARATION
//----------------------------------------------------------------------------------
class FragmentShader::Imp {
public:
    virtual ~Imp();
    static FragmentShader::Imp* Create(const char* fragment_shader_name);
    static FragmentShader::Imp* Create(std::vector<std::string> enable_macros, const char* uber_shader_file_name);

public:
    uint32_t GetHandle() const;

protected:
    Imp();

private:
    uint32_t m_FragmentShader;
};

//-----------------------------------------------------------------------------------
// Program::Imp DECLARATION
//----------------------------------------------------------------------------------
class Program::Imp {
public:
    virtual ~Imp();
    static Program::Imp* Create(const char* vertex_shader_file, const char* fragment_shader_file);
    static Program::Imp* Create(Descriptor desc);

public:
    void SetID(int32_t id);
    ShaderLayout GetShaderLayout();
    Descriptor GetShaderDescriptor();
    void* GetNativeShader();
    std::vector<uniform::UniformEntry>& GetUniforms();

protected:
    Imp();

private:
    int32_t                              m_ID;
    uint32_t                             m_Program;
    VertexShader*                        m_VertexShader;
    FragmentShader*                      m_FragmentShader;
    ShaderLayout                         m_ShaderLayout;
    Descriptor                           m_ShaderDescptor;
    std::vector<uniform::UniformEntry>   m_Uniforms;
};

};  // namespace shader

};  // namespace glb

#endif  // GLB_PLATFORM_OPENGL

#endif  // GLB_GLBSHADER_IMP_H_