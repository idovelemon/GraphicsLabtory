//-----------------------------------------------------------------------
// Declaration: Copyright (c), i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017/03/18
// Brief: Implement the shader with opengl
//-----------------------------------------------------------------------
#ifdef GLB_PLATFORM_OPENGL

#include "glbshaderimp.h"

#include <memory.h>
#include <stdio.h>
#include <string.h>

#include <GL\glew.h>
#include <GL\GL.h>

#include "math/glbvector.h"

#include "render/glbuniform.h"

#include "util/glblog.h"
#include "util/glbmacro.h"

namespace glb {

namespace render {

namespace shader {

//----------------------------------------------------------------------------------

char* glbLoadShaderBufferFromFile(const char* fileName) {
    char* shader_str = nullptr;
    if (fileName != nullptr) {
        // @TODO:Must use rb mode to read shader string, otherwise it will encounter some weird compile error.
        // Don't know why now.
        FILE* shaderFile = fopen(fileName, "rb");

        if (shaderFile != nullptr) {
            fseek(shaderFile, 0, SEEK_END);
            int32_t size = ftell(shaderFile);

            shader_str = new char[size + 1];
            if (shader_str != nullptr) {
                memset(shader_str, 0, size);

                fseek(shaderFile, 0, SEEK_SET);
                fread(shader_str, sizeof(char), size, shaderFile);
                shader_str[size] = '\0';  // @Warning:Must have a null-terminator
            } else {
                GLB_SAFE_ASSERT(false);
            }
        } else {
            std::string msg = fileName;
            msg += " doesn't exsit";
            GLB_SAFE_ASSERT_LOG(false, msg.c_str());
        }

        if (shaderFile != NULL) {
            fclose(shaderFile);
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

//--------------------------------------------------------------------------------------

VertexShader::Imp::Imp()
:m_VertexShader(0) {
}

VertexShader::Imp::~Imp() {
    if (m_VertexShader != 0) {
        glDeleteShader(m_VertexShader);
        m_VertexShader = NULL;
    }
}

VertexShader::Imp* VertexShader::Imp::Create(const char* vertex_shader_name) {
    VertexShader::Imp* result = NULL;

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

                strcpy(info_log, vertex_shader_name);
                int32_t vertex_shader_name_len = strlen(vertex_shader_name);
                info_log[vertex_shader_name_len++] = ' ';

                glGetShaderInfoLog(vertex_shader, sizeof(info_log) - vertex_shader_name_len, NULL, info_log + vertex_shader_name_len);

                printf(reinterpret_cast<char*>(info_log));
                util::log::LogPrint(info_log);

                GLB_SAFE_ASSERT_LOG(false, info_log);
            } else {
                result = new VertexShader::Imp();
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

VertexShader::Imp* VertexShader::Imp::Create(std::vector<std::string> enable_macros, const char* uber_shader_file_name) {
    VertexShader::Imp* result = NULL;

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
            // @Warning: Should use glShaderSource(xxx, xxx, xxx, NULLL). It means string will be null-terminator.
            glShaderSource(vertex_shader, enable_macros.size() + 1, const_cast<const GLchar**>(shader_buf), NULL);
            glCompileShader(vertex_shader);

            GLint success = 0;
            glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
            if (success == 0) {
                GLchar info_log[256];
                glGetShaderInfoLog(vertex_shader, sizeof(info_log), NULL, info_log);
                util::log::LogPrint(info_log);
                GLB_SAFE_ASSERT(false);
            } else {
                result = new VertexShader::Imp();
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

uint32_t VertexShader::Imp::GetHandle() const {
    return m_VertexShader;
}

//--------------------------------------------------------------------------------------

GeometryShader::Imp::Imp()
: m_GeometryShader(0) {
}

GeometryShader::Imp::~Imp() {
    if (m_GeometryShader != 0) {
        glDeleteShader(m_GeometryShader);
        m_GeometryShader = NULL;
    }
}

GeometryShader::Imp* GeometryShader::Imp::Create(const char* geometry_shader_name) {
    GeometryShader::Imp* result = NULL;

    if (geometry_shader_name != NULL) {
        GLuint geometry_shader = glCreateShader(GL_GEOMETRY_SHADER_ARB);

        // Load the shader program string from file
        char* shader_str = glbLoadShaderBufferFromFile(geometry_shader_name);

        if (shader_str != NULL) {
            GLint len = static_cast<GLint>(strlen(shader_str));
            glShaderSource(geometry_shader, 1, const_cast<const GLchar**>(&shader_str), &len);
            glCompileShader(geometry_shader);

            GLint success = 0;
            glGetShaderiv(geometry_shader, GL_COMPILE_STATUS, &success);
            if (success == 0) {
                GLchar info_log[256];
                glGetShaderInfoLog(geometry_shader, sizeof(info_log), NULL, info_log);
                printf(reinterpret_cast<char*>(info_log));
                char err[128];
                sprintf_s(err, sizeof(err), "Compile shader failed:%s", geometry_shader_name);
                GLB_SAFE_ASSERT(false);
            } else {
                result = new GeometryShader::Imp();
                if (result != NULL) {
                    result->m_GeometryShader = geometry_shader;
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

uint32_t GeometryShader::Imp::GetHandle() const {
    return m_GeometryShader;
}

//--------------------------------------------------------------------------------------

FragmentShader::Imp::Imp()
: m_FragmentShader(0) {
}

FragmentShader::Imp::~Imp() {
    if (m_FragmentShader != 0) {
        glDeleteShader(m_FragmentShader);
        m_FragmentShader = NULL;
    }
}

FragmentShader::Imp* FragmentShader::Imp::Create(const char* fragment_shader_name) {
    FragmentShader::Imp* result = NULL;

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
                util::log::LogPrint("Compile fragment shader failed!");
                util::log::LogPrint("Failed fragment shader name:%s", fragment_shader_name);
                util::log::LogPrint("Failed fragment shader log:%s", info_log);
                GLB_SAFE_ASSERT_LOG(false, info_log);
            } else {
                result = new FragmentShader::Imp();
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

FragmentShader::Imp* FragmentShader::Imp::Create(std::vector<std::string> enable_macros, const char* uber_shader_file_name) {
    FragmentShader::Imp* result = NULL;

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
            // @Warning: Should use glShaderSource(xxx, xxx, xxx, NULLL). It means string will be null-terminator.
            glShaderSource(fragment_shader, enable_macros.size() + 1, const_cast<const GLchar**>(shader_buf), NULL);
            glCompileShader(fragment_shader);

            GLint success = 0;
            glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
            if (success == 0) {
                GLchar info_log[256];
                glGetShaderInfoLog(fragment_shader, sizeof(info_log), NULL, info_log);
                util::log::LogPrint("Compile fragment shader failed!");
                util::log::LogPrint("Failed fragment shader name:%s", uber_shader_file_name);
                util::log::LogPrint("Failed fragment shader macro:");
                int32_t size = enable_macros.size();
                for (int32_t i = 0; i < size; i++) {
                    util::log::LogPrint("%s", enable_macros[i].c_str());
                }
                util::log::LogPrint("%s", info_log);
                GLB_SAFE_ASSERT(false);
            } else {
                result = new FragmentShader::Imp();
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

uint32_t FragmentShader::Imp::GetHandle() const {
    return m_FragmentShader;
}

//-----------------------------------------------------------------------------------

UberProgram::Imp::Imp()
: m_ID(-1)
, m_Program(0)
, m_VertexShader(0)
, m_FragmentShader(0)
, m_ShaderLayout()
, m_ShaderDescptor() {
    memset(&m_ShaderLayout, 0, sizeof(m_ShaderLayout));
    m_ShaderParameter.clear();
}

UberProgram::Imp::~Imp() {
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

UberProgram::Imp* UberProgram::Imp::Create(const char* vertex_shader_file, const char* fragment_shader_file, const char* geometry_shader_file) {
    UberProgram::Imp* shader_program = NULL;
    VertexShader* vertex_shader = VertexShader::Create(vertex_shader_file);
    FragmentShader* fragment_shader = FragmentShader::Create(fragment_shader_file);
    GeometryShader* geometry_shader = NULL;
    if (geometry_shader) {
        geometry_shader = GeometryShader::Create(geometry_shader_file);
    }

    if (vertex_shader != NULL && fragment_shader != NULL) {
        GLuint program = glCreateProgram();

        if (program != 0) {
            glAttachShader(program, vertex_shader->GetHandle());
            glAttachShader(program, fragment_shader->GetHandle());
            if (geometry_shader) {
                glAttachShader(program, geometry_shader->GetHandle());
            }

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
                layout.layouts[i].attriType = UberProgram::GetVertexAttribute(name);
                layout.layouts[i].location = glGetAttribLocation(program, name);
                memcpy(layout.layouts[i].name, name, length);
            }

            // Get uniforms
            GLint uniform_num = 0;
            glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &uniform_num);
            std::vector<uniform::UniformEntry> uniforms;
            std::vector<ShaderParameter> shaderParameters;
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

                ShaderParameter parameter;
                memcpy(parameter.name, name, strlen(name));
                parameter.name[strlen(name)] = '\0';
                if (type == GL_INT) {
                    parameter.format = PARAMETER_FORMAT_INT;
                } else if (type == GL_FLOAT) {
                    parameter.format = PARAMETER_FORMAT_FLOAT;
                } else if (type == GL_SAMPLER_2D) {
                    parameter.format = PARAMETER_FORMAT_TEXTURE_2D;
                } else if (type == GL_SAMPLER_3D) {
                    parameter.format = PARAMETER_FORMAT_TEXTURE_3D;
                } else if (type == GL_SAMPLER_CUBE) {
                    parameter.format = PARAMETER_FORMAT_TEXTURE_CUBE;
                } else if (type == GL_FLOAT_VEC3) {
                    parameter.format = PARAMETER_FORMAT_FLOAT3;
                } else if (type == GL_FLOAT_VEC4) {
                    parameter.format = PARAMETER_FORMAT_FLOAT4;
                } else if (type == GL_FLOAT_MAT4) {
                    parameter.format = PARAMETER_FORMAT_MATRIX;
                } else {
                    GLB_SAFE_ASSERT(false);
                }
                shaderParameters.push_back(parameter);

                entry.location = glGetUniformLocation(program, name);

                uniforms.push_back(entry);
            }

            GLint success = 0;
            glGetProgramiv(program, GL_LINK_STATUS, &success);
            if (success == 0) {
                GLchar info_log[256];
                glGetProgramInfoLog(program, sizeof(info_log), NULL, info_log);
                util::log::LogPrint("%s", info_log);
                GLB_SAFE_ASSERT(false);
            } else {
                // Save all the valid objects
                shader_program = new UberProgram::Imp();
                if (shader_program != NULL) {
                    shader_program->m_Program = program;
                    shader_program->m_VertexShader = vertex_shader;
                    shader_program->m_FragmentShader = fragment_shader;
                    memcpy(&shader_program->m_ShaderLayout, &layout, sizeof(layout));
                    shader_program->m_Uniforms = uniforms;
                    shader_program->m_Type = UBER_PROGRAM;
                    shader_program->m_ShaderParameter = shaderParameters;
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

UberProgram::Imp* UberProgram::Imp::Create(Descriptor desc) {
    UberProgram::Imp* shader_program = NULL;

    std::vector<std::string> enable_macros;
    enable_macros.push_back("#version 450\n");
    for (int32_t i = 0; i < Descriptor::kFlagBitNum; i++) {
        if (desc.GetFlag(i)) {
            enable_macros.push_back(std::string(kEnableMacros[i]));
        }
    }

    VertexShader* vs = VertexShader::Create(enable_macros, "..\\glb\\shader\\uber.vs");
    FragmentShader* fs = FragmentShader::Create(enable_macros, "..\\glb\\shader\\uber.fs");

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
                layout.layouts[i].attriType = UberProgram::GetVertexAttribute(name);
                layout.layouts[i].location = glGetAttribLocation(program, name);
                memcpy(layout.layouts[i].name, name, length);
            }

            // Get uniforms
            GLint uniform_num = 0;
            glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &uniform_num);
            std::vector<uniform::UniformEntry> uniforms;
            std::vector<ShaderParameter> shaderParameters;
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

                ShaderParameter parameter;
                memcpy(parameter.name, name, strlen(name));
                parameter.name[strlen(name)] = '\0';
                if (type == GL_INT) {
                    parameter.format = PARAMETER_FORMAT_INT;
                } else if (type == GL_FLOAT) {
                    parameter.format = PARAMETER_FORMAT_FLOAT;
                } else if (type == GL_SAMPLER_2D) {
                    parameter.format = PARAMETER_FORMAT_TEXTURE_2D;
                } else if (type == GL_SAMPLER_3D) {
                    parameter.format = PARAMETER_FORMAT_TEXTURE_3D;
                } else if (type == GL_SAMPLER_CUBE) {
                    parameter.format = PARAMETER_FORMAT_TEXTURE_CUBE;
                } else if (type == GL_FLOAT_VEC3) {
                    parameter.format = PARAMETER_FORMAT_FLOAT3;
                } else if (type == GL_FLOAT_VEC4) {
                    parameter.format = PARAMETER_FORMAT_FLOAT4;
                } else if (type == GL_FLOAT_MAT4) {
                    parameter.format = PARAMETER_FORMAT_MATRIX;
                } else {
                    GLB_SAFE_ASSERT(false);
                }
                shaderParameters.push_back(parameter);

                entry.location = glGetUniformLocation(program, name);

                uniforms.push_back(entry);
            }

            GLint success = 0;
            glGetProgramiv(program, GL_LINK_STATUS, &success);
            if (success == 0) {
                GLchar info_log[256];
                glGetProgramInfoLog(program, sizeof(info_log), NULL, info_log);
                util::log::LogPrint("%s", info_log);
                GLB_SAFE_ASSERT(false);
            } else {
                // Save all the valid objects
                shader_program = new UberProgram::Imp();
                if (shader_program != NULL) {
                    shader_program->m_Program = program;
                    shader_program->m_VertexShader = vs;
                    shader_program->m_FragmentShader = fs;
                    memcpy(&shader_program->m_ShaderLayout, &layout, sizeof(layout));
                    shader_program->m_Uniforms = uniforms;
                    shader_program->m_ShaderParameter = shaderParameters;
                    shader_program->m_ShaderDescptor = desc;
                    shader_program->m_Type = UBER_PROGRAM;
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

void UberProgram::Imp::SetID(int32_t shader_id) {
    m_ID = shader_id;
}

void UberProgram::Imp::SetProgramType(int32_t type) {
    m_Type = type;
}

int32_t UberProgram::Imp::GetProgramType() {
    return m_Type;
}

ShaderLayout UberProgram::Imp::GetShaderLayout() {
    return m_ShaderLayout;
}

Descriptor UberProgram::Imp::GetShaderDescriptor() {
    return m_ShaderDescptor;
}

void* UberProgram::Imp::GetNativeShader() {
    return reinterpret_cast<void*>(m_Program);
}

std::vector<ShaderParameter> UberProgram::Imp::GetProgramParameter() {
    return m_ShaderParameter;
}

std::vector<uniform::UniformEntry>& UberProgram::Imp::GetUniforms() {
    return m_Uniforms;
}

//------------------------------------------------------------------------------

UserProgram::Imp::Imp()
: m_ID(-1)
, m_Program(0)
, m_VertexShader(0)
, m_FragmentShader(0)
, m_ShaderLayout() {
    memset(&m_ShaderLayout, 0, sizeof(m_ShaderLayout));
    m_ShaderParameter.clear();
}

UserProgram::Imp::~Imp() {
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

UserProgram::Imp* UserProgram::Imp::Create(const char* vertex_shader_file, const char* fragment_shader_file, const char* geometry_shader_file) {
    UserProgram::Imp* shader_program = NULL;
    VertexShader* vertex_shader = VertexShader::Create(vertex_shader_file);
    FragmentShader* fragment_shader = FragmentShader::Create(fragment_shader_file);
    GeometryShader* geometry_shader = NULL;
    if (geometry_shader) {
        geometry_shader = GeometryShader::Create(geometry_shader_file);
    }

    if (vertex_shader != NULL && fragment_shader != NULL) {
        GLuint program = glCreateProgram();

        if (program != 0) {
            glAttachShader(program, vertex_shader->GetHandle());
            glAttachShader(program, fragment_shader->GetHandle());
            if (geometry_shader) {
                glAttachShader(program, geometry_shader->GetHandle());
            }

            glLinkProgram(program);

            // Get layout attributes
            GLint attributes_num = 0;
            glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &attributes_num);
            ShaderLayout layout;
            memset(&layout, 0, sizeof(layout));
            layout.count = attributes_num;

            std::vector<ShaderParameter> shaderParameters;
            for (int32_t i = 0; i < attributes_num; i++) {
                GLsizei length = 0;
                GLint size = 0;
                GLenum type = 0;
                GLchar name[kMaxVertexAttributeName];
                glGetActiveAttrib(program, i, kMaxVertexAttributeName, &length, &size, &type, name);
                layout.layouts[i].attriType = UserProgram::GetVertexAttribute(name);
                layout.layouts[i].location = glGetAttribLocation(program, name);
                memcpy(layout.layouts[i].name, name, length);

                ShaderParameter parameter;
                memcpy(parameter.name, name, strlen(name));
                parameter.name[strlen(name)] = '\0';
                if (type == GL_INT) {
                    parameter.format = PARAMETER_FORMAT_INT;
                } else if (type == GL_FLOAT) {
                    parameter.format = PARAMETER_FORMAT_FLOAT;
                } else if (type == GL_SAMPLER_2D) {
                    parameter.format = PARAMETER_FORMAT_TEXTURE_2D;
                } else if (type == GL_SAMPLER_3D) {
                    parameter.format = PARAMETER_FORMAT_TEXTURE_3D;
                } else if (type == GL_SAMPLER_CUBE) {
                    parameter.format = PARAMETER_FORMAT_TEXTURE_CUBE;
                } else if (type == GL_FLOAT_VEC2) {
                    parameter.format = PARAMETER_FORMAT_FLOAT3;  // TODO:
                } else if (type == GL_FLOAT_VEC3) {
                    parameter.format = PARAMETER_FORMAT_FLOAT3;
                } else if (type == GL_FLOAT_VEC4) {
                    parameter.format = PARAMETER_FORMAT_FLOAT4;
                } else if (type == GL_FLOAT_MAT4) {
                    parameter.format = PARAMETER_FORMAT_MATRIX;
                } else {
                    GLB_SAFE_ASSERT(false);
                }
                shaderParameters.push_back(parameter);
            }

            GLint success = 0;
            glGetProgramiv(program, GL_LINK_STATUS, &success);
            if (success == 0) {
                GLchar info_log[256];
                glGetProgramInfoLog(program, sizeof(info_log), NULL, info_log);
                util::log::LogPrint("%s", info_log);
                GLB_SAFE_ASSERT(false);
            } else {
                // Save all the valid objects
                shader_program = new UserProgram::Imp();
                if (shader_program != NULL) {
                    shader_program->m_Program = program;
                    shader_program->m_VertexShader = vertex_shader;
                    shader_program->m_FragmentShader = fragment_shader;
                    memcpy(&shader_program->m_ShaderLayout, &layout, sizeof(layout));
                    shader_program->m_Type = USER_PROGRAM;
                    shader_program->m_ShaderParameter = shaderParameters;
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

void UserProgram::Imp::SetID(int32_t shader_id) {
    m_ID = shader_id;
}

void UserProgram::Imp::SetProgramType(int32_t type) {
    m_Type = type;
}

int32_t UserProgram::Imp::GetProgramType() {
    return m_Type;
}

ShaderLayout UserProgram::Imp::GetShaderLayout() {
    return m_ShaderLayout;
}

void* UserProgram::Imp::GetNativeShader() {
    return reinterpret_cast<void*>(m_Program);
}

std::vector<ShaderParameter> UserProgram::Imp::GetProgramParameter() {
    return m_ShaderParameter;
}

int32_t UserProgram::Imp::GetUniformLocation(const char* uniform_name) {
    return glGetUniformLocation(m_Program, uniform_name);
}

};  // namespace shader

};  // namespace render

};  // namespace glb

#endif  // GLB_PLATFORM_OPENGL