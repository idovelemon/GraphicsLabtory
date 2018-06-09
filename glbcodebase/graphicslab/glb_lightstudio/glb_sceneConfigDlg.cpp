// glb_sceneConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "glb_lightstudio.h"
#include "glb_sceneConfigDlg.h"
#include "afxdialogex.h"

#include <math.h>

// glb_sceneConfigDlg dialog

static const int kLightMapBakeTotalIterateMin = 1;
static const int kLightMapBakeTotalIterateMax = 10;

IMPLEMENT_DYNAMIC(CGLBSceneConfigDlg, CDialogEx)

CGLBSceneConfigDlg::CGLBSceneConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGLBSceneConfigDlg::IDD, pParent)
    , m_LightMapBakeTotalIterate(kLightMapBakeTotalIterateMin)
{

}

CGLBSceneConfigDlg::~CGLBSceneConfigDlg()
{
}

void CGLBSceneConfigDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_RESOLUTION_COMBO, m_ResolutionComboBox);
    DDX_Text(pDX, IDC_ITERATE_EDIT, m_LightMapBakeTotalIterate);
	DDV_MinMaxInt(pDX, m_LightMapBakeTotalIterate, kLightMapBakeTotalIterateMin, kLightMapBakeTotalIterateMax);
}


BEGIN_MESSAGE_MAP(CGLBSceneConfigDlg, CDialogEx)
    ON_NOTIFY(UDN_DELTAPOS, IDC_ITERATE_SPIN, &CGLBSceneConfigDlg::OnDeltaposIterateSpin)
END_MESSAGE_MAP()


// glb_sceneConfigDlg message handlers


BOOL CGLBSceneConfigDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  Add extra initialization here
    m_ResolutionComboBox.AddString(L"32 * 32");
    m_ResolutionComboBox.AddString(L"64 * 64");
    m_ResolutionComboBox.AddString(L"128 * 128");
    m_ResolutionComboBox.AddString(L"256 * 256");
    m_ResolutionComboBox.AddString(L"512 * 512");
    m_ResolutionComboBox.AddString(L"1024 * 1024");
    m_ResolutionComboBox.AddString(L"2048 * 2048");
    m_ResolutionComboBox.SetCurSel(0);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


void CGLBSceneConfigDlg::OnOK()
{
    // TODO: Add your specialized code here and/or call the base class

    //CDialogEx::OnOK();
}


void CGLBSceneConfigDlg::OnCancel()
{
    // TODO: Add your specialized code here and/or call the base class

    //CDialogEx::OnCancel();
}


void CGLBSceneConfigDlg::OnDeltaposIterateSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    // TODO: Add your control notification handler code here
    *pResult = 0;

    UpdateData(true);

    if (pNMUpDown->iDelta == -1)
    {
        m_LightMapBakeTotalIterate++;
        if (m_LightMapBakeTotalIterate > kLightMapBakeTotalIterateMax)
        {
            m_LightMapBakeTotalIterate = kLightMapBakeTotalIterateMax;
        }
    }
    else if (pNMUpDown->iDelta == 1)
    {
        m_LightMapBakeTotalIterate--;
        if (m_LightMapBakeTotalIterate < kLightMapBakeTotalIterateMin)
        {
            m_LightMapBakeTotalIterate = kLightMapBakeTotalIterateMin;
        }
    }

    UpdateData(false);
}

void CGLBSceneConfigDlg::UpdateConfigLightMapSize(int width, int height) {
    int sizeTbl[] = {32, 64, 128, 256, 512, 1024, 2048};
    int sel = 0;
    for (int i = 0; i < sizeof(sizeTbl) / sizeof(sizeTbl[0]); i++)
    {
        if (sizeTbl[i] == width)  // Assume width == height
        {
            sel = i;
            break;
        }
    }

    m_ResolutionComboBox.SetCurSel(sel);
    UpdateData(false);
}

void CGLBSceneConfigDlg::UpdateConfigLightIterate(int iterate) {
    m_LightMapBakeTotalIterate = iterate;
    UpdateData(false);
}

void CGLBSceneConfigDlg::GetConfigLightMapSize(int& width, int& height)
{
    UpdateData(true);
    int sel = m_ResolutionComboBox.GetCurSel();
    int sizeTbl[] = {32, 64, 128, 256, 512, 1024, 2048};
    width = sizeTbl[sel];
    height = sizeTbl[sel];
}

int CGLBSceneConfigDlg::GetConfigLightMapIterate()
{
    UpdateData(true);
    return m_LightMapBakeTotalIterate;
}
