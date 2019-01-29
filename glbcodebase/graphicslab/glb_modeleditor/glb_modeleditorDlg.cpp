
// glb_modeleditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "glb_modeleditor.h"
#include "glb_modeleditorDlg.h"
#include "AddMaterialDlg.h"
#include "AddPassDlg.h"
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
    , m_ChoosePass(-1)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void Cglb_modeleditorDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MATTREE, m_MatTreeCtrl);
    DDX_Control(pDX, IDC_PASS_PARAM_PROPERTY_GRID, m_PassParamGridCtrl);
}

BEGIN_MESSAGE_MAP(Cglb_modeleditorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDOK, &Cglb_modeleditorDlg::OnBnClickedOk)
    ON_COMMAND(ID_FILE_IMPORT, &Cglb_modeleditorDlg::OnFileImport)
    ON_COMMAND(ID_FILE_EXPORT, &Cglb_modeleditorDlg::OnFileExport)
    ON_WM_TIMER()
    ON_COMMAND(ID_FILE_PREVIEW, &Cglb_modeleditorDlg::OnFilePreview)
    ON_COMMAND(ID_MATERIAL_ADD, &Cglb_modeleditorDlg::OnMaterialAdd)
    ON_NOTIFY(NM_RCLICK, IDC_MATTREE, &Cglb_modeleditorDlg::OnNMRClickMattree)
    ON_COMMAND(ID_MATERIAL_ADDPASS, &Cglb_modeleditorDlg::OnMaterialAddpass)
    ON_COMMAND(ID_PASS_COMPILE, &Cglb_modeleditorDlg::OnPassCompile)
    ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, &Cglb_modeleditorDlg::OnPropertyChanged)
    ON_COMMAND(ID_MATERIAL_SAVE, &Cglb_modeleditorDlg::OnMaterialSave)
    ON_COMMAND(ID_MATERIAL_ADDEXSIT, &Cglb_modeleditorDlg::OnMaterialAddexsit)
END_MESSAGE_MAP()


// Cglb_modeleditorDlg message handlers

void Cglb_modeleditorDlg::OnPassParamFloatUpdate(int passIndex, int paramIndex, float value)
{
    // Update m_MaterialInfo
    m_MaterialInfo.passParameters[passIndex][paramIndex].floatValue = value;

    // Update Tree node
    CString keyName(TEXT(""));
    keyName.Format(TEXT("Pass:%s-Param:%s"), m_MaterialInfo.passName[passIndex], CString(m_MaterialInfo.passParameters[passIndex][paramIndex].name));

    CString text(TEXT(""));
    text.Format(TEXT("%s (Type:Float Value:%f)"), CString(m_MaterialInfo.passParameters[passIndex][paramIndex].name), value);
    HTREEITEM paramItemHandle = m_TreeItemTbl[keyName];
    m_MatTreeCtrl.SetItemText(paramItemHandle, text);

    // Update glb material
    // TODO:
}


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
    GetMenu()->EnableMenuItem(ID_MATERIAL_SAVE, MF_DISABLED);
    GetMenu()->EnableMenuItem(1, MF_DISABLED | MF_BYPOSITION);

    // Setup pass property grid control
    HDITEM item;
    item.cxy = 120;
    item.mask = HDI_WIDTH;
    m_PassParamGridCtrl.GetHeaderCtrl().SetItem(0, &item);

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
        GetMenu()->EnableMenuItem(ID_FILE_IMPORT, MF_DISABLED);
        GetMenu()->EnableMenuItem(ID_FILE_EXPORT, MF_ENABLED);
        GetMenu()->EnableMenuItem(ID_FILE_PREVIEW, MF_DISABLED);
        GetMenu()->EnableMenuItem(1, MF_ENABLED | MF_BYPOSITION);

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
        GetMenu()->EnableMenuItem(ID_MATERIAL_ADDEXSIT, MF_ENABLED);
        GetMenu()->EnableMenuItem(1, MF_ENABLED | MF_BYPOSITION);

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

        UpdateData(false);
    }
}


void Cglb_modeleditorDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: Add your message handler code here and/or call default
    glb::app::Application::Update();

    CDialog::OnTimer(nIDEvent);
}


void Cglb_modeleditorDlg::OnMaterialAdd()
{
    // TODO: Add your command handler code here
    auto WideToMultiByte = [](const wchar_t* src)
    {
        int len = WideCharToMultiByte(CP_ACP, 0, src, -1, nullptr, 0, nullptr, nullptr);
        if (len == 0)
        {
            return (char*)(nullptr);
        }

        char* pResult = new char[len];
        WideCharToMultiByte(CP_ACP, 0, src, -1, pResult, len, nullptr, nullptr);
        return pResult;
    };

    auto DestroyMultiByte = [](char** src)
    {
        delete[] (*src);
        (*src) = nullptr;
    };

    CAddMaterialDlg addMaterialDlg;
    if (IDOK == addMaterialDlg.DoModal())
    {
        // Disable MATERIAL->Add menu
        GetMenu()->EnableMenuItem(ID_MATERIAL_ADD, MF_DISABLED);
        GetMenu()->EnableMenuItem(ID_MATERIAL_ADDEXSIT, MF_DISABLED);
        GetMenu()->EnableMenuItem(ID_MATERIAL_SAVE, MF_ENABLED);

        // Save material name
        m_MaterialInfo.materialName = addMaterialDlg.GetMaterialName();

        // Add Material Node to tree
        m_TreeItemTbl[CString(TEXT("MaterialName"))] = m_MatTreeCtrl.InsertItem(m_MaterialInfo.materialName, TVI_ROOT);

        // Get material name
        char* materialName = WideToMultiByte(m_MaterialInfo.materialName.GetString());

        // Create material
        if (!ApplicationCore::GetInstance()->AddEmptyMaterial(materialName))
        {
            // Do nothing
        }
    }
}


void Cglb_modeleditorDlg::OnMaterialSave()
{
    // TODO: Add your command handler code here
    TCHAR szFilter[] = L"Material File(*.mat)|*.mat||";
    CFileDialog fileDlg(FALSE, L"mat", L"Untitled.mat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
    if (IDOK == fileDlg.DoModal())
    {
        CString filePath = fileDlg.GetPathName();

        // Try save material
        char *pcstr = (char *)new char[2 * wcslen(filePath.GetBuffer(0)) + 1] ;
        memset(pcstr, 0, 2 * wcslen(filePath.GetBuffer(0)) + 1);
        wcstombs(pcstr, filePath.GetBuffer(0), wcslen(filePath.GetBuffer(0)));

        if (!ApplicationCore::GetInstance()->SaveMaterial(pcstr))
        {
            ::MessageBox(NULL, L"Save material failed", L"ERROR", MB_OK);
            exit(0);
        }

        delete[] pcstr;
        pcstr = NULL;
    }
}


void Cglb_modeleditorDlg::OnMaterialAddexsit()
{
    // TODO: Add your command handler code here
    TCHAR szFilter[] = L"Material File(*.mat)|*.mat||";
    CFileDialog fileDlg(TRUE, L"", L"", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
    if (IDOK == fileDlg.DoModal())
    {
        CString filePath = fileDlg.GetPathName();

        // Disable menu
        GetMenu()->EnableMenuItem(ID_FILE_IMPORT, MF_DISABLED);
        GetMenu()->EnableMenuItem(ID_FILE_EXPORT, MF_DISABLED);
        GetMenu()->EnableMenuItem(ID_FILE_PREVIEW, MF_DISABLED);
        GetMenu()->EnableMenuItem(1, MF_DISABLED | MF_BYPOSITION);

        // Load material
        char *pcstr = (char *)new char[2 * wcslen(filePath.GetBuffer(0)) + 1] ;
        memset(pcstr, 0, 2 * wcslen(filePath.GetBuffer(0)) + 1);
        wcstombs(pcstr, filePath.GetBuffer(0), wcslen(filePath.GetBuffer(0)));
        if (!ApplicationCore::GetInstance()->AddMaterial(pcstr))
        {
            ::MessageBox(NULL, L"Add material failed", L"ERROR", MB_OK);
            exit(0);
        }
        delete[] pcstr;
        pcstr = nullptr;
    }
}


void Cglb_modeleditorDlg::OnNMRClickMattree(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: Add your control notification handler code here
    *pResult = 0;

    CPoint screenClickPt, clientClickPt;
    GetCursorPos(&screenClickPt);

    clientClickPt = screenClickPt;
    m_MatTreeCtrl.ScreenToClient(&clientClickPt);

    UINT uFlag = 0;
    HTREEITEM hTreeItem = m_MatTreeCtrl.HitTest(clientClickPt, &uFlag);
    if ((hTreeItem != nullptr) && (TVHT_ONITEM & uFlag))
    {
        if (m_TreeItemTbl[CString(TEXT("MaterialName"))] == hTreeItem)
        {
            CMenu menu;
            menu.LoadMenuW(IDR_CLICK_MATNAME);
            menu.GetSubMenu(0)->TrackPopupMenu(TPM_RIGHTBUTTON, screenClickPt.x, screenClickPt.y, this);
        }
        else
        {
            for (int i = 0; i < m_MaterialInfo.passName.GetCount(); i++)
            {
                CString keyName(TEXT(""));
                keyName.Format(TEXT("Pass:%s"), m_MaterialInfo.passName[i]);
                if (m_TreeItemTbl[keyName] == hTreeItem)
                {
                    CMenu menu;
                    menu.LoadMenuW(IDR_CLICK_PASSNAME);
                    menu.GetSubMenu(0)->TrackPopupMenu(TPM_RIGHTBUTTON, screenClickPt.x, screenClickPt.y, this);

                    m_ChoosePass = i;
                    break;
                }
            }
        }
    }
}


void Cglb_modeleditorDlg::OnMaterialAddpass()
{
    // TODO: Add your command handler code here
    CAddPassDlg addPassDlg;
    if (IDOK == addPassDlg.DoModal())
    {
        // Save pass name
        m_MaterialInfo.passName.Add(addPassDlg.GetPassName());

        // Save shader name
        m_MaterialInfo.vertexShaderName.Add(addPassDlg.GetVertexShaderName());
        m_MaterialInfo.fragmentShaderName.Add(addPassDlg.GetFragmentShaderName());

        // Add Pass node to tree
        CString name(TEXT(""));
        name.Format(TEXT("Pass:%s VertexShader:%s FragmentShader:%s"), addPassDlg.GetPassName(), addPassDlg.GetVertexShaderName(), addPassDlg.GetFragmentShaderName());

        CString keyName(TEXT(""));
        keyName.Format(TEXT("Pass:%s"), addPassDlg.GetPassName());

        m_TreeItemTbl[keyName] = m_MatTreeCtrl.InsertItem(name, m_TreeItemTbl[CString(TEXT("MaterialName"))]);

        auto cleanUp = [=]()
        {
            m_MaterialInfo.passName.RemoveAt(m_MaterialInfo.passName.GetCount() - 1, 1);
            m_MaterialInfo.vertexShaderName.RemoveAt(m_MaterialInfo.vertexShaderName.GetCount() - 1, 1);
            m_MaterialInfo.fragmentShaderName.RemoveAt(m_MaterialInfo.fragmentShaderName.GetCount() - 1, 1);
            m_TreeItemTbl.erase(m_TreeItemTbl.find(keyName));
        };

        // Create vertex and fragment shader file
        if (INVALID_HANDLE_VALUE == CreateFile(addPassDlg.GetVertexShaderName(), 0, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr))
        {
            cleanUp();
            return;
        }

        if (INVALID_HANDLE_VALUE == CreateFile(addPassDlg.GetFragmentShaderName(), 0, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr))
        {
            cleanUp();
            return;
        }
    }
}


void Cglb_modeleditorDlg::OnPassCompile()
{
    // TODO: Add your command handler code here
    auto WideToMultiByte = [](const wchar_t* src)
    {
        int len = WideCharToMultiByte(CP_ACP, 0, src, -1, nullptr, 0, nullptr, nullptr);
        if (len == 0)
        {
            return (char*)(nullptr);
        }

        char* pResult = new char[len];
        WideCharToMultiByte(CP_ACP, 0, src, -1, pResult, len, nullptr, nullptr);
        return pResult;
    };

    auto DestroyMultiByte = [](char** src)
    {
        delete[] (*src);
        (*src) = nullptr;
    };

    // Get shader name
    char* vertexName = WideToMultiByte(m_MaterialInfo.vertexShaderName[m_ChoosePass].GetString());
    char* fragmentName = WideToMultiByte(m_MaterialInfo.fragmentShaderName[m_ChoosePass].GetString());

    // Get pass name
    char* passName = WideToMultiByte(m_MaterialInfo.passName[m_ChoosePass].GetString());

    // GLB_SRP generate file name
    char glbFragmentShaderName[128];
    sprintf_s(glbFragmentShaderName, "glb_%s", fragmentName);

    // Python command line
    char commandLine[128];
    sprintf_s(commandLine, "python GLB_SRP.py %s %s", fragmentName, glbFragmentShaderName);

    // Run GLB_SRP python script
    system(commandLine);

    // Try compile shader
    if (ApplicationCore::GetInstance()->TryCompileShader(passName, vertexName, glbFragmentShaderName))
    {
        // Collect all parameters
        std::vector<glb::render::material::PassMaterial::ParameterEntry>& passParameters = ApplicationCore::GetInstance()->GetPassParameters(passName);
        if (m_MaterialInfo.passParameters.size() <= static_cast<size_t>(m_ChoosePass))
        {
            m_MaterialInfo.passParameters.push_back(passParameters);
        }
        else
        {
            m_MaterialInfo.passParameters[m_ChoosePass] = passParameters;
        }

        // Clear all pass parameters of property grid
        m_PassParamGridCtrl.RemoveAll();

        // Add pass parameter to property grid
        CMFCPropertyGridProperty* intParamGroupProperty = nullptr;
        CMFCPropertyGridProperty* floatParamGroupProperty = nullptr;
        CMFCPropertyGridProperty* vec3ParamGroupProperty = nullptr;
        CMFCPropertyGridProperty* vec4ParamGroupProperty = nullptr;
        CMFCPropertyGridProperty* texParamGroupProperty = nullptr;

        for (auto& param : passParameters)
        {
            if (param.type == glb::render::material::PassMaterial::PARAMETER_TYPE_USER)
            {
                switch (param.format)
                {
                case glb::render::PARAMETER_FORMAT_INT:
                    {
                        if (!intParamGroupProperty) intParamGroupProperty = new CMFCPropertyGridProperty(TEXT("Int Pass Parameter"));
                        CMFCPropertyGridProperty* intProperty = new CMFCPropertyGridProperty(CString(param.name), COleVariant(static_cast<long>(param.intValue)));
                        intParamGroupProperty->AddSubItem(intProperty);
                    }
                    break;
                case glb::render::PARAMETER_FORMAT_FLOAT:
                    {
                        if (!floatParamGroupProperty) floatParamGroupProperty = new CMFCPropertyGridProperty(TEXT("Float Pass Parameter"));
                        CMFCPropertyGridProperty* floatProperty = new CMFCPropertyGridProperty(CString(param.name), COleVariant(param.floatValue));
                        floatParamGroupProperty->AddSubItem(floatProperty);
                    }
                    break;
                case glb::render::PARAMETER_FORMAT_FLOAT3:
                    {
                        if (!vec3ParamGroupProperty) vec3ParamGroupProperty = new CMFCPropertyGridProperty(TEXT("Vec3 Pass Parameter"));
                        CMFCPropertyGridProperty* float3Property = new CMFCPropertyGridProperty(CString(param.name));
                        CMFCPropertyGridProperty* xProperty = new CMFCPropertyGridProperty(CString("X"), COleVariant(param.vecValue.x));
                        CMFCPropertyGridProperty* yProperty = new CMFCPropertyGridProperty(CString("Y"), COleVariant(param.vecValue.y));
                        CMFCPropertyGridProperty* zProperty = new CMFCPropertyGridProperty(CString("Z"), COleVariant(param.vecValue.z));
                        float3Property->AddSubItem(xProperty);
                        float3Property->AddSubItem(yProperty);
                        float3Property->AddSubItem(zProperty);
                        vec3ParamGroupProperty->AddSubItem(float3Property);
                    }
                    break;
                case glb::render::PARAMETER_FORMAT_FLOAT4:
                    {
                        if (!vec4ParamGroupProperty) vec4ParamGroupProperty = new CMFCPropertyGridProperty(TEXT("Vec4 Pass Parameter"));
                        CMFCPropertyGridProperty* float4Property = new CMFCPropertyGridProperty(CString(param.name));
                        CMFCPropertyGridProperty* xProperty = new CMFCPropertyGridProperty(CString("X"), COleVariant(param.vecValue.x));
                        CMFCPropertyGridProperty* yProperty = new CMFCPropertyGridProperty(CString("Y"), COleVariant(param.vecValue.y));
                        CMFCPropertyGridProperty* zProperty = new CMFCPropertyGridProperty(CString("Z"), COleVariant(param.vecValue.z));
                        CMFCPropertyGridProperty* wProperty = new CMFCPropertyGridProperty(CString("W"), COleVariant(param.vecValue.w));
                        float4Property->AddSubItem(xProperty);
                        float4Property->AddSubItem(yProperty);
                        float4Property->AddSubItem(zProperty);
                        float4Property->AddSubItem(wProperty);
                        vec4ParamGroupProperty->AddSubItem(float4Property);
                    }
                    break;
                case glb::render::PARAMETER_FORMAT_TEXTURE_2D:
                    {
                        if (!texParamGroupProperty) texParamGroupProperty = new CMFCPropertyGridProperty(TEXT("Texture Pass Parameter"));
                        CMFCPropertyGridFileProperty* tex2DProperty = new CMFCPropertyGridFileProperty(CString(param.name), TRUE, CString(glb::render::texture::Mgr::GetTextureById(param.intValue)->GetName()));
                        texParamGroupProperty->AddSubItem(tex2DProperty);
                    }
                    break;
                case glb::render::PARAMETER_FORMAT_TEXTURE_CUBE:
                    {
                        if (!texParamGroupProperty) texParamGroupProperty = new CMFCPropertyGridProperty(TEXT("Texture Pass Parameter"));
                        CMFCPropertyGridFileProperty* texCubeProperty = new CMFCPropertyGridFileProperty(CString(param.name), TRUE, CString(glb::render::texture::Mgr::GetTextureById(param.intValue)->GetName()));
                        texParamGroupProperty->AddSubItem(texCubeProperty);
                    }
                    break;
                }
            }
        }

        if (intParamGroupProperty) m_PassParamGridCtrl.AddProperty(intParamGroupProperty);
        if (floatParamGroupProperty) m_PassParamGridCtrl.AddProperty(floatParamGroupProperty);
        if (vec3ParamGroupProperty) m_PassParamGridCtrl.AddProperty(vec3ParamGroupProperty);
        if (vec4ParamGroupProperty) m_PassParamGridCtrl.AddProperty(vec4ParamGroupProperty);
        if (texParamGroupProperty) m_PassParamGridCtrl.AddProperty(texParamGroupProperty);
    }
    else
    {
        CString errorMsg;
        while (true)
        {
            std::string msg = glb::util::user_error_popmsg();
            if (msg.empty()) break;

            errorMsg = errorMsg + CString(msg.c_str());
        }

        MessageBox(errorMsg, TEXT("Compile error"), MB_OK);
    }

    // Cleanup
    DestroyMultiByte(&passName);
    DestroyMultiByte(&fragmentName);
    DestroyMultiByte(&vertexName);
}


LRESULT Cglb_modeleditorDlg::OnPropertyChanged(WPARAM wparam, LPARAM lparam)
{
    auto WideToMultiByte = [](const wchar_t* src)
    {
        int len = WideCharToMultiByte(CP_ACP, 0, src, -1, nullptr, 0, nullptr, nullptr);
        if (len == 0)
        {
            return (char*)(nullptr);
        }

        char* pResult = new char[len];
        WideCharToMultiByte(CP_ACP, 0, src, -1, pResult, len, nullptr, nullptr);
        return pResult;
    };

    auto DestroyMultiByte = [](char** src)
    {
        delete[] (*src);
        (*src) = nullptr;
    };

    char* passName = WideToMultiByte(m_MaterialInfo.passName[m_ChoosePass]);

    CMFCPropertyGridProperty* property = reinterpret_cast<CMFCPropertyGridProperty*>(lparam);
    char* parameterName = WideToMultiByte(property->GetName());
    char* valueWiseName = nullptr;
    if (!strcmp(parameterName, "X") || !strcmp(parameterName, "Y") || !strcmp(parameterName, "Z") || !strcmp(parameterName, "W"))
    {
        valueWiseName = parameterName;
        parameterName = WideToMultiByte(property->GetParent()->GetName());
    }

    for (auto& param : m_MaterialInfo.passParameters[m_ChoosePass])
    {
        if (!strcmp(param.name, parameterName))
        {
            switch (param.format)
            {
            case glb::render::PARAMETER_FORMAT_INT:
                {
                    param.intValue = property->GetValue().intVal;
                    ApplicationCore::GetInstance()->SetPassParameterInt(passName, param.name, param.intValue);
                }
                break;
            case glb::render::PARAMETER_FORMAT_FLOAT:
                {
                    param.floatValue = property->GetValue().fltVal;
                    ApplicationCore::GetInstance()->SetPassParameterFloat(passName, param.name, param.floatValue);
                }
                break;
            case glb::render::PARAMETER_FORMAT_FLOAT3:
            case glb::render::PARAMETER_FORMAT_FLOAT4:
                {
                    if (!strcmp(valueWiseName, "X"))
                    {
                        param.vecValue.x = property->GetValue().fltVal;
                    }
                    else if (!strcmp(valueWiseName, "Y"))
                    {
                        param.vecValue.y = property->GetValue().fltVal;
                    }
                    else if (!strcmp(valueWiseName, "Z"))
                    {
                        param.vecValue.z = property->GetValue().fltVal;
                    }
                    else if (!strcmp(valueWiseName, "W"))
                    {
                        param.vecValue.w = property->GetValue().fltVal;
                    }
                    ApplicationCore::GetInstance()->SetPassParameterVec(passName, param.name, param.vecValue);
                }
                break;
            case glb::render::PARAMETER_FORMAT_TEXTURE_2D:
                {
                    char* textureName = WideToMultiByte(property->GetValue().bstrVal);
                    param.intValue = glb::render::texture::Mgr::LoadTexture(textureName);
                    ApplicationCore::GetInstance()->SetPassParameterTex(passName, param.name, param.intValue);
                    DestroyMultiByte(&textureName);
                }
                break;
            case glb::render::PARAMETER_FORMAT_TEXTURE_CUBE:
                {
                    char* textureName = WideToMultiByte(property->GetValue().bstrVal);
                    param.intValue = glb::render::texture::Mgr::LoadPFCTexture(textureName);
                    ApplicationCore::GetInstance()->SetPassParameterTex(passName, param.name, param.intValue);
                    DestroyMultiByte(&textureName);
                }
                break;
            }
            break;
        }
    }

    // Cleanup
    DestroyMultiByte(&passName);
    DestroyMultiByte(&parameterName);
    if (valueWiseName)
    {
        DestroyMultiByte(&valueWiseName);
    }

    return 0;
}
