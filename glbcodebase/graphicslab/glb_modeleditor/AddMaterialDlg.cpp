// AddMaterialDlg.cpp : implementation file
//

#include "stdafx.h"
#include "glb_modeleditor.h"
#include "AddMaterialDlg.h"
#include "afxdialogex.h"


// CAddMaterialDlg dialog

IMPLEMENT_DYNAMIC(CAddMaterialDlg, CDialogEx)

CAddMaterialDlg::CAddMaterialDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddMaterialDlg::IDD, pParent)
    , m_MaterialName(_T(""))
{

}

CAddMaterialDlg::~CAddMaterialDlg()
{
}

void CAddMaterialDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_MATNAME_EDIT, m_MaterialName);
}


BEGIN_MESSAGE_MAP(CAddMaterialDlg, CDialogEx)
    ON_BN_CLICKED(IDOK, &CAddMaterialDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CAddMaterialDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CAddMaterialDlg message handlers


void CAddMaterialDlg::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    CDialogEx::OnOK();
}


void CAddMaterialDlg::OnBnClickedCancel()
{
    // TODO: Add your control notification handler code here
    CDialogEx::OnCancel();
}


CString CAddMaterialDlg::GetMaterialName()
{
    return m_MaterialName;
}
