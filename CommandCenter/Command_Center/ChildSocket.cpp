// ChildSocket.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Command_Center.h"
#include "Command_CenterDlg.h"
#include "ChildSocket.h"
#include "ListenSocket.h"

// CChildSocket

CChildSocket::CChildSocket()
	: m_pListenSocket(NULL)
	, pDlg(NULL)
{
}

CChildSocket::~CChildSocket()
{
}


// CChildSocket ��� �Լ�


void CChildSocket::SetListenSocket(CAsyncSocket* pSocket)
{
	m_pListenSocket=pSocket;
}


void CChildSocket::OnClose(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
	pServerSocket->CloseClientSocket(this);
	CSocket::OnClose(nErrorCode);
}


void CChildSocket::OnReceive(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	// ���� �� �ֵ��� �÷��� SET
	CString temp = _T(""), strIPAddress = _T("");
	UINT uPortNumber = 0;
	GetPeerName(pDlg->strIPAddress, uPortNumber);
	int len;
	memset(&mcu2pc, 0, sizeof(mcu2pc));				/* Ŭ���� */
	if((len = Receive(&mcu2pc, sizeof(mcu2pc))) > 0){
		pDlg->isCanSend = true;
		pDlg->q.push(mcu2pc);
	}
	CSocket::OnReceive(nErrorCode);
}


void CChildSocket::Append2Graph(void)
{
	if(q.empty())
		return;
	MCU2PC temp = q.front();
	q.pop();
	double value[3] = {temp.monitoring.angle, 
		temp.monitoring.encoder.left, temp.monitoring.encoder.right};
	pDlg->pGraph_Angle->AppendPoints(value);								/* ���� ���� �ΰ� �߰� */
	//pDlg->pGraph_Encoder->AppendPoints(value+1);							/* �׷����� ������ ���������� */
	pDlg->pGraph_Angle->AppendPoints(value);
	//pDlg->pGraph_Encoder->AppendPoints(value+1);
}


void CChildSocket::delay_ms(clock_t n)
{
	clock_t start = clock();
	while(clock() - start < n);
}
