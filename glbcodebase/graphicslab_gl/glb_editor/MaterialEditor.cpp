// MaterialEditor.cpp : 实现文件
//

#include "stdafx.h"
#include "comm/comm.h"
#include "glb_editor.h"
#include "MaterialEditor.h"
#include "afxdialogex.h"
#include "glbcomm/glbcomm.h"

// CMaterialEditor 对话框

IMPLEMENT_DYNAMIC(CMaterialEditor, CDialogEx)

CMaterialEditor::CMaterialEditor(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMaterialEditor::IDD, pParent)
    , m_AmbientX(0.0f)
    , m_AmbientY(0.0f)
    , m_AmbientZ(0.0f)
    , m_DiffuseX(0.0f)
    , m_DiffuseY(0.0f)
    , m_DiffuseZ(0.0f)
    , m_SpecularX(0.0f)
    , m_SpecularY(0.0f)
    , m_SpecularZ(0.0f)
    , m_EmissionX(0.0f)
    , m_EmissionY(0.0f)
    , m_EmissionZ(0.0f)
    , m_Pow(0.0f)
    , m_CurSelMatItem(0)
{

}

CMaterialEditor::~CMaterialEditor()
{
}

void CMaterialEditor::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MATLIST, m_MaterialList);
    DDX_Text(pDX, IDC_AMBIENT_X, m_AmbientX);
    DDX_Text(pDX, IDC_AMBIENT_Y, m_AmbientY);
    DDX_Text(pDX, IDC_AMBIENT_Z, m_AmbientZ);
    DDX_Text(pDX, IDC_DIFFUSE_X, m_DiffuseX);
    DDX_Text(pDX, IDC_DIFFUSE_Y, m_DiffuseY);
    DDX_Text(pDX, IDC_DIFFUSE_Z, m_DiffuseZ);
    DDX_Text(pDX, IDC_SPE_X, m_SpecularX);
    DDX_Text(pDX, IDC_SPE_Y, m_SpecularY);
    DDX_Text(pDX, IDC_SPE_Z, m_SpecularZ);
    DDX_Text(pDX, IDC_EMI_X, m_EmissionX);
    DDX_Text(pDX, IDC_EMI_Y, m_EmissionY);
    DDX_Text(pDX, IDC_EMI_Z, m_EmissionZ);
    DDX_Text(pDX, IDC_POW, m_Pow);
}

void CMaterialEditor::PostMaterial()
{
    comm::Packet packet;
    packet.type = comm::kMaterialList;
    int32_t mat_num = glb::GlbMaterialMgr::GetMaterialCount();
    glb::GlbMaterial* mats = new glb::GlbMaterial[mat_num];
    if (mats != NULL) {
        for (int32_t i = 0; i < mat_num; i++) {
            glb::GlbMaterial* mat = glb::GlbMaterialMgr::GetMaterial(i + 1);
            mats[i].ambient_x = mat->ambient_x;
            mats[i].ambient_y = mat->ambient_y;
            mats[i].ambient_z = mat->ambient_z;
            mats[i].diffuse_x = mat->diffuse_x;
            mats[i].diffuse_y = mat->diffuse_y;
            mats[i].diffuse_z = mat->diffuse_z;
            mats[i].specular_x = mat->specular_x;
            mats[i].specular_y = mat->specular_y;
            mats[i].specular_z = mat->specular_z;
            mats[i].emission_x = mat->emission_x;
            mats[i].emission_y = mat->emission_y;
            mats[i].emission_z = mat->emission_z;
            mats[i].pow = mat->pow;
            mats[i].id = mat->id;
            memset(mats[i].material_name, 0, sizeof(mats[i].material_name));
            memcpy(mats[i].material_name, mat->material_name, strlen(mat->material_name));
        }
    }
    packet.size_in_bytes = sizeof(glb::GlbMaterial) * mat_num;
    packet.data = mats;
    comm::Comm::Send(&packet);
    delete[] mats;
    mats = NULL;
}


BEGIN_MESSAGE_MAP(CMaterialEditor, CDialogEx)
    ON_NOTIFY(NM_DBLCLK, IDC_MATLIST, &CMaterialEditor::OnDblclkMatlist)
    ON_WM_TIMER()
END_MESSAGE_MAP()


// CMaterialEditor 消息处理程序


BOOL CMaterialEditor::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Set style
    DWORD dwStyle = m_MaterialList.GetExtendedStyle(); 
    dwStyle |= LVS_EX_FULLROWSELECT;
    dwStyle |= LVS_EX_GRIDLINES;
    m_MaterialList.SetExtendedStyle(dwStyle);

    // Insert list header
    m_MaterialList.InsertColumn(0, "ID", LVCFMT_CENTER, 40);
    m_MaterialList.InsertColumn(1, "Name", LVCFMT_CENTER, 100);

    // Insert list content
    int32_t mat_num = glb::GlbMaterialMgr::GetMaterialCount();
    for (int32_t i = 0; i < mat_num; i++) {
        glb::GlbMaterial* mat = glb::GlbMaterialMgr::GetMaterial(i + 1);
        m_MaterialList.InsertItem(i, "");

        // ID
        CString str;
        str.Format("%d", mat->id);
        m_MaterialList.SetItemText(i, 0, str);

        // Name
        str.Empty();
        str += mat->material_name;
        m_MaterialList.SetItemText(i, 1, str);
    }

    // Set default material
    glb::GlbMaterial* mat = glb::GlbMaterialMgr::GetMaterial(1);
    m_AmbientX = mat->ambient_x;
    m_AmbientY = mat->ambient_y;
    m_AmbientZ = mat->ambient_z;
    m_DiffuseX = mat->diffuse_x;
    m_DiffuseY = mat->diffuse_y;
    m_DiffuseZ = mat->diffuse_z;
    m_SpecularX = mat->specular_x;
    m_SpecularY = mat->specular_y;
    m_SpecularZ = mat->specular_z;
    m_EmissionX = mat->emission_x;
    m_EmissionY = mat->emission_y;
    m_EmissionZ = mat->emission_z;
    m_Pow = mat->pow;
    UpdateData(FALSE);

    // Create timer
    SetTimer(1, 500, NULL);

    return TRUE;  // return TRUE unless you set the focus to a control
}


void CMaterialEditor::OnDblclkMatlist(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    *pResult = 0;
    LPNMITEMACTIVATE lpnmactive = (LPNMITEMACTIVATE)pNMHDR;
    int32_t mat_num = glb::GlbMaterialMgr::GetMaterialCount();
    if (0 <= lpnmactive->iItem && lpnmactive->iItem < mat_num) {
        m_CurSelMatItem = lpnmactive->iItem;
        glb::GlbMaterial* mat = glb::GlbMaterialMgr::GetMaterial(lpnmactive->iItem + 1);
        m_AmbientX = mat->ambient_x;
        m_AmbientY = mat->ambient_y;
        m_AmbientZ = mat->ambient_z;
        m_DiffuseX = mat->diffuse_x;
        m_DiffuseY = mat->diffuse_y;
        m_DiffuseZ = mat->diffuse_z;
        m_SpecularX = mat->specular_x;
        m_SpecularY = mat->specular_y;
        m_SpecularZ = mat->specular_z;
        m_EmissionX = mat->emission_x;
        m_EmissionY = mat->emission_y;
        m_EmissionZ = mat->emission_z;
        m_Pow = mat->pow;
        UpdateData(FALSE);
    }
}


void CMaterialEditor::SendData()
{
    // TODO: 在此添加控件通知处理程序代码
    if (IsWindowVisible()) {
        if (UpdateData(TRUE)) {
            glb::GlbMaterial mat;
            mat.id = m_CurSelMatItem + 1;
            mat.ambient_x = m_AmbientX;
            mat.ambient_y = m_AmbientY;
            mat.ambient_z = m_AmbientZ;
            mat.diffuse_x = m_DiffuseX;
            mat.diffuse_y = m_DiffuseY;
            mat.diffuse_z = m_DiffuseZ;
            mat.specular_x = m_SpecularX;
            mat.specular_y = m_SpecularY;
            mat.specular_z = m_SpecularZ;
            mat.emission_x = m_EmissionX;
            mat.emission_y = m_EmissionY;
            mat.emission_z = m_EmissionZ;
            mat.pow = m_Pow;
            glb::GlbMaterialMgr::ChangeMaterial(mat, m_CurSelMatItem + 1);

            PostMaterial();
        }
    }
}


void CMaterialEditor::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    SendData();
    CDialogEx::OnTimer(nIDEvent);
}
