
// glb_modeleditorDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include <map>
#include <vector>
#include "glb.h"
#include "afxpropertygridctrl.h"

// Material struct
struct SMaterial
{
    CString materialName;
    CArray<CString> passName;
    CArray<CString> vertexShaderName;
    CArray<CString> fragmentShaderName;
    std::vector<std::vector<glb::render::material::PassMaterial::ParameterEntry>> passParameters;

    SMaterial()
        : materialName(TEXT(""))
    {
    }
};

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

public:
    void OnPassParamFloatUpdate(int passIndex, int paramIndex, float value);

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
    afx_msg void OnMaterialAdd();
    afx_msg void OnMaterialSave();
    afx_msg void OnMaterialAddexsit();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnNMRClickMattree(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMClickMattree(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnMaterialAddpass();
    afx_msg void OnPassCompile();
    afx_msg LRESULT OnPropertyChanged(WPARAM wparam, LPARAM lparam);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	DECLARE_MESSAGE_MAP()

protected:
    bool LoadFileToRender(const char* materialFilePath);
    void BuildMaterial(CString fileName, const char* filePath);
    bool CheckIsAllFileReady(CString filePath);
    void CopyFileToWorkSpace(CString filePath);
    void LoadFileToEditor();

    void RefreshMaterialParameters();
    void RefreshPassParameters();

private:
    CTreeCtrl m_MatTreeCtrl;
    CMFCPropertyGridCtrl m_PassParamGridCtrl;
    SMaterial m_MaterialInfo;
    CString m_WorkSpaceDirectory;

    CMFCPropertyGridProperty* m_CurSelProperty;

    std::map<CString, HTREEITEM>    m_TreeItemTbl;
    int                             m_ChoosePass;
};
