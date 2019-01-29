// AddPassDlg.cpp : implementation file
//

#include "stdafx.h"
#include "glb_modeleditor.h"
#include "AddPassDlg.h"
#include "afxdialogex.h"


// CAddPassDlg dialog

IMPLEMENT_DYNAMIC(CAddPassDlg, CDialogEx)

CAddPassDlg::CAddPassDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddPassDlg::IDD, pParent)
    , m_VertexShaderName(_T(""))
    , m_FragmentShaderName(_T(""))
    , m_PassName(_T(""))
{

}

CAddPassDlg::~CAddPassDlg()
{
}

void CAddPassDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_VERTEXSHADERNAME_EDIT, m_VertexShaderName);
    DDX_Text(pDX, IDC_FRAGMENTSHADERNAME_EDIT, m_FragmentShaderName);
    DDX_CBString(pDX, IDC_PASSNAME_COMBO, m_PassName);
    DDX_Control(pDX, IDC_PASSNAME_COMBO, m_PassNameCtrl);
}


BEGIN_MESSAGE_MAP(CAddPassDlg, CDialogEx)
    ON_BN_CLICKED(IDOK, &CAddPassDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CAddPassDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CAddPassDlg message handlers


void CAddPassDlg::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    CDialogEx::OnOK();
}


void CAddPassDlg::OnBnClickedCancel()
{
    // TODO: Add your control notification handler code here
    CDialogEx::OnCancel();
}


BOOL CAddPassDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    m_PassNameCtrl.InsertString(0, CString(TEXT("lightloop")));
    m_PassNameCtrl.InsertString(1, CString(TEXT("shadow")));
    m_PassNameCtrl.InsertString(2, CString(TEXT("decal")));

    return TRUE;  // return TRUE unless you set the focus to a control
}


CString CAddPassDlg::GetPassName()
{
    return m_PassName;
}

CString CAddPassDlg::GetVertexShaderName()
{
    return m_VertexShaderName;
}


CString CAddPassDlg::GetFragmentShaderName()
{
    return m_FragmentShaderName;
}
