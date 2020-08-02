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
	//AfxBeginThread(Thread_AppendPoints, this);
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
		//CCommand_CenterDlg* pMain = (CCommand_CenterDlg*)AfxGetMainWnd();
		pDlg->isCanSend = true;
		//pDlg->flags.receive = true;
		pDlg->q.push(mcu2pc);
		//pDlg->pMcu2pc = &mcu2pc;
		//if(mcu2pc.command & 0x80){					/* MSB�� �ݹ�, �ݹ��� ��ε�ĳ���� ���� ���� */
		//	if(mcu2pc.command & 0x1F){				/* ������ */
		//		temp.Format(_T("%s: Robot is Driving,, "), strIPAddress);
		//		if(mcu2pc.command & 0x08){					/* ��ȸ�� */
		//			temp += "Right";
		//		}
		//		else if(mcu2pc.command & 0x04){				/* ��ȸ�� */
		//			temp += "Left";
		//		}
		//		else if(mcu2pc.command & 0x02){				/* ���� */
		//			temp += "Forward";
		//		}
		//		else if(mcu2pc.command & 0x01){				/* ���� */
		//			temp += "Stop";
		//		}
		//		else if(mcu2pc.command & 0x10){				/* ���� */
		//			temp += "Backward";
		//		}
		//	}
		//	else{
		//		temp.Format(_T("%s: Tuning Complete,, "), strIPAddress);
		//	}
		//	pMain->m_List_State.AddString(temp);
		//	pMain->m_List_State.SetCurSel(pMain->m_List_State.GetCount()-1);
		//}
		//else if(mcu2pc.command & 0x40){						/* �غ� �ƴٴ� �޼��� */
		//	double value[3] = {mcu2pc.monitoring.angle,};
		//	pDlg->pGraph_Angle->AppendPoints(value);
		//	temp.Format(_T("%s: Robot is Ready,, "), strIPAddress);
		//	pMain->m_List_State.AddString(temp);
		//	pMain->m_List_State.SetCurSel(pMain->m_List_State.GetCount()-1);
		//}
		//if(mcu2pc.command == 0x00){
		//	//AfxBeginThread(Thread_AppendPoints, this);				/* ����ÿ� ���ø����̼��� �����̵Ǵ� ���� �������� ������� ���� */
		//	pDlg->q.push(mcu2pc);
		//}
	}
	CSocket::OnReceive(nErrorCode);
}


UINT CChildSocket::Thread_AppendPoints(LPVOID pParam)				/* �׷����� ���� �߰��ϴ� ������ */
{
	CChildSocket* pChildSocket = (CChildSocket*)pParam;
	while(1){
		pChildSocket->delay_ms(50);
		pChildSocket->Append2Graph();
	}

	return 0;
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
