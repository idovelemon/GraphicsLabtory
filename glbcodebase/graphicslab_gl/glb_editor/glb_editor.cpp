
// glb_editor.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "glb_editor.h"
#include "glb_editorDlg.h"
#include "comm/comm.h"
#include "glbcomm/glbcomm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGlbEditorApp

BEGIN_MESSAGE_MAP(CGlbEditorApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CGlbEditorApp construction

CGlbEditorApp::CGlbEditorApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CGlbEditorApp object

CGlbEditorApp theApp;


// CGlbEditorApp initialization

BOOL CGlbEditorApp::InitInstance()
{
    char cur_dir[128];
    int32_t real_len = GetCurrentDirectory(sizeof(cur_dir), cur_dir);
    memcpy(cur_dir + real_len, "\\editor.ini", sizeof("\\editor.ini"));

    // Initialize the comm library
    int32_t post_port = ::GetPrivateProfileInt("Port", "Post", 0, cur_dir);
    int32_t accept_port = ::GetPrivateProfileInt("Port", "Accept", 0, cur_dir);
    comm::Comm::Initialize(accept_port, post_port, true);
    glb::GlbMaterialMgr::Initialize();
    comm::Packet* packet = comm::Comm::Recieve();
    int32_t mat_num = packet->size_in_bytes / sizeof(glb::GlbMaterial);
    int32_t offset = 0;
    for (int32_t i = 0; i < mat_num; i++) {
        glb::GlbMaterial mat;
        memcpy(&mat, reinterpret_cast<char*>(packet->data) + offset, sizeof(glb::GlbMaterial));
        offset += sizeof(glb::GlbMaterial);
        glb::GlbMaterialMgr::AddMaterial(mat);
    }
    delete packet;
    packet = NULL;

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CGlbEditorDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
		TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

    // Terminate the material data base
    glb::GlbMaterialMgr::Terminate();

    // Terminate the comm library
    comm::Comm::Terminate();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

