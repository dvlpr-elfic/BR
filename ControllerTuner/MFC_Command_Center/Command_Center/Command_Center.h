
// Command_Center.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CCommand_CenterApp:
// �� Ŭ������ ������ ���ؼ��� Command_Center.cpp�� �����Ͻʽÿ�.
//

class CCommand_CenterApp : public CWinApp
{
public:
	CCommand_CenterApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CCommand_CenterApp theApp;