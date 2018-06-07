
// glb_lightstudioDlg.h : header file
//

#pragma once


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

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnKickIdle();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnClose();
};
