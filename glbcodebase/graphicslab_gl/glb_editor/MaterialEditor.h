#pragma once
#include "afxcmn.h"

#include <stdint.h>


// CMaterialEditor 对话框

class CMaterialEditor : public CDialogEx
{
	DECLARE_DYNAMIC(CMaterialEditor)

public:
	CMaterialEditor(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMaterialEditor();

// 对话框数据
	enum { IDD = IDD_MATEDITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    void PostMaterial();

	DECLARE_MESSAGE_MAP()
private:
    CListCtrl m_MaterialList;
    float m_AmbientX;
    float m_AmbientY;
    float m_AmbientZ;
    float m_DiffuseX;
    float m_DiffuseY;
    float m_DiffuseZ;
    float m_SpecularX;
    float m_SpecularY;
    float m_SpecularZ;
    float m_EmissionX;
    float m_EmissionY;
    float m_EmissionZ;
    float m_Pow;
    int32_t m_CurSelMatItem;

public:
    virtual BOOL OnInitDialog();
    void SendData();
    afx_msg void OnDblclkMatlist(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};
