
// glb_editor.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CGlbEditorApp:
// See glb_editor.cpp for the implementation of this class
//

class CGlbEditorApp : public CWinApp
{
public:
	CGlbEditorApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CGlbEditorApp theApp;