
// glb_modeleditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "glb_modeleditor.h"
#include "glb_modeleditorDlg.h"
#include "afxdialogex.h"
#include "glb.h"
#include "glb_modeleditor_core.h"

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
	: CDialog(Cglb_modeleditorDlg::IDD, pParent)
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
    , m_DiffusePFCTexName(_T(""))
    , m_SpecularPFCTexName(_T(""))
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void Cglb_modeleditorDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_ALBEDO_EDIT, m_AlbedoTexName);
    DDX_Text(pDX, IDC_ROUGHNESS_EDIT, m_RoughnessTexName);
    DDX_Text(pDX, IDC_METALLIC_EDIT, m_MetallicTexName);
    DDX_Text(pDX, IDC_NORMAL_EDIT, m_NormalTexName);
    DDX_Text(pDX, IDC_ALPHA_EDIT, m_AlphaTexName);
    DDX_Text(pDX, IDC_EMISSION_EDIT, m_EmissionTexName);
    DDX_Text(pDX, IDC_LIGHT_0_EDIT, m_LightTex0Name);
    DDX_Text(pDX, IDC_LIGHT_1_EDIT, m_LightTex1Name);
    DDX_Text(pDX, IDC_LIGHT_2_EDIT, m_LightTex2Name);
    DDX_Text(pDX, IDC_DIFFUSE_PFC_EDIT, m_DiffusePFCTexName);
    DDX_Text(pDX, IDC_SPECULAR_PFC_EDIT, m_SpecularPFCTexName);
}

BEGIN_MESSAGE_MAP(Cglb_modeleditorDlg, CDialog)
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
    ON_BN_CLICKED(IDC_DIFFUSE_PFC_FILE_BUTTON, &Cglb_modeleditorDlg::OnBnClickedDiffusePFCFileButton)
    ON_BN_CLICKED(IDC_SPECULAR_PFC_FILE_BUTTON, &Cglb_modeleditorDlg::OnBnClickedDiffusePFCFileButton)
    ON_BN_CLICKED(IDC_LIGHT_0_FILE_BUTTON, &Cglb_modeleditorDlg::OnBnClickedLight0FileButton)
    ON_BN_CLICKED(IDC_LIGHT_1_FILE_BUTTON, &Cglb_modeleditorDlg::OnBnClickedLight1FileButton)
    ON_BN_CLICKED(IDC_LIGHT_2_FILE_BUTTON, &Cglb_modeleditorDlg::OnBnClickedLight2FileButton)
    ON_WM_TIMER()
    ON_COMMAND(ID_FILE_PREVIEW, &Cglb_modeleditorDlg::OnFilePreview)
    ON_EN_SETFOCUS(IDC_ALBEDO_EDIT, &Cglb_modeleditorDlg::OnEnSetfocusAlbedoEdit)
    ON_EN_SETFOCUS(IDC_ROUGHNESS_EDIT, &Cglb_modeleditorDlg::OnEnSetfocusRoughnessEdit)
    ON_EN_SETFOCUS(IDC_METALLIC_EDIT, &Cglb_modeleditorDlg::OnEnSetfocusMetallicEdit)
    ON_EN_SETFOCUS(IDC_NORMAL_EDIT, &Cglb_modeleditorDlg::OnEnSetfocusNormalEdit)
    ON_EN_SETFOCUS(IDC_ALPHA_EDIT, &Cglb_modeleditorDlg::OnEnSetfocusAlphaEdit)
    ON_EN_SETFOCUS(IDC_EMISSION_EDIT, &Cglb_modeleditorDlg::OnEnSetfocusEmissionEdit)
    ON_EN_SETFOCUS(IDC_DIFFUSE_PFC_EDIT, &Cglb_modeleditorDlg::OnEnSetfocusDiffusePFCEdit)
    ON_EN_SETFOCUS(IDC_SPECULAR_PFC_EDIT, &Cglb_modeleditorDlg::OnEnSetfocusSpecularPFCEdit)
    ON_EN_SETFOCUS(IDC_LIGHT_0_EDIT, &Cglb_modeleditorDlg::OnEnSetfocusLight0Edit)
    ON_EN_SETFOCUS(IDC_LIGHT_1_EDIT, &Cglb_modeleditorDlg::OnEnSetfocusLight1Edit)
    ON_EN_SETFOCUS(IDC_LIGHT_2_EDIT, &Cglb_modeleditorDlg::OnEnSetfocusLight2Edit)
END_MESSAGE_MAP()


// Cglb_modeleditorDlg message handlers

BOOL Cglb_modeleditorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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
    // Add timer
    SetTimer(1, 1, NULL);

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
    GetDlgItem(IDC_DIFFUSE_PFC_FILE_BUTTON)->EnableWindow(FALSE);
    GetDlgItem(IDC_SPECULAR_PFC_FILE_BUTTON)->EnableWindow(FALSE);
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
		CDialog::OnSysCommand(nID, lParam);
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
		CDialog::OnPaint();
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
    CDialog::OnOK();
}


void Cglb_modeleditorDlg::OnFileImport()
{
    // TODO: Add your command handler code here
    TCHAR szFilter[] = L"FBX File(*.fbx)|*.fbx|Obj File(*.obj)|*.obj|All File(*.*)|*.*||";
    CFileDialog fileDlg(TRUE, L"", L"", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
    if (IDOK == fileDlg.DoModal())
    {
        CString filePath = fileDlg.GetPathName();

        // Disable menu
        GetMenu()->EnableMenuItem(ID_FILE_EXPORT, MF_ENABLED);
        GetMenu()->EnableMenuItem(ID_FILE_PREVIEW, MF_DISABLED);

        // Disable button
        GetDlgItem(IDC_ALBEDO_FILE_BUTTON)->EnableWindow(TRUE);
        GetDlgItem(IDC_ROUGHNESS_FILE_BUTTON)->EnableWindow(TRUE);
        GetDlgItem(IDC_METALLIC_FILE_BUTTON)->EnableWindow(TRUE);
        GetDlgItem(IDC_NORMAL_FILE_BUTTON)->EnableWindow(TRUE);
        GetDlgItem(IDC_ALPHA_FILE_BUTTON)->EnableWindow(TRUE);
        GetDlgItem(IDC_EMISSION_FILE_BUTTON)->EnableWindow(TRUE);
        GetDlgItem(IDC_DIFFUSE_PFC_FILE_BUTTON)->EnableWindow(TRUE);
        GetDlgItem(IDC_SPECULAR_PFC_FILE_BUTTON)->EnableWindow(TRUE);
        GetDlgItem(IDC_LIGHT_0_FILE_BUTTON)->EnableWindow(TRUE);
        GetDlgItem(IDC_LIGHT_1_FILE_BUTTON)->EnableWindow(TRUE);
        GetDlgItem(IDC_LIGHT_2_FILE_BUTTON)->EnableWindow(TRUE);

        // Initialize glb
        glb::app::AppConfig config;
        config.wnd = GetDlgItem(IDC_VIEW)->GetSafeHwnd();
        RECT rect;
        GetDlgItem(IDC_VIEW)->GetClientRect(&rect);
        config.screen_width = rect.right - rect.left;
        config.screen_height = rect.bottom - rect.top;
        config.shadow_map_width = 32;
        config.shadow_map_height = 32;
        config.decalMapWidth = 32;
        config.decalMapHeight = 32;
        if (!glb::app::Application::Initialize(ApplicationCore::Create, AfxGetInstanceHandle(), config))
        {
            ::MessageBox(NULL, L"Initialize GLB library failed", L"ERROR", MB_OK);
            exit(0);
        }

        // Try add model
        char *pcstr = (char *)new char[2 * wcslen(filePath.GetBuffer(0))+1] ;
        memset(pcstr , 0 , 2 * wcslen(filePath.GetBuffer(0))+1 );
        wcstombs(pcstr, filePath.GetBuffer(0), wcslen(filePath.GetBuffer(0))) ;

        if (!ApplicationCore::GetInstance()->AddModel(pcstr))
        {
            ::MessageBox(NULL, L"Invalid file format", L"ERROR", MB_OK);
            exit(0);
        }

        // Display default diffuse and specular prefilter cube map
        m_DiffusePFCTexName = CString("N/A");
        m_SpecularPFCTexName = CString("N/A");
        UpdateData(false);

        delete[] pcstr;
        pcstr = NULL;
    }
}


void Cglb_modeleditorDlg::OnFileExport()
{
    // TODO: Add your command handler code here
    TCHAR szFilter[] = L"OBJ File(*.obj)||";
    CFileDialog fileDlg(FALSE, L"obj", L"Untitled.obj", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
    if (IDOK == fileDlg.DoModal())
    {
        CString filePath = fileDlg.GetPathName();

        // Try save model
        char *pcstr = (char *)new char[2 * wcslen(filePath.GetBuffer(0))+1] ;
        memset(pcstr , 0 , 2 * wcslen(filePath.GetBuffer(0))+1 );
        wcstombs(pcstr, filePath.GetBuffer(0), wcslen(filePath.GetBuffer(0))) ;

        if (!ApplicationCore::GetInstance()->SaveModel(pcstr))
        {
            ::MessageBox(NULL, L"Export model failed", L"ERROR", MB_OK);
            exit(0);
        }

        delete[] pcstr;
        pcstr = NULL;
    }
}


void Cglb_modeleditorDlg::OnFilePreview()
{
    // TODO: Add your command handler code here
    TCHAR szFilter[] = L"Obj File(*.obj)|*.obj||";
    CFileDialog fileDlg(TRUE, L"", L"", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
    if (IDOK == fileDlg.DoModal())
    {
        CString filePath = fileDlg.GetPathName();

        // Disable menu
        GetMenu()->EnableMenuItem(ID_FILE_IMPORT, MF_DISABLED);
        GetMenu()->EnableMenuItem(ID_FILE_EXPORT, MF_DISABLED);
        GetMenu()->EnableMenuItem(ID_FILE_PREVIEW, MF_DISABLED);

        // Initialize glb
        glb::app::AppConfig config;
        config.wnd = GetDlgItem(IDC_VIEW)->GetSafeHwnd();
        RECT rect;
        GetDlgItem(IDC_VIEW)->GetClientRect(&rect);
        config.screen_width = rect.right - rect.left;
        config.screen_height = rect.bottom - rect.top;
        config.shadow_map_width = 32;
        config.shadow_map_height = 32;
        config.decalMapWidth = 32;
        config.decalMapHeight = 32;
        if (!glb::app::Application::Initialize(ApplicationCore::Create, AfxGetInstanceHandle(), config))
        {
            ::MessageBox(NULL, L"Initialize GLB library failed", L"ERROR", MB_OK);
            exit(0);
        }

        // Try add model
        char *pcstr = (char *)new char[2 * wcslen(filePath.GetBuffer(0))+1] ;
        memset(pcstr , 0 , 2 * wcslen(filePath.GetBuffer(0))+1 );
        wcstombs(pcstr, filePath.GetBuffer(0), wcslen(filePath.GetBuffer(0))) ;

        if (!ApplicationCore::GetInstance()->Preview(pcstr))
        {
            ::MessageBox(NULL, L"Invalid file format", L"ERROR", MB_OK);
            exit(0);
        }

        delete[] pcstr;
        pcstr = NULL;

        m_AlbedoTexName = CString("N/A");
        m_RoughnessTexName = CString("N/A");
        m_MetallicTexName = CString("N/A");
        m_NormalTexName = CString("N/A");
        m_AlphaTexName = CString("N/A");
        m_EmissionTexName = CString("N/A");
        m_DiffusePFCTexName = CString("N/A");
        m_SpecularPFCTexName = CString("N/A");
        m_LightTex0Name = CString("N/A");
        m_LightTex1Name = CString("N/A");
        m_LightTex2Name = CString("N/A");
        UpdateData(false);
    }
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


void Cglb_modeleditorDlg::OnBnClickedDiffusePFCFileButton()
{
    // TODO: Add your control notification handler code here
}


void Cglb_modeleditorDlg::OnBnClickedSpecularPFCFileButton()
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


void Cglb_modeleditorDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: Add your message handler code here and/or call default
    glb::app::Application::Update();

    CDialog::OnTimer(nIDEvent);
}


void Cglb_modeleditorDlg::OnEnSetfocusAlbedoEdit()
{
    // TODO: Add your control notification handler code here
    GetDlgItem(IDC_ALBEDO_EDIT)->HideCaret();
}


void Cglb_modeleditorDlg::OnEnSetfocusRoughnessEdit()
{
    // TODO: Add your control notification handler code here
    GetDlgItem(IDC_ROUGHNESS_EDIT)->HideCaret();
}


void Cglb_modeleditorDlg::OnEnSetfocusMetallicEdit()
{
    // TODO: Add your control notification handler code here
    GetDlgItem(IDC_METALLIC_EDIT)->HideCaret();
}


void Cglb_modeleditorDlg::OnEnSetfocusNormalEdit()
{
    // TODO: Add your control notification handler code here
    GetDlgItem(IDC_NORMAL_EDIT)->HideCaret();
}


void Cglb_modeleditorDlg::OnEnSetfocusAlphaEdit()
{
    // TODO: Add your control notification handler code here
    GetDlgItem(IDC_ALPHA_EDIT)->HideCaret();
}


void Cglb_modeleditorDlg::OnEnSetfocusEmissionEdit()
{
    // TODO: Add your control notification handler code here
    GetDlgItem(IDC_EMISSION_EDIT)->HideCaret();
}


void Cglb_modeleditorDlg::OnEnSetfocusDiffusePFCEdit()
{
    // TODO: Add your control notification handler code here
    GetDlgItem(IDC_DIFFUSE_PFC_EDIT)->HideCaret();
}


void Cglb_modeleditorDlg::OnEnSetfocusSpecularPFCEdit()
{
    // TODO: Add your control notification handler code here
    GetDlgItem(IDC_SPECULAR_PFC_EDIT)->HideCaret();
}


void Cglb_modeleditorDlg::OnEnSetfocusLight0Edit()
{
    // TODO: Add your control notification handler code here
    GetDlgItem(IDC_LIGHT_0_EDIT)->HideCaret();
}


void Cglb_modeleditorDlg::OnEnSetfocusLight1Edit()
{
    // TODO: Add your control notification handler code here
    GetDlgItem(IDC_LIGHT_1_EDIT)->HideCaret();
}


void Cglb_modeleditorDlg::OnEnSetfocusLight2Edit()
{
    // TODO: Add your control notification handler code here
    GetDlgItem(IDC_LIGHT_2_EDIT)->HideCaret();
}
