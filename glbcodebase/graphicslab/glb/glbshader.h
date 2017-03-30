//-------------------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 06 / 05
// Brief: shader::Program is a class that deal with shader in opengl
//-------------------------------------------------------------------------------------
#ifndef GLB_GLBSHADERPROGRAM_H_
#define GLB_GLBSHADERPROGRAM_H_

#include <stdint.h>
#include <vector>

#include "glbstructs.h"
#include "glbuniform.h"

namespace glb {

namespace shader {

enum {
    GLB_COLOR_IN_VERTEX = 0,
    GLB_NORMAL_IN_VERTEX,
    GLB_TANGENT_IN_VERTEX,
    GLB_BINORMAL_IN_VERTEX,
    GLB_TEXCOORD_IN_VERTEX,
    GLB_ENABLE_LIGHTING,
    GLB_ENABLE_SHADOW,
    GLB_USE_PARALLEL_LIGHT,
    GLB_ENABLE_DIFFUSE_TEX,
    GLB_ENABLE_ALPHA_TEX,
    GLB_ENABLE_NORMAL_TEX,
    GLB_ENABLE_AO,
    GLB_ENABLE_REFLECT_TEX,
};

static const char* kEnableMacros[] = {
    "#define GLB_COLOR_IN_VERTEX\n",
    "#define GLB_NORMAL_IN_VERTEX\n",
    "#define GLB_TANGENT_IN_VERTEX\n",
    "#define GLB_BINORMAL_IN_VERTEX\n",
    "#define GLB_TEXCOORD_IN_VERTEX\n",
    "#define GLB_ENABLE_LIGHTING\n",
    "#define GLB_ENABLE_SHADOW\n",
    "#define GLB_USE_PARALLEL_LIGHT\n",
    "#define GLB_ENABLE_DIFFUSE_TEX\n",
    "#define GLB_ENABLE_ALPHA_TEX\n",
    "#define GLB_ENABLE_NORMAL_TEX\n",
    "#define GLB_ENABLE_AO\n",
    "#define GLB_ENABLE_REFLECT_TEX\n",
};

class Descriptor {
public:
    static const int32_t kFlagBitNum = 128;

public:
    Descriptor();
    virtual ~Descriptor();

public:
    virtual void SetFlag(int32_t flag_bit, bool enable);
    virtual bool GetFlag(int32_t flag_bit);
    virtual bool Equal(const Descriptor& desc);
    virtual std::string GetString();

private:
    char m_ShaderFlag[16];  // 128 bit flag
};

class VertexShader {
public:
    virtual ~VertexShader();
    static VertexShader* Create(const char* vertex_shader_name);
    static VertexShader* Create(std::vector<std::string> enable_macros, const char* uber_shader_file_name);

public:
    uint32_t GetHandle() const;

protected:
    VertexShader();

private:
    class Imp;
    Imp*    m_Imp;
};

class GeometryShader {
public:
    virtual ~GeometryShader();
    static GeometryShader* Create(const char* geometry_shader_name);

public:
    uint32_t GetHandle() const;

protected:
    GeometryShader();

private:
    class Imp;
    Imp*    m_Imp;
};

class FragmentShader {
public:
    virtual ~FragmentShader();
    static FragmentShader* Create(const char* fragment_shader_name);
    static FragmentShader* Create(std::vector<std::string> enable_macros, const char* uber_shader_file_name);

public:
    uint32_t GetHandle() const;

protected:
    FragmentShader();

private:
    class Imp;
    Imp*    m_Imp;
};

class Program {
public:
    virtual ~Program();
    static Program* Create(const char* vertex_shader_file, const char* fragment_shader_file, const char* geometry_shader = NULL);
    static Program* Create(Descriptor desc);

public:
    void SetID(int32_t id);
    ShaderLayout GetShaderLayout();
    Descriptor GetShaderDescriptor();
    void* GetNativeShader();
    std::vector<uniform::UniformEntry>& GetUniforms();

protected:
    Program();

    //--------------------------------------------------------------------------------------------
    // @brief: Get vertex layout attribute type according to the attribute name
    // @param: If failed, return VA_UNKNOWN. Otherwise return VertexAttribute
    //--------------------------------------------------------------------------------------------
    static VertexAttribute GetVertexAttribute(const char* attribute_name);

private:
    class Imp;
    Imp*    m_Imp;
};

class Mgr {
public:
    static void Initialize();
    static void Destroy();

public:
    static void SetCurShader(int32_t cur_shader_id);
    static int32_t GetCurShader();
    static int32_t AddShader(const char* vertex_shader_file, const char* fragment_shader_file, const char* geometry_shader_file = NULL);
    static Program* GetShader(int32_t shader_id);
    static int32_t GetShader(Descriptor desc);
};

};  // namespace shader

};  // namespace glb

#endif  // GLB_GLBSHADERPROGRAM_H_
