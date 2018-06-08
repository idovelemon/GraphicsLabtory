// glb_LightSourceConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "glb_lightstudio.h"
#include "glb_LightSourceConfigDlg.h"
#include "afxdialogex.h"

#include "glb_lightstudio_core.h"


// CGLBLightSourceConfigDlg dialog

IMPLEMENT_DYNAMIC(CGLBLightSourceConfigDlg, CDialogEx)

CGLBLightSourceConfigDlg::CGLBLightSourceConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGLBLightSourceConfigDlg::IDD, pParent)
    , m_LightSourcePosX(0)
    , m_LightSourcePosY(0)
    , m_LightSourcePosZ(0)
    , m_LightSourceRotX(0)
    , m_LightSourceRotY(0)
    , m_LightSourceRotZ(0)
    , m_LightSourceScaleX(1.0f)
    , m_LightSourceScaleY(1.0f)
    , m_LightSourceScaleZ(1.0f)
    , m_LightSourceColorX(0)
    , m_LightSourceColorY(0)
    , m_LightSourceColorZ(0)
{

}

CGLBLightSourceConfigDlg::~CGLBLightSourceConfigDlg()
{
}

void CGLBLightSourceConfigDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_POS_X_EDIT, m_LightSourcePosX);
    DDX_Text(pDX, IDC_POS_Y_EDIT, m_LightSourcePosY);
    DDX_Text(pDX, IDC_POS_Z_EDIT, m_LightSourcePosZ);
    DDX_Text(pDX, IDC_ROT_X_EDIT, m_LightSourceRotX);
    DDX_Text(pDX, IDC_ROT_Y_EDIT, m_LightSourceRotY);
    DDX_Text(pDX, IDC_ROT_Z_EDIT, m_LightSourceRotZ);
    DDX_Text(pDX, IDC_SCALE_X_EDIT, m_LightSourceScaleX);
    DDX_Text(pDX, IDC_SCALE_Y_EDIT, m_LightSourceScaleY);
    DDX_Text(pDX, IDC_SCALE_Z_EDIT, m_LightSourceScaleZ);
    DDX_Text(pDX, IDC_COLOR_X_EDIT, m_LightSourceColorX);
    DDX_Text(pDX, IDC_COLOR_Y_EDIT, m_LightSourceColorY);
    DDX_Text(pDX, IDC_COLOR_Z_EDIT, m_LightSourceColorZ);
}

void CGLBLightSourceConfigDlg::UpdateLightSourceInfo()
{
    CListCtrl* list = static_cast<CListCtrl*>(GetParent()->GetDlgItem(IDC_OUTLINE_LIST));
    POSITION pos = list->GetFirstSelectedItemPosition();
    if (pos != NULL)
    {
        int index = list->GetNextSelectedItem(pos);
        CString lightSourceId = list->GetItemText(index, 1);

        char *pcstr = (char *)new char[2 * wcslen(lightSourceId.GetBuffer(0))+1] ;
        memset(pcstr , 0 , 2 * wcslen(lightSourceId.GetBuffer(0))+1 );
        wcstombs(pcstr, lightSourceId.GetBuffer(0), wcslen(lightSourceId.GetBuffer(0)));
        int id = atoi(pcstr);

        ApplicationCore::GetInstance()->SetLightSourcePos(id, m_LightSourcePosX, m_LightSourcePosY, m_LightSourcePosZ);
        ApplicationCore::GetInstance()->SetLightSourceRot(id, m_LightSourceRotX, m_LightSourceRotY, m_LightSourceRotZ);
        ApplicationCore::GetInstance()->SetLightSourceScale(id, m_LightSourceScaleX, m_LightSourceScaleY, m_LightSourceScaleZ);
        ApplicationCore::GetInstance()->SetLightSourceColor(id, m_LightSourceColorX, m_LightSourceColorY, m_LightSourceColorZ);

        delete[] pcstr;
        pcstr = NULL;
    }
    else
    {
        assert(false);  // Must have one line selected
    }
}

BEGIN_MESSAGE_MAP(CGLBLightSourceConfigDlg, CDialogEx)
END_MESSAGE_MAP()


// CGLBLightSourceConfigDlg message handlers


void CGLBLightSourceConfigDlg::OnCancel()
{
    // TODO: Add your specialized code here and/or call the base class

    //CDialogEx::OnCancel();
}


void CGLBLightSourceConfigDlg::OnOK()
{
    // TODO: Add your specialized code here and/or call the base class

    //CDialogEx::OnOK();
}


BOOL CGLBLightSourceConfigDlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
    {
        int editID[] = 
        {
            IDC_POS_X_EDIT, IDC_POS_Y_EDIT, IDC_POS_Z_EDIT,
            IDC_ROT_X_EDIT, IDC_ROT_Y_EDIT, IDC_ROT_Z_EDIT,
            IDC_SCALE_X_EDIT, IDC_SCALE_Y_EDIT, IDC_SCALE_Z_EDIT,
            IDC_COLOR_X_EDIT, IDC_COLOR_Y_EDIT, IDC_COLOR_Z_EDIT,
        };
        bool isEditorControl = false;
        for (int i = 0; i < sizeof(editID) / sizeof(editID[0]); i++)
        {
            if (GetDlgItem(editID[i])->m_hWnd == pMsg->hwnd && pMsg->wParam == VK_RETURN)
            {
                isEditorControl = true;
                break;
            }
        }

        if (isEditorControl)
        {
            UpdateData(true);
            UpdateLightSourceInfo();
        }
    }

    return CDialogEx::PreTranslateMessage(pMsg);
}
