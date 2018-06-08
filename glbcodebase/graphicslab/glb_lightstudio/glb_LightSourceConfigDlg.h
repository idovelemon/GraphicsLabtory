#pragma once


// CGLBLightSourceConfigDlg dialog

class CGLBLightSourceConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGLBLightSourceConfigDlg)

public:
	CGLBLightSourceConfigDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGLBLightSourceConfigDlg();

// Dialog Data
	enum { IDD = IDD_LIGHT_SOURCE_CONFIG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    void UpdateLightSourceInfo();

	DECLARE_MESSAGE_MAP()
    virtual void OnCancel();
    virtual void OnOK();
private:
    float m_LightSourcePosX;
    float m_LightSourcePosY;
    float m_LightSourcePosZ;
    float m_LightSourceRotX;
    float m_LightSourceRotY;
    float m_LightSourceRotZ;
    float m_LightSourceScaleX;
    float m_LightSourceScaleY;
    float m_LightSourceScaleZ;
    float m_LightSourceColorX;
    float m_LightSourceColorY;
    float m_LightSourceColorZ;

public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};
