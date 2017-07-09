
// MFC-MyDAQ-DMM.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMFCMyDAQDMMApp:
// See MFC-MyDAQ-DMM.cpp for the implementation of this class
//

class CMFCMyDAQDMMApp : public CWinApp
{
public:
	CMFCMyDAQDMMApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMFCMyDAQDMMApp theApp;
