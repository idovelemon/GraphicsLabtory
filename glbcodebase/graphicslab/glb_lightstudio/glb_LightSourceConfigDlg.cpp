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
    , m_LightSourceScaleX(0)
    , m_LightSourceScaleY(0)
    , m_LightSourceScaleZ(0)
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
    int line = list->GetSelectionMark();
    if (line != -1)
    {
        CString lightSourcePath = list->GetItemText(line, 1);

        char *pcstr = (char *)new char[2 * wcslen(lightSourcePath.GetBuffer(0))+1] ;
        memset(pcstr , 0 , 2 * wcslen(lightSourcePath.GetBuffer(0))+1 );
        wcstombs(pcstr, lightSourcePath.GetBuffer(0), wcslen(lightSourcePath.GetBuffer(0))) ;

        ApplicationCore::GetInstance()->SetLightSourcePos(pcstr, m_LightSourcePosX, m_LightSourcePosX, m_LightSourcePosZ);
        ApplicationCore::GetInstance()->SetLightSourceRot(pcstr, m_LightSourceRotX, m_LightSourceRotX, m_LightSourceRotZ);
        ApplicationCore::GetInstance()->SetLightSourceScale(pcstr, m_LightSourceScaleX, m_LightSourceScaleX, m_LightSourceScaleZ);
        ApplicationCore::GetInstance()->SetLightSourceColor(pcstr, m_LightSourceColorX, m_LightSourceColorX, m_LightSourceColorZ);

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
            if (GetDlgItem(editID[i])->m_hWnd == pMsg->hwnd)
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
