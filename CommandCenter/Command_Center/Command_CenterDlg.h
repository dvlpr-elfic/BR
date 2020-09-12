
// Command_CenterDlg.h : ��� ����
//

#pragma once
#include "ListenSocket.h"
#include "afxwin.h"
#include "OScopeCtrl.h"
#include <cmath>
#include "childsocket.h"
#include <string.h>
#include <queue>

using namespace std;

#define TIM_ID_GRAPH 1
#define TIM_ID_DRAW_GRAPH 2
#define TIM_RECEIVE 3
// Remote Control
#define FORWARD 1
#define LEFT 2
#define RIGHT 3
#define BACKWARD 4
#define STOP 5
#define TUNING 6

#define DEFAULT_DRIVE_SPEED 250

#define PORT_NUMBER 9000
// CCommand_CenterDlg ��ȭ ����
class CCommand_CenterDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CCommand_CenterDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_COMMAND_CENTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListenSocket* pListenSocket;
	CListBox m_List_State;
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonSend();
	double Gain_P;
	double Gain_I;
	double Gain_D;
	virtual void OnOK();
	COScopeCtrl* pGraph_Angle;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	double I_Centroid;
	double P_Centroid;
	double D_Centroid;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CButton m_BtnF;
	CButton m_BtnL;
	CButton m_BtnR;
	CButton m_BtnB;
	double m_speed_cmd;
	Flag flags;
	bool CheckDriveClicked(void);
	void SetDriveClicked(int mode);
	double P_Speed;
	bool isCanSend;
	double I_Speed;
	bool ReadGains(void);
	bool SaveGains(Gain gain);
	void GraphConfiguration(void);
	COScopeCtrl* pGraph_Encoder;
	PC2MCU pc2mcu;
	void DriveCommand(char dir);
	void CreateCheckSum_PC2MCU(void);
	CString strIPAddress;
	queue<MCU2PC> q;
};
