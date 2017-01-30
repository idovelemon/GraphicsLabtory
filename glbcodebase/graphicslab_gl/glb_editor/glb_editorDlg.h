
// glb_editorDlg.h : header file
//

#pragma once

#include "MaterialEditor.h"

// CGlbEditorDlg dialog
class CGlbEditorDlg : public CDialogEx
{
// Construction
public:
	CGlbEditorDlg(CWnd* pParent = NULL);	// standard constructor
    virtual ~CGlbEditorDlg();

// Dialog Data
	enum { IDD = IDD_GLB_EDITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
    CMaterialEditor* m_MaterialDlg;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedMateditor();
};
