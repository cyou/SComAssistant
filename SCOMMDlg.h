// SCOMMDlg.h : header file
//

#include "SerialPort.h"
#include "HyperLink.h"
#include "label.h"
#include "PushPin.h"
#include "AnimateDlgIcon.h"

#include "CommInfo.h"
#include "Device.h"


#include "winsock.h"
#include "mysql.h"

#if !defined(AFX_SCOMMDLG_H__666127A8_FEE4_40AA_9309_1B3B55EEDAFC__INCLUDED_)
#define AFX_SCOMMDLG_H__666127A8_FEE4_40AA_9309_1B3B55EEDAFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define	MAX_NUM_SW_PORT		4
#define MAX_NUM_DEVICE      4
/////////////////////////////////////////////////////////////////////////////
// CSCOMMDlg dialog

class CSCOMMDlg : public CDialog
{
// Construction
public:
	BOOL m_bVisible;   //程序是否浮在最上面 用于图钉按钮功能
	BOOL m_bStopDispRXData; //是否显示接收字符
	CString m_strTempSendFilePathName; //发送文件路径名
	long m_nFileLength;  //文件长度
	BOOL m_bSendFile;    //是否发送文件
	HICON m_hIconRed;    //串口打开时的红灯图标句柄
	HICON m_hIconOff;    //串口关闭时的指示图标句柄
	HICON m_hIconGreen;

	int m_nBaud;       //波特率
	int m_nCom;         //串口号
	char m_cParity;    //校验
	int m_nDatabits;    //数据位
	int m_nStopbits;    //停止位
	CSerialPort m_Port;  //CSerialPort类对象
	CAnimateDlgIcon m_animIcon;  //动画图标
	CSCOMMDlg(CWnd* pParent = NULL);	// standard constructor
	~CSCOMMDlg();


	CommInfo        m_CommInfo[MAX_NUM_SW_PORT];
	CSerialPort		m_Ports[MAX_NUM_SW_PORT];

	Device*         p_Devices[MAX_NUM_DEVICE];

	BOOL            m_bOpenPorts[MAX_NUM_SW_PORT];
    DWORD           m_CommEvents[MAX_NUM_SW_PORT];


	CString			m_strReceived[MAX_NUM_SW_PORT];
	UINT			m_HwPortID[MAX_NUM_SW_PORT];


// Dialog Data
	//{{AFX_DATA(CSCOMMDlg)
	enum { IDD = IDD_SCOMM_DIALOG };
	CComboBox	m_ctrSendChannel;
	CStatic	m_ctrlIconOpenoff2;
	CButton	m_ctrlOpenPort4;
	CButton	m_ctrlOpenPort3;
	CButton	m_ctrlOpenPort2;
	CStatic	m_ctrlIconOpenoff4;
	CStatic	m_ctrlIconOpenoff3;
	CComboBox	m_usePort3;
	CComboBox	m_usePort4;
	CComboBox	m_usePort;
	CComboBox	m_usePort2;
	CButton	m_setPort4;
	CButton	m_setPort3;
	CButton	m_setPort2;
	CButton	m_setPort;
	CButton	m_devStart4;
	CButton	m_devStart3;
	CButton	m_devStart2;
	CButton	m_devStart;
	CComboBox	m_Protocal4;
	CComboBox	m_Protocal3;
	CComboBox	m_Protocal2;
	CComboBox	m_Protocal;
	CComboBox	m_StopBits4;
	CComboBox	m_StopBits3;
	CComboBox	m_StopBits2;
	CComboBox	m_Speed4;
	CComboBox	m_Speed3;
	CComboBox	m_Speed2;
	CComboBox	m_Parity4;
	CComboBox	m_Parity3;
	CComboBox	m_Parity2;
	CComboBox	m_DataBits4;
	CComboBox	m_DataBits3;
	CComboBox	m_DataBits2;
	CComboBox	m_Com4;
	CComboBox	m_Com3;
	CComboBox	m_Com2;
	CButton	m_ctrlWriteDB;
	CPushPinButton	m_ctrlPushPin;
	CButton	m_ctrlSendFile;
	CEdit	m_ctrlEditSendFile;
	CStatic	m_ctrlTXCount;
	CStatic	m_ctrlPortStatus;
	CStatic	m_ctrlRXCOUNT;
	CEdit	m_ctrlSavePath;
	CButton	m_ctrlManualSend;
	CHyperLink	m_ctrlHyperLink2;
	CButton	m_ctrlClearTXData;
	CButton	m_ctrlClose;
	CButton	m_ctrlCounterReset;
	CEdit	m_ctrlEditSend;
	CEdit	m_ctrlReceiveData;
	CButton	m_ctrlAutoClear;
	CStatic	m_ctrlIconOpenoff;
	CComboBox	m_StopBits;
	CComboBox	m_DataBits;
	CComboBox	m_Parity;
	CButton	m_ctrlAutoSend;
	CButton	m_ctrlHexSend;
	CButton	m_ctrlStopDisp;
	CButton	m_ctrlOpenPort;
	CButton	m_ctrlHexReceieve;
	CComboBox	m_Speed;
	CComboBox	m_Com;
	CString	m_ReceiveData;
	CString	m_strSendData;
	CString	m_strCurPath;
	CString	m_strSendFilePathName;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSCOMMDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nCycleTime;
	BOOL m_bAutoSend;
	int Str2Hex(CString str, char *data);
	char HexChar(char c);
	DWORD m_dwCommEvents;
	BOOL m_bOpenPort;
	HICON m_hIcon;

	MYSQL* m_mysql;


	BOOL m_AutoAddCR;

	// Generated message map functions
	//{{AFX_MSG(CSCOMMDlg)
	virtual BOOL OnInitDialog();
	afx_msg LONG OnCommunication(WPARAM ch, LPARAM port);
	afx_msg LONG OnFileSendingEnded(WPARAM wParam,LPARAM port);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonClearReciArea();
	afx_msg void OnButtonOpenport();
	afx_msg void OnButtonStopdisp();
	afx_msg void OnButtonManualsend();
	afx_msg void OnCheckAutosend();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChangeEditCycletime();
	afx_msg void OnChangeEditSend();
	afx_msg void OnButtonClearrecasenda();
	afx_msg void OnSelendokComboComselect();
	afx_msg void OnSelendokComboSpeed();
	afx_msg void OnSelendokComboParity();
	afx_msg void OnSelendokComboDatabits();
	afx_msg void OnSelendokComboStopbits();
	afx_msg void OnButtonSavedata();
	afx_msg void OnButtonDirbrowser();
	afx_msg void OnButtonSendfile();
	afx_msg void OnButtonCountreset();
	afx_msg void OnButtonClose();
	afx_msg void OnButtonFilebrowser();
	afx_msg void OnButtonPushpin();
	afx_msg void OnDestroy();
	afx_msg void OnButtonHelp();
	afx_msg void OnButtonWritedb();
	afx_msg void OnButtonOpenport2();
	afx_msg void OnButtonOpenport3();
	afx_msg void OnButtonOpenport4();
	afx_msg void OnEditchangeComboComselect2();
	afx_msg void OnEditchangeComboComselect3();
	afx_msg void OnEditchangeComboComselect4();
	afx_msg void OnEditchangeComboSpeed2();
	afx_msg void OnEditchangeComboSpeed3();
	afx_msg void OnEditchangeComboSpeed4();
	afx_msg void OnEditchangeComboParity2();
	afx_msg void OnEditchangeComboParity3();
	afx_msg void OnEditchangeComboParity4();
	afx_msg void OnEditchangeComboDatabits2();
	afx_msg void OnEditchangeComboDatabits3();
	afx_msg void OnEditchangeComboDatabits4();
	afx_msg void OnEditchangeComboStopbits2();
	afx_msg void OnEditchangeComboStopbits3();
	afx_msg void OnEditchangeComboStopbits4();
	afx_msg void OnSelendokComboSpeed2();
	afx_msg void OnSelendokComboComselect2();
	afx_msg void OnSelendokComboSpeed3();
	afx_msg void OnSelendokComboSpeed4();
	afx_msg void OnSelendokComboComselect3();
	afx_msg void OnSelendokComboComselect4();
	afx_msg void OnSelendokComboParity2();
	afx_msg void OnSelendokComboParity3();
	afx_msg void OnSelendokComboParity4();
	afx_msg void OnSelendokComboDatabits2();
	afx_msg void OnSelendokComboDatabits3();
	afx_msg void OnSelendokComboDatabits4();
	afx_msg void OnSelendokComboStopbits2();
	afx_msg void OnSelendokComboStopbits3();
	afx_msg void OnSelendokComboStopbits4();
	afx_msg void OnButtonSetport1();
	afx_msg void OnButtonSetport2();
	afx_msg void OnButtonSetport3();
	afx_msg void OnButtonSetport4();
	afx_msg void OnButtonDevstart1();
	afx_msg void OnButtonDevstart2();
	afx_msg void OnButtonDevstart3();
	afx_msg void OnButtonDevstart4();
	afx_msg void OnEditchangeComboComselect();
	afx_msg void OnCheckCr();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	//DECLARE_DYNAMIC_MAP()



private:

	void StartOrStopPort(int i, CButton* btn, CStatic* sta);

	void ChangeComm(int i, int value);
	void ChangeBoud(int i, int value);
	void ChangeParity(int i, int value);
	void ChangeDatabits(int i, int value);
	void ChangeStopbits(int i, int value);

	void UpdateCommFromEdit(int i, CComboBox &c);
	void SetDeviceSettings(int i, int pIndex, int cIndex);
	
public:
	afx_msg void OnEnChangeEditRecive();
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCOMMDLG_H__666127A8_FEE4_40AA_9309_1B3B55EEDAFC__INCLUDED_)

