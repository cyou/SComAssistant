// SCOMMDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SCOMM.h"
#include "SCOMMDlg.h"
#include "io.h"
#include "math.h"
#include ".\scommdlg.h"
#include "Device.h"
#include "Protocol.h"

#include "DSCProtocol.h"
#include "ModbusProtocol.h"
#include "WMTProtocol.h"
#include "PTUProtocol.h"
#include "CRC16.h"

#include "IniReader.h"
#include "IniWriter.h"
#include "DB.h"


#define DB_HOST			"localhost"
#define DB_USER			"root"
#define DB_PASSW		"root"
#define DB_NAME			"comm"
#define DB_PORT			3306
#define DB_UNIX_SOCKET	NULL


//#define HEAD_LEN  4
//#define BODY_LEN  215


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BufSize 512

#define Settings_CFG ".\\settings.ini"

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CHyperLink	m_ctrlHyperLinkWWWXFS;
	CHyperLink	m_ctrlHyperLink1;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_STATIC_WWWXFS, m_ctrlHyperLinkWWWXFS);
	DDX_Control(pDX, IDC_STATIC_MAILXFS1, m_ctrlHyperLink1);
	//}}AFX_DATA_MAP
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ctrlHyperLink1.SetURL(_T("mailto:webmaster@gjwtech.com"));
	m_ctrlHyperLink1.SetUnderline(TRUE);
	m_ctrlHyperLink1.SetLinkCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_HAND));

	m_ctrlHyperLinkWWWXFS.SetURL(_T("http://www.gjwtech.com"));
	m_ctrlHyperLinkWWWXFS.SetUnderline(TRUE);
	m_ctrlHyperLinkWWWXFS.SetLinkCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_HAND));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSCOMMDlg dialog

CSCOMMDlg::CSCOMMDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSCOMMDlg::IDD, pParent), m_crc16(CRC16::instance())
{
	//{{AFX_DATA_INIT(CSCOMMDlg)
	m_ReceiveData = _T("");
	m_strSendData = _T("");
	m_strCurPath = _T("");
	m_strSendFilePathName = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bAutoSend=FALSE;
	m_bVisible=FALSE;
	m_bSendFile=FALSE;
	m_bStopDispRXData=TRUE;
	m_bOpenPort=FALSE;
	m_nCycleTime=1000;
	m_strCurPath="C:\\COMDATA";
	m_strSendFilePathName="还没有选择文件";//"No File Selected!";
	m_nFileLength=0;

	m_is_DeviceData_ready = FALSE;
	p_activeDevice = NULL;

	m_writeDB = FALSE;
}


CSCOMMDlg::~CSCOMMDlg()
{

	for (int j =0; j < MAX_NUM_DEVICE; j++)
	{
		delete p_Devices[j];
	}
}


void CSCOMMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSCOMMDlg)
	DDX_Control(pDX, IDC_BUTTONSTART, m_ctrlProfileStart);
	DDX_Control(pDX, IDC_COMBO_CHANNEL, m_ctrSendChannel);
	DDX_Control(pDX, IDC_STATIC_OPENOFF2, m_ctrlIconOpenoff2);
	DDX_Control(pDX, IDC_BUTTON_OPENPORT4, m_ctrlOpenPort4);
	DDX_Control(pDX, IDC_BUTTON_OPENPORT3, m_ctrlOpenPort3);
	DDX_Control(pDX, IDC_BUTTON_OPENPORT2, m_ctrlOpenPort2);
	DDX_Control(pDX, IDC_STATIC_OPENOFF4, m_ctrlIconOpenoff4);
	DDX_Control(pDX, IDC_STATIC_OPENOFF3, m_ctrlIconOpenoff3);
	DDX_Control(pDX, IDC_COMBO_USEDPORT3, m_usePort3);
	DDX_Control(pDX, IDC_COMBO_USEDPORT4, m_usePort4);
	DDX_Control(pDX, IDC_COMBO_USEDPORT1, m_usePort);
	DDX_Control(pDX, IDC_COMBO_USEDPORT2, m_usePort2);
	DDX_Control(pDX, IDC_BUTTON_SETPORT4, m_setPort4);
	DDX_Control(pDX, IDC_BUTTON_SETPORT3, m_setPort3);
	DDX_Control(pDX, IDC_BUTTON_SETPORT2, m_setPort2);
	DDX_Control(pDX, IDC_BUTTON_SETPORT1, m_setPort);
	DDX_Control(pDX, IDC_BUTTON_DEVSTART4, m_devStart4);
	DDX_Control(pDX, IDC_BUTTON_DEVSTART3, m_devStart3);
	DDX_Control(pDX, IDC_BUTTON_DEVSTART2, m_devStart2);
	DDX_Control(pDX, IDC_BUTTON_DEVSTART1, m_devStart);
	DDX_Control(pDX, IDC_COMBO_PROTOCAL4, m_Protocal4);
	DDX_Control(pDX, IDC_COMBO_PROTOCAL3, m_Protocal3);
	DDX_Control(pDX, IDC_COMBO_PROTOCAL2, m_Protocal2);
	DDX_Control(pDX, IDC_COMBO_PROTOCAL1, m_Protocal);
	DDX_Control(pDX, IDC_COMBO_STOPBITS4, m_StopBits4);
	DDX_Control(pDX, IDC_COMBO_STOPBITS3, m_StopBits3);
	DDX_Control(pDX, IDC_COMBO_STOPBITS2, m_StopBits2);
	DDX_Control(pDX, IDC_COMBO_SPEED4, m_Speed4);
	DDX_Control(pDX, IDC_COMBO_SPEED3, m_Speed3);
	DDX_Control(pDX, IDC_COMBO_SPEED2, m_Speed2);
	DDX_Control(pDX, IDC_COMBO_PARITY4, m_Parity4);
	DDX_Control(pDX, IDC_COMBO_PARITY3, m_Parity3);
	DDX_Control(pDX, IDC_COMBO_PARITY2, m_Parity2);
	DDX_Control(pDX, IDC_COMBO_DATABITS4, m_DataBits4);
	DDX_Control(pDX, IDC_COMBO_DATABITS3, m_DataBits3);
	DDX_Control(pDX, IDC_COMBO_DATABITS2, m_DataBits2);
	DDX_Control(pDX, IDC_COMBO_COMSELECT4, m_Com4);
	DDX_Control(pDX, IDC_COMBO_COMSELECT3, m_Com3);
	DDX_Control(pDX, IDC_COMBO_COMSELECT2, m_Com2);
	DDX_Control(pDX, IDC_BUTTON_WRITEDB, m_ctrlWriteDB);
	DDX_Control(pDX, IDC_BUTTON_PUSHPIN, m_ctrlPushPin);
	DDX_Control(pDX, IDC_BUTTON_SENDFILE, m_ctrlSendFile);
	DDX_Control(pDX, IDC_EDIT_SENDFILE, m_ctrlEditSendFile);
	DDX_Control(pDX, IDC_STATIC_TXCOUNT, m_ctrlTXCount);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_ctrlPortStatus);
	DDX_Control(pDX, IDC_STATIC_RXCOUNT, m_ctrlRXCOUNT);
	DDX_Control(pDX, IDC_EDIT_SAVEPATH, m_ctrlSavePath);
	DDX_Control(pDX, IDC_BUTTON_MANUALSEND, m_ctrlManualSend);
	DDX_Control(pDX, IDC_BUTTON_CLEARRECASENDA, m_ctrlClearTXData);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_ctrlClose);
	DDX_Control(pDX, IDC_BUTTON_COUNTRESET, m_ctrlCounterReset);
	DDX_Control(pDX, IDC_EDIT_SEND, m_ctrlEditSend);
	DDX_Control(pDX, IDC_EDIT_RECIVE, m_ctrlReceiveData);
	DDX_Control(pDX, IDC_CHECK_AUTOCLEAR, m_ctrlAutoClear);
	DDX_Control(pDX, IDC_STATIC_OPENOFF, m_ctrlIconOpenoff);
	DDX_Control(pDX, IDC_COMBO_STOPBITS, m_StopBits);
	DDX_Control(pDX, IDC_COMBO_DATABITS, m_DataBits);
	DDX_Control(pDX, IDC_COMBO_PARITY, m_Parity);
	DDX_Control(pDX, IDC_CHECK_AUTOSEND, m_ctrlAutoSend);
	DDX_Control(pDX, IDC_CHECK_HEXSEND, m_ctrlHexSend);
	DDX_Control(pDX, IDC_BUTTON_STOPDISP, m_ctrlStopDisp);
	DDX_Control(pDX, IDC_BUTTON_OPENPORT, m_ctrlOpenPort);
	DDX_Control(pDX, IDC_CHECK_HEXRECIEVE, m_ctrlHexReceieve);
	DDX_Control(pDX, IDC_COMBO_SPEED, m_Speed);
	DDX_Control(pDX, IDC_COMBO_COMSELECT, m_Com);
	DDX_Text(pDX, IDC_EDIT_RECIVE, m_ReceiveData);
	DDX_Text(pDX, IDC_EDIT_SEND, m_strSendData);
	DDX_Text(pDX, IDC_EDIT_SAVEPATH, m_strCurPath);
	DDV_MaxChars(pDX, m_strCurPath, 200);
	DDX_Text(pDX, IDC_EDIT_SENDFILE, m_strSendFilePathName);
	DDV_MaxChars(pDX, m_strSendFilePathName, 200);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSCOMMDlg, CDialog)
	//{{AFX_MSG_MAP(CSCOMMDlg)
	ON_MESSAGE(WM_COMM_RXCHAR, OnCommunication)
	ON_MESSAGE(WM_COMM_TXEMPTY_DETECTED, OnFileSendingEnded)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_RECI_AREA, OnButtonClearReciArea)
	ON_BN_CLICKED(IDC_BUTTON_OPENPORT, OnButtonOpenport)
	ON_BN_CLICKED(IDC_BUTTON_STOPDISP, OnButtonStopdisp)
	ON_BN_CLICKED(IDC_BUTTON_MANUALSEND, OnButtonManualsend)
	ON_BN_CLICKED(IDC_CHECK_AUTOSEND, OnCheckAutosend)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_CYCLETIME, OnChangeEditCycletime)
	ON_EN_CHANGE(IDC_EDIT_SEND, OnChangeEditSend)
	ON_BN_CLICKED(IDC_BUTTON_CLEARRECASENDA, OnButtonClearrecasenda)
	ON_CBN_SELENDOK(IDC_COMBO_COMSELECT, OnSelendokComboComselect)
	ON_CBN_SELENDOK(IDC_COMBO_SPEED, OnSelendokComboSpeed)
	ON_CBN_SELENDOK(IDC_COMBO_PARITY, OnSelendokComboParity)
	ON_CBN_SELENDOK(IDC_COMBO_DATABITS, OnSelendokComboDatabits)
	ON_CBN_SELENDOK(IDC_COMBO_STOPBITS, OnSelendokComboStopbits)
	ON_BN_CLICKED(IDC_BUTTON_SAVEDATA, OnButtonSavedata)
	ON_BN_CLICKED(IDC_BUTTON_DIRBROWSER, OnButtonDirbrowser)
	ON_BN_CLICKED(IDC_BUTTON_SENDFILE, OnButtonSendfile)
	ON_BN_CLICKED(IDC_BUTTON_COUNTRESET, OnButtonCountreset)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_FILEBROWSER, OnButtonFilebrowser)
	ON_BN_CLICKED(IDC_BUTTON_PUSHPIN, OnButtonPushpin)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_HELP, OnButtonHelp)
	ON_BN_CLICKED(IDC_BUTTON_WRITEDB, OnButtonWritedb)
	ON_BN_CLICKED(IDC_BUTTON_OPENPORT2, OnButtonOpenport2)
	ON_BN_CLICKED(IDC_BUTTON_OPENPORT3, OnButtonOpenport3)
	ON_BN_CLICKED(IDC_BUTTON_OPENPORT4, OnButtonOpenport4)
	ON_CBN_EDITCHANGE(IDC_COMBO_COMSELECT2, OnEditchangeComboComselect2)
	ON_CBN_EDITCHANGE(IDC_COMBO_COMSELECT3, OnEditchangeComboComselect3)
	ON_CBN_EDITCHANGE(IDC_COMBO_COMSELECT4, OnEditchangeComboComselect4)
	ON_CBN_EDITCHANGE(IDC_COMBO_SPEED2, OnEditchangeComboSpeed2)
	ON_CBN_EDITCHANGE(IDC_COMBO_SPEED3, OnEditchangeComboSpeed3)
	ON_CBN_EDITCHANGE(IDC_COMBO_SPEED4, OnEditchangeComboSpeed4)
	ON_CBN_EDITCHANGE(IDC_COMBO_PARITY2, OnEditchangeComboParity2)
	ON_CBN_EDITCHANGE(IDC_COMBO_PARITY3, OnEditchangeComboParity3)
	ON_CBN_EDITCHANGE(IDC_COMBO_PARITY4, OnEditchangeComboParity4)
	ON_CBN_EDITCHANGE(IDC_COMBO_DATABITS2, OnEditchangeComboDatabits2)
	ON_CBN_EDITCHANGE(IDC_COMBO_DATABITS3, OnEditchangeComboDatabits3)
	ON_CBN_EDITCHANGE(IDC_COMBO_DATABITS4, OnEditchangeComboDatabits4)
	ON_CBN_EDITCHANGE(IDC_COMBO_STOPBITS2, OnEditchangeComboStopbits2)
	ON_CBN_EDITCHANGE(IDC_COMBO_STOPBITS3, OnEditchangeComboStopbits3)
	ON_CBN_EDITCHANGE(IDC_COMBO_STOPBITS4, OnEditchangeComboStopbits4)
	ON_CBN_SELENDOK(IDC_COMBO_SPEED2, OnSelendokComboSpeed2)
	ON_CBN_SELENDOK(IDC_COMBO_COMSELECT2, OnSelendokComboComselect2)
	ON_CBN_SELENDOK(IDC_COMBO_SPEED3, OnSelendokComboSpeed3)
	ON_CBN_SELENDOK(IDC_COMBO_SPEED4, OnSelendokComboSpeed4)
	ON_CBN_SELENDOK(IDC_COMBO_COMSELECT3, OnSelendokComboComselect3)
	ON_CBN_SELENDOK(IDC_COMBO_COMSELECT4, OnSelendokComboComselect4)
	ON_CBN_SELENDOK(IDC_COMBO_PARITY2, OnSelendokComboParity2)
	ON_CBN_SELENDOK(IDC_COMBO_PARITY3, OnSelendokComboParity3)
	ON_CBN_SELENDOK(IDC_COMBO_PARITY4, OnSelendokComboParity4)
	ON_CBN_SELENDOK(IDC_COMBO_DATABITS2, OnSelendokComboDatabits2)
	ON_CBN_SELENDOK(IDC_COMBO_DATABITS3, OnSelendokComboDatabits3)
	ON_CBN_SELENDOK(IDC_COMBO_DATABITS4, OnSelendokComboDatabits4)
	ON_CBN_SELENDOK(IDC_COMBO_STOPBITS2, OnSelendokComboStopbits2)
	ON_CBN_SELENDOK(IDC_COMBO_STOPBITS3, OnSelendokComboStopbits3)
	ON_CBN_SELENDOK(IDC_COMBO_STOPBITS4, OnSelendokComboStopbits4)
	ON_BN_CLICKED(IDC_BUTTON_SETPORT1, OnButtonSetport1)
	ON_BN_CLICKED(IDC_BUTTON_SETPORT2, OnButtonSetport2)
	ON_BN_CLICKED(IDC_BUTTON_SETPORT3, OnButtonSetport3)
	ON_BN_CLICKED(IDC_BUTTON_SETPORT4, OnButtonSetport4)
	ON_BN_CLICKED(IDC_BUTTON_DEVSTART1, OnButtonDevstart1)
	ON_BN_CLICKED(IDC_BUTTON_DEVSTART2, OnButtonDevstart2)
	ON_BN_CLICKED(IDC_BUTTON_DEVSTART3, OnButtonDevstart3)
	ON_BN_CLICKED(IDC_BUTTON_DEVSTART4, OnButtonDevstart4)
	ON_CBN_EDITCHANGE(IDC_COMBO_COMSELECT, OnEditchangeComboComselect)
	ON_BN_CLICKED(IDC_CHECK_CR, OnCheckCr)
	ON_BN_CLICKED(IDC_BUTTONSTART, OnButtonstart)
	ON_BN_CLICKED(IDC_CHECK_CRC, OnCheckCrc)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_EDIT_RECIVE, OnEnChangeEditRecive)
END_MESSAGE_MAP()
/*
BEGIN_DYNAMIC_MAP(CSCOMMDlg,CDialog)
	DYNAMIC_MAP_ENTRY(IDC_STATIC_SENDCHAR,   mdNone, mdRepos)
	DYNAMIC_MAP_ENTRY(IDC_CHECK_HEXSEND,     mdNone, mdRepos)
	//DYNAMIC_MAP_ENTRY(IDC_BUTTON_MANUALSEND, mdNone, mdRepos)
	//DYNAMIC_MAP_ENTRY(IDC_CHECK_AUTOSEND,    mdNone, mdRepos)
	DYNAMIC_MAP_ENTRY(IDC_STATIC_CYCLETIME,  mdNone, mdRepos)
	DYNAMIC_MAP_ENTRY(IDC_EDIT_CYCLETIME,    mdNone, mdRepos)
	DYNAMIC_MAP_ENTRY(IDC_STATIC_MS,         mdNone, mdRepos)
	DYNAMIC_MAP_ENTRY(IDC_BUTTON_FILEBROWSER,mdNone, mdRepos)
	DYNAMIC_MAP_ENTRY(IDC_EDIT_SENDFILE,     mdNone, mdRepos)
	DYNAMIC_MAP_ENTRY(IDC_BUTTON_SENDFILE,   mdNone, mdRepos)
	DYNAMIC_MAP_ENTRY(IDC_STATIC_GJW,        mdRepos, mdRepos)

//	DYNAMIC_MAP_ENTRY(IDC_PROGRESS,          mdResize, mdRepos)
//	DYNAMIC_MAP_ENTRY(IDC_STATIC_GJW,        mdRepos, mdRepos)

END_DYNAMIC_MAP()
*/
/////////////////////////////////////////////////////////////////////////////
// CSCOMMDlg message handlers

BOOL CSCOMMDlg::OnInitDialog()
{
	//CDialog::OnInitDialog();
	//BOOL b = CDialog::OnInitDialog(0,fdAll,true); 
    BOOL b = CDialog::OnInitDialog(); 
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}


	m_hIconRed  = AfxGetApp()->LoadIcon(IDI_ICON_RED);
	m_hIconOff	= AfxGetApp()->LoadIcon(IDI_ICON_OFF);
	m_hIconGreen= AfxGetApp()->LoadIcon(IDI_ICON_GREEN);

//	m_ctrlIconDatareceive.SetIcon(m_hIconOff);

	//初始化串口信息
	for (int i =0; i < MAX_NUM_SW_PORT; i++)
	{
		m_CommInfo[i].setBand(9600);
		m_CommInfo[i].setCom(i+1);
		m_CommInfo[i].setParity('N');
		m_CommInfo[i].setDatabits(8);
		m_CommInfo[i].setStopbits(1);

		m_bOpenPorts[i] = false;

		m_CommEvents[i] =  EV_RXFLAG | EV_RXCHAR;
	}
	//initial UI display and get values from settings.ini file.

	CIniReader iniReader(Settings_CFG);
 
	m_Com.SetCurSel(iniReader.ReadInteger("Setting", "com1_num", 0));
	m_Speed.SetCurSel(iniReader.ReadInteger("Setting", "com1_speed", 5));
	m_Parity.SetCurSel(iniReader.ReadInteger("Setting", "com1_parity", 0));
	m_DataBits.SetCurSel(iniReader.ReadInteger("Setting", "com1_databits", 0));
	m_StopBits.SetCurSel(iniReader.ReadInteger("Setting", "com1_stopbits", 0));

	this->ChangeComm(0, m_Com.GetCurSel());
	this->ChangeBoud(0, m_Speed.GetCurSel());
	this->ChangeParity(0, m_Parity.GetCurSel());
	this->ChangeDatabits(0, m_DataBits.GetCurSel());
	this->ChangeStopbits(0, m_StopBits.GetCurSel());

	m_Com2.SetCurSel(iniReader.ReadInteger("Setting", "com2_num", 1));
	m_Speed2.SetCurSel(iniReader.ReadInteger("Setting", "com2_speed", 5));
	m_Parity2.SetCurSel(iniReader.ReadInteger("Setting", "com2_parity", 0));
	m_DataBits2.SetCurSel(iniReader.ReadInteger("Setting", "com2_databits", 0));
	m_StopBits2.SetCurSel(iniReader.ReadInteger("Setting", "com2_stopbits", 0));

	this->ChangeComm(1, m_Com2.GetCurSel());
	this->ChangeBoud(1, m_Speed2.GetCurSel());
	this->ChangeParity(1, m_Parity2.GetCurSel());
	this->ChangeDatabits(1, m_DataBits2.GetCurSel());
	this->ChangeStopbits(1, m_StopBits2.GetCurSel());

	m_Com3.SetCurSel(iniReader.ReadInteger("Setting", "com3_num", 2));
	m_Speed3.SetCurSel(iniReader.ReadInteger("Setting", "com3_speed", 5));
	m_Parity3.SetCurSel(iniReader.ReadInteger("Setting", "com3_parity", 0));
	m_DataBits3.SetCurSel(iniReader.ReadInteger("Setting", "com3_databits", 0));
	m_StopBits3.SetCurSel(iniReader.ReadInteger("Setting", "com3_stopbits", 0));

	this->ChangeComm(2, m_Com3.GetCurSel());
	this->ChangeBoud(2, m_Speed3.GetCurSel());
	this->ChangeParity(2, m_Parity3.GetCurSel());
	this->ChangeDatabits(2, m_DataBits3.GetCurSel());
	this->ChangeStopbits(2, m_StopBits3.GetCurSel());

	m_Com4.SetCurSel(iniReader.ReadInteger("Setting", "com4_num", 3));
	m_Speed4.SetCurSel(iniReader.ReadInteger("Setting", "com4_speed", 5));
	m_Parity4.SetCurSel(iniReader.ReadInteger("Setting", "com4_parity", 0));
	m_DataBits4.SetCurSel(iniReader.ReadInteger("Setting", "com4_databits", 0));
	m_StopBits4.SetCurSel(iniReader.ReadInteger("Setting", "com4_stopbits", 0));

	this->ChangeComm(3, m_Com4.GetCurSel());
	this->ChangeBoud(3, m_Speed4.GetCurSel());
	this->ChangeParity(3, m_Parity4.GetCurSel());
	this->ChangeDatabits(3, m_DataBits4.GetCurSel());
	this->ChangeStopbits(3, m_StopBits4.GetCurSel());

	// initial device UI.
	this->m_Protocal.SetCurSel(iniReader.ReadInteger("Setting", "channel1_protocal", 0));
	this->m_Protocal2.SetCurSel(iniReader.ReadInteger("Setting", "channel2_protocal", 0));
	this->m_Protocal3.SetCurSel(iniReader.ReadInteger("Setting", "channel3_protocal", 0));
	this->m_Protocal4.SetCurSel(iniReader.ReadInteger("Setting", "channel4_protocal", 0));

	this->m_usePort.SetCurSel(iniReader.ReadInteger("Setting", "channel1_port", 0));
	this->m_usePort2.SetCurSel(iniReader.ReadInteger("Setting", "channel2_port", 0));
	this->m_usePort3.SetCurSel(iniReader.ReadInteger("Setting", "channel3_port", 0));
	this->m_usePort4.SetCurSel(iniReader.ReadInteger("Setting", "channel4_port", 0));


	this->m_ctrSendChannel.SetCurSel(0);

    this->m_nLongReceiveTimeout = iniReader.ReadInteger("Setting", "long_receive_timeout", 500);
	this->m_nShortReceiveTimeout = iniReader.ReadInteger("Setting", "short_receive_timeout", 20);

	// initial device object.
	for (int j =0; j < MAX_NUM_DEVICE; j++)
	{
		p_Devices[j] = new Device(j, 0);
		//p_Devices[j]->setCommInfo(m_CommInfo[j%MAX_NUM_DEVICE]);
		//p_Devices[j]->setSerialPort(&this->m_Ports[j%MAX_NUM_DEVICE]);

		SetDeviceSettings(j, 0, 0);
	}

	// initial inteval time.
	this->m_nIntervalTime = 5000;
	
	CEdit* pEdit2=(CEdit*)GetDlgItem(IDC_EDIT_INTERVAL);
	CString strText2;
	strText2.Format("%d",m_nIntervalTime);
	pEdit2->SetWindowText(strText2);

	m_ProfileStart = false;


	m_nBaud=9600;
	m_nCom=1;
	m_cParity='N';
	m_nDatabits=8;
	m_nStopbits=1;
	m_dwCommEvents = EV_RXFLAG | EV_RXCHAR;
	//if (m_Port.InitPort(this, 1, 9600,'N',8,1,dwCommEvents,512))
	CString strStatus;
	/*if (m_Port.InitPort(this, m_nCom, m_nBaud,m_cParity,m_nDatabits,m_nStopbits,m_dwCommEvents,512))
	{
		m_Port.StartMonitoring();
		strStatus.Format("STATUS：COM%d OPENED，%d,%c,%d,%d",m_nCom, m_nBaud,m_cParity,m_nDatabits,m_nStopbits);
		m_ctrlIconOpenoff.SetIcon(m_hIconRed);
		//m_ctrlIconOpenoff.SetIcon(m_hIconOff);

		//"当前状态：串口打开，无奇偶校验，8数据位，1停止位");
	}
	else
	{
		AfxMessageBox("没有发现此串口");
		m_ctrlIconOpenoff.SetIcon(m_hIconOff);
	}*/
	m_ctrlPortStatus.SetWindowText(strStatus);

	CEdit* pEdit=(CEdit*)GetDlgItem(IDC_EDIT_CYCLETIME);
	CString strText;
	strText.Format("%d",m_nCycleTime);
	pEdit->SetWindowText(strText);   //Display cycle time

	m_strSendData="http://www.gjwtech.com";
	char cr=13,lf=10;;
	m_strSendData+=cr+lf;

	m_ctrlAutoClear.SetCheck(1);
	m_ctrlEditSendFile.SetWindowText("No File Selected!");

	m_animIcon.SetImageList(IDB_ANIM_IMGLIST,4,RGB(0,0,0));   
	//SetTimer(4,200,NULL);   

	CButton* pBtn = (CButton*) GetDlgItem(IDC_CHECK_CR);
	pBtn->SetCheck(0);// check it
	m_AutoAddCR = FALSE;
	m_AutoAddCRC = FALSE;

	m_hProfileEvent = NULL;
	
	UpdateData(FALSE);


	ShowWindow(SW_SHOW); 
//	return TRUE;  // return TRUE  unless you set the focus to a control
	return b;
}

void CSCOMMDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSCOMMDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSCOMMDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

Device* CSCOMMDlg::GetActiveDevice()
{
	for (int i = 0; i < MAX_NUM_DEVICE; i++){
		if (this->p_Devices[i]->isActive())
			return this->p_Devices[i];
	}
	return NULL;
}

static long rxdatacount=0;  //该变量用于接收字符计数
LONG CSCOMMDlg::OnCommunication(WPARAM ch, LPARAM port)
{
	if (port <= 0 || port > MAX_NUM_HW_PORT)
		return -1;

	if (p_activeDevice && p_activeDevice->isDeviceOpen()) {
		p_activeDevice->getProtocol()->AddDataToBuffer((char) ch);
		this->SetTimer(RECEIVE_CHAR_TIMEOUT_EVENT_ID, this->m_nShortReceiveTimeout, NULL);
	}

	rxdatacount++;   //接收的字节计数
	CString strTemp;
	strTemp.Format("%ld",rxdatacount);
	strTemp="RX:"+strTemp;
	m_ctrlRXCOUNT.SetWindowText(strTemp);  //显示接收计数
	
	if(m_bStopDispRXData)   //如果选择了“停止显示”接收数据，则返回
		return -1;          //注意，这种情况下，计数仍在继续，只是不显示
	//若设置了“自动清空”，则达到50行后，自动清空接收编辑框中显示的数据
	if((m_ctrlAutoClear.GetCheck())&&(m_ctrlReceiveData.GetLineCount()>=50))
	{
		m_ReceiveData.Empty();
		UpdateData(FALSE);
	}
	//如果没有“自动清空”，数据行达到400后，也自动清空
	//因为数据过多，影响接收速度，显示是最费CPU时间的操作
	if(m_ctrlReceiveData.GetLineCount()>400)
	{
		m_ReceiveData.Empty();
		m_ReceiveData="***The Length of the Text is too long, Emptied Automaticly!!!***\r\n";        
		UpdateData(FALSE);
	}

	//如果选择了"十六进制显示"，则显示十六进制值
	CString str;
	if(m_ctrlHexReceieve.GetCheck())
		str.Format("%02X ",ch);
	else 
		str.Format("%c",ch);
	//以下是将接收的字符加在字符串的最后，这里费时很多
	//但考虑到数据需要保存成文件，所以没有用List Control
	int nLen=m_ctrlReceiveData.GetWindowTextLength();
	m_ctrlReceiveData.SetSel(nLen, nLen);
	m_ctrlReceiveData.ReplaceSel(str);
	nLen+=str.GetLength();

	m_ReceiveData+=str;
	return 0;
}

//清空接收区
void CSCOMMDlg::OnButtonClearReciArea() 
{
	// TODO: Add your control notification handler code here
	m_ReceiveData.Empty();
	UpdateData(FALSE);
}

void CSCOMMDlg::StartOrStopPort(int i, CButton* openPort, CStatic* statusIcon)
{
	// TODO: Add your control notification handler code here
	
	if(m_bOpenPorts[i])  //关闭串口
	{

		openPort->SetWindowText("打开串口");
		m_Ports[i].ClosePort();//关闭串口
		m_ctrlPortStatus.SetWindowText("STATUS：COM Port Closed");
		statusIcon->SetIcon(m_hIconOff);

		m_bOpenPorts[i] =! m_bOpenPorts[i];
	}
	else  //打开串口
	{
		openPort->SetWindowText("关闭串口");
		

		int m_nCom = this->m_CommInfo[i].getCom();
		int m_nBaud = this->m_CommInfo[i].getBand();
		char m_cParity = this->m_CommInfo[i].getParity();
		int m_nDatabits = this->m_CommInfo[i].getDatabits();
		int m_nStopbits = this->m_CommInfo[i].getStopbits();
		int m_dwCommEvents = this->m_CommEvents[i];

		CString strStatus;
		if (m_Ports[i].InitPort(this, m_nCom, m_nBaud,m_cParity,m_nDatabits,m_nStopbits,m_dwCommEvents,512))
		{
			m_Ports[i].StartMonitoring();
			statusIcon->SetIcon(m_hIconRed);
			strStatus.Format("STATUS：COM%d OPENED，%d,%c,%d,%d",m_nCom, m_nBaud,m_cParity,m_nDatabits,m_nStopbits);
	
			//"当前状态：串口打开，无奇偶校验，8数据位，1停止位");

			m_bOpenPorts[i] =! m_bOpenPorts[i];
		}
		else
		{
			AfxMessageBox("没有发现此串口或被占用");
			
			openPort->SetWindowText("打开串口");
			statusIcon->SetIcon(m_hIconOff);
		}
		m_ctrlPortStatus.SetWindowText(strStatus);
	}

	

}

//打开/关闭串口
void CSCOMMDlg::OnButtonOpenport() 
{
	StartOrStopPort(0, &m_ctrlOpenPort, &m_ctrlIconOpenoff);
}


//停止/继续显示接收数据
void CSCOMMDlg::OnButtonStopdisp() 
{
	// TODO: Add your control notification handler code here
	m_bStopDispRXData=!m_bStopDispRXData;
	if(m_bStopDispRXData)
		m_ctrlStopDisp.SetWindowText("继续显示");
	else
		m_ctrlStopDisp.SetWindowText("停止显示");
}


char CSCOMMDlg::HexChar(char c)
{
	if((c>='0')&&(c<='9'))
		return c-0x30;
	else if((c>='A')&&(c<='F'))
		return c-'A'+10;
	else if((c>='a')&&(c<='f'))
		return c-'a'+10;
	else 
		return 0x10;
}


//将一个字符串作为十六进制串转化为一个字节数组，字节间可用空格分隔，
//返回转换后的字节数组长度，同时字节数组长度自动设置。
int CSCOMMDlg::Str2Hex(CString str, char* data)
{
	int t,t1;
	int rlen=0,len=str.GetLength();
	//data.SetSize(len/2);
	for(int i=0;i<len;)
	{
		char l,h=str[i];
		if(h==' ')
		{
			i++;
			continue;
		}
		i++;
		if(i>=len)
			break;
		l=str[i];
		t=HexChar(h);
		t1=HexChar(l);
		if((t==16)||(t1==16))
			break;
		else 
			t=t*16+t1;
		i++;
		data[rlen]=(char)t;
		rlen++;
	}
	return rlen;

}

long TX_count=0;
void CSCOMMDlg::OnButtonManualsend() 
{
	// TODO: Add your control notification handler code here
	int swPortID;

	swPortID = m_ctrSendChannel.GetCurSel();

	if(m_Ports[swPortID].m_hComm==NULL)
	{
		m_ctrlAutoSend.SetCheck(0);
		AfxMessageBox("串口没有打开，请打开串口");
		return;
	}
	else
	{
		UpdateData(TRUE);
		
		if(m_ctrlHexSend.GetCheck())
		{
			char data[512];
			int len=Str2Hex(m_strSendData,data);
			if (m_AutoAddCR)
			{
				data[len] =13;
				data[len+1]=10;
				len +=2;
			}
			// add CRC check.
			if (m_AutoAddCRC)
			{
				this->m_crc16.Modbus_CRC16((unsigned char*)data, len);
				data[len] =(char)m_crc16.CRC16_Lo();
				data[len+1]=(char)m_crc16.CRC16_Hi();
				len +=2;
			}

			m_Ports[swPortID].WriteToPort(data,len);
			TX_count+=(long)((m_strSendData.GetLength()+1)/3);
			//m_Port.WriteToPort(hexdata);	
		}
		else 
		{
			CString tmpStr;
			tmpStr = m_strSendData;
			if (m_AutoAddCR)
			{
				tmpStr += '\n';
				tmpStr += '\r';
			}
			
			// add CRC check.
			if (m_AutoAddCRC)
			{
				this->m_crc16.Modbus_CRC16((unsigned char*)(LPCTSTR)tmpStr, tmpStr.GetLength());
				tmpStr +=(char)m_crc16.CRC16_Lo();
				tmpStr +=(char)m_crc16.CRC16_Hi();
			}

			m_Ports[swPortID].WriteToPort((LPCTSTR)tmpStr);	//发送数据
			TX_count+=m_strSendData.GetLength();
		}
		CString strTemp;
		strTemp.Format("TX:%d",TX_count);
		m_ctrlTXCount.SetWindowText(strTemp);
	}
}

void CSCOMMDlg::OnCheckAutosend() 
{
	// TODO: Add your control notification handler code here
	int swPortID;
	swPortID = m_ctrSendChannel.GetCurSel();

	m_bAutoSend=!m_bAutoSend;
	if(m_bAutoSend)
	{
		if(m_Ports[swPortID].m_hComm==NULL)
		{
			m_bAutoSend=!m_bAutoSend;
			m_ctrlAutoSend.SetCheck(0);
			AfxMessageBox("串口没有打开，请打开串口");
			return;
		}
		else
			SetTimer(1,m_nCycleTime,NULL);
	}
	else
	{
		KillTimer(1);
	}

}

void CSCOMMDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CString strStatus;
	switch(nIDEvent)
	{
	case 1:
		OnButtonManualsend();
		break;
	case 2:
		m_ctrlSavePath.SetWindowText(m_strCurPath);
		KillTimer(2);
		break;
	case 3:
		m_ctrlManualSend.EnableWindow(TRUE);
		m_ctrlAutoSend.EnableWindow(TRUE);
		m_ctrlSendFile.EnableWindow(TRUE);
		m_strSendFilePathName=m_strTempSendFilePathName;
		m_ctrlEditSendFile.SetWindowText(m_strSendFilePathName);//m_strSendFilePathName
		KillTimer(3);

		if(!(m_ctrlAutoSend.GetCheck()))
		{
			int i = m_ctrSendChannel.GetCurSel();
			int m_nCom = this->m_CommInfo[i].getCom();
			int m_nBaud = this->m_CommInfo[i].getBand();
			char m_cParity = this->m_CommInfo[i].getParity();
			int m_nDatabits = this->m_CommInfo[i].getDatabits();
			int m_nStopbits = this->m_CommInfo[i].getStopbits();
			int m_dwCommEvents = this->m_CommEvents[i];
			if (m_Ports[i].InitPort(this, m_nCom, m_nBaud,m_cParity,m_nDatabits,m_nStopbits,m_dwCommEvents,512))
			{
				m_Ports[i].StartMonitoring();
				strStatus.Format("STATUS：COM%d OPENED，%d,%c,%d,%d",m_nCom, m_nBaud,m_cParity,m_nDatabits,m_nStopbits);
				m_ctrlIconOpenoff.SetIcon(m_hIconRed);
			}
			else
			{
				AfxMessageBox("Failed to reset send buffer size!");
				m_ctrlIconOpenoff.SetIcon(m_hIconOff);
			}
			m_ctrlPortStatus.SetWindowText(strStatus);
		}
		break;
	case 4:
		m_animIcon.ShowNextImage();
		break;

	case PROFILE_EVENT_ID:
		this->OnProfileEvent();
		break;
	case RECEIVE_TIMEOUT_EVENT_ID:
		this->OnReceiveTimeOutEvent(RECEIVE_TIMEOUT_EVENT_ID);
		KillTimer(RECEIVE_TIMEOUT_EVENT_ID);
		break;
	case RECEIVE_CHAR_TIMEOUT_EVENT_ID:
		this->OnReceiveTimeOutEvent(RECEIVE_CHAR_TIMEOUT_EVENT_ID);
		KillTimer(RECEIVE_CHAR_TIMEOUT_EVENT_ID);
		break;
	default:
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

void CSCOMMDlg::OnChangeEditCycletime() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CEdit* pEdit=(CEdit*)GetDlgItem(IDC_EDIT_CYCLETIME);
	CString strText;
	pEdit->GetWindowText(strText);
	m_nCycleTime=atoi(strText);
}

void CSCOMMDlg::OnChangeEditSend() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

void CSCOMMDlg::OnButtonClearrecasenda() 
{
	// TODO: Add your control notification handler code here
	m_strSendData.Empty();
	UpdateData(FALSE);
}



void CSCOMMDlg::ChangeComm(int i, int selected)
{
    this->m_CommInfo[i].setCom(selected+1);
}


void CSCOMMDlg::ChangeBoud(int j, int selected)
{
	// TODO: Add your control notification handler code here
	//CString temp;
	int i=selected;
	switch(i)
	{
	case 0:
		i=300;
		break;
	case 1:
		i=600;
		break;
	case 2:
		i=1200;
		break;
	case 3:
		i=2400;
		break;
	case 4:
		i=4800;
		break;
	case 5:
		i=9600;
		break;
	case 6:
		i=19200;
		break;
	case 7:
		i=38400;
		break;
	case 8:
		i=43000;
		break;
	case 9:
		i=56000;
		break;
	case 10:
		i=57600;
		break;
	case 11:
		i=115200;
		break;
	default:
		break;

	}
	this->m_CommInfo[j].setBand(i);
}

void CSCOMMDlg::ChangeParity(int i, int selected)
{
	//int i=m_Parity.GetCurSel();
	char temp;
	switch(selected)
	{
	case 0:
		temp='N';
		break;
	case 1:
		temp='O';
		break;
	case 2:
		temp='E';
		break;
	}
	this->m_CommInfo[i].setParity(temp);

}

void CSCOMMDlg::ChangeDatabits(int j, int selected)
{
	int i=selected;
	switch(i)
	{
	case 0:
		i=8;
		break;
	case 1:
		i=7;
		break;
	case 2:
		i=6;
		break;
	}
	this->m_CommInfo[j].setDatabits(i);
}

void CSCOMMDlg::ChangeStopbits(int j, int selected)
{
	int i=selected;
	switch(i)
	{
	case 0:
		i=1;
		break;
	case 1:
		i=2;
		break;
	}
	m_nStopbits=i;

	this->m_CommInfo[j].setStopbits(i);

}

void CSCOMMDlg::UpdateCommFromEdit(int i, CComboBox &c) 
{


// TODO: Add your control notification handler code here
	CString comm;
	//m_Com.GetLBText(m_Com.GetCurSel(), comm);
	c.GetWindowText(comm);
	UINT HwPortId = 1;
	if (1 == sscanf(comm, "COM%d", &HwPortId))
	{
		this->ChangeComm(i, HwPortId-1);
	}
}


void CSCOMMDlg::OnSelendokComboComselect() 
{

	this->ChangeComm(0, m_Com.GetCurSel());
	// TODO: Add your control notification handler code here
	/*if(m_Port.m_hComm!=NULL)
	{
		m_Port.ClosePort();
	}

	m_nCom=m_Com.GetCurSel()+1;
	CString strStatus;
	if (m_Port.InitPort(this, m_nCom, m_nBaud,m_cParity,m_nDatabits,m_nStopbits,m_dwCommEvents,512))
	{
		if(!m_bOpenPort)
		{
			m_Port.StartMonitoring();
			m_ctrlIconOpenoff.SetIcon(m_hIconRed);
			strStatus.Format("STATUS：COM%d OPENED，%d,%c,%d,%d",m_nCom, m_nBaud,m_cParity,m_nDatabits,m_nStopbits);
		}
		else
		{
			m_ctrlIconOpenoff.SetIcon(m_hIconOff);
			strStatus.Format("STATUS：COM%d CLOSED，%d,%c,%d,%d",m_nCom, m_nBaud,m_cParity,m_nDatabits,m_nStopbits);
		}
		m_ctrlPortStatus.SetWindowText(strStatus);

	}
	else
	{
		m_ctrlIconOpenoff.SetIcon(m_hIconOff);
		AfxMessageBox("没有发现此串口或被占用");
	}*/

	
}

void CSCOMMDlg::OnSelendokComboSpeed() 
{
	this->ChangeBoud(0, m_Speed.GetCurSel());

	// TODO: Add your control notification handler code here
	/*CString temp;
	int i=m_Speed.GetCurSel();
	switch(i)
	{
	case 0:
		i=300;
		break;
	case 1:
		i=600;
		break;
	case 2:
		i=1200;
		break;
	case 3:
		i=2400;
		break;
	case 4:
		i=4800;
		break;
	case 5:
		i=9600;
		break;
	case 6:
		i=19200;
		break;
	case 7:
		i=38400;
		break;
	case 8:
		i=43000;
		break;
	case 9:
		i=56000;
		break;
	case 10:
		i=57600;
		break;
	case 11:
		i=115200;
		break;
	default:
		break;

	}
	m_nBaud=i;
	CString strStatus;
	if (m_Port.InitPort(this, m_nCom, m_nBaud,m_cParity,m_nDatabits,m_nStopbits,m_dwCommEvents,512))
	{
		if(!m_bOpenPort)
		{
			m_Port.StartMonitoring();
			m_ctrlIconOpenoff.SetIcon(m_hIconRed);
			

			strStatus.Format("STATUS：COM%d OPENED，%d,%c,%d,%d",m_nCom, m_nBaud,m_cParity,m_nDatabits,m_nStopbits);
		}
		else
		{
			m_ctrlIconOpenoff.SetIcon(m_hIconOff);
			strStatus.Format("STATUS：COM%d CLOSED，%d,%c,%d,%d",m_nCom, m_nBaud,m_cParity,m_nDatabits,m_nStopbits);
		}
		m_ctrlPortStatus.SetWindowText(strStatus);

	}
	else
	{
		AfxMessageBox("没有成功，请重试");
		m_ctrlIconOpenoff.SetIcon(m_hIconOff);

	}

	*/
}

void CSCOMMDlg::OnSelendokComboParity() 
{

	this->ChangeParity(0, m_Parity.GetCurSel());
	// TODO: Add your control notification handler code here
	/*char temp;
	int i=m_Parity.GetCurSel();
	switch(i)
	{
	case 0:
		temp='N';
		break;
	case 1:
		temp='O';
		break;
	case 2:
		temp='E';
		break;
	}
	m_cParity=temp;
	CString strStatus;
	if (m_Port.InitPort(this, m_nCom, m_nBaud,m_cParity,m_nDatabits,m_nStopbits,m_dwCommEvents,512))
	{
		if(!m_bOpenPort)
		{
			m_Port.StartMonitoring();
			m_ctrlIconOpenoff.SetIcon(m_hIconRed);
			strStatus.Format("STATUS：COM%d OPENED，%d,%c,%d,%d",m_nCom, m_nBaud,m_cParity,m_nDatabits,m_nStopbits);
		}
		else
		{
			m_ctrlIconOpenoff.SetIcon(m_hIconOff);
			strStatus.Format("STATUS：COM%d CLOSED，%d,%c,%d,%d",m_nCom, m_nBaud,m_cParity,m_nDatabits,m_nStopbits);
		}
		m_ctrlPortStatus.SetWindowText(strStatus);

	}
	else
	{
		AfxMessageBox("没有成功，请重试");
		m_ctrlIconOpenoff.SetIcon(m_hIconOff);
	}*/

	
}

void CSCOMMDlg::OnSelendokComboDatabits() 
{
	this->ChangeDatabits(0, m_DataBits.GetCurSel());

	/*
	// TODO: Add your control notification handler code here
	//char temp;
	int i=m_DataBits.GetCurSel();
	switch(i)
	{
	case 0:
		i=8;
		break;
	case 1:
		i=7;
		break;
	case 2:
		i=6;
		break;
	}
	m_nDatabits=i;
	CString strStatus;
	if (m_Port.InitPort(this, m_nCom, m_nBaud,m_cParity,m_nDatabits,m_nStopbits,m_dwCommEvents,512))
	{
		if(!m_bOpenPort)
		{
			m_Port.StartMonitoring();
			m_ctrlIconOpenoff.SetIcon(m_hIconRed);
			strStatus.Format("STATUS：COM%d OPENED，%d,%c,%d,%d",m_nCom, m_nBaud,m_cParity,m_nDatabits,m_nStopbits);
		}
		else
		{
			m_ctrlIconOpenoff.SetIcon(m_hIconOff);
			strStatus.Format("STATUS：COM%d CLOSED，%d,%c,%d,%d",m_nCom, m_nBaud,m_cParity,m_nDatabits,m_nStopbits);
		}
		m_ctrlPortStatus.SetWindowText(strStatus);

	}
	else
	{
		AfxMessageBox("没有成功，请重试");
		m_ctrlIconOpenoff.SetIcon(m_hIconOff);
	}*/
	
	
}

void CSCOMMDlg::OnSelendokComboStopbits() 
{
	this->ChangeStopbits(0, m_StopBits.GetCurSel());

	// TODO: Add your control notification handler code here
	/*int i=m_StopBits.GetCurSel();
	switch(i)
	{
	case 0:
		i=1;
		break;
	case 1:
		i=2;
		break;
	}
	m_nStopbits=i;
	CString strStatus;
	if (m_Port.InitPort(this, m_nCom, m_nBaud,m_cParity,m_nDatabits,m_nStopbits,m_dwCommEvents,512))
	{
		if(!m_bOpenPort)
		{
			m_Port.StartMonitoring();
			m_ctrlIconOpenoff.SetIcon(m_hIconRed);
			strStatus.Format("STATUS：COM%d OPENED，%d,%c,%d,%d",m_nCom, m_nBaud,m_cParity,m_nDatabits,m_nStopbits);
		}
		else
		{
			m_ctrlIconOpenoff.SetIcon(m_hIconOff);
			strStatus.Format("STATUS：COM%d CLOSED，%d,%c,%d,%d",m_nCom, m_nBaud,m_cParity,m_nDatabits,m_nStopbits);
		}
		m_ctrlPortStatus.SetWindowText(strStatus);

	}
	else
	{
		AfxMessageBox("没有成功，请重试");
		m_ctrlIconOpenoff.SetIcon(m_hIconOff);
	}*/
	
}

//保存显示数据
void CSCOMMDlg::OnButtonSavedata() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int		nLength;
	nLength = m_strCurPath.GetLength();

	for( int nCount = 0; nCount < nLength; nCount++ )
	{
		if( m_strCurPath.GetAt( nCount ) == '\\' )
			CreateDirectory( m_strCurPath.Left( nCount + 1 ), NULL );
	}
	CreateDirectory( m_strCurPath, NULL );

	CFile m_rFile;
	LPCSTR	lpszPath =m_strCurPath;// "c:\\comdata";
	SetCurrentDirectory( lpszPath );
    
	//文件名为Rec**.txt,以下代码自动检测文件名是否存在，若存在，则后面序号自动递增
	//如Rec00.txt Rec01.txt,程序自动为正要保存的文件命名为Rec02.txt.
	char buf[20];   
	for(int j=0;j<100;j++)
	{
		sprintf(buf,"Rec%02d.txt",j);
		if( (access( buf, 0 )) == -1 )
			break;
	}
    
	if(!m_rFile.Open(buf,CFile::modeCreate | CFile::modeWrite )) 
	{
		AfxMessageBox( "创建记录文件失败！");
		return;
	}
	if((access(buf,0))==-1)
	{
		AfxMessageBox("failed");
		return;
	}
	CTime t = CTime::GetCurrentTime();
	CString str=t.Format("%Y年%m月%d日%H时%M分%S秒\r\n");
	m_rFile.Write((LPCTSTR)str,str.GetLength());
	m_rFile.Write((LPCTSTR)m_ReceiveData,m_ReceiveData.GetLength());
	m_rFile.Flush();
	m_rFile.Close();

    str="OK,";
	for(int i=0;i<5;i++)
	    str+=buf[i];
	str+=".txt saved";
	m_ctrlSavePath.SetWindowText(str);
	SetTimer(2,5000,NULL);   //在定时器中显示保存文件状态
}


//改变文件保存路径
void CSCOMMDlg::OnButtonDirbrowser() 
{
	// TODO: Add your control notification handler code here
	static char displayname[MAX_PATH];
	static char path[MAX_PATH];
    LPITEMIDLIST pidlBrowse;    // PIDL selected by user 
	BROWSEINFO bi;
	bi.hwndOwner = this->m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = displayname;
	bi.lpszTitle = "请选择要保存接收数据的文件夹";
	bi.ulFlags = BIF_EDITBOX ;
	bi.lpfn = NULL;
	pidlBrowse=SHBrowseForFolder( &bi);
	if(pidlBrowse!=NULL)
	{
		SHGetPathFromIDList(pidlBrowse,path);
	}
	CString str=path;  //得到路径
	if(str.IsEmpty()) return;  //如果没有选择，就返回
	m_strCurPath=str;	//接收路径编辑框对应变量
	UpdateData(FALSE);	
}



void CSCOMMDlg::OnButtonCountreset() 
{
	// TODO: Add your control notification handler code here
	rxdatacount=0;
	CString strTemp;
	strTemp.Format("%ld",rxdatacount);
	strTemp="RX:"+strTemp;
	m_ctrlRXCOUNT.SetWindowText(strTemp);
	TX_count=0;
	strTemp.Format("%ld",TX_count);
	strTemp="TX:"+strTemp;
	m_ctrlTXCount.SetWindowText(strTemp);
	
}


void CSCOMMDlg::OnButtonClose() 
{
	//CString str;
	//str.Format("RoaringWindSoft You can use all the functions of the SComAssistant,But if you registered:\r\nNo popup Message and No marks of RoaringWindStudio\r\nFree Updates(Send to you Automaticly by Email)\r\n Thank You");
	//AfxMessageBox(str);
	this->SaveOptionsStatus();
	CSCOMMDlg::OnOK();
}


//选择要发送的文件
void CSCOMMDlg::OnButtonFilebrowser() 
{
	// TODO: Add your control notification handler code here
	LPCSTR	lpszPath = "c:\\comdata";
	SetCurrentDirectory( lpszPath );
	static char BASED_CODE szFilter[] = "文本文件(*.txt)|*.txt|所有文件(*.*)|*.*||";

	CFileDialog FileDlg( TRUE,  NULL,  NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		 szFilter );
	FileDlg.m_ofn.lpstrInitialDir = lpszPath ;

	if( FileDlg.DoModal() ==IDOK )
	{

		CString strFileName = FileDlg.GetFileName( );
		CString strFileExt = FileDlg.GetFileExt( );
		CString lpstrName =  FileDlg.GetPathName( );
		m_strSendFilePathName=lpstrName;
		UpdateData(FALSE);
	}

}

//发送文件
void CSCOMMDlg::OnButtonSendfile() 
{
	// TODO: Add your control notification handler code here
	CFile fp;
	if(!(fp.Open((LPCTSTR)m_strSendFilePathName,CFile::modeRead))) 
	{
		AfxMessageBox("Open file failed!");
		return;
	}
	fp.SeekToEnd();
	unsigned long fplength=fp.GetLength();
	m_nFileLength=fplength;
	char* fpBuff;
	fpBuff=new char[fplength];
	fp.SeekToBegin();
	if(fp.Read(fpBuff,fplength)<1)
	{
		fp.Close();
		return;
	}
	fp.Close();

	CString strStatus;
	int i = m_ctrSendChannel.GetCurSel();
	int m_nCom = this->m_CommInfo[i].getCom();
	int m_nBaud = this->m_CommInfo[i].getBand();
	char m_cParity = this->m_CommInfo[i].getParity();
	int m_nDatabits = this->m_CommInfo[i].getDatabits();
	int m_nStopbits = this->m_CommInfo[i].getStopbits();
	int m_dwCommEvents = this->m_CommEvents[i] | WM_COMM_TXEMPTY_DETECTED;

	if (m_Ports[i].InitPort(this, m_nCom, m_nBaud, m_cParity, m_nDatabits, m_nStopbits, m_dwCommEvents, fplength))
	{
		m_Ports[i].StartMonitoring();
		strStatus.Format("STATUS：COM%d OPENED，%d,%c,%d,%d",m_nCom, m_nBaud,m_cParity,m_nDatabits,m_nStopbits);
		m_ctrlIconOpenoff.SetIcon(m_hIconRed);
		m_bSendFile=TRUE;
		m_strTempSendFilePathName=m_strSendFilePathName;
		m_ctrlEditSendFile.SetWindowText("正在发送......");
		//发送文件时，以下功能不能使用
		m_ctrlManualSend.EnableWindow(FALSE); 
		m_ctrlAutoSend.EnableWindow(FALSE);
		m_ctrlSendFile.EnableWindow(FALSE);
		m_Ports[i].WriteToPort((LPCTSTR)fpBuff,fplength);
	}
	else
	{
		AfxMessageBox("Failed to send file!");
		m_ctrlIconOpenoff.SetIcon(m_hIconOff);
	}
	delete fpBuff;
}


//检测文件是否发送完毕
LONG CSCOMMDlg::OnFileSendingEnded(WPARAM wParam,LPARAM port)
{
	if(m_bSendFile)
	{
		m_ctrlEditSendFile.SetWindowText("发送完毕!");//m_strSendFilePathName
		TX_count+=m_nFileLength;
		SetTimer(3,3000,NULL);
		CString strTemp;
		strTemp.Format("TX:%d",TX_count);
		m_ctrlTXCount.SetWindowText(strTemp);
		m_bSendFile=FALSE;
	}
	return 0;
}


void CSCOMMDlg::OnButtonPushpin() 
{
	// TODO: Add your control notification handler code here
	m_ctrlPushPin.ProcessClick();
	m_bVisible=!m_bVisible;
	if(m_bVisible)
	{
		SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	}
	else
	{
		SetWindowPos(&wndBottom, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOREDRAW);
		BringWindowToTop();
	}
}

void CSCOMMDlg::OnDestroy() 
{
	int i = 0;
	CDialog::OnDestroy();
	m_ctrlAutoSend.SetCheck(0);  //强行关闭自动发送
	KillTimer(1);   //关闭定时器
	KillTimer(4);

	while (i++ < MAX_NUM_SW_PORT)
	{
		if(m_bOpenPorts[i])  //关闭串口
		{
			m_Ports[i].ClosePort();//关闭串口
			m_bOpenPorts[i] =! m_bOpenPorts[i];
		}
	}
	m_ReceiveData.Empty();  //清空接收数据字符串
}

void CSCOMMDlg::OnButtonHelp() 
{
	// TODO: Add your control notification handler code here
	TCHAR exeFullPath[MAX_PATH]; 
	GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	CString strlpPath;
	strlpPath.Format("%s",exeFullPath);
	strlpPath.MakeUpper();
	strlpPath.Replace("串口调试助手V2.2.EXE","");
	ShellExecute(NULL,NULL,_T("help.htm"),NULL,strlpPath,SW_SHOW);
}

void CSCOMMDlg::OnEnChangeEditRecive()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CSCOMMDlg::OnButtonWritedb() 
{
	if (m_writeDB)
	{
		this->m_ctrlWriteDB.SetWindowText("开始写数据");
		AfxMessageBox("停止写数据到DB.");
	
	}else
	{
		this->m_ctrlWriteDB.SetWindowText("停止写数据");
		AfxMessageBox("开始写数据到DB.");
	}


	m_writeDB = !m_writeDB;

   //p_Devices[0]->getProtocol()->ParseDataFromSerialPort((LPCTSTR)m_strSendData);

   //p_Devices[0]->getProtocol()->

   //p_Devices[0]->writeDB();

   /*
   CString strHeader[HEAD_LEN];
   CString strBody[BODY_LEN];
   //CStringArray strArry;
   for(int i=0;i<HEAD_LEN;i++)
   {
	   AfxExtractSubString(strHeader[i], m_strSendData, i, _T(','));
   }
   strHeader[3].Replace("DSC", ""); // remove DSC
   for(int j=0;j<BODY_LEN;j++)
   {
	   AfxExtractSubString(strBody[j], strHeader[3], j, _T(';'));
	   if (strBody[j].Compare("=") == 0) {
		   break;
	   }
   }

   for(int k=0;k<BODY_LEN;k++)
   {
	  strBody[k].TrimLeft(_T("\t \r \n \r\n"));
	  MessageBox(strBody[k]);
	  if (strBody[k].Compare("=") == 0) {
		   break;
	   }
   }*/
 
	/*AfxMessageBox("start to write data to mysql");

	CString dummy("no");
	CMySQLDB db;
	db.WriteToTable(dummy);

	AfxMessageBox("Done!!");*/


	// TODO: Add your control notification handler code here
	//MYSQL*ssock;
	
	//mysql_init(&m_mysql);
	/*m_mysql = mysql_init(NULL);
	if (m_mysql)
	{
		if (!mysql_real_connect(m_mysql,NULL,DB_USER,DB_PASSW,DB_NAME,0,NULL,0))
		{
			AfxMessageBox("error");
		  return;
		}
		else
		{
			AfxMessageBox("Start to insert data.");
			for (int i = 0; i < 100; i++)
			{
				CString insert_sql;
				insert_sql.Format("INSERT INTO comm_t VALUES (%d, '%s', %f)", i, "testname", 22.7);
				mysql_query(m_mysql, insert_sql);
			}
		}
		mysql_close(m_mysql);
	}*/
}

void CSCOMMDlg::OnButtonOpenport2() 
{
	// TODO: Add your control notification handler code here
	this->StartOrStopPort(1, &m_ctrlOpenPort2, &m_ctrlIconOpenoff2);

	//CString str;
	//str.Format("%d", this->m_CommInfo[1].getBand());

	//AfxMessageBox(str);
	
}

void CSCOMMDlg::OnButtonOpenport3() 
{
	// TODO: Add your control notification handler code here
	this->StartOrStopPort(2, &m_ctrlOpenPort3, &m_ctrlIconOpenoff3);
	
}

void CSCOMMDlg::OnButtonOpenport4() 
{
	// TODO: Add your control notification handler code here
	this->StartOrStopPort(3, &m_ctrlOpenPort4, &m_ctrlIconOpenoff4);
	
}

void CSCOMMDlg::OnEditchangeComboComselect2() 
{
	// TODO: Add your control notification handler code here
	UpdateCommFromEdit(1, m_Com2);
	
}

void CSCOMMDlg::OnEditchangeComboComselect3() 
{
	// TODO: Add your control notification handler code here
	UpdateCommFromEdit(2, m_Com3);
	
}

void CSCOMMDlg::OnEditchangeComboComselect4() 
{
	// TODO: Add your control notification handler code here
	//this->ChangeComm(3, m_Com4.GetCurSel());
	UpdateCommFromEdit(3, m_Com4);
}

void CSCOMMDlg::OnEditchangeComboSpeed2() 
{
	// TODO: Add your control notification handler code here
	this->ChangeBoud(1, m_Speed2.GetCurSel());
	
}

void CSCOMMDlg::OnEditchangeComboSpeed3() 
{
	// TODO: Add your control notification handler code here
	this->ChangeBoud(2, m_Speed3.GetCurSel());
	
}

void CSCOMMDlg::OnEditchangeComboSpeed4() 
{
	// TODO: Add your control notification handler code here
	this->ChangeBoud(3, m_Speed4.GetCurSel());
	
}

void CSCOMMDlg::OnEditchangeComboParity2() 
{
	// TODO: Add your control notification handler code here
	this->ChangeParity(1, m_Parity2.GetCurSel());
	
}

void CSCOMMDlg::OnEditchangeComboParity3() 
{
	// TODO: Add your control notification handler code here
	this->ChangeParity(2, m_Parity3.GetCurSel());
	
}

void CSCOMMDlg::OnEditchangeComboParity4() 
{
	// TODO: Add your control notification handler code here
	this->ChangeParity(3, m_Parity4.GetCurSel());
	
}

void CSCOMMDlg::OnEditchangeComboDatabits2() 
{
	// TODO: Add your control notification handler code here
	this->ChangeDatabits(1, m_DataBits2.GetCurSel());
}

void CSCOMMDlg::OnEditchangeComboDatabits3() 
{
	// TODO: Add your control notification handler code here
	this->ChangeDatabits(2, m_DataBits3.GetCurSel());
}

void CSCOMMDlg::OnEditchangeComboDatabits4() 
{
	// TODO: Add your control notification handler code here
	this->ChangeDatabits(3, m_DataBits4.GetCurSel());
}

void CSCOMMDlg::OnEditchangeComboStopbits2() 
{
	// TODO: Add your control notification handler code here
	this->ChangeStopbits(1, m_StopBits2.GetCurSel());	
}

void CSCOMMDlg::OnEditchangeComboStopbits3() 
{
	// TODO: Add your control notification handler code here
	this->ChangeStopbits(2, m_StopBits3.GetCurSel());	
}

void CSCOMMDlg::OnEditchangeComboStopbits4() 
{
	// TODO: Add your control notification handler code here
	this->ChangeStopbits(3, m_StopBits4.GetCurSel());	
}

void CSCOMMDlg::OnSelendokComboSpeed2() 
{
	// TODO: Add your control notification handler code here
	this->ChangeBoud(1, m_Speed2.GetCurSel());
	
}

void CSCOMMDlg::OnSelendokComboComselect2() 
{
	// TODO: Add your control notification handler code here
	this->ChangeComm(1, m_Com2.GetCurSel());
}

void CSCOMMDlg::OnSelendokComboSpeed3() 
{
	// TODO: Add your control notification handler code here
	this->ChangeBoud(2, m_Speed3.GetCurSel());
	
}

void CSCOMMDlg::OnSelendokComboSpeed4() 
{
	// TODO: Add your control notification handler code here
	this->ChangeBoud(3, m_Speed4.GetCurSel());
}

void CSCOMMDlg::OnSelendokComboComselect3() 
{
	// TODO: Add your control notification handler code here
	this->ChangeComm(2, m_Com3.GetCurSel());
	
}

void CSCOMMDlg::OnSelendokComboComselect4() 
{
	// TODO: Add your control notification handler code here
	this->ChangeComm(3, m_Com4.GetCurSel());
	
}

void CSCOMMDlg::OnSelendokComboParity2() 
{
	// TODO: Add your control notification handler code here
	this->ChangeParity(1, m_Parity2.GetCurSel());
}

void CSCOMMDlg::OnSelendokComboParity3() 
{
	// TODO: Add your control notification handler code here
	this->ChangeParity(2, m_Parity3.GetCurSel());
}

void CSCOMMDlg::OnSelendokComboParity4() 
{
	// TODO: Add your control notification handler code here
	this->ChangeParity(3, m_Parity4.GetCurSel());
}

void CSCOMMDlg::OnSelendokComboDatabits2() 
{
	// TODO: Add your control notification handler code here
	this->ChangeDatabits(1, m_DataBits2.GetCurSel());
}

void CSCOMMDlg::OnSelendokComboDatabits3() 
{
	// TODO: Add your control notification handler code here
	this->ChangeDatabits(2, m_DataBits3.GetCurSel());
}

void CSCOMMDlg::OnSelendokComboDatabits4() 
{
	// TODO: Add your control notification handler code here
	this->ChangeDatabits(3, m_DataBits4.GetCurSel());
}

void CSCOMMDlg::OnSelendokComboStopbits2() 
{
	// TODO: Add your control notification handler code here
	this->ChangeStopbits(1, m_StopBits2.GetCurSel());
}

void CSCOMMDlg::OnSelendokComboStopbits3() 
{
	// TODO: Add your control notification handler code here
	this->ChangeStopbits(2, m_StopBits3.GetCurSel());
}

void CSCOMMDlg::OnSelendokComboStopbits4() 
{
	// TODO: Add your control notification handler code here
	this->ChangeStopbits(3, m_StopBits4.GetCurSel());
}


void CSCOMMDlg::SetDeviceSettings(int i, int pIndex, int cIndex)
{
	p_Devices[i]->setCommInfo(m_CommInfo[cIndex]);
	p_Devices[i]->setSerialPort(&this->m_Ports[cIndex]);
	//int index = this->m_Protocal.GetCurSel()+1;
	Protocol* p = p_Devices[i]->getProtocol();

	if (pIndex != p_Devices[i]->getDeviceType() || p == NULL) 
	{
		if (p) {
			delete p;
		}
		if (pIndex == 0) {
			p = new DSCProtocol("dsc");
		}
		else if (pIndex == 1)
		{
			p = new ModbusProtocol("modbus");
		}
		else if (pIndex == 2)
		{
			p = new PTUProtocol("ptu");
		}
		else 
		{
			p = new WMTProtocol("wmt");
		}
		p_Devices[i]->setDeviceType(pIndex);
		p_Devices[i]->setProtocol(p);
	}
}

void CSCOMMDlg::OnButtonSetport1() 
{
	// TODO: Add your control notification handler code here
	int pIndex = this->m_Protocal.GetCurSel();
	int cIndex = this->m_usePort.GetCurSel();
	SetDeviceSettings(0, pIndex, cIndex);
	// AfxMessageBox("仪表1设置成功。");
}

void CSCOMMDlg::OnButtonSetport2() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	int pIndex = this->m_Protocal2.GetCurSel();
	int cIndex = this->m_usePort2.GetCurSel();
	SetDeviceSettings(1, pIndex, cIndex);
	// AfxMessageBox("仪表2设置成功。");
}
	

void CSCOMMDlg::OnButtonSetport3() 
{
	// TODO: Add your control notification handler code here
	int pIndex = this->m_Protocal3.GetCurSel();
	int cIndex = this->m_usePort3.GetCurSel();
	SetDeviceSettings(2, pIndex, cIndex);
	// AfxMessageBox("仪表3设置成功。");
}
	
void CSCOMMDlg::OnButtonSetport4() 
{
	// TODO: Add your control notification handler code here
	int pIndex = this->m_Protocal4.GetCurSel();
	int cIndex = this->m_usePort4.GetCurSel();
	SetDeviceSettings(3, pIndex, cIndex);
	// AfxMessageBox("仪表4设置成功。");
}


void CSCOMMDlg::OnButtonDevstart1() 
{
	// TODO: Add your control notification handler code here
	if (this->p_Devices[0]->isDeviceOpen()){

		CWnd * pwnd = GetDlgItem(IDC_COMBO_PROTOCAL1);
		pwnd->EnableWindow(TRUE);
		pwnd = GetDlgItem(IDC_COMBO_USEDPORT1);
		pwnd->EnableWindow(TRUE);
		this->m_devStart.SetWindowText("开启");
		this->p_Devices[0]->closeDevice();
		// AfxMessageBox("仪表1关闭成功。");
	}else{
		CWnd * pwnd = GetDlgItem(IDC_COMBO_PROTOCAL1);
		pwnd->EnableWindow(FALSE);
		pwnd = GetDlgItem(IDC_COMBO_USEDPORT1);
		pwnd->EnableWindow(FALSE);
		this->m_devStart.SetWindowText("关闭");

		int pIndex = this->m_Protocal.GetCurSel();
		int cIndex = this->m_usePort.GetCurSel();
		SetDeviceSettings(0, pIndex, cIndex);

		this->p_Devices[0]->openDevice();
		// AfxMessageBox("仪表1开启成功。");
	}
}

void CSCOMMDlg::OnButtonDevstart2() 
{
	// TODO: Add your control notification handler code here
	if (this->p_Devices[1]->isDeviceOpen()){
		CWnd * pwnd = GetDlgItem(IDC_COMBO_PROTOCAL2);
		pwnd->EnableWindow(TRUE);
		pwnd = GetDlgItem(IDC_COMBO_USEDPORT2);
		pwnd->EnableWindow(TRUE);
		this->m_devStart2.SetWindowText("开启");
		this->p_Devices[1]->closeDevice();
		// AfxMessageBox("仪表2关闭成功。");
	}else{
        CWnd * pwnd = GetDlgItem(IDC_COMBO_PROTOCAL2);
		pwnd->EnableWindow(FALSE);
		pwnd = GetDlgItem(IDC_COMBO_USEDPORT2);
		pwnd->EnableWindow(FALSE);
		this->m_devStart2.SetWindowText("关闭");
		
		int pIndex = this->m_Protocal2.GetCurSel();
		int cIndex = this->m_usePort2.GetCurSel();
		SetDeviceSettings(1, pIndex, cIndex);
		this->p_Devices[1]->openDevice();
		// AfxMessageBox("仪表2开启成功。");
	}
	
}

void CSCOMMDlg::OnButtonDevstart3() 
{
	if (this->p_Devices[2]->isDeviceOpen()){
        CWnd * pwnd = GetDlgItem(IDC_COMBO_PROTOCAL3);
		pwnd->EnableWindow(TRUE);
		pwnd = GetDlgItem(IDC_COMBO_USEDPORT3);
		pwnd->EnableWindow(TRUE);
		this->m_devStart3.SetWindowText("开启");
		this->p_Devices[2]->closeDevice();
		// AfxMessageBox("仪表3关闭成功。");
	}else{
		CWnd * pwnd = GetDlgItem(IDC_COMBO_PROTOCAL3);
		pwnd->EnableWindow(FALSE);
		pwnd = GetDlgItem(IDC_COMBO_USEDPORT3);
		pwnd->EnableWindow(FALSE);
		this->m_devStart3.SetWindowText("关闭");
		
		int pIndex = this->m_Protocal3.GetCurSel();
		int cIndex = this->m_usePort3.GetCurSel();
		SetDeviceSettings(2, pIndex, cIndex);
		this->p_Devices[2]->openDevice();
		// AfxMessageBox("仪表3开启成功。");
	}
	
}

void CSCOMMDlg::OnButtonDevstart4() 
{
	if (this->p_Devices[3]->isDeviceOpen()){
        CWnd * pwnd = GetDlgItem(IDC_COMBO_PROTOCAL4);
		pwnd->EnableWindow(TRUE);
		pwnd = GetDlgItem(IDC_COMBO_USEDPORT4);
		pwnd->EnableWindow(TRUE);
		this->m_devStart4.SetWindowText("开启");
		this->p_Devices[3]->closeDevice();
		// AfxMessageBox("仪表4关闭成功。");
	}else{
        CWnd * pwnd = GetDlgItem(IDC_COMBO_PROTOCAL4);
		pwnd->EnableWindow(FALSE);
		pwnd = GetDlgItem(IDC_COMBO_USEDPORT4);
		pwnd->EnableWindow(FALSE);
		this->m_devStart4.SetWindowText("关闭");
		
		
		int pIndex = this->m_Protocal4.GetCurSel();
		int cIndex = this->m_usePort4.GetCurSel();
		SetDeviceSettings(3, pIndex, cIndex);		
		this->p_Devices[3]->openDevice();
		// AfxMessageBox("仪表4开启成功。");
	}
	
}

void CSCOMMDlg::OnEditchangeComboComselect() 
{
	UpdateCommFromEdit(0, m_Com);
}

void CSCOMMDlg::OnCheckCr() 
{
	// TODO: Add your control notification handler code here
	m_AutoAddCR = !m_AutoAddCR;
}

void CSCOMMDlg::OnButtonstart() 
{
	// TODO: Add your control notification handler code here
	CEdit* pEdit=(CEdit*)GetDlgItem(IDC_EDIT_INTERVAL);
	CString strText;
	pEdit->GetWindowText(strText);
	this->m_nIntervalTime =atoi(strText);

	if (this->m_nIntervalTime < 2000) {
		AfxMessageBox("采样间隔不能小于2000毫秒");
		return;
	}

	m_ProfileStart = !m_ProfileStart;

	//AfxMessageBox(strText);
	if(!this->m_ProfileStart)
	{
		CWnd * pwnd = GetDlgItem(IDC_EDIT_INTERVAL);
		pwnd->EnableWindow(TRUE);
		this->m_ctrlProfileStart.SetWindowText("开始采样");
		//stop profiling.
		KillTimer(4);
		KillTimer(PROFILE_EVENT_ID);
		m_ctrlPortStatus.SetWindowText("系统停止采样!");
		SetEvent(m_hProfileEvent);
		CloseHandle(m_hProfileEvent);
	}
	else
	{
		CWnd * pwnd = GetDlgItem(IDC_EDIT_INTERVAL);
		pwnd->EnableWindow(FALSE);
		this->m_ctrlProfileStart.SetWindowText("停止采样");
	
		m_hProfileEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		// start new thread to recieve data from serial port.
		m_pThread = AfxBeginThread(ThreadFunc, this);

		//SetEvent(m_hProfileEvent);

		//set time to start profile
		SetTimer(PROFILE_EVENT_ID, this->m_nIntervalTime, NULL);
		SetTimer(4,200,NULL); 
		//AfxMessageBox(strText);
		m_ctrlPortStatus.SetWindowText("系统正在采样数据...");
	}
}

void CSCOMMDlg::OnReceiveTimeOutEvent(int timerId)
{
	if (this->p_activeDevice){
		if (timerId == RECEIVE_TIMEOUT_EVENT_ID){
			this->p_activeDevice->handleTimeout(2);
		}else{
			if (this->p_activeDevice->handleTimeout(1)){
				KillTimer(RECEIVE_TIMEOUT_EVENT_ID);
			};
		}
	}
}

UINT CSCOMMDlg::ThreadFunc(LPVOID pParam)
{
	CSCOMMDlg* pDlg = (CSCOMMDlg *) pParam;

	while (pDlg->m_ProfileStart) 
	{
		pDlg->m_deviceData.ResetDeviceData(); // reset device data before new test.

		for (int i = 0; i < MAX_NUM_DEVICE; i++)
		{
			// check if device is started.
			if (!pDlg->p_Devices[i]->isDeviceOpen()){
				pDlg->p_activeDevice = NULL;
				continue;
			}

			pDlg->p_Devices[i]->setActive(true);
			pDlg->p_activeDevice = pDlg->p_Devices[i];

			//send request command to serial port.
			pDlg->p_activeDevice->sendCommand();
			pDlg->SetTimer(RECEIVE_TIMEOUT_EVENT_ID, pDlg->m_nLongReceiveTimeout, NULL);

			ProtocolData* p = pDlg->p_activeDevice->getCommandResponse();

			//Get data from debug input
			//this->p_activeDevice->getProtocol()->ParseDataFromSerialPort((LPCTSTR)m_strSendData); 
			//ProtocolData* p = this->p_activeDevice->getProtocol()->GetProtocolData();
			pDlg->p_activeDevice->convertToDeviceData(&pDlg->m_deviceData, p);
		}

		// Write data to mysql after pull from all devices and data is ready.
		if (pDlg->m_writeDB) {
			pDlg->m_db.WriteProtocolData(pDlg->m_nIntervalTime, &pDlg->m_deviceData);
		}

		pDlg->m_is_DeviceData_ready = TRUE;

		//wait for timer event notification.
		WaitForSingleObject(pDlg->m_hProfileEvent, INFINITE);
	}
	return 0;
}

void CSCOMMDlg::OnProfileEvent()
{
	if (m_is_DeviceData_ready) {
		m_is_DeviceData_ready = FALSE;

		// notify blocked thread to profile device data.
		SetEvent(m_hProfileEvent);
		// start new thread to recieve data from serial port.
		//m_pThread = AfxBeginThread(ThreadFunc, this);
	}	
}

void CSCOMMDlg::OnCheckCrc() 
{
	// TODO: Add your control notification handler code here
	m_AutoAddCRC = !m_AutoAddCRC;
	
}

void CSCOMMDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	this->SaveOptionsStatus();
	CDialog::OnClose();
}

void CSCOMMDlg::SaveOptionsStatus()
{
	// Write current settings into ini file.
	CIniWriter iniWriter(Settings_CFG);

	iniWriter.WriteInteger("Setting", "com1_num", m_Com.GetCurSel());
	iniWriter.WriteInteger("Setting", "com1_speed", m_Speed.GetCurSel());
	iniWriter.WriteInteger("Setting", "com1_parity", m_Parity.GetCurSel());
	iniWriter.WriteInteger("Setting", "com1_databits", m_DataBits.GetCurSel());
	iniWriter.WriteInteger("Setting", "com1_stopbits", m_StopBits.GetCurSel());

	iniWriter.WriteInteger("Setting", "com2_num", m_Com2.GetCurSel());
	iniWriter.WriteInteger("Setting", "com2_speed", m_Speed2.GetCurSel());
	iniWriter.WriteInteger("Setting", "com2_parity", m_Parity2.GetCurSel());
	iniWriter.WriteInteger("Setting", "com2_databits", m_DataBits2.GetCurSel());
	iniWriter.WriteInteger("Setting", "com2_stopbits", m_StopBits2.GetCurSel());


	iniWriter.WriteInteger("Setting", "com3_num", m_Com3.GetCurSel());
	iniWriter.WriteInteger("Setting", "com3_speed", m_Speed3.GetCurSel());
	iniWriter.WriteInteger("Setting", "com3_parity", m_Parity3.GetCurSel());
	iniWriter.WriteInteger("Setting", "com3_databits", m_DataBits3.GetCurSel());
	iniWriter.WriteInteger("Setting", "com3_stopbits", m_StopBits3.GetCurSel());


	iniWriter.WriteInteger("Setting", "com4_num", m_Com4.GetCurSel());
	iniWriter.WriteInteger("Setting", "com4_speed", m_Speed4.GetCurSel());
	iniWriter.WriteInteger("Setting", "com4_parity", m_Parity4.GetCurSel());
	iniWriter.WriteInteger("Setting", "com4_databits", m_DataBits4.GetCurSel());
	iniWriter.WriteInteger("Setting", "com4_stopbits", m_StopBits4.GetCurSel());


	iniWriter.WriteInteger("Setting", "channel1_protocal", m_Protocal.GetCurSel());
	iniWriter.WriteInteger("Setting", "channel1_port", m_usePort.GetCurSel());
	iniWriter.WriteInteger("Setting", "channel2_protocal", m_Protocal2.GetCurSel());
	iniWriter.WriteInteger("Setting", "channel2_port", m_usePort2.GetCurSel());
	iniWriter.WriteInteger("Setting", "channel3_protocal", m_Protocal3.GetCurSel());

	iniWriter.WriteInteger("Setting", "channel3_port", m_usePort3.GetCurSel());
	iniWriter.WriteInteger("Setting", "channel4_protocal", m_Protocal4.GetCurSel());
	iniWriter.WriteInteger("Setting", "channel4_port", m_usePort4.GetCurSel());

	iniWriter.WriteInteger("Setting", "long_receive_timeout", this->m_nLongReceiveTimeout);
	iniWriter.WriteInteger("Setting", "short_receive_timeout", this->m_nShortReceiveTimeout);

}
