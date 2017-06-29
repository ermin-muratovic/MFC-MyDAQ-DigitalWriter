
// MFC MyDAQ DigitalWriterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC MyDAQ DigitalWriter.h"
#include "MFC MyDAQ DigitalWriterDlg.h"
#include "afxdialogex.h"
#include <string>
#include <NIDAQmx.h>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

TaskHandle writeTask = 0;
int led = 0;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCMyDAQDigitalWriterDlg dialog



CMFCMyDAQDigitalWriterDlg::CMFCMyDAQDigitalWriterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCMYDAQDIGITALWRITER_DIALOG, pParent)
	, mydaq_name(_T(""))
	, box1(FALSE)
	, box2(FALSE)
	, box3(FALSE)
	, box4(FALSE)
	, hex_value(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCMyDAQDigitalWriterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MYDAQ_NAME, mydaq_name);
	DDX_Check(pDX, IDC_CHECK1, box1);
	DDX_Check(pDX, IDC_CHECK2, box2);
	DDX_Check(pDX, IDC_CHECK3, box3);
	DDX_Check(pDX, IDC_CHECK4, box4);
	DDX_Text(pDX, IDC_HEX_INPUT, hex_value);
}

BEGIN_MESSAGE_MAP(CMFCMyDAQDigitalWriterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_MYDAQ_NAME, &CMFCMyDAQDigitalWriterDlg::OnChangeMydaqName)
	ON_BN_CLICKED(IDC_CHECK1, &CMFCMyDAQDigitalWriterDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CMFCMyDAQDigitalWriterDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CMFCMyDAQDigitalWriterDlg::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &CMFCMyDAQDigitalWriterDlg::OnBnClickedCheck4)
	ON_EN_CHANGE(IDC_HEX_INPUT, &CMFCMyDAQDigitalWriterDlg::OnChangeHexInput)
END_MESSAGE_MAP()


// CMFCMyDAQDigitalWriterDlg message handlers

BOOL CMFCMyDAQDigitalWriterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	UpdateData(TRUE);
	mydaq_name = "myDAQ1";
	char* mydaq_name_c = CMFCMyDAQDigitalWriterDlg::convertToCharArr(mydaq_name);
	UpdateData(FALSE);

	led = 0;
	writeTask = CMFCMyDAQDigitalWriterDlg::createTask(mydaq_name_c);
	if (writeTask != NULL) {
		CMFCMyDAQDigitalWriterDlg::writeDO(writeTask, led);
	}
	else {
		UpdateData(TRUE);
		mydaq_name = "Falscher Name";
		UpdateData(FALSE);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCMyDAQDigitalWriterDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCMyDAQDigitalWriterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CPaintDC dc(this);
		CBrush off(RGB(255, 255, 255));
		CBrush on(RGB(255, 0, 0));

		if ((led & (1 << 4)) == (1 << 4)) dc.SelectObject(&on);
		else dc.SelectObject(&off);
		dc.Ellipse(100, 100, 140, 140);

		if ((led & (1 << 5)) == (1 << 5)) dc.SelectObject(&on);
		else dc.SelectObject(&off);
		dc.Ellipse(160, 100, 200, 140);

		if ((led & (1 << 6)) == (1 << 6)) dc.SelectObject(&on);
		else dc.SelectObject(&off);
		dc.Ellipse(220, 100, 260, 140);

		if ((led & (1 << 7)) == (1 << 7)) dc.SelectObject(&on);
		else dc.SelectObject(&off);
		dc.Ellipse(280, 100, 320, 140);

		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCMyDAQDigitalWriterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCMyDAQDigitalWriterDlg::OnChangeMydaqName()
{
	// TODO:
	// 1. DAQmxClearTask(writeTask);
	// UpdateData(TRUE);
	// 2. writeTask = createWriteTask(mydaq_name);
	// UpdateData(FALSE);
}

TaskHandle CMFCMyDAQDigitalWriterDlg::createTask(char* name)
{
	TaskHandle  Task = 0;

	string s = name;
	s += "/port0/line4:7";

	int32 error = 0;
	error = DAQmxCreateTask("", &Task);
	if (error != 0)
		return NULL;
	error = DAQmxCreateDOChan(Task, s.c_str(), "", DAQmx_Val_ChanForAllLines);
	if (error != 0)
		return NULL;

	return Task;
}


void CMFCMyDAQDigitalWriterDlg::writeDO(TaskHandle Task, uInt32 value)
{
	int32       written;
	DAQmxStartTask(Task);
	DAQmxWriteDigitalU32(Task, 1, 0, 10.0, DAQmx_Val_GroupByChannel, &value, &written, NULL);
	DAQmxStopTask(Task);
}


void CMFCMyDAQDigitalWriterDlg::OnBnClickedCheck1()
{
	UpdateData(TRUE);
	if (box1)
		led += (1 << 4);
	else
		led -= (1 << 4);

	hex_value = CMFCMyDAQDigitalWriterDlg::convertToHexString(led / 16);

	UpdateData(FALSE);

	CMFCMyDAQDigitalWriterDlg::writeDO(writeTask, led);
	Invalidate();
}


void CMFCMyDAQDigitalWriterDlg::OnBnClickedCheck2()
{
	UpdateData(TRUE);
	if (box2)
		led += (1 << 5);
	else
		led -= (1 << 5);

	hex_value = CMFCMyDAQDigitalWriterDlg::convertToHexString(led / 16);

	UpdateData(FALSE);

	CMFCMyDAQDigitalWriterDlg::writeDO(writeTask, led);
	Invalidate();
}


void CMFCMyDAQDigitalWriterDlg::OnBnClickedCheck3()
{
	UpdateData(TRUE);
	if (box3)
		led += (1 << 6);
	else
		led -= (1 << 6);

	hex_value = CMFCMyDAQDigitalWriterDlg::convertToHexString(led / 16);

	UpdateData(FALSE);

	CMFCMyDAQDigitalWriterDlg::writeDO(writeTask, led);
	Invalidate();
}


void CMFCMyDAQDigitalWriterDlg::OnBnClickedCheck4()
{
	UpdateData(TRUE);
	if (box4)
		led += (1 << 7);
	else
		led -= (1 << 7);

	hex_value = CMFCMyDAQDigitalWriterDlg::convertToHexString(led / 16);

	UpdateData(FALSE);

	CMFCMyDAQDigitalWriterDlg::writeDO(writeTask, led);
	Invalidate();
}


void CMFCMyDAQDigitalWriterDlg::OnChangeHexInput()
{
	UpdateData(TRUE);

	int hex;
	char* hex_c = CMFCMyDAQDigitalWriterDlg::convertToCharArr(hex_value);
	sscanf_s(hex_c, "%x", &hex);

	if (hex >= 0 && hex <= 15) {
		led = (hex*16);
		if ((led & (1 << 4)) == (1 << 4)) box1 = true;
		else box1 = false;

		if ((led & (1 << 5)) == (1 << 5)) box2 = true;
		else box2 = false;

		if ((led & (1 << 6)) == (1 << 6)) box3 = true;
		else box3 = false;

		if ((led & (1 << 7)) == (1 << 7)) box4 = true;
		else box4 = false;
	}
	else {
		hex_value = "";
	}
	UpdateData(FALSE);

	CMFCMyDAQDigitalWriterDlg::writeDO(writeTask, led);
	Invalidate();
}

char* CMFCMyDAQDigitalWriterDlg::convertToCharArr(CString str)
{
	int x = 0;
	string s = "";
	while (x < str.GetLength()) {
		char c = str.GetAt(x++);
		s += c;
	}
	char * output = (char *)calloc(str.GetLength() + 1, sizeof(char));
	memcpy(output, s.c_str(), str.GetLength() + 1);
	return output;
}


CString CMFCMyDAQDigitalWriterDlg::convertToHexString(int zahl)
{
	CString s;
	char hexString[20];
	_itoa_s(zahl, hexString, 16);
	s = hexString;
	return s.MakeUpper();
}
