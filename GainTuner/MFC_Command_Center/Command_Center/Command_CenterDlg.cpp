
// Command_CenterDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "Command_Center.h"
#include "Command_CenterDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCommand_CenterDlg ��ȭ ����



CCommand_CenterDlg::CCommand_CenterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCommand_CenterDlg::IDD, pParent)
	, pListenSocket(NULL)
	, Gain_P(0)
	, Gain_I(0)
	, Gain_D(0)
	, pGraph_Angle(NULL)
	, I_Centroid(0)
	, P_Centroid(0)
	, D_Centroid(0)
	, m_speed_cmd(0)
	, P_Speed(0)
	, isCanSend(false)
	, I_Speed(0)
	, pGraph_Encoder(NULL)
	, strIPAddress(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	memset(&flags, 0, sizeof(Flag));
}

void CCommand_CenterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_STATE, m_List_State);
	DDX_Text(pDX, IDC_EDIT_PGAIN, Gain_P);
	DDX_Text(pDX, IDC_EDIT_IGAIN, Gain_I);
	DDX_Text(pDX, IDC_EDIT_DGAIN, Gain_D);
	DDX_Text(pDX, IDC_EDIT_ICEN, I_Centroid);
	DDX_Text(pDX, IDC_EDIT_PCEN, P_Centroid);
	DDX_Text(pDX, IDC_EDIT_DCEN, D_Centroid);
	DDX_Control(pDX, IDC_CHECK_F, m_BtnF);
	DDX_Control(pDX, IDC_CHECK_L, m_BtnL);
	DDX_Control(pDX, IDC_CHECK_R, m_BtnR);
	DDX_Control(pDX, IDC_CHECK_B, m_BtnB);
	DDX_Text(pDX, IDC_EDIT_SPEED, m_speed_cmd);
	DDX_Text(pDX, IDC_EDIT_PSPEED, P_Speed);
	DDX_Text(pDX, IDC_EDIT_ISPEED, I_Speed);
}

BEGIN_MESSAGE_MAP(CCommand_CenterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CCommand_CenterDlg::OnBnClickedButtonSend)
	ON_WM_TIMER()
//	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CCommand_CenterDlg �޽��� ó����
BOOL CCommand_CenterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	// Configuration Socket
	pListenSocket = new CListenSocket;
	pListenSocket->pDlg = this;
	if(pListenSocket->Create(PORT_NUMBER, SOCK_STREAM)){
		if(pListenSocket->Listen()){

		}else{
			AfxMessageBox(_T("Failed to LISTEN"));
		}
	}
	else{
		AfxMessageBox(_T("Failed to create socket for listen."));
	}

	// Configuration Graph
	GraphConfiguration();
	//SetTimer(TIM_ID_GRAPH, 1, NULL);						/* OnReceive���� ��� ó�� */
	// Edit Control Init
	SetTimer(TIM_RECEIVE, 1, NULL);
	m_speed_cmd = DEFAULT_DRIVE_SPEED;
	UpdateData(FALSE);
	// Flag Init
	flags.button_clicked = false;
	if(!(ReadGains())){										/* ������ ����� ���ΰ� �б� */
		AfxMessageBox(_T("Cannot read gains from file"));
	}
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CCommand_CenterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CCommand_CenterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CCommand_CenterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCommand_CenterDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	POSITION pos;
	pos=pListenSocket->ChildSocket_List.GetHeadPosition();
	CChildSocket* pChild = NULL;
	while(pos != NULL){
		pChild=(CChildSocket*)pListenSocket->ChildSocket_List.GetNext(pos);
		if(pChild != NULL){
			pChild->ShutDown();
			pChild->Close();
			delete pChild;
		}
	}
	pListenSocket->ShutDown();
	pListenSocket->Close();
	UpdateData(TRUE);																				/* ����Ʈ ��Ʈ�ѿ� �ִ� ���� ������ ���� */
	Gain gain = {Gain_P, Gain_I, Gain_D, P_Centroid, I_Centroid, D_Centroid, P_Speed, I_Speed};		/* ���ΰ� ���� */
	if(!(SaveGains(gain))){																			/* ������ ���ΰ� ���� */
		AfxMessageBox(_T("Cannot save gain in file"));
	}
	delete pGraph_Angle;
	//delete pGraph_Encoder;
}

void CCommand_CenterDlg::OnBnClickedButtonSend()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	memset(&pc2mcu, 0, sizeof(pc2mcu));						/* Ŭ���� */
	pc2mcu.gain.pPosture = Gain_P;
	pc2mcu.gain.iPosture = Gain_I;
	pc2mcu.gain.dPosture = Gain_D;
	pc2mcu.gain.pCentroid = P_Centroid;
	pc2mcu.gain.iCentroid = I_Centroid;
	pc2mcu.gain.dCentroid = D_Centroid;
	pc2mcu.gain.pPosition = P_Speed;
	pc2mcu.gain.iPosition = I_Speed;
	pc2mcu.drive.dir = (char)TUNING;
	pc2mcu.drive.speed = 0;
	CreateCheckSum_PC2MCU();								/* üũ�� �ʵ� ������ ���� */
	pListenSocket->BroadCast(&pc2mcu, sizeof(pc2mcu));		/* Ŭ���̾�Ʈ�� ���� */
	if(!(SaveGains(pc2mcu.gain))){							/* �׶��׶� ���ΰ� ���� */
		AfxMessageBox(_T("Cannot save gain in file"));
	}
	memset(&pc2mcu, 0, sizeof(pc2mcu));						/* Ŭ���� */
}

void CCommand_CenterDlg::OnOK()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	this->OnBnClickedButtonSend();
	//CDialogEx::OnOK();
}

void CCommand_CenterDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	//if(nIDEvent == TIM_ID_GRAPH){
	//	double t = (double)GetTickCount()/1000.0;
	//	double value[3] = {5.0*cos(t), 5.0*sin(t), 5.0*tan(t) };
	//	pGraph_Angle->AppendPoints(value);
	//}
	//else if(nIDEvent == TIM_ID_DRAW_GRAPH){
	//	pListenSocket->pChildSocket->Append2Graph();
	//}
	if(nIDEvent == TIM_RECEIVE){				// �κ����� � �޼��� �����Ѱ��� ó��
		if(!q.empty()){
			CString temp;
			MCU2PC mcu2pc = q.front();	q.pop();
			if(mcu2pc.command & 0x80){					/* MSB�� �ݹ�, �ݹ��� ��ε�ĳ���� ���� ���� */
				if(mcu2pc.command & 0x1F){				/* ������ */
					temp.Format(_T("%s: Robot is Driving,, "), strIPAddress);
					if(mcu2pc.command & 0x08){					/* ��ȸ�� */
						temp += "Right";
					}
					else if(mcu2pc.command & 0x04){				/* ��ȸ�� */
						temp += "Left";
					}
					else if(mcu2pc.command & 0x02){				/* ���� */
						temp += "Forward";
					}
					else if(mcu2pc.command & 0x01){				/* ���� */
						temp += "Stop";
					}
					else if(mcu2pc.command & 0x10){				/* ���� */
						temp += "Backward";
					}
				}
				else{
					temp.Format(_T("%s: Tuning Complete,, "), strIPAddress);
				}
				m_List_State.AddString(temp);
				m_List_State.SetCurSel(m_List_State.GetCount()-1);
			}
			else if(mcu2pc.command & 0x40){						/* �غ� �ƴٴ� �޼��� */
				double value[3] = {mcu2pc.monitoring.angle,};
				pGraph_Angle->AppendPoints(value);
				temp.Format(_T("%s: Robot is Ready,, "), strIPAddress);
				m_List_State.AddString(temp);
				m_List_State.SetCurSel(m_List_State.GetCount()-1);
			}
			else if(mcu2pc.command == 0x00){
				double value[3] = {mcu2pc.monitoring.angle, 
					mcu2pc.monitoring.encoder.left, mcu2pc.monitoring.encoder.right};
				//for(int i=0; i<3; i++){
				//	pGraph_Angle->AppendPoints(value);								/* ���� ���� �ΰ� �߰� */
				//	pGraph_Encoder->AppendPoints(value+1);							/* �׷����� ������ ���������� */
				//}
				pGraph_Angle->AppendPoints(value);
				pGraph_Encoder->AppendPoints(value+1);
				pGraph_Angle->AppendPoints(value);
				pGraph_Encoder->AppendPoints(value+1);
			}
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}


BOOL CCommand_CenterDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if(pMsg->message == WM_KEYDOWN){
		if(!CheckDriveClicked()){
			SetDriveClicked(WM_KEYDOWN);
			switch (pMsg->wParam)
			{
			case VK_LEFT:
				m_BtnL.SetCheck(true);
				DriveCommand(LEFT);
				break;
			case VK_RIGHT:
				m_BtnR.SetCheck(true);
				DriveCommand(RIGHT);
				break;
			case VK_UP:
				m_BtnF.SetCheck(true);
				DriveCommand(FORWARD);
				break;
			case VK_DOWN:
				m_BtnB.SetCheck(true);
				DriveCommand(BACKWARD);
				break;
			default:
				break;
			}
		}
	}
	else if(pMsg->message == WM_KEYUP){
		SetDriveClicked(WM_KEYUP);
		switch (pMsg->wParam)
		{
		case VK_LEFT:
			m_BtnL.SetCheck(false);
			DriveCommand(STOP);
			break;
		case VK_RIGHT:
			m_BtnR.SetCheck(false);
			DriveCommand(STOP);
			break;
		case VK_UP:
			m_BtnF.SetCheck(false);
			DriveCommand(STOP);
			break;
		case VK_DOWN:
			m_BtnB.SetCheck(false);
			DriveCommand(STOP);
			break;
		default:
			break;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

bool CCommand_CenterDlg::CheckDriveClicked(void)
{
	return flags.button_clicked;
}

void CCommand_CenterDlg::SetDriveClicked(int mode)
{
	switch (mode)
	{
	case WM_KEYUP:
		flags.button_clicked = false;
		break;
	case WM_KEYDOWN:
		flags.button_clicked = true;
		break;
	default:
		break;
	}
}


bool CCommand_CenterDlg::ReadGains(void)
{
	// Variables
	FILE *pFile;
	Gain gain;
	// File Open
	if(!(pFile = fopen("gain.txt", "r")))
		return false;
	// Get gains
	if((fscanf(pFile, "%lf %lf %lf %lf %lf %lf %lf", &gain.pPosture, &gain.iPosture, &gain.dPosture, 
		&gain.pCentroid, &gain.iCentroid, &gain.dCentroid, &gain.pPosition)) == EOF)
		return false;
	// Output to Edit Control
	Gain_P = gain.pPosture;
	Gain_I = gain.iPosture;
	Gain_D = gain.dPosture;
	P_Centroid = gain.pCentroid;
	I_Centroid = gain.iCentroid;
	D_Centroid = gain.dCentroid;	
	P_Speed = gain.pPosition;
	UpdateData(FALSE);
	// close file
	fclose(pFile);
	// end..
	return true;
}


bool CCommand_CenterDlg::SaveGains(Gain gain)
{
	// Variables
	FILE *pFile;
	// file open
	if(!(pFile = fopen("C:\\temp\\gain.txt", "w")))
		return false;
	// Save gain values in file
	fprintf(pFile, "%f\n", gain.pPosture);
	fprintf(pFile, "%f\n", gain.iPosture);
	fprintf(pFile, "%f\n", gain.dPosture);
	fprintf(pFile, "%f\n", gain.pCentroid);
	fprintf(pFile, "%f\n", gain.iCentroid);
	fprintf(pFile, "%f\n", gain.dCentroid);
	fprintf(pFile, "%f\n", gain.pPosition);
	// close file
	fclose(pFile);
	// end..
	return true;
}


void CCommand_CenterDlg::GraphConfiguration(void)
{
	CRect Graph_Rect_Angle, Graph_Rect_Encoder;
	GetDlgItem(IDC_STATIC_GRAPH_ENGLE)->GetWindowRect(Graph_Rect_Angle);							/* CRect ���� */
	GetDlgItem(IDC_STATIC_GRAPH_ENCODER)->GetWindowRect(Graph_Rect_Encoder);
	ScreenToClient(Graph_Rect_Angle);																/* Screen To Client */
	ScreenToClient(Graph_Rect_Encoder);

	pGraph_Angle = new COScopeCtrl(1);																/* ������ ���� �׷��� ���� */
	pGraph_Angle->Create(WS_VISIBLE | WS_CHILD, Graph_Rect_Angle, this, IDC_STATIC_GRAPH_ENGLE);
	pGraph_Angle->SetRanges(-27.0, 27.0);
	pGraph_Angle->autofitYscale = true;
	pGraph_Angle->SetYUnits(_T("Value"));
	pGraph_Angle->SetXUnits(_T("Time"));
	pGraph_Angle->SetLegendLabel(_T("Angle"), 0);
	pGraph_Angle->SetPlotColor(RGB(255,0,0), 0);
	pGraph_Angle->InvalidateCtrl();

	pGraph_Encoder = new COScopeCtrl(2);															/* ���ڴ��� ���� �׷��� ���� */
	pGraph_Encoder->Create(WS_VISIBLE | WS_CHILD, Graph_Rect_Encoder, this, IDC_STATIC_GRAPH_ENGLE);
	pGraph_Encoder->SetRanges(-2000.0, 2000.0);
	pGraph_Encoder->autofitYscale = true;
	pGraph_Encoder->SetYUnits(_T("Value"));
	pGraph_Encoder->SetXUnits(_T("Time"));
	pGraph_Encoder->SetLegendLabel(_T("Step Pulse(Left)"), 0);
	pGraph_Encoder->SetLegendLabel(_T("Step Pulse(Right)"), 1);
	pGraph_Encoder->SetPlotColor(RGB(0,0,255), 0);
	pGraph_Encoder->SetPlotColor(RGB(0,255,0), 1);
	pGraph_Encoder->InvalidateCtrl();
}


void CCommand_CenterDlg::DriveCommand(char dir)
{
	memset(&pc2mcu, 0, sizeof(pc2mcu));
	pc2mcu.drive.dir = dir;
	UpdateData(TRUE), pc2mcu.drive.speed = (short)((int)((long)m_speed_cmd));
	CreateCheckSum_PC2MCU();
	pListenSocket->BroadCast(&pc2mcu, sizeof(pc2mcu));
}


void CCommand_CenterDlg::CreateCheckSum_PC2MCU(void)
{
	char* ptr = (char*)(&pc2mcu);
	int temp = (int)NULL;
	for(int i=1; i<sizeof(pc2mcu); i++){
		temp += ptr[i];								/* ��� ���ϱ� */
		temp &= 0x000000FF;							/* 8��Ʈ�� ����ŷ */
	}
	pc2mcu.checksum = (char)temp;
}