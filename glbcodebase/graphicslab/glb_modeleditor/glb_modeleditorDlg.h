
// glb_modeleditorDlg.h : header file
//

#pragma once


// Cglb_modeleditorDlg dialog
class Cglb_modeleditorDlg : public CDialog
{
// Construction
public:
	Cglb_modeleditorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_GLB_MODELEDITOR_DIALOG };

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
    afx_msg void OnBnClickedOk();
    afx_msg void OnFileImport();
    afx_msg void OnFileExport();
    afx_msg void OnFilePreview();
    afx_msg void OnBnClickedAlbedoFileButton();
    afx_msg void OnBnClickedRoughnessFileButton();
    afx_msg void OnBnClickedMetallicFileButton();
    afx_msg void OnBnClickedNormalFileButton();
    afx_msg void OnBnClickedAlphaFileButton();
    afx_msg void OnBnClickedEmissionFileButton();
    afx_msg void OnBnClickedEnvFileButton();
    afx_msg void OnBnClickedLight0FileButton();
    afx_msg void OnBnClickedLight1FileButton();
    afx_msg void OnBnClickedLight2FileButton();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

private:
    CString m_AlbedoTexName;
    CString m_RoughnessTexName;
    CString m_MetallicTexName;
    CString m_NormalTexName;
    CString m_AlphaTexName;
    CString m_EmissionTexName;
    CString m_EnvTexName;
    CString m_LightTex0Name;
    CString m_LightTex1Name;
    CString m_LightTex2Name;
};
