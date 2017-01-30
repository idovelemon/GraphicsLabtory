//-----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 09 / 05
// Brief: Deal with communication with glb editor
//-----------------------------------------------------------
#include "glbeditorcomm.h"

#include <process.h>
#include <stdint.h>

#include <Windows.h>

#include "comm.h"
#include "glbcomm.h"
#include "glbmacro.h"
#include "glbmaterial.h"

namespace glb {

//----------------------------------------------------------------------------------------
// DECLARATION
//----------------------------------------------------------------------------------------
class EditorCommImp;
static EditorCommImp* s_EditorCommImp = NULL;

//----------------------------------------------------------------------------------------
// @brief: Communicate with Editor
//----------------------------------------------------------------------------------------
class EditorCommImp {
 public:
    EditorCommImp();
    virtual ~EditorCommImp();

 public:
    void Initialize();
    void Destroy();
    void PostData();
    void AcceptData();

 protected:
     void PostMaterial();
};

//--------------------------------------------------------------------------------------
// DEFINITION
//--------------------------------------------------------------------------------------
void _cdecl threadfunc_RecvEditorData(void* param) {
    UNREFERENCED_PARAMETER(param);
    while(true) {
        EditorComm::AcceptData();
        Sleep(500);
    }
}

//--------------------------------------------------------------------------------
// @class:EditorCommImp
//--------------------------------------------------------------------------------
EditorCommImp::EditorCommImp() {
}

EditorCommImp::~EditorCommImp() {
}

void EditorCommImp::Initialize() {
    // Close editor first
    system("taskkill /im glb_editor.exe /f");
    Sleep(500);

    // Open editor
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory( &pi, sizeof(pi) );
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    if (!CreateProcessA("glb_editor.exe", "", NULL, NULL, FALSE, 0, NULL, NULL, reinterpret_cast<LPSTARTUPINFOA>(&si), &pi)) {
        GLB_SAFE_ASSERT(false);
    }

    // Build comm
    char buffer[128];
    int32_t real_len = ::GetCurrentDirectoryA(sizeof(buffer), buffer);
    memcpy(buffer + real_len, "\\view.ini", sizeof("\\view.ini"));
    int32_t post_port = ::GetPrivateProfileIntA("Port", "Post", 0, buffer);
    int32_t accept_port = ::GetPrivateProfileIntA("Port", "Accept", 0, buffer);

    comm::Comm::Initialize(accept_port, post_port, false);

    // Create recieve data thread
    _beginthread(threadfunc_RecvEditorData, 0, NULL);
}

void EditorCommImp::Destroy() {
    comm::Comm::Terminate();
}

void EditorCommImp::PostData() {
    PostMaterial();
}

void EditorCommImp::AcceptData() {
    comm::Packet* packet = comm::Comm::Recieve();
    if (packet != NULL) {
        int32_t mat_num = packet->size_in_bytes / sizeof(CommMaterial);
        int32_t offset = 0;
        for (int32_t i = 0; i < mat_num; i++) {
            CommMaterial mat;
            memcpy(&mat, reinterpret_cast<char*>(packet->data) + offset, sizeof(CommMaterial));
            offset += sizeof(CommMaterial);

            material::Material store_mat;
            store_mat.ambient = glb::Vector(mat.ambient_x, mat.ambient_y, mat.ambient_z);
            store_mat.diffuse = glb::Vector(mat.diffuse_x, mat.diffuse_y, mat.diffuse_z);
            store_mat.specular = glb::Vector(mat.specular_x, mat.specular_y, mat.specular_z);
            store_mat.emission = glb::Vector(mat.emission_x, mat.emission_y, mat.emission_z);
            store_mat.specular_pow = mat.pow;
            material::Mgr::ChangeMaterial(store_mat, mat.id);
        }
    }
}

void EditorCommImp::PostMaterial() {
    comm::Packet packet;
    packet.type = comm::kMaterialList;
    int32_t mat_num = material::Mgr::GetMaterialCount();
    CommMaterial* mats = new CommMaterial[mat_num];
    if (mats != NULL) {
        for (int32_t i = 0; i < mat_num; i++) {
            material::Material mat = material::Mgr::GetMaterial(i + 1);
            mats[i].ambient_x = mat.ambient.x;
            mats[i].ambient_y = mat.ambient.y;
            mats[i].ambient_z = mat.ambient.z;
            mats[i].diffuse_x = mat.diffuse.x;
            mats[i].diffuse_y = mat.diffuse.y;
            mats[i].diffuse_z = mat.diffuse.z;
            mats[i].specular_x = mat.specular.x;
            mats[i].specular_y = mat.specular.y;
            mats[i].specular_z = mat.specular.z;
            mats[i].emission_x = mat.emission.x;
            mats[i].emission_y = mat.emission.y;
            mats[i].emission_z = mat.emission.z;
            mats[i].pow = mat.specular_pow;
            mats[i].id = mat.id;
            memset(mats[i].material_name, 0, sizeof(mats[i].material_name));
            memcpy(mats[i].material_name, mat.mat_name, strlen(mat.mat_name));
        }
    }
    packet.size_in_bytes = sizeof(CommMaterial) * mat_num;
    packet.data = mats;
    comm::Comm::Send(&packet);
    delete[] mats;
    mats = NULL;
}
//--------------------------------------------------------------------------------
// @class:EditorComm
//--------------------------------------------------------------------------------
void EditorComm::Initialize() {
    if (s_EditorCommImp == NULL) {
        s_EditorCommImp = new EditorCommImp;
        if (s_EditorCommImp != NULL) {
            s_EditorCommImp->Initialize();
        }
    }
}

void EditorComm::Destroy() {
    if (s_EditorCommImp != NULL) {
        s_EditorCommImp->Destroy();
        delete s_EditorCommImp;
        s_EditorCommImp = NULL;
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void EditorComm::PostData() {
    if (s_EditorCommImp != NULL) {
        s_EditorCommImp->PostData();
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void EditorComm::AcceptData() {
    if (s_EditorCommImp != NULL) {
        s_EditorCommImp->AcceptData();
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

};  // namespace glb
