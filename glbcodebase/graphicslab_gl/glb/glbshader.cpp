//-------------------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 06 / 05
// Brief: shader::Program is a class that deal with shader in opengl
//-------------------------------------------------------------------------------------
#include "glbshader.h"

#include <memory.h>
#include <stdio.h>
#include <string.h>

#include <GL\glew.h>
#include <GL\GL.h>

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
class MgrImp;
static MgrImp* s_MgrImp = NULL;

//-----------------------------------------------------------------------------------
// CLASS DECLARATION
//-----------------------------------------------------------------------------------
class MgrImp {
public:
    MgrImp();
    virtual ~MgrImp();

public:
    void Initialize();
    void Destroy();

public:
    void SetCurShader(int32_t cur_shader_id);
    int32_t GetCurShader();
    int32_t AddShader(const char* vertex_shader_file, const char* fragment_shader_file);
    Program* GetShader(int32_t shader_id);
    int32_t GetShader(Descriptor desc);

private:
    std::vector<Program*> m_ShaderDataBase;
    int32_t                     m_CurShader;
};

//----------------------------------------------------------------------------------
// HELP METHOD
//----------------------------------------------------------------------------------
char* glbLoadShaderBufferFromFile(const char* file_name) {
    char* shader_str = NULL;
    if (file_name != NULL) {
        // @TD:Must use rb mode to read shader string, otherwise it will encounter some weird compile error.
        // Don't know why now.
        FILE* shader_file = fopen(file_name, "rb");

        if (shader_file != NULL) {
            fseek(shader_file, 0, SEEK_END);
            int32_t size = ftell(shader_file);

            shader_str = new char[size];
            if (shader_str != NULL) {
                memset(shader_str, 0, size);

                fseek(shader_file, 0, SEEK_SET);
                fread(shader_str, sizeof(char), size, shader_file);
            } else {
                GLB_SAFE_ASSERT(false);
            }
        } else {
            char buffer[64];
            sprintf(buffer, "Do not exist shader file:%s", file_name);
            GLB_SAFE_ASSERT(false);
        }

        if (shader_file != NULL) {
            fclose(shader_file);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return shader_str;
}


void glbReleaseBuffer(char** buffer) {
    GLB_SAFE_ASSERT(buffer != NULL && *buffer != NULL);
    if (buffer != NULL && *buffer != NULL) {
        delete[] (*buffer);
        (*buffer) = NULL;
    }
}

//---------------------------------------------------------------------------------------------------
// CLASS DEFINITION
//---------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// VertexShader DEFINITION
//-----------------------------------------------------------------------------------
VertexShader::VertexShader()
:m_VertexShader(0) {
}

VertexShader::~VertexShader() {
    if (m_VertexShader != 0) {
        glDeleteShader(m_VertexShader);
        m_VertexShader = NULL;
    }
}

VertexShader* VertexShader::Create(const char* vertex_shader_name) {
    VertexShader* result = NULL;

    if (vertex_shader_name != NULL) {
        GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

        // Load the shader program string from file
        char* shader_str = glbLoadShaderBufferFromFile(vertex_shader_name);

        if (shader_str != NULL) {
            GLint len = static_cast<GLint>(strlen(shader_str));
            glShaderSource(vertex_shader, 1, const_cast<const GLchar**>(&shader_str), &len);
            glCompileShader(vertex_shader);

            GLint success = 0;
            glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
            if (success == 0) {
                GLchar info_log[256];
                glGetShaderInfoLog(vertex_shader, sizeof(info_log), NULL, info_log);
                printf(reinterpret_cast<char*>(info_log));
                GLB_SAFE_ASSERT(false);
            } else {
                result = new VertexShader();
                if (result != NULL) {
                    result->m_VertexShader = vertex_shader;
                } else {
                    GLB_SAFE_ASSERT(false);
                }
            }

            glbReleaseBuffer(&shader_str);
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

VertexShader* VertexShader::Create(std::vector<std::string> enable_macros, const char* uber_shader_file_name) {
    VertexShader* result = NULL;

    if (uber_shader_file_name != NULL) {
        GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

        // Load the shader program string from file
        char* shader_str = glbLoadShaderBufferFromFile(uber_shader_file_name);

        if (shader_str != NULL) {
            // Gather lenght information
            GLint total_len = 0;
            GLint* lens_array = new GLint[enable_macros.size() + 1];
            for (int32_t i = 0; i < static_cast<int32_t>(enable_macros.size()); i++) {
                lens_array[i] = enable_macros[i].length() + 1;
                total_len += lens_array[i];
            }
            lens_array[enable_macros.size()] = strlen(shader_str) + 1;
            total_len += lens_array[enable_macros.size()];

            // Gather shader string array
            GLchar** shader_buf = new GLchar*[enable_macros.size() + 1];
            for (int32_t i = 0; i < static_cast<int32_t>(enable_macros.size()); i++) {
                shader_buf[i] = new GLchar[lens_array[i]];
                memcpy(shader_buf[i], enable_macros[i].c_str(), enable_macros[i].length());
                shader_buf[i][lens_array[i] - 1] = '\0';
            }
            shader_buf[enable_macros.size()] = new GLchar[lens_array[enable_macros.size()]];
            memcpy(shader_buf[enable_macros.size()], shader_str, lens_array[enable_macros.size()]);
            shader_buf[enable_macros.size()][lens_array[enable_macros.size()] - 1] = '\0';

            // Pass shader string
            glShaderSource(vertex_shader, enable_macros.size() + 1, const_cast<const GLchar**>(shader_buf), lens_array);
            glCompileShader(vertex_shader);

            GLint success = 0;
            glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
            if (success == 0) {
                GLchar info_log[256];
                glGetShaderInfoLog(vertex_shader, sizeof(info_log), NULL, info_log);
                log::LogPrint(info_log);
                GLB_SAFE_ASSERT(false);
            } else {
                result = new VertexShader();
                if (result != NULL) {
                    result->m_VertexShader = vertex_shader;
                } else {
                    GLB_SAFE_ASSERT(false);
                }
            }

            for (int32_t i = 0; i < static_cast<int32_t>(enable_macros.size()) + 1; i++) {
                GLB_SAFE_DELETE_ARRAY(shader_buf[i]);
            }
            GLB_SAFE_DELETE_ARRAY(shader_buf);
            GLB_SAFE_DELETE_ARRAY(lens_array);
            glbReleaseBuffer(&shader_str);
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

uint32_t VertexShader::GetHandle() const {
    return m_VertexShader;
}

//-----------------------------------------------------------------------------------
// FragmentShader DEFINITION
//-----------------------------------------------------------------------------------
FragmentShader::FragmentShader()
: m_FragmentShader(0) {
}

FragmentShader::~FragmentShader() {
    if (m_FragmentShader != 0) {
        glDeleteShader(m_FragmentShader);
        m_FragmentShader = NULL;
    }
}

FragmentShader* FragmentShader::Create(const char* fragment_shader_name) {
    FragmentShader* result = NULL;

    if (fragment_shader_name != NULL) {
        GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

        // Load the shader program string from file
        char* shader_str = glbLoadShaderBufferFromFile(fragment_shader_name);

        if (shader_str != NULL) {
            GLint len = static_cast<GLint>(strlen(shader_str));
            glShaderSource(fragment_shader, 1, const_cast<const GLchar**>(&shader_str), &len);
            glCompileShader(fragment_shader);

            GLint success = 0;
            glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
            if (success == 0) {
                GLchar info_log[256];
                glGetShaderInfoLog(fragment_shader, sizeof(info_log), NULL, info_log);
                printf(reinterpret_cast<char*>(info_log));
                char err[128];
                sprintf_s(err, sizeof(err), "Compile shader failed:%s", fragment_shader_name);
                GLB_SAFE_ASSERT(false);
            } else {
                result = new FragmentShader();
                if (result != NULL) {
                    result->m_FragmentShader = fragment_shader;
                } else {
                    GLB_SAFE_ASSERT(false);
                }
            }

            glbReleaseBuffer(&shader_str);
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

FragmentShader* FragmentShader::Create(std::vector<std::string> enable_macros, const char* uber_shader_file_name) {
    FragmentShader* result = NULL;

    if (uber_shader_file_name != NULL) {
        GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

        // Load the shader program string from file
        char* shader_str = glbLoadShaderBufferFromFile(uber_shader_file_name);

        if (shader_str != NULL) {
            // Gather lenght information
            GLint total_len = 0;
            GLint* lens_array = new GLint[enable_macros.size() + 1];
            for (int32_t i = 0; i < static_cast<int32_t>(enable_macros.size()); i++) {
                lens_array[i] = enable_macros[i].length() + 1;
                total_len += lens_array[i];
            }
            lens_array[enable_macros.size()] = strlen(shader_str) + 1;
            total_len += lens_array[enable_macros.size()];

            // Gather shader string array
            GLchar** shader_buf = new GLchar*[enable_macros.size() + 1];
            for (int32_t i = 0; i < static_cast<int32_t>(enable_macros.size()); i++) {
                shader_buf[i] = new GLchar[lens_array[i]];
                memcpy(shader_buf[i], enable_macros[i].c_str(), enable_macros[i].length());
                shader_buf[i][lens_array[i] - 1] = '\0';
            }
            shader_buf[enable_macros.size()] = new GLchar[lens_array[enable_macros.size()]];
            memcpy(shader_buf[enable_macros.size()], shader_str, lens_array[enable_macros.size()]);
            shader_buf[enable_macros.size()][lens_array[enable_macros.size()] - 1] = '\0';

            // Pass shader string
            glShaderSource(fragment_shader, enable_macros.size() + 1, const_cast<const GLchar**>(shader_buf), lens_array);
            glCompileShader(fragment_shader);

            GLint success = 0;
            glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
            if (success == 0) {
                GLchar info_log[256];
                glGetShaderInfoLog(fragment_shader, sizeof(info_log), NULL, info_log);
                printf(reinterpret_cast<char*>(info_log));
                char err[128];
                sprintf_s(err, sizeof(err), "Compile shader failed:%s", uber_shader_file_name);
                GLB_SAFE_ASSERT(false);
            } else {
                result = new FragmentShader();
                if (result != NULL) {
                    result->m_FragmentShader = fragment_shader;
                } else {
                    GLB_SAFE_ASSERT(false);
                }
            }

            for (int32_t i = 0; i < static_cast<int32_t>(enable_macros.size()) + 1; i++) {
                GLB_SAFE_DELETE_ARRAY(shader_buf[i]);
            }
            GLB_SAFE_DELETE_ARRAY(shader_buf);
            GLB_SAFE_DELETE_ARRAY(lens_array);
            glbReleaseBuffer(&shader_str);
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

uint32_t FragmentShader::GetHandle() const {
    return m_FragmentShader;
}

//-----------------------------------------------------------------------------------
// Program DEFINITION
//-----------------------------------------------------------------------------------
Program::Program()
: m_ID(-1)
, m_Program(0)
, m_VertexShader(0)
, m_FragmentShader(0)
, m_ShaderLayout()
, m_ShaderDescptor() {
    memset(&m_ShaderLayout, 0, sizeof(m_ShaderLayout));
}

Program::~Program() {
    if (m_VertexShader != NULL) {
        GLB_SAFE_DELETE(m_VertexShader);
    }

    if (m_FragmentShader != NULL) {
        GLB_SAFE_DELETE(m_FragmentShader);
    }

    if (m_Program != 0) {
        glDeleteProgram(m_Program);
        m_Program = 0;
    }
}

Program* Program::Create(const char* vertex_shader_file, const char* fragment_shader_file) {
    Program* shader_program = NULL;
    VertexShader* vertex_shader = VertexShader::Create(vertex_shader_file);
    FragmentShader* fragment_shader = FragmentShader::Create(fragment_shader_file);

    if (vertex_shader != NULL && fragment_shader != NULL) {
        GLuint program = glCreateProgram();

        if (program != 0) {
            glAttachShader(program, vertex_shader->GetHandle());
            glAttachShader(program, fragment_shader->GetHandle());

            glLinkProgram(program);

            // Get layout attributes
            GLint attributes_num = 0;
            glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &attributes_num);
            ShaderLayout layout;
            memset(&layout, 0, sizeof(layout));
            layout.count = attributes_num;
            for (int32_t i = 0; i < attributes_num; i++) {
                GLsizei length = 0;
                GLint size = 0;
                GLenum type = 0;
                GLchar name[kMaxVertexAttributeName];
                glGetActiveAttrib(program, i, kMaxVertexAttributeName, &length, &size, &type, name);
                layout.layouts[i].attriType = Program::GetVertexAttribute(name);
                layout.layouts[i].location = glGetAttribLocation(program, name);
                memcpy(layout.layouts[i].name, name, length);
            }

            // Get uniforms
            GLint uniform_num = 0;
            glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &uniform_num);
            std::vector<uniform::UniformEntry> uniforms;
            for (int32_t i = 0; i < uniform_num; i++) {
                GLsizei length = 0;
                GLint size = 0;
                GLenum type = 0;
                GLchar name[kMaxUniformName];
                glGetActiveUniform(program, i, kMaxUniformName, &length, &size, &type, name);
                uniform::UniformEntry entry;
                entry.id = -1;

                for (int32_t j = 0; j < sizeof(uniform::kEngineUniforms) / sizeof(uniform::kEngineUniforms[0]); j++) {
                    if (!strcmp(uniform::kEngineUniforms[j].name, name)) {
                        entry.id = uniform::kEngineUniforms[j].id;
                        entry.flag = uniform::kEngineUniforms[j].flag;
                        break;
                    }
                }
                GLB_SAFE_ASSERT(entry.id != -1);

                entry.location = glGetUniformLocation(program, name);

                uniforms.push_back(entry);
            }

            GLint success = 0;
            glGetProgramiv(program, GL_LINK_STATUS, &success);
            if (success == 0) {
                GLchar info_log[256];
                glGetProgramInfoLog(program, sizeof(info_log), NULL, info_log);
                log::LogPrint("%s", info_log);
            } else {
                // Save all the valid objects
                shader_program = new Program();
                if (shader_program != NULL) {
                    shader_program->m_Program = program;
                    shader_program->m_VertexShader = vertex_shader;
                    shader_program->m_FragmentShader = fragment_shader;
                    memcpy(&shader_program->m_ShaderLayout, &layout, sizeof(layout));
                    shader_program->m_Uniforms = uniforms;
                } else {
                    GLB_SAFE_ASSERT(false);
                }
            }
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return shader_program;
}

Program* Program::Create(Descriptor desc) {
    Program* shader_program = NULL;

    std::vector<std::string> enable_macros;
    enable_macros.push_back("#version 330 core\n");
    for (int32_t i = 0; i < Descriptor::kFlagBitNum; i++) {
        if (desc.GetFlag(i)) {
            enable_macros.push_back(std::string(kEnableMacros[i]));
        }
    }

    VertexShader* vs = VertexShader::Create(enable_macros, "..\\glb\\shader\\uber.vs");
    FragmentShader* fs = FragmentShader::Create(enable_macros, "..\\glb\\shader\\uber.ps");

    if (vs != NULL && fs != NULL) {
        GLuint program = glCreateProgram();

        if (program != 0) {
            glAttachShader(program, vs->GetHandle());
            glAttachShader(program, fs->GetHandle());

            glLinkProgram(program);

            // Get layout attributes
            GLint attributes_num = 0;
            glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &attributes_num);
            ShaderLayout layout;
            memset(&layout, 0, sizeof(layout));
            layout.count = attributes_num;
            for (int32_t i = 0; i < attributes_num; i++) {
                GLsizei length = 0;
                GLint size = 0;
                GLenum type = 0;
                GLchar name[kMaxVertexAttributeName];
                glGetActiveAttrib(program, i, kMaxVertexAttributeName, &length, &size, &type, name);
                layout.layouts[i].attriType = Program::GetVertexAttribute(name);
                layout.layouts[i].location = glGetAttribLocation(program, name);
                memcpy(layout.layouts[i].name, name, length);
            }

            // Get uniforms
            GLint uniform_num = 0;
            glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &uniform_num);
            std::vector<uniform::UniformEntry> uniforms;
            for (int32_t i = 0; i < uniform_num; i++) {
                GLsizei length = 0;
                GLint size = 0;
                GLenum type = 0;
                GLchar name[kMaxUniformName];
                glGetActiveUniform(program, i, kMaxUniformName, &length, &size, &type, name);
                uniform::UniformEntry entry;

                for (int32_t j = 0; j < sizeof(uniform::kEngineUniforms) / sizeof(uniform::kEngineUniforms[0]); j++) {
                    if (!strcmp(uniform::kEngineUniforms[j].name, name)) {
                        entry.id = uniform::kEngineUniforms[j].id;
                        entry.flag = uniform::kEngineUniforms[j].flag;
                        break;
                    }
                }

                entry.location = glGetUniformLocation(program, name);

                uniforms.push_back(entry);
            }

            GLint success = 0;
            glGetProgramiv(program, GL_LINK_STATUS, &success);
            if (success == 0) {
                GLchar info_log[256];
                glGetProgramInfoLog(program, sizeof(info_log), NULL, info_log);
                log::LogPrint("%s", info_log);
                GLB_SAFE_ASSERT(false);
            } else {
                // Save all the valid objects
                shader_program = new Program();
                if (shader_program != NULL) {
                    shader_program->m_Program = program;
                    shader_program->m_VertexShader = vs;
                    shader_program->m_FragmentShader = fs;
                    memcpy(&shader_program->m_ShaderLayout, &layout, sizeof(layout));
                    shader_program->m_Uniforms = uniforms;
                    shader_program->m_ShaderDescptor = desc;
                } else {
                    GLB_SAFE_ASSERT(false);
                }
            }
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return shader_program;
}

void Program::SetID(int32_t shader_id) {
    m_ID = shader_id;
}

const ShaderLayout& Program::GetShaderLayout() {
    return m_ShaderLayout;
}

const Descriptor& Program::GetShaderDescriptor() {
    return m_ShaderDescptor;
}

const uint32_t Program::GetShader() {
    return m_Program;
}

std::vector<uniform::UniformEntry>& Program::GetUniforms() {
    return m_Uniforms;
}

VertexAttribute Program::GetVertexAttribute(const char* attribute_name) {
    VertexAttribute result = VA_UNKNOWN;

    if (attribute_name != NULL) {
        if (!strcmp("glb_Pos", attribute_name)) {
            result = VA_POS;
        } else if (!strcmp("glb_Color", attribute_name)) {
            result = VA_COLOR;
        } else if (!strcmp("glb_TexCoord", attribute_name)) {
            result = VA_TEXCOORD;
        } else if (!strcmp("glb_Normal", attribute_name)) {
            result = VA_NORMAL;
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

//-----------------------------------------------------------------------------------
// Descriptor DEFINITION
//-----------------------------------------------------------------------------------
Descriptor::Descriptor() {
    memset(m_ShaderFlag, 0, sizeof(m_ShaderFlag));
}

Descriptor::~Descriptor() {
}

void Descriptor::SetFlag(int32_t flag_bit, bool enable) {
    int32_t index = flag_bit / (sizeof(char) * 8);
    int32_t bit_offset = flag_bit % (sizeof(char) * 8);

    if (0 <= index && index < sizeof(m_ShaderFlag)/sizeof(m_ShaderFlag[0])) {
        if (enable) {
            m_ShaderFlag[index] = ((1 << bit_offset) | m_ShaderFlag[index]);
        } else {
            m_ShaderFlag[index] = ((~(1 << bit_offset)) & m_ShaderFlag[index]);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

bool Descriptor::GetFlag(int32_t flag_bit) {
    bool result = false;

    int32_t index = flag_bit / (sizeof(char) * 8);
    int32_t bit_offset = flag_bit % (sizeof(char) * 8);

    if (0 <= index && index < sizeof(m_ShaderFlag)/sizeof(m_ShaderFlag[0])) {
        int32_t shader_flag = (m_ShaderFlag[index] & (1 << bit_offset));

        if (shader_flag >= 1) {
            result = true;
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

bool Descriptor::Equal(const Descriptor& desc) {
    bool result = true;

    for (int32_t i = 0; i < sizeof(m_ShaderFlag) / sizeof(m_ShaderFlag[0]); i++) {
        if (m_ShaderFlag[i] != desc.m_ShaderFlag[i]) {
            result = false;
            break;
        }
    }

    return result;
}

std::string Descriptor::GetString() {
    return std::string(m_ShaderFlag);
}

//-----------------------------------------------------------------------------------
// ShaderMgrImp DEFINITION
//-----------------------------------------------------------------------------------
MgrImp::MgrImp()
: m_CurShader(-1) {
}

MgrImp::~MgrImp() {
    Destroy();
}

void MgrImp::Initialize() {
}

void MgrImp::Destroy() {
    for (int32_t i = 0; i < static_cast<int32_t>(m_ShaderDataBase.size()); i++) {
        GLB_SAFE_DELETE(m_ShaderDataBase[i]);
    }
    m_ShaderDataBase.clear();
    m_CurShader = -1;
}

void MgrImp::SetCurShader(int32_t cur_shader_id) {
    m_CurShader = cur_shader_id;
}

int32_t MgrImp::GetCurShader() {
    return m_CurShader;
}

int32_t MgrImp::AddShader(const char* vertex_shader_file, const char* fragment_shader_file) {
    int32_t id = -1;

    Program* program = Program::Create(vertex_shader_file, fragment_shader_file);
    if (program != NULL) {
        id = m_ShaderDataBase.size();
        program->SetID(id);
        m_ShaderDataBase.push_back(program);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return id;
}

Program* MgrImp::GetShader(int32_t shader_id) {
    Program* program = NULL;

    if (0 <= shader_id && shader_id < static_cast<int32_t>(m_ShaderDataBase.size())) {
        program = m_ShaderDataBase[shader_id];
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return program;
}

int32_t MgrImp::GetShader(Descriptor desc) {
    int32_t result = -1;

    // Find the shader
    bool find_shader = false;
    for (int32_t i = 0; i < static_cast<int32_t>(m_ShaderDataBase.size()); i++) {
        if (desc.Equal(m_ShaderDataBase[i]->GetShaderDescriptor())) {
            result = i;
            find_shader = true;
            break;
        }
    }

    // Check if we need generate new shader
    if (find_shader == false) {
        Program* program = NULL;
        program = Program::Create(desc);
        result = m_ShaderDataBase.size();
        m_ShaderDataBase.push_back(program);
    }

    return result;
}

//-----------------------------------------------------------------------------------
// Mgr DEFINITION
//-----------------------------------------------------------------------------------
void Mgr::Initialize() {
    if (s_MgrImp == NULL) {
        s_MgrImp = new MgrImp;
        if (s_MgrImp != NULL) {
            s_MgrImp->Initialize();
        } else {
            GLB_SAFE_ASSERT(false);
        }
    }
}

void Mgr::Destroy() {
    if (s_MgrImp != NULL) {
        s_MgrImp->Destroy();
        GLB_SAFE_DELETE(s_MgrImp);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Mgr::SetCurShader(int32_t cur_shader_id) {
    if (s_MgrImp != NULL) {
        s_MgrImp->SetCurShader(cur_shader_id);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

int32_t Mgr::GetCurShader() {
    int32_t result = -1;

    if (s_MgrImp != NULL) {
        result = s_MgrImp->GetCurShader();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

int32_t Mgr::AddShader(const char* vertex_shader_file, const char* fragment_shader_file) {
    int32_t result = -1;

    if (s_MgrImp != NULL) {
        result = s_MgrImp->AddShader(vertex_shader_file, fragment_shader_file);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

Program* Mgr::GetShader(int32_t shader_id) {
    Program* program = NULL;

    if (s_MgrImp != NULL) {
        program = s_MgrImp->GetShader(shader_id);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return program;
}

int32_t Mgr::GetShader(Descriptor desc) {
    int32_t program = -1;

    if (s_MgrImp != NULL) {
        program = s_MgrImp->GetShader(desc);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return program;
}

};  // namespace shader

};  // namespace glb
