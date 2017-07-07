
// MFC MyDAQ DigitalWriterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC MyDAQ DigitalWriter.h"
#include "MFC MyDAQ DigitalWriterDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include <string>
#include <list>
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
	DDX_Check(pDX, IDC_CHECK1, box1);
	DDX_Check(pDX, IDC_CHECK2, box2);
	DDX_Check(pDX, IDC_CHECK3, box3);
	DDX_Check(pDX, IDC_CHECK4, box4);
	DDX_Text(pDX, IDC_HEX_INPUT, hex_value);
	DDX_Control(pDX, IDC_MYDAQCOMBO, devicesCombobox);
}

BEGIN_MESSAGE_MAP(CMFCMyDAQDigitalWriterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHECK1, &CMFCMyDAQDigitalWriterDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CMFCMyDAQDigitalWriterDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CMFCMyDAQDigitalWriterDlg::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &CMFCMyDAQDigitalWriterDlg::OnBnClickedCheck4)
	ON_EN_CHANGE(IDC_HEX_INPUT, &CMFCMyDAQDigitalWriterDlg::OnChangeHexInput)
	ON_CBN_DROPDOWN(IDC_MYDAQCOMBO, &CMFCMyDAQDigitalWriterDlg::OnCbnDropdownMydaqcombo)
	ON_CBN_SELCHANGE(IDC_MYDAQCOMBO, &CMFCMyDAQDigitalWriterDlg::OnCbnSelchangeMydaqcombo)
	ON_BN_CLICKED(IDC_BUTTONTOGGLE, &CMFCMyDAQDigitalWriterDlg::OnBnClickedButtontoggle)
	ON_BN_CLICKED(IDC_BUTTONROTATE, &CMFCMyDAQDigitalWriterDlg::OnBnClickedButtonrotate)
	ON_BN_CLICKED(IDC_BUTTONSHIFTL, &CMFCMyDAQDigitalWriterDlg::OnBnClickedButtonshiftl)
	ON_BN_CLICKED(IDC_BUTTONSHIFTR, &CMFCMyDAQDigitalWriterDlg::OnBnClickedButtonshiftr)
	ON_BN_CLICKED(IDC_BUTTONLOADFILE, &CMFCMyDAQDigitalWriterDlg::OnBnClickedButtonloadfile)
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

	led = 0;
	UpdateData(TRUE);
	hex_value = "0";
	UpdateData(FALSE);
	updateDevicesCombobox();
	updateTask();

	return TRUE;  // return TRUE unless you set the focus to a control
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

		CRect rect;
		GetClientRect(&rect);

		int circleDiameter = (rect.Width() - 80)/4;
		int padding = 5;

		CBrush off(RGB(255, 255, 255));
		CBrush on(RGB(255, 0, 0));

		for (int i = 0; i < 4; i++) {
			if ((led & (1 << (i+4))) == (1 << (i+4))) dc.SelectObject(&on);
			else dc.SelectObject(&off);
			dc.Ellipse(40 + (i*circleDiameter)+padding, 125+padding, 40 + ((i + 1)*circleDiameter)-padding, 125 + (circleDiameter-padding));
		}

		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCMyDAQDigitalWriterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

TaskHandle CMFCMyDAQDigitalWriterDlg::createTask(char* name)
{
	TaskHandle  Task = 0;

	string s = name;
	s += "/port0/line4:7";

	int32 error = 0;
	DAQmxClearTask(writeTask);
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
	if (hex_value.GetLength() > 0) {
		char* hex_c = (CT2CA)hex_value;
		sscanf_s(hex_c, "%x", &hex);

		if (hex >= 0 && hex <= 15) {
			led = (hex * 16);
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
}

CString CMFCMyDAQDigitalWriterDlg::convertToHexString(int zahl)
{
	CString s;
	char hexString[20];
	_itoa_s(zahl, hexString, 16);
	s = hexString;
	return s.MakeUpper();
}

list<string> CMFCMyDAQDigitalWriterDlg::getConnectedDevices()
{
	char namesBuffer[1000] = { '\0' };
	DAQmxGetSysDevNames(namesBuffer, 1000);
	string names(namesBuffer);
	list<string> devices = splitString(names, ',', 2);
	return devices;
}


list<string> CMFCMyDAQDigitalWriterDlg::splitString(string value, char separator, int spacing)
{
	list<string> values;
	if (value.length() > 0) {
		size_t found = value.find_first_of(separator);
		while (found != string::npos)
		{
			values.push_back(value.substr(0, found));
			value = value.substr(found + spacing);
			found = value.find_first_of(separator);
		}
		values.push_back(value);
	}
	return values;
}

void CMFCMyDAQDigitalWriterDlg::OnCbnDropdownMydaqcombo()
{
	updateDevicesCombobox();
}

void CMFCMyDAQDigitalWriterDlg::updateDevicesCombobox() {
	devicesCombobox.ResetContent();
	list<string> devices = getConnectedDevices();
	list<string>::iterator it;
	for (it = devices.begin(); it != devices.end(); it++) {
		devicesCombobox.AddString((LPCTSTR)(CA2T)(*it).c_str());
	}
	if (devices.size() > 0) {
		devicesCombobox.SetCurSel(0);
	}
	else {
		AfxMessageBox(L"No device connected");
	}
}


void CMFCMyDAQDigitalWriterDlg::OnCbnSelchangeMydaqcombo()
{
	updateTask();
}


void CMFCMyDAQDigitalWriterDlg::updateTask() {
	int idxC = devicesCombobox.GetCurSel();
	if (idxC >= 0) {
		CString myDAQname;
		devicesCombobox.GetLBText(idxC, myDAQname);
		writeTask = CMFCMyDAQDigitalWriterDlg::createTask((CT2CA)myDAQname);
		if (writeTask != NULL) {
			CMFCMyDAQDigitalWriterDlg::writeDO(writeTask, led);
		}
	}
}

void CMFCMyDAQDigitalWriterDlg::OnBnClickedButtontoggle()
{
	UpdateData(TRUE);
	box1 = !box1;
	box2 = !box2;
	box3 = !box3;
	box4 = !box4;

	led = 0;
	if (box1)
		led += (1 << 4);
	if (box2)
		led += (1 << 5);
	if (box3)
		led += (1 << 6);
	if (box4)
		led += (1 << 7);

	hex_value = CMFCMyDAQDigitalWriterDlg::convertToHexString(led / 16);
	CMFCMyDAQDigitalWriterDlg::writeDO(writeTask, led);
	UpdateData(FALSE);
	Invalidate();
}


void CMFCMyDAQDigitalWriterDlg::OnBnClickedButtonrotate()
{
	UpdateData(TRUE);
	bool b1=box1, b2=box2, b3=box3, b4=box4;

	box1 = b4;
	box2 = b3;
	box3 = b2;
	box4 = b1;

	led = 0;
	if (box1)
		led += (1 << 4);
	if (box2)
		led += (1 << 5);
	if (box3)
		led += (1 << 6);
	if (box4)
		led += (1 << 7);

	hex_value = CMFCMyDAQDigitalWriterDlg::convertToHexString(led / 16);
	CMFCMyDAQDigitalWriterDlg::writeDO(writeTask, led);
	UpdateData(FALSE);
	Invalidate();
}


void CMFCMyDAQDigitalWriterDlg::OnBnClickedButtonshiftl()
{
	UpdateData(TRUE);
	bool b1 = box1, b2 = box2, b3 = box3, b4 = box4;

	box1 = b2;
	box2 = b3;
	box3 = b4;
	box4 = b1;

	led = 0;
	if (box1)
		led += (1 << 4);
	if (box2)
		led += (1 << 5);
	if (box3)
		led += (1 << 6);
	if (box4)
		led += (1 << 7);

	hex_value = CMFCMyDAQDigitalWriterDlg::convertToHexString(led / 16);
	CMFCMyDAQDigitalWriterDlg::writeDO(writeTask, led);
	UpdateData(FALSE);
	Invalidate();
}


void CMFCMyDAQDigitalWriterDlg::OnBnClickedButtonshiftr()
{
	UpdateData(TRUE);
	bool b1 = box1, b2 = box2, b3 = box3, b4 = box4;

	box1 = b4;
	box2 = b1;
	box3 = b2;
	box4 = b3;

	led = 0;
	if (box1)
		led += (1 << 4);
	if (box2)
		led += (1 << 5);
	if (box3)
		led += (1 << 6);
	if (box4)
		led += (1 << 7);

	hex_value = CMFCMyDAQDigitalWriterDlg::convertToHexString(led / 16);
	CMFCMyDAQDigitalWriterDlg::writeDO(writeTask, led);
	UpdateData(FALSE);
	Invalidate();
}

void CMFCMyDAQDigitalWriterDlg::readFromFile() {
	string filePath, defaultExtension = "", defaultName = "", extensions = "";
	CFileDialog dlg(true, 
		(LPCTSTR)(CA2T)defaultExtension.c_str(), 
		(LPCTSTR)(CA2T)defaultName.c_str(), 
		OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR, 
		(LPCTSTR)(CA2T)extensions.c_str());

	auto result = dlg.DoModal();
	if (result != IDOK) {
		// TODO: failed?
	} else {
		filePath.assign((CT2CA)dlg.GetPathName());
		ifstream file(filePath);
		string str;
		while (getline(file, str))
		{
			if (str.length() == 4) {

				UpdateData(TRUE);
				box1 = !(str.at(0) == '0');
				box2 = !(str.at(1) == '0');
				box3 = !(str.at(2) == '0');
				box4 = !(str.at(3) == '0');

				led = 0;
				if (box1)
					led += (1 << 4);
				if (box2)
					led += (1 << 5);
				if (box3)
					led += (1 << 6);
				if (box4)
					led += (1 << 7);

				hex_value = CMFCMyDAQDigitalWriterDlg::convertToHexString(led / 16);
				CMFCMyDAQDigitalWriterDlg::writeDO(writeTask, led);
				UpdateData(FALSE);
				Invalidate();
			}
			else {
				AfxMessageBox(L"Wrong data format in file.");
			}
			Sleep(100);
		}
	}
}


void CMFCMyDAQDigitalWriterDlg::OnBnClickedButtonloadfile()
{
	readFromFile();
}


CMFCMyDAQDigitalWriterDlg::~CMFCMyDAQDigitalWriterDlg() {
	DAQmxClearTask(writeTask);
}