
// glb_modeleditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "glb_modeleditor.h"
#include "glb_modeleditorDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cglb_modeleditorDlg dialog



Cglb_modeleditorDlg::Cglb_modeleditorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cglb_modeleditorDlg::IDD, pParent)
    , m_AlbedoTexName(_T(""))
    , m_RoughnessTexName(_T(""))
    , m_MetallicTexName(_T(""))
    , m_NormalTexName(_T(""))
    , m_AlphaTexName(_T(""))
    , m_EmissionTexName(_T(""))
    , m_EnvTexName(_T(""))
    , m_LightTex0Name(_T(""))
    , m_LightTex1Name(_T(""))
    , m_LightTex2Name(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cglb_modeleditorDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_ALBEDO_EDIT, m_AlbedoTexName);
    DDX_Text(pDX, IDC_ROUGHNESS_EDIT, m_RoughnessTexName);
    DDX_Text(pDX, IDC_METALLIC_EDIT, m_MetallicTexName);
    DDX_Text(pDX, IDC_NORMAL_EDIT, m_NormalTexName);
    DDX_Text(pDX, IDC_ALPHA_EDIT, m_AlphaTexName);
    DDX_Text(pDX, IDC_EMISSION_EDIT, m_EmissionTexName);
    DDX_Text(pDX, IDC_ENV_EDIT, m_EnvTexName);
    DDX_Text(pDX, IDC_LIGHT_0_EDIT, m_LightTex0Name);
    DDX_Text(pDX, IDC_LIGHT_1_EDIT, m_LightTex1Name);
    DDX_Text(pDX, IDC_LIGHT_2_EDIT, m_LightTex2Name);
}

BEGIN_MESSAGE_MAP(Cglb_modeleditorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDOK, &Cglb_modeleditorDlg::OnBnClickedOk)
    ON_COMMAND(ID_FILE_IMPORT, &Cglb_modeleditorDlg::OnFileImport)
    ON_COMMAND(ID_FILE_EXPORT, &Cglb_modeleditorDlg::OnFileExport)
    ON_BN_CLICKED(IDC_ALBEDO_FILE_BUTTON, &Cglb_modeleditorDlg::OnBnClickedAlbedoFileButton)
    ON_BN_CLICKED(IDC_ROUGHNESS_FILE_BUTTON, &Cglb_modeleditorDlg::OnBnClickedRoughnessFileButton)
    ON_BN_CLICKED(IDC_METALLIC_FILE_BUTTON, &Cglb_modeleditorDlg::OnBnClickedMetallicFileButton)
    ON_BN_CLICKED(IDC_NORMAL_FILE_BUTTON, &Cglb_modeleditorDlg::OnBnClickedNormalFileButton)
    ON_BN_CLICKED(IDC_ALPHA_FILE_BUTTON, &Cglb_modeleditorDlg::OnBnClickedAlphaFileButton)
    ON_BN_CLICKED(IDC_EMISSION_FILE_BUTTON, &Cglb_modeleditorDlg::OnBnClickedEmissionFileButton)
    ON_BN_CLICKED(IDC_ENV_FILE_BUTTON, &Cglb_modeleditorDlg::OnBnClickedEnvFileButton)
    ON_BN_CLICKED(IDC_LIGHT_0_FILE_BUTTON, &Cglb_modeleditorDlg::OnBnClickedLight0FileButton)
    ON_BN_CLICKED(IDC_LIGHT_1_FILE_BUTTON, &Cglb_modeleditorDlg::OnBnClickedLight1FileButton)
    ON_BN_CLICKED(IDC_LIGHT_2_FILE_BUTTON, &Cglb_modeleditorDlg::OnBnClickedLight2FileButton)
END_MESSAGE_MAP()


// Cglb_modeleditorDlg message handlers

BOOL Cglb_modeleditorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
    // Disable menu
    GetMenu()->EnableMenuItem(ID_FILE_IMPORT, MF_ENABLED);
    GetMenu()->EnableMenuItem(ID_FILE_EXPORT, MF_DISABLED);

    // Disable button
    GetDlgItem(IDC_ALBEDO_FILE_BUTTON)->EnableWindow(FALSE);
    GetDlgItem(IDC_ROUGHNESS_FILE_BUTTON)->EnableWindow(FALSE);
    GetDlgItem(IDC_METALLIC_FILE_BUTTON)->EnableWindow(FALSE);
    GetDlgItem(IDC_NORMAL_FILE_BUTTON)->EnableWindow(FALSE);
    GetDlgItem(IDC_ALPHA_FILE_BUTTON)->EnableWindow(FALSE);
    GetDlgItem(IDC_EMISSION_FILE_BUTTON)->EnableWindow(FALSE);
    GetDlgItem(IDC_ENV_FILE_BUTTON)->EnableWindow(FALSE);
    GetDlgItem(IDC_LIGHT_0_FILE_BUTTON)->EnableWindow(FALSE);
    GetDlgItem(IDC_LIGHT_1_FILE_BUTTON)->EnableWindow(FALSE);
    GetDlgItem(IDC_LIGHT_2_FILE_BUTTON)->EnableWindow(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void Cglb_modeleditorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Cglb_modeleditorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR Cglb_modeleditorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cglb_modeleditorDlg::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    CDialogEx::OnOK();
}


void Cglb_modeleditorDlg::OnFileImport()
{
    // TODO: Add your command handler code here
    // Disable menu
    GetMenu()->EnableMenuItem(ID_FILE_EXPORT, MF_ENABLED);

    // Disable button
    GetDlgItem(IDC_ALBEDO_FILE_BUTTON)->EnableWindow(TRUE);
    GetDlgItem(IDC_ROUGHNESS_FILE_BUTTON)->EnableWindow(TRUE);
    GetDlgItem(IDC_METALLIC_FILE_BUTTON)->EnableWindow(TRUE);
    GetDlgItem(IDC_NORMAL_FILE_BUTTON)->EnableWindow(TRUE);
    GetDlgItem(IDC_ALPHA_FILE_BUTTON)->EnableWindow(TRUE);
    GetDlgItem(IDC_EMISSION_FILE_BUTTON)->EnableWindow(TRUE);
    GetDlgItem(IDC_ENV_FILE_BUTTON)->EnableWindow(TRUE);
    GetDlgItem(IDC_LIGHT_0_FILE_BUTTON)->EnableWindow(TRUE);
    GetDlgItem(IDC_LIGHT_1_FILE_BUTTON)->EnableWindow(TRUE);
    GetDlgItem(IDC_LIGHT_2_FILE_BUTTON)->EnableWindow(TRUE);
}


void Cglb_modeleditorDlg::OnFileExport()
{
    // TODO: Add your command handler code here
}


void Cglb_modeleditorDlg::OnBnClickedAlbedoFileButton()
{
    // TODO: Add your control notification handler code here
}


void Cglb_modeleditorDlg::OnBnClickedRoughnessFileButton()
{
    // TODO: Add your control notification handler code here
}


void Cglb_modeleditorDlg::OnBnClickedMetallicFileButton()
{
    // TODO: Add your control notification handler code here
}


void Cglb_modeleditorDlg::OnBnClickedNormalFileButton()
{
    // TODO: Add your control notification handler code here
}


void Cglb_modeleditorDlg::OnBnClickedAlphaFileButton()
{
    // TODO: Add your control notification handler code here
}


void Cglb_modeleditorDlg::OnBnClickedEmissionFileButton()
{
    // TODO: Add your control notification handler code here
}


void Cglb_modeleditorDlg::OnBnClickedEnvFileButton()
{
    // TODO: Add your control notification handler code here
}


void Cglb_modeleditorDlg::OnBnClickedLight0FileButton()
{
    // TODO: Add your control notification handler code here
}


void Cglb_modeleditorDlg::OnBnClickedLight1FileButton()
{
    // TODO: Add your control notification handler code here
}


void Cglb_modeleditorDlg::OnBnClickedLight2FileButton()
{
    // TODO: Add your control notification handler code here
}
