
// glb_lightstudioDlg.h : header file
//

#pragma once

#include "tinyxml/tinyxml.h"
#include "afxcmn.h"
#include "glb_sceneConfigDlg.h"
#include "glb_LightSourceConfigDlg.h"

#include <map>

// CGLBLightStudioDlg dialog
class CGLBLightStudioDlg : public CDialog
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

    void ClearProject();
    void SaveProperties();
    void AddSceneMesh(const char* name);
    int AddLightMesh(int lightID, const char* name);

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnKickIdle();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnClose();
    afx_msg void OnFileOpen();
    afx_msg void OnFileNew();
    afx_msg void OnFileSave();
    afx_msg void OnFileExportlightmap();
    afx_msg void OnAddScene();
    afx_msg void OnAddLight();
    afx_msg void OnBnClickedBakeButton();
    afx_msg void OnBnClickedBakeCancelButton();
    afx_msg void OnNMCustomdrawOutlineList(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnHdnItemclickOutlineList(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMClickOutlineList(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()

private:
    CListCtrl m_OutlineList;
    CGLBSceneConfigDlg* m_SceneConfigDlg;
    typedef std::map<int, CGLBLightSourceConfigDlg*> LightSourceDlgArray;
    LightSourceDlgArray m_LightSourceConfigDlgs;
    CWnd*   m_CurDispConfigDlg;
};
