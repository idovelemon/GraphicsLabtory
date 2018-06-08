#pragma once
#include "afxwin.h"


// CGLBSceneConfigDlg dialog

class CGLBSceneConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGLBSceneConfigDlg)

public:
	CGLBSceneConfigDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGLBSceneConfigDlg();

// Dialog Data
	enum { IDD = IDD_SCENE_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnDeltaposIterateSpin(NMHDR *pNMHDR, LRESULT *pResult);

public:
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();

public:
    void GetConfigLightMapSize(int& width, int& height);
    int GetConfigLightMapIterate();

private:
    CComboBox m_ResolutionComboBox;
    int m_LightMapBakeTotalIterate;
};
