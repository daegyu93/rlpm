
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		
#include "rlpmApi.h"


//#ifdef __USE_CONSOLE__
#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")
//#endif

class CrlpmappApp : public CWinApp
{
public:
	CrlpmappApp();
	CRlpmApi mRlpmApi;
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CrlpmappApp theApp;
