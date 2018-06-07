
// glb_lightstudioDlg.h : header file
//

#pragma once

#include "tinyxml/tinyxml.h"
#include "afxcmn.h"

// CGLBLightStudioDlg dialog
class CGLBLightStudioDlg : public CDialogEx
{
// Construction
public:
	CGLBLightStudioDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_GLB_LIGHTSTUDIO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
    TiXmlDocument*   m_ProjectXML;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnKickIdle();
    afx_msg void OnClose();
    afx_msg void OnFileNew();
    afx_msg void OnFileSave();
    afx_msg void OnAddScene();
    afx_msg void OnAddLight();
	DECLARE_MESSAGE_MAP()

private:
    CListCtrl m_OutlineList;
};
