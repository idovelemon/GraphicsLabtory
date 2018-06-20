
// glb_lightstudioDlg.cpp : implementation file
//

#include "stdafx.h"
#include "glb_lightstudio.h"
#include "glb_lightstudioDlg.h"
#include "afxdialogex.h"

#include "glb_lightstudio_core.h"

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


// CGLBLightStudioDlg dialog
static const int kSubDialogPosX = 10;
static const int kSubDialogPosY = 300;
static const int kSubDialogWidth = 108 * 2;
static const int kSubDialogHeight = 190 * 2;

CGLBLightStudioDlg::CGLBLightStudioDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGLBLightStudioDlg::IDD, pParent)
    , m_ProjectXML(NULL)
    , m_SceneConfigDlg(NULL)
    , m_SunConfigDlg(NULL)
    , m_CurDispConfigDlg(NULL)
{
    // Using GLB icon
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_hIcon = AfxGetApp()->LoadIcon(IDI_GLB);
    m_LightSourceConfigDlgs.clear();
}

void CGLBLightStudioDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    if (m_ProjectXML)
    {
        delete m_ProjectXML;
        m_ProjectXML = NULL;
    }
    DDX_Control(pDX, IDC_OUTLINE_LIST, m_OutlineList);
}

void CGLBLightStudioDlg::ClearProject()
{
    // If project active
    if (m_ProjectXML)
    {
        if (::MessageBox(NULL, L"Do you want to save old project?", L"Info", MB_OKCANCEL) == IDOK) {
            OnFileSave();  // Save old file
        }

        if (m_ProjectXML) {
            delete m_ProjectXML;
            m_ProjectXML = NULL;
        }
    }

    m_CurDispConfigDlg = NULL;

    // Close scene config dialog
    if (m_SceneConfigDlg)
    {
        m_SceneConfigDlg->SendMessage(WM_CLOSE);
        delete m_SceneConfigDlg;
        m_SceneConfigDlg = NULL;
    }

    // Close light source config dialog
    for (LightSourceDlgArray::iterator it = m_LightSourceConfigDlgs.begin(); it != m_LightSourceConfigDlgs.end(); ++it)
    {
        if (it->second)
        {
            it->second->SendMessage(WM_CLOSE);
            delete it->second;
            it->second = NULL;
        }
    }
    m_LightSourceConfigDlgs.clear();

    // Clear outline
    m_OutlineList.DeleteAllItems();

    glb::app::Application::Destroy();
}

void CGLBLightStudioDlg::SaveProperties()
{
    if (m_ProjectXML)
    {
        // Find Root Element
        TiXmlElement* root = m_ProjectXML->FirstChildElement();
        TiXmlElement* child = root->FirstChildElement();
        while (child != NULL)
        {
            if (!strcmp(child->Value(), "SCENE"))  // Is Scene Element?
            {
                if (m_SceneConfigDlg)
                {
                    int width = 0, height = 0;
                    m_SceneConfigDlg->GetConfigLightMapSize(width, height);
                    child->SetAttribute("LIGHT_MAP_WIDTH", width);
                    child->SetAttribute("LIGHT_MAP_HEIGHT", height);

                    child->SetAttribute("LIGHT_MAP_ITERATE", m_SceneConfigDlg->GetConfigLightMapIterate());
                }
                else
                {
                    assert(false);  // Must have a scene config dialog
                }
            }
            else if (!strcmp(child->Value(), "LIGHT"))  // Is Light Element?
            {
                int lightID = -1;
                child->Attribute("ID", &lightID);

                LightSourceDlgArray::iterator it = m_LightSourceConfigDlgs.find(lightID);
                if (it != m_LightSourceConfigDlgs.end())
                {
                    child->SetDoubleAttribute("POSITION_X", it->second->GetLightSourcePosX());
                    child->SetDoubleAttribute("POSITION_Y", it->second->GetLightSourcePosY());
                    child->SetDoubleAttribute("POSITION_Z", it->second->GetLightSourcePosZ());
                    child->SetDoubleAttribute("ROTATION_X", it->second->GetLightSourceRotX());
                    child->SetDoubleAttribute("ROTATION_Y", it->second->GetLightSourceRotY());
                    child->SetDoubleAttribute("ROTATION_Z", it->second->GetLightSourceRotZ());
                    child->SetDoubleAttribute("SCALE_X", it->second->GetLightSourceScaleX());
                    child->SetDoubleAttribute("SCALE_Y", it->second->GetLightSourceScaleY());
                    child->SetDoubleAttribute("SCALE_Z", it->second->GetLightSourceScaleZ());
                    child->SetDoubleAttribute("COLOR_X", it->second->GetLightSourceColorX());
                    child->SetDoubleAttribute("COLOR_Y", it->second->GetLightSourceColorY());
                    child->SetDoubleAttribute("COLOR_Z", it->second->GetLightSourceColorZ());
                }
                else
                {
                    assert(false);  // Something wrong
                }
            }
            else if (!strcmp(child->Value(), "SUN"))  // Is Sun Element?
            {
                int lightID = -1;
                child->Attribute("ID", &lightID);

                if (m_SunConfigDlg)
                {
                    child->SetDoubleAttribute("COLOR_X", m_SunConfigDlg->GetSunColorX());
                    child->SetDoubleAttribute("COLOR_Y", m_SunConfigDlg->GetSunColorY());
                    child->SetDoubleAttribute("COLOR_Z", m_SunConfigDlg->GetSunColorZ());
                    child->SetDoubleAttribute("ROTATION_X", m_SunConfigDlg->GetSunRotX());
                    child->SetDoubleAttribute("ROTATION_Y", m_SunConfigDlg->GetSunRotY());
                    child->SetDoubleAttribute("ROTATION_Z", m_SunConfigDlg->GetSunRotZ());
                }
                else
                {
                    assert(false);  // Something wrong
                }
            }
            else
            {
                assert(false);  // Unknown xml element
            }

            child = child->NextSiblingElement();
        }
    }
    else
    {
        assert(false);  // Must have project xml file now
    }
}

void CGLBLightStudioDlg::AddSceneMesh(const char* name)
{
    if (!ApplicationCore::GetInstance()->AddSceneMesh(name))
    {
        ::MessageBox(NULL, L"Invalid scene mesh object file", L"ERROR", MB_OK);
        return;
    }

    // Hide ADD-Scene menu
    GetMenu()->EnableMenuItem(ID_ADD_SCENE, MF_DISABLED);

    // Set Outline text
    int count = m_OutlineList.GetItemCount();
    CString id;
    id.Format(L"%d", 1);

    m_OutlineList.InsertItem(count, L"");
    m_OutlineList.SetItemText(count, 0, L"Scene");
    m_OutlineList.SetItemText(count, 1, id);
    m_OutlineList.SetItemText(count, 2, CString(name));

    m_OutlineList.SetItemState(count, LVIS_SELECTED, LVIS_SELECTED);

    // Create scene config dialog
    m_SceneConfigDlg = new CGLBSceneConfigDlg(this);
    m_SceneConfigDlg->Create(IDD_SCENE_CONFIG);
    m_SceneConfigDlg->ShowWindow(SW_SHOW);
    m_SceneConfigDlg->MoveWindow(kSubDialogPosX, kSubDialogPosY, kSubDialogWidth, kSubDialogHeight, TRUE);

    // Display bake button
    GetDlgItem(IDC_BAKE_BUTTON)->ShowWindow(SW_SHOW);
    GetDlgItem(IDC_BAKE_CANCEL_BUTTON)->ShowWindow(SW_SHOW);
    GetDlgItem(IDC_BAKE_PROGRESS)->ShowWindow(SW_SHOW);
    GetDlgItem(IDC_BAKE_CANCEL_BUTTON)->EnableWindow(FALSE);

    // Set current display config dialog
    if (m_CurDispConfigDlg)
    {
        m_CurDispConfigDlg->ShowWindow(SW_HIDE);
    }
    m_CurDispConfigDlg = m_SceneConfigDlg;
}

int CGLBLightStudioDlg::AddLightMesh(int lightID, const char* name)
{
    int id = ApplicationCore::GetInstance()->AddLightMesh(lightID, name);
    if (id == -1)
    {
        ::MessageBox(NULL, L"Invalid light source mesh object file", L"ERROR", MB_OK);
        return id;
    }

    // Set Outline text
    int count = m_OutlineList.GetItemCount();
    CString idStr;
    idStr.Format(L"%d", id);

    m_OutlineList.InsertItem(count, L"");
    m_OutlineList.SetItemText(count, 0, L"Light");
    m_OutlineList.SetItemText(count, 1, idStr);
    m_OutlineList.SetItemText(count, 2, CString(name));
    m_OutlineList.SetItemState(count, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

    // Create light source config dialog
    CGLBLightSourceConfigDlg* dlg = new CGLBLightSourceConfigDlg(this);
    dlg->Create(IDD_LIGHT_SOURCE_CONFIG_DIALOG);
    dlg->ShowWindow(SW_SHOW);
    dlg->MoveWindow(kSubDialogPosX, kSubDialogPosY, kSubDialogWidth, kSubDialogHeight, TRUE);
    m_LightSourceConfigDlgs.insert(std::pair<int, CGLBLightSourceConfigDlg*>(id, dlg));

    // Set current display config dialog
    if (m_CurDispConfigDlg)
    {
        m_CurDispConfigDlg->ShowWindow(SW_HIDE);
    }
    m_CurDispConfigDlg = dlg;

    return id;
}


int CGLBLightStudioDlg::AddSunMesh(const char* name)
{
    int id = ApplicationCore::GetInstance()->AddSunMesh(name);
    if (id == -1)
    {
        ::MessageBox(NULL, L"Invalid light source mesh object file", L"ERROR", MB_OK);
        return id;
    }

    // Set Outline text
    int count = m_OutlineList.GetItemCount();
    CString idStr;
    idStr.Format(L"%d", id);

    m_OutlineList.InsertItem(count, L"");
    m_OutlineList.SetItemText(count, 0, L"Sun");
    m_OutlineList.SetItemText(count, 1, idStr);
    m_OutlineList.SetItemText(count, 2, CString(name));
    m_OutlineList.SetItemState(count, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

    // Create sun source config dialog
    m_SunConfigDlg = new CGLBSunConfigDlg(this);
    m_SunConfigDlg->Create(IDD_SUN_CONFIG_DIALOG);
    m_SunConfigDlg->ShowWindow(SW_SHOW);
    m_SunConfigDlg->MoveWindow(kSubDialogPosX, kSubDialogPosY, kSubDialogWidth, kSubDialogHeight, TRUE);

    // Set current display config dialog
    if (m_CurDispConfigDlg)
    {
        m_CurDispConfigDlg->ShowWindow(SW_HIDE);
    }
    m_CurDispConfigDlg = m_SunConfigDlg;

    return id;
}

BEGIN_MESSAGE_MAP(CGLBLightStudioDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    //ON_MESSAGE_VOID(WM_KICKIDLE, OnKickIdle)
    ON_WM_CLOSE()
    ON_COMMAND(ID_FILE_NEW, &CGLBLightStudioDlg::OnFileNew)
    ON_COMMAND(ID_FILE_SAVE, &CGLBLightStudioDlg::OnFileSave)
    ON_COMMAND(ID_ADD_SCENE, &CGLBLightStudioDlg::OnAddScene)
    ON_COMMAND(ID_ADD_LIGHT, &CGLBLightStudioDlg::OnAddLight)
    ON_BN_CLICKED(IDC_BAKE_BUTTON, &CGLBLightStudioDlg::OnBnClickedBakeButton)
    ON_BN_CLICKED(IDC_BAKE_CANCEL_BUTTON, &CGLBLightStudioDlg::OnBnClickedBakeCancelButton)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_OUTLINE_LIST, &CGLBLightStudioDlg::OnNMCustomdrawOutlineList)
    ON_NOTIFY(HDN_ITEMCLICK, 0, &CGLBLightStudioDlg::OnHdnItemclickOutlineList)
    ON_NOTIFY(NM_CLICK, IDC_OUTLINE_LIST, &CGLBLightStudioDlg::OnNMClickOutlineList)
    ON_COMMAND(ID_FILE_OPEN, &CGLBLightStudioDlg::OnFileOpen)
    ON_COMMAND(ID_FILE_EXPORTLIGHTMAP, &CGLBLightStudioDlg::OnFileExportlightmap)
    ON_WM_TIMER()
    ON_COMMAND(ID_ADD_SUN, &CGLBLightStudioDlg::OnAddSun)
END_MESSAGE_MAP()


// CGLBLightStudioDlg message handlers

BOOL CGLBLightStudioDlg::OnInitDialog()
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

    // Hide Viewport first
    GetDlgItem(IDC_VIEW)->ShowWindow(SW_HIDE);

    // Disable menu
    GetMenu()->EnableMenuItem(ID_FILE_SAVE, MF_DISABLED);
    GetMenu()->EnableMenuItem(ID_ADD_SCENE, MF_DISABLED);
    GetMenu()->EnableMenuItem(ID_ADD_LIGHT, MF_DISABLED);
    GetMenu()->EnableMenuItem(ID_ADD_SUN, MF_DISABLED);
    GetMenu()->EnableMenuItem(ID_FILE_EXPORTLIGHTMAP, MF_DISABLED);

    // Hide control
    DWORD style = m_OutlineList.GetExtendedStyle();
    m_OutlineList.SetExtendedStyle(style | LVS_EX_FULLROWSELECT);
    m_OutlineList.InsertColumn(0, L"Type", LVCFMT_CENTER, 100);
    m_OutlineList.InsertColumn(1, L"ID", LVCFMT_CENTER, 100);
    m_OutlineList.InsertColumn(2, L"File", LVCFMT_CENTER, 100);
    m_OutlineList.ShowWindow(SW_HIDE);

    GetDlgItem(IDC_BAKE_BUTTON)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_BAKE_CANCEL_BUTTON)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_BAKE_PROGRESS)->ShowWindow(SW_HIDE);

    CProgressCtrl* progress = reinterpret_cast<CProgressCtrl*>(GetDlgItem(IDC_BAKE_PROGRESS));
    progress->SetRange(0, 100);

    // Set Timer
    SetTimer(1, 1, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGLBLightStudioDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGLBLightStudioDlg::OnPaint()
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
HCURSOR CGLBLightStudioDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CGLBLightStudioDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: Add your message handler code here and/or call default

    if (m_ProjectXML)
    {
        while (true)
        {
            if (m_SunConfigDlg)
            {
                ApplicationCore::GetInstance()->SetSunRotation(m_SunConfigDlg->GetSunRotX() * 360.0f, m_SunConfigDlg->GetSunRotY() * 360.0f, m_SunConfigDlg->GetSunRotZ() * 360.0f);
            }

            glb::app::Application::Update();

            CProgressCtrl* progress = reinterpret_cast<CProgressCtrl*>(GetDlgItem(IDC_BAKE_PROGRESS));
            progress->SetPos(static_cast<int>(ApplicationCore::GetInstance()->GetCurProgress() * 100));

            if (progress->GetPos() == 100)
            {
                // Enable Bake Button
                GetDlgItem(IDC_BAKE_BUTTON)->EnableWindow(TRUE);

                // Disable Cancel Button
                GetDlgItem(IDC_BAKE_CANCEL_BUTTON)->EnableWindow(FALSE);

                break;
            }

            if (!ApplicationCore::GetInstance()->IsBaking()) break;
        }
    }

    CDialog::OnTimer(nIDEvent);
}

void CGLBLightStudioDlg::OnKickIdle()
{
    if (m_ProjectXML)
    {
        glb::app::Application::Update();

        CProgressCtrl* progress = reinterpret_cast<CProgressCtrl*>(GetDlgItem(IDC_BAKE_PROGRESS));
        progress->SetPos(static_cast<int>(ApplicationCore::GetInstance()->GetCurProgress() * 100));

        if (progress->GetPos() == 100)
        {
            // Enable Bake Button
            GetDlgItem(IDC_BAKE_BUTTON)->EnableWindow(TRUE);

            // Disable Cancel Button
            GetDlgItem(IDC_BAKE_CANCEL_BUTTON)->EnableWindow(FALSE);
        }
    }
}

void CGLBLightStudioDlg::OnClose()
{
    // TODO: Add your message handler code here and/or call default
    ClearProject();

    CDialog::OnClose();
}


void CGLBLightStudioDlg::OnFileOpen()
{
    ClearProject();

    glb::app::AppConfig config;
    config.wnd = GetDlgItem(IDC_VIEW)->GetSafeHwnd();
    RECT rect;
    GetDlgItem(IDC_VIEW)->GetClientRect(&rect);
    config.screen_width = rect.right - rect.left;
    config.screen_height = rect.bottom - rect.top;
    config.shadow_map_width = 32;
    config.shadow_map_height = 32;
    if (!glb::app::Application::Initialize(ApplicationCore::Create, AfxGetInstanceHandle(), config))
    {
        ::MessageBox(NULL, L"Initliaze GLB library failed", L"ERROR", MB_OK);
        exit(0);
    }

    TCHAR szFilter[] = L"XML File(*.xml)|*.xml||";
    CFileDialog fileDlg(TRUE, L"xml", L"", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
    if (IDOK == fileDlg.DoModal())
    {
        CString filePath = fileDlg.GetPathName();

        char *pcstr = (char *)new char[2 * wcslen(filePath.GetBuffer(0))+1] ;
        memset(pcstr , 0 , 2 * wcslen(filePath.GetBuffer(0))+1 );
        wcstombs(pcstr, filePath.GetBuffer(0), wcslen(filePath.GetBuffer(0))) ;

        m_ProjectXML = new TiXmlDocument(pcstr);
        m_ProjectXML->LoadFile();

        delete[] pcstr;
        pcstr = NULL;

        TiXmlElement* root = m_ProjectXML->FirstChildElement();
        TiXmlElement* child = root->FirstChildElement();

        while (child != NULL)
        {
            if (!strcmp(child->Value(), "SCENE"))  // Is Scene Element?
            {
                int lightMapWidth = 0, lightMapHeight = 0, lightMapIterate = 1;
                child->Attribute("LIGHT_MAP_WIDTH", &lightMapWidth);
                child->Attribute("LIGHT_MAP_HEIGHT", &lightMapHeight);
                child->Attribute("LIGHT_MAP_ITERATE", &lightMapIterate);

                AddSceneMesh(child->Attribute("FILE"));

                m_SceneConfigDlg->UpdateConfigLightMapSize(lightMapWidth, lightMapHeight);
                m_SceneConfigDlg->UpdateConfigLightIterate(lightMapIterate);

                ApplicationCore::GetInstance()->ChangeLightMapSize(lightMapWidth, lightMapHeight);
                ApplicationCore::GetInstance()->SetBakeIterate(lightMapIterate);
            }
            else if (!strcmp(child->Value(), "LIGHT"))  // Is Light Element?
            {
                int id = -1;
                child->Attribute("ID", &id);
                AddLightMesh(id, child->Attribute("FILE"));

                double px = 0.0f, py = 0.0f, pz = 0.0f;
                double rx = 0.0f, ry = 0.0f, rz = 0.0f;
                double sx = 0.0f, sy = 0.0f, sz = 0.0f;
                double cx = 0.0f, cy = 0.0f, cz = 0.0f;
                child->Attribute("POSITION_X", &px);
                child->Attribute("POSITION_Y", &py);
                child->Attribute("POSITION_Z", &pz);
                child->Attribute("ROTATION_X", &rx);
                child->Attribute("ROTATION_Y", &ry);
                child->Attribute("ROTATION_Z", &rz);
                child->Attribute("SCALE_X", &sx);
                child->Attribute("SCALE_Y", &sy);
                child->Attribute("SCALE_Z", &sz);
                child->Attribute("COLOR_X", &cx);
                child->Attribute("COLOR_Y", &cy);
                child->Attribute("COLOR_Z", &cz);

                LightSourceDlgArray::iterator it = m_LightSourceConfigDlgs.find(id);
                if (it != m_LightSourceConfigDlgs.end() && it->second != NULL)
                {
                    it->second->UpdateLightSourcePos(static_cast<float>(px), static_cast<float>(py), static_cast<float>(pz));
                    it->second->UpdateLightSourceRot(static_cast<float>(rx), static_cast<float>(ry), static_cast<float>(rz));
                    it->second->UpdateLightSourceScale(static_cast<float>(sx), static_cast<float>(sy), static_cast<float>(sz));
                    it->second->UpdateLightSourceColor(static_cast<float>(cx), static_cast<float>(cy), static_cast<float>(cz));
                    ApplicationCore::GetInstance()->SetLightSourcePos(id, static_cast<float>(px), static_cast<float>(py), static_cast<float>(pz));
                    ApplicationCore::GetInstance()->SetLightSourceRot(id, static_cast<float>(rx), static_cast<float>(ry), static_cast<float>(rz));
                    ApplicationCore::GetInstance()->SetLightSourceScale(id, static_cast<float>(sx), static_cast<float>(sy), static_cast<float>(sz));
                    ApplicationCore::GetInstance()->SetLightSourceColor(id, static_cast<float>(cx), static_cast<float>(cy), static_cast<float>(cz));
                }
                else
                {
                    assert(false);  // Something wrong
                }
            }
            else if (!strcmp(child->Value(), "SUN"))  // Is Sun Element?
            {
                AddSunMesh("res/sun.obj");

                double rx = 0.0f, ry = 0.0f, rz = 0.0f;
                double cx = 0.0f, cy = 0.0f, cz = 0.0f;
                child->Attribute("ROTATION_X", &rx);
                child->Attribute("ROTATION_Y", &ry);
                child->Attribute("ROTATION_Z", &rz);
                child->Attribute("COLOR_X", &cx);
                child->Attribute("COLOR_Y", &cy);
                child->Attribute("COLOR_Z", &cz);
                m_SunConfigDlg->UpdateSunColor(cx, cy, cz);
                m_SunConfigDlg->UpdateSunRotation(rx, ry, rz);
                ApplicationCore::GetInstance()->SetSunColor(cx, cy, cz);
                ApplicationCore::GetInstance()->SetSunRotation(rx * 360.0f, ry * 360.0f, rz * 360.0f);
            }
            else
            {
                assert(false);  // Unknown xml element
            }
            child = child->NextSiblingElement();
        }

        // Display view
        GetDlgItem(IDC_VIEW)->ShowWindow(SW_SHOW);

        // Enable Save menu
        GetMenu()->EnableMenuItem(ID_FILE_SAVE, MF_ENABLED);
        GetMenu()->EnableMenuItem(ID_ADD_SCENE, MF_DISABLED);
        GetMenu()->EnableMenuItem(ID_ADD_LIGHT, MF_ENABLED);
        GetMenu()->EnableMenuItem(ID_ADD_SUN, MF_ENABLED);
        GetMenu()->EnableMenuItem(ID_FILE_EXPORTLIGHTMAP, MF_ENABLED);

        // Display control
        m_OutlineList.ShowWindow(SW_SHOW);
    }
}


void CGLBLightStudioDlg::OnFileNew()
{
    ClearProject();

    glb::app::AppConfig config;
    config.wnd = GetDlgItem(IDC_VIEW)->GetSafeHwnd();
    RECT rect;
    GetDlgItem(IDC_VIEW)->GetClientRect(&rect);
    config.screen_width = rect.right - rect.left;
    config.screen_height = rect.bottom - rect.top;
    config.shadow_map_width = 32;
    config.shadow_map_height = 32;
    if (!glb::app::Application::Initialize(ApplicationCore::Create, AfxGetInstanceHandle(), config))
    {
        ::MessageBox(NULL, L"Initliaze GLB library failed", L"ERROR", MB_OK);
        exit(0);
    }

    // Create new project
    m_ProjectXML = new TiXmlDocument();

    TiXmlElement* root = new TiXmlElement("GLB_LIGHT_STUDIO");
    root->SetAttribute("Author", "GLB");
    m_ProjectXML->LinkEndChild(root);

    // Display view
    GetDlgItem(IDC_VIEW)->ShowWindow(SW_SHOW);

    // Enable Save menu
    GetMenu()->EnableMenuItem(ID_FILE_SAVE, MF_ENABLED);
    GetMenu()->EnableMenuItem(ID_ADD_SCENE, MF_ENABLED);
    GetMenu()->EnableMenuItem(ID_FILE_EXPORTLIGHTMAP, MF_ENABLED);

    // Display control
    m_OutlineList.ShowWindow(SW_SHOW);
}


void CGLBLightStudioDlg::OnFileSave()
{
    SaveProperties();

    if (!strcmp(m_ProjectXML->Value(), ""))
    {
        TCHAR szFilter[] = L"XML File(*.xml)|*.xml|All Files(*.*)|*.*||";
        CFileDialog fileDlg(FALSE, L"xml", L"Untitled.xml", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);

        if (IDOK == fileDlg.DoModal())
        {
            CString filePath = fileDlg.GetPathName();

            char *pcstr = (char *)new char[2 * wcslen(filePath.GetBuffer(0))+1] ;
            memset(pcstr , 0 , 2 * wcslen(filePath.GetBuffer(0))+1 );
            wcstombs(pcstr, filePath.GetBuffer(0), wcslen(filePath.GetBuffer(0))) ;

            m_ProjectXML->SetValue(pcstr);
            m_ProjectXML->SaveFile();

            delete[] pcstr;
            pcstr = NULL;
        }
    }
    else
    {
        m_ProjectXML->SaveFile();
    }
}


void CGLBLightStudioDlg::OnFileExportlightmap()
{
    // TODO: Add your command handler code here
    TCHAR szFilter[] = L"HDR File(*.hdr)|*.xml||";
    CFileDialog fileDlg(FALSE, L"hdr", L"Untitled.hdr", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
    if (IDOK == fileDlg.DoModal())
    {
        CString filePath = fileDlg.GetPathName();

        char *pcstr = (char *)new char[2 * wcslen(filePath.GetBuffer(0))+1];
        memset(pcstr , 0 , 2 * wcslen(filePath.GetBuffer(0))+1 );
        wcstombs(pcstr, filePath.GetBuffer(0), wcslen(filePath.GetBuffer(0)));

        ApplicationCore::GetInstance()->SaveLightMap(pcstr);

        delete[] pcstr;
        pcstr = NULL;

        ::MessageBox(NULL, L"Export Light Map OK", L"Export", MB_OK);
    }
}


void CGLBLightStudioDlg::OnAddScene()
{
    TCHAR szFilter[] = L"Wave Object File(*.obj)|*.obj||";
    CFileDialog fileDlg(TRUE, L"obj", L"", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);

    if (IDOK == fileDlg.DoModal())
    {
        CString filePath = fileDlg.GetPathName();

        char *pcstr = (char *)new char[2 * wcslen(filePath.GetBuffer(0))+1] ;
        memset(pcstr , 0 , 2 * wcslen(filePath.GetBuffer(0))+1 );
        wcstombs(pcstr, filePath.GetBuffer(0), wcslen(filePath.GetBuffer(0))) ;

        AddSceneMesh(pcstr);
        ApplicationCore::GetInstance()->ChangeLightMapSize(32, 32);  // Create light map with default size

        // Log to xml
        TiXmlElement* sceneElement = new TiXmlElement("SCENE");
        m_ProjectXML->FirstChildElement()->LinkEndChild(sceneElement);

        sceneElement->SetAttribute("ID", 1);
        sceneElement->SetAttribute("FILE", pcstr);

        delete[] pcstr;
        pcstr = NULL;

        // Enable Add Light Menu
        GetMenu()->EnableMenuItem(ID_ADD_LIGHT, MF_ENABLED);
        GetMenu()->EnableMenuItem(ID_ADD_SUN, MF_ENABLED);
    }
}


void CGLBLightStudioDlg::OnAddLight()
{
    TCHAR szFilter[] = L"Wave Object File(*.obj)|*.obj||";
    CFileDialog fileDlg(TRUE, L"obj", L"", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);

    if (IDOK == fileDlg.DoModal())
    {
        CString filePath = fileDlg.GetPathName();

        char *pcstr = (char *)new char[2 * wcslen(filePath.GetBuffer(0))+1] ;
        memset(pcstr , 0 , 2 * wcslen(filePath.GetBuffer(0))+1 );
        wcstombs(pcstr, filePath.GetBuffer(0), wcslen(filePath.GetBuffer(0))) ;

        int id = AddLightMesh(-1, pcstr);

        // Log to xml
        TiXmlElement* lightElement = new TiXmlElement("LIGHT");
        m_ProjectXML->FirstChildElement()->LinkEndChild(lightElement);

        lightElement->SetAttribute("ID", id);
        lightElement->SetAttribute("FILE", pcstr);

        delete[] pcstr;
        pcstr = NULL;
    }
}


void CGLBLightStudioDlg::OnAddSun()
{
    // TODO: Add your command handler code here
    char sunName[] = "res/sun.obj";
    int id = AddSunMesh(sunName);

    // Log to xml
    TiXmlElement* lightElement = new TiXmlElement("SUN");
    m_ProjectXML->FirstChildElement()->LinkEndChild(lightElement);

    lightElement->SetAttribute("ID", id);
    lightElement->SetAttribute("FILE", sunName);
}


void CGLBLightStudioDlg::OnBnClickedBakeButton()
{
    // TODO: Add your control notification handler code here
    if (m_SceneConfigDlg)
    {
        int lightMapWidth = 32, lightMapHeight = 32;
        m_SceneConfigDlg->GetConfigLightMapSize(lightMapWidth, lightMapHeight);
        int totalBakeIterate = m_SceneConfigDlg->GetConfigLightMapIterate();

        ApplicationCore::GetInstance()->ChangeLightMapSize(lightMapWidth, lightMapHeight);
        ApplicationCore::GetInstance()->SetBakeIterate(totalBakeIterate);
        ApplicationCore::GetInstance()->Bake();

        // Disable Bake Button
        GetDlgItem(IDC_BAKE_BUTTON)->EnableWindow(FALSE);

        // Enable Cancel Button
        GetDlgItem(IDC_BAKE_CANCEL_BUTTON)->EnableWindow(TRUE);
    }
    else
    {
        assert(false); // Must add scene mesh first
    }
}


void CGLBLightStudioDlg::OnBnClickedBakeCancelButton()
{
    // TODO: Add your control notification handler code here
    if (m_SceneConfigDlg)
    {
        ApplicationCore::GetInstance()->CancelBake();

        // Enable Bake Button
        GetDlgItem(IDC_BAKE_BUTTON)->EnableWindow(TRUE);

        // Disable Cancel Button
        GetDlgItem(IDC_BAKE_CANCEL_BUTTON)->EnableWindow(FALSE);
    }
    else
    {
        assert(false); // Must add scene mesh first
    }
}


void CGLBLightStudioDlg::OnNMCustomdrawOutlineList(NMHDR *pNMHDR, LRESULT *pResult)
{
    NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

    *pResult = 0;
}


void CGLBLightStudioDlg::OnHdnItemclickOutlineList(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
    // TODO: Add your control notification handler code here
    *pResult = 0;
}


void CGLBLightStudioDlg::OnNMClickOutlineList(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: Add your control notification handler code here
    *pResult = 0;

    int index = pNMItemActivate->iItem;
    if (index != -1)
    {
        CListCtrl* ctrl = static_cast<CListCtrl*>(GetDlgItem(IDC_OUTLINE_LIST));
        CString type = ctrl->GetItemText(index, 0);
        CString id = ctrl->GetItemText(index, 1);
        if (!type.Compare(L"Scene"))
        {
            if (m_CurDispConfigDlg)
            {
                m_CurDispConfigDlg->ShowWindow(SW_HIDE);
            }
            m_SceneConfigDlg->ShowWindow(SW_SHOW);
            m_CurDispConfigDlg = m_SceneConfigDlg;
        }
        else if (!type.Compare(L"Sun"))
        {
            if (m_CurDispConfigDlg)
            {
                m_CurDispConfigDlg->ShowWindow(SW_HIDE);
            }
            m_SunConfigDlg->ShowWindow(SW_SHOW);
            m_CurDispConfigDlg = m_SunConfigDlg;
        }
        else
        {
            char *pcstr = (char *)new char[2 * wcslen(id.GetBuffer(0))+1];
            memset(pcstr , 0 , 2 * wcslen(id.GetBuffer(0))+1 );
            wcstombs(pcstr, id.GetBuffer(0), wcslen(id.GetBuffer(0)));

            LightSourceDlgArray::iterator it = m_LightSourceConfigDlgs.find(atoi(pcstr));
            if (it != m_LightSourceConfigDlgs.end())
            {
                if (m_CurDispConfigDlg)
                {
                    m_CurDispConfigDlg->ShowWindow(SW_HIDE);
                }
                it->second->ShowWindow(SW_SHOW);
                m_CurDispConfigDlg = it->second;
            }
            else
            {
                assert(false);
            }

            delete[] pcstr;
            pcstr = NULL;
        }
    }
}
