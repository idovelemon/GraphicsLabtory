
// glb_lightstudio.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CGLBLightStudioApp:
// See glb_lightstudio.cpp for the implementation of this class
//

class CGLBLightStudioApp : public CWinApp
{
public:
	CGLBLightStudioApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CGLBLightStudioApp theApp;