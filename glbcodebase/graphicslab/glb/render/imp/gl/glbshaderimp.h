//-----------------------------------------------------------------------
// Declaration: Copyright (c), i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/03/18
// Brief: Implement the shader with opengl
//-----------------------------------------------------------------------
#ifdef GLB_PLATFORM_OPENGL

#ifndef GLB_GLBSHADER_IMP_H_
#define GLB_GLBSHADER_IMP_H_

#include "render/glbshader.h"

namespace glb {

namespace render {

namespace shader {

//----------------------------------------------------------------------------------
static const int32_t kShaderNameMaxLength = 258;

//----------------------------------------------------------------------------------

class VertexShader::Imp {
public:
    virtual ~Imp();
    static VertexShader::Imp* Create(const char* vertex_shader_name);
    static VertexShader::Imp* Create(std::vector<std::string> enable_macros, const char* uber_shader_file_name);

public:
    const char* GetShaderName() const;
    uint32_t GetHandle() const;

protected:
    Imp();

private:
    uint32_t m_VertexShader;
    char m_ShaderName[kShaderNameMaxLength];
};

//----------------------------------------------------------------------------------

class GeometryShader::Imp {
public:
    virtual ~Imp();
    static GeometryShader::Imp* Create(const char* geometry_shader_name);

public:
    uint32_t GetHandle() const;

protected:
    Imp();

private:
    uint32_t m_GeometryShader;
};

//----------------------------------------------------------------------------------

class FragmentShader::Imp {
public:
    virtual ~Imp();
    static FragmentShader::Imp* Create(const char* fragment_shader_name);
    static FragmentShader::Imp* Create(std::vector<std::string> enable_macros, const char* uber_shader_file_name);

public:
    const char* GetShaderName() const;
    uint32_t GetHandle() const;

protected:
    Imp();

private:
    char m_ShaderName[kShaderNameMaxLength];
    uint32_t m_FragmentShader;
};


//----------------------------------------------------------------------------------

class UberProgram::Imp {
public:
    virtual ~Imp();
    static UberProgram::Imp* Create(const char* vertex_shader_file, const char* fragment_shader_file, const char* geometry_shader_file = nullptr);
    static UberProgram::Imp* Create(Descriptor desc);

public:
    void SetID(int32_t id);
    const char* GetShaderName() const;
    const char* GetVertexShaderName() const;
    const char* GetFragmentShaderName() const;
    void SetProgramType(int32_t type);
    int32_t GetProgramType();
    ShaderLayout GetShaderLayout();
    void* GetNativeShader();
    std::vector<ShaderParameter> GetProgramParameter();

    Descriptor GetShaderDescriptor();
    std::vector<uniform::UniformEntry>& GetUniforms();

protected:
    Imp();

private:
    int32_t                              m_ID;
    int32_t                              m_Type;
    uint32_t                             m_Program;
    char                                 m_ShaderName[kShaderNameMaxLength];
    VertexShader*                        m_VertexShader;
    FragmentShader*                      m_FragmentShader;
    ShaderLayout                         m_ShaderLayout;
    Descriptor                           m_ShaderDescptor;
    std::vector<ShaderParameter>         m_ShaderParameter;
    std::vector<uniform::UniformEntry>   m_Uniforms;
};

//----------------------------------------------------------------------------------

class UserProgram::Imp {
public:
    virtual ~Imp();
    static UserProgram::Imp* Create(const char* vertex_shader_file, const char* fragment_shader_file, const char* geometry_shader_file = nullptr);

public:
    void SetID(int32_t id);
    void SetProgramType(int32_t type);
    int32_t GetProgramType();
    ShaderLayout GetShaderLayout();
    void* GetNativeShader();
    std::vector<ShaderParameter> GetProgramParameter();

    int32_t GetUniformLocation(const char* uniform_name);

protected:
    Imp();

private:
    int32_t                              m_ID;
    int32_t                              m_Type;
    uint32_t                             m_Program;
    VertexShader*                        m_VertexShader;
    FragmentShader*                      m_FragmentShader;
    ShaderLayout                         m_ShaderLayout;
    std::vector<ShaderParameter>         m_ShaderParameter;
};

};  // namespace shader

};  // namespace render

};  // namespace glb

#endif  // GLB_GLBSHADER_IMP_H_

#endif  // GLB_PLATFORM_OPENGL