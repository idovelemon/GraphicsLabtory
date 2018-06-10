
// glb_modeleditor.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CGLBModelEditorApp:
// See glb_modeleditor.cpp for the implementation of this class
//

class CGLBModelEditorApp : public CWinApp
{
public:
	CGLBModelEditorApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CGLBModelEditorApp theApp;