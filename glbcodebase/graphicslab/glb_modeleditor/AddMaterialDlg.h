#pragma once


// CAddMaterialDlg dialog

class CAddMaterialDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddMaterialDlg)

public:
	CAddMaterialDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddMaterialDlg();

// Dialog Data
	enum { IDD = IDD_ADD_MATERIAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();

public:
    CString GetMaterialName();

private:
    CString m_MaterialName;
};
