//-------------------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 06 / 05
// Brief: shader::Program is a class that deal with shader
//-------------------------------------------------------------------------------------
#include "glbshader.h"

#include "imp/dx11/glbshaderimp.h"
#include "imp/gl/glbshaderimp.h"

namespace glb {

namespace render {

namespace shader {

//-----------------------------------------------------------------------------------

class MgrImp;
static MgrImp* s_MgrImp = NULL;

//-----------------------------------------------------------------------------------

class MgrImp {
public:
    MgrImp();
    virtual ~MgrImp();

public:
    void Initialize();
    void Destroy();

public:
    int32_t AddUberShader(const char* vertex_shader_file, const char* fragment_shader_file, const char* geometry_shader_file);
    Program* GetShader(int32_t shader_id);
    int32_t GetUberShaderID(Descriptor desc);

private:
    std::vector<Program*> m_ShaderDataBase;
};

//---------------------------------------------------------------------------------------------------

VertexShader::VertexShader()
: m_Imp(NULL) {
}

VertexShader::~VertexShader() {
    GLB_SAFE_DELETE(m_Imp);
}

VertexShader* VertexShader::Create(const char* vertex_shader_name) {
    VertexShader* result = NULL;
    VertexShader::Imp* imp = VertexShader::Imp::Create(vertex_shader_name);
    if (imp != NULL) {
        result = new VertexShader;
        if (result != NULL) {
            result->m_Imp = imp;
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
    VertexShader::Imp* imp = VertexShader::Imp::Create(enable_macros, uber_shader_file_name);
    if (imp != NULL) {
        result = new VertexShader;
        if (result != NULL) {
            result->m_Imp = imp;
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

uint32_t VertexShader::GetHandle() const {
    uint32_t handle = -1;

    if (m_Imp != NULL) {
        handle = m_Imp->GetHandle();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return handle;
}

//-----------------------------------------------------------------------------------

GeometryShader::GeometryShader()
: m_Imp(NULL) {
}

GeometryShader::~GeometryShader() {
    GLB_SAFE_DELETE(m_Imp);
}

GeometryShader* GeometryShader::Create(const char* geometry_shader_name) {
    GeometryShader* result = NULL;
    GeometryShader::Imp* imp = GeometryShader::Imp::Create(geometry_shader_name);
    if (imp != NULL) {
        result = new GeometryShader;
        if (result != NULL) {
            result->m_Imp = imp;
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

uint32_t GeometryShader::GetHandle() const {
    uint32_t handle = -1;

    if (m_Imp != NULL) {
        handle = m_Imp->GetHandle();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return handle;
}

//-----------------------------------------------------------------------------------

FragmentShader::FragmentShader()
: m_Imp(NULL) {
}

FragmentShader::~FragmentShader() {
    GLB_SAFE_DELETE(m_Imp);
}

FragmentShader* FragmentShader::Create(const char* fragment_shader_name) {
    FragmentShader* result = NULL;
    FragmentShader::Imp* imp = FragmentShader::Imp::Create(fragment_shader_name);
    if (imp != NULL) {
        result = new FragmentShader;
        if (result != NULL) {
            result->m_Imp = imp;
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
    FragmentShader::Imp* imp = FragmentShader::Imp::Create(enable_macros, uber_shader_file_name);
    if (imp != NULL) {
        result = new FragmentShader;
        if (result != NULL) {
            result->m_Imp = imp;
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

uint32_t FragmentShader::GetHandle() const {
    uint32_t handle = -1;

    if (m_Imp != NULL) {
        handle = m_Imp->GetHandle();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return handle;
}

//-----------------------------------------------------------------------------------

UberProgram::UberProgram()
: m_Imp(NULL) {
}

UberProgram::~UberProgram() {
    GLB_SAFE_DELETE(m_Imp);
}

UberProgram* UberProgram::Create(const char* vertex_shader_file, const char* fragment_shader_file, const char* geometry_shader_file) {
    UberProgram* shader_program = NULL;
    UberProgram::Imp* imp = UberProgram::Imp::Create(vertex_shader_file, fragment_shader_file, geometry_shader_file);
    if (imp != NULL) {
        shader_program = new UberProgram;
        if (shader_program != NULL) {
            shader_program->m_Imp = imp;
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return shader_program;
}

UberProgram* UberProgram::Create(Descriptor desc) {
    UberProgram* shader_program = NULL;
    UberProgram::Imp* imp = UberProgram::Imp::Create(desc);

    if (imp != NULL) {
        shader_program = new UberProgram;
        if (shader_program != NULL) {
            shader_program->m_Imp = imp;
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return shader_program;
}

void UberProgram::SetID(int32_t shader_id) {
    if (m_Imp != NULL) {
        m_Imp->SetID(shader_id);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

int32_t UberProgram::GetProgramType() {
    int32_t type = Program::UNKOWN_PROGRAM;

    if (m_Imp != NULL) {
        type = m_Imp->GetProgramType();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return type;
}

void UberProgram::SetProgramType(int32_t type) {
    if (m_Imp != NULL) {
        m_Imp->SetProgramType(type);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

ShaderLayout UberProgram::GetShaderLayout() {
    ShaderLayout layout;

    if (m_Imp != NULL) {
        layout = m_Imp->GetShaderLayout();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return layout;
}

Descriptor UberProgram::GetShaderDescriptor() {
    Descriptor desc;

    if (m_Imp != NULL) {
        desc = m_Imp->GetShaderDescriptor();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return desc;
}

void* UberProgram::GetNativeShader() {
    void* shader = NULL;

    if (m_Imp != NULL) {
        shader = m_Imp->GetNativeShader();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return shader;
}

std::vector<uniform::UniformEntry>& UberProgram::GetUniforms() {
    return m_Imp->GetUniforms();
}

VertexAttribute UberProgram::GetVertexAttribute(const char* attribute_name) {
    VertexAttribute result = VA_UNKNOWN;

    if (attribute_name != NULL) {
        if (!strcmp("glb_Pos", attribute_name)) {
            result = VA_POS;
        } else if (!strcmp("glb_Color", attribute_name)) {
            result = VA_COLOR;
        } else if (!strcmp("glb_TexCoord", attribute_name)) {
            result = VA_TEXCOORD;
        } else if (!strcmp("glb_LightMapTexCoord", attribute_name)) {
            result = VA_LIGHT_MAP_TEXCOORD;
        } else if (!strcmp("glb_Normal", attribute_name)) {
            result = VA_NORMAL;
        } else if (!strcmp("glb_Tangent", attribute_name)) {
            result = VA_TANGENT;
        } else if (!strcmp("glb_Binormal", attribute_name)) {
            result = VA_BINORMAL;
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

//-----------------------------------------------------------------------------------

UserProgram::UserProgram()
: m_Imp(NULL) {
}

UserProgram::~UserProgram() {
    GLB_SAFE_DELETE(m_Imp);
}

UserProgram* UserProgram::Create(const char* vertex_shader_file, const char* fragment_shader_file, const char* geometry_shader_file) {
    UserProgram* shader_program = NULL;
    UserProgram::Imp* imp = UserProgram::Imp::Create(vertex_shader_file, fragment_shader_file, geometry_shader_file);
    if (imp != NULL) {
        shader_program = new UserProgram;
        if (shader_program != NULL) {
            shader_program->m_Imp = imp;
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return shader_program;
}

void UserProgram::SetID(int32_t shader_id) {
    if (m_Imp != NULL) {
        m_Imp->SetID(shader_id);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

int32_t UserProgram::GetProgramType() {
    int32_t type = Program::UNKOWN_PROGRAM;

    if (m_Imp != NULL) {
        type = m_Imp->GetProgramType();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return type;
}

void UserProgram::SetProgramType(int32_t type) {
    if (m_Imp != NULL) {
        m_Imp->SetProgramType(type);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

ShaderLayout UserProgram::GetShaderLayout() {
    ShaderLayout layout;

    if (m_Imp != NULL) {
        layout = m_Imp->GetShaderLayout();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return layout;
}

void* UserProgram::GetNativeShader() {
    void* shader = NULL;

    if (m_Imp != NULL) {
        shader = m_Imp->GetNativeShader();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return shader;
}

int32_t UserProgram::GetUniformLocation(const char* uniform_name) {
    int32_t location = -1;

    if (m_Imp != NULL) {
        location = m_Imp->GetUniformLocation(uniform_name);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return location;
}

VertexAttribute UserProgram::GetVertexAttribute(const char* attribute_name) {
    VertexAttribute result = VA_UNKNOWN;

    if (attribute_name != NULL) {
        if (!strcmp("glb_Pos", attribute_name)) {
            result = VA_POS;
        } else if (!strcmp("glb_Color", attribute_name)) {
            result = VA_COLOR;
        } else if (!strcmp("glb_TexCoord", attribute_name)) {
            result = VA_TEXCOORD;
        } else if (!strcmp("glb_LightMapTexCoord", attribute_name)) {
            result = VA_LIGHT_MAP_TEXCOORD;
        } else if (!strcmp("glb_Normal", attribute_name)) {
            result = VA_NORMAL;
        } else if (!strcmp("glb_Tangent", attribute_name)) {
            result = VA_TANGENT;
        } else if (!strcmp("glb_Binormal", attribute_name)) {
            result = VA_BINORMAL;
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

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

MgrImp::MgrImp() {
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
}

int32_t MgrImp::AddUberShader(const char* vertex_shader_file, const char* fragment_shader_file, const char* geometry_shader_file) {
    int32_t id = -1;

    UberProgram* program = UberProgram::Create(vertex_shader_file, fragment_shader_file, geometry_shader_file);
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

int32_t MgrImp::GetUberShaderID(Descriptor desc) {
    int32_t result = -1;

    // Find the shader
    bool find_shader = false;
    for (int32_t i = 0; i < static_cast<int32_t>(m_ShaderDataBase.size()); i++) {
        if (m_ShaderDataBase[i]->GetProgramType() != Program::UBER_PROGRAM) continue;

        UberProgram* uber = static_cast<UberProgram*>(m_ShaderDataBase[i]);
        if (desc.Equal(uber->GetShaderDescriptor())) {
            result = i;
            find_shader = true;
            break;
        }
    }

    // Check if we need generate new shader
    if (find_shader == false) {
        UberProgram* program = NULL;
        program = UberProgram::Create(desc);
        result = m_ShaderDataBase.size();
        m_ShaderDataBase.push_back(program);
    }

    return result;
}

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

int32_t Mgr::AddUberShader(const char* vertex_shader_file, const char* fragment_shader_file, const char* geometry_shader_file) {
    int32_t result = -1;

    if (s_MgrImp != NULL) {
        result = s_MgrImp->AddUberShader(vertex_shader_file, fragment_shader_file, geometry_shader_file);
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

int32_t Mgr::GetUberShaderID(Descriptor desc) {
    int32_t program = -1;

    if (s_MgrImp != NULL) {
        program = s_MgrImp->GetUberShaderID(desc);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return program;
}

};  // namespace shader

};  // namespace render

};  // namespace glb
