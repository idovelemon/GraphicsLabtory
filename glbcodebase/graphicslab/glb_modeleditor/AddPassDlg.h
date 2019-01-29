#pragma once
#include "afxwin.h"


// CAddPassDlg dialog

class CAddPassDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddPassDlg)

public:
	CAddPassDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddPassDlg();

// Dialog Data
	enum { IDD = IDD_ADD_PASS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();

public:
    virtual BOOL OnInitDialog();

public:
    CString GetPassName();
    CString GetVertexShaderName();
    CString GetFragmentShaderName();

private:
    CString m_VertexShaderName;
    CString m_FragmentShaderName;
    CString m_PassName;
    CComboBox m_PassNameCtrl;
};
