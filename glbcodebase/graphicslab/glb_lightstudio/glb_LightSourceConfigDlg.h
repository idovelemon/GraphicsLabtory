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

public:
    float GetLightSourcePosX() const { return m_LightSourcePosX; }
    float GetLightSourcePosY() const { return m_LightSourcePosY; }
    float GetLightSourcePosZ() const { return m_LightSourcePosZ; }
    float GetLightSourceRotX() const { return m_LightSourceRotX; }
    float GetLightSourceRotY() const { return m_LightSourceRotY; }
    float GetLightSourceRotZ() const { return m_LightSourceRotZ; }
    float GetLightSourceScaleX() const { return m_LightSourceScaleX; }
    float GetLightSourceScaleY() const { return m_LightSourceScaleY; }
    float GetLightSourceScaleZ() const { return m_LightSourceScaleZ; }
    float GetLightSourceColorX() const { return m_LightSourceColorX; }
    float GetLightSourceColorY() const { return m_LightSourceColorY; }
    float GetLightSourceColorZ() const { return m_LightSourceColorZ; }
    void UpdateLightSourcePos(float x, float y, float z);
    void UpdateLightSourceRot(float x, float y, float z);
    void UpdateLightSourceScale(float x, float y, float z);
    void UpdateLightSourceColor(float x, float y, float z);
};
