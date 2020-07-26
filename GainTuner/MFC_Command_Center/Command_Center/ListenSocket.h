#pragma once
#include "afxcoll.h"

// CListenSocket ��� ����Դϴ�.
#include "ChildSocket.h"
class CCommand_CenterDlg;
class CListenSocket : public CAsyncSocket
{
public:
	CListenSocket();
	virtual ~CListenSocket();
	CPtrList ChildSocket_List;
	virtual void OnAccept(int nErrorCode);
	void CloseClientSocket(CSocket* pChild);
	void BroadCast(PC2MCU* pszBuffer, int len);
	CCommand_CenterDlg* pDlg;
	CChildSocket* pChildSocket;
	bool isCanSend;
};


