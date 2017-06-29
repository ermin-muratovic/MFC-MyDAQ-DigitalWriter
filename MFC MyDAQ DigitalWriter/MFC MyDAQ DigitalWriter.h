
// MFC MyDAQ DigitalWriter.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMFCMyDAQDigitalWriterApp:
// See MFC MyDAQ DigitalWriter.cpp for the implementation of this class
//

class CMFCMyDAQDigitalWriterApp : public CWinApp
{
public:
	CMFCMyDAQDigitalWriterApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMFCMyDAQDigitalWriterApp theApp;
