
// MFC MyDAQ DigitalWriterDlg.h : header file
//
#include <string>
#include <list>
#include <NIDAQmx.h>
#include "afxwin.h"

using namespace std;
#pragma once


// CMFCMyDAQDigitalWriterDlg dialog
class CMFCMyDAQDigitalWriterDlg : public CDialogEx
{
// Construction
public:
	CMFCMyDAQDigitalWriterDlg(CWnd* pParent = NULL);	// standard constructor
	~CMFCMyDAQDigitalWriterDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCMYDAQDIGITALWRITER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	BOOL box1;
	BOOL box2;
	BOOL box3;
	BOOL box4;
	CString hex_value;
	CString mydaq_name;
	CComboBox devicesCombobox;
	
	void updateDevicesCombobox();
	void updateTask();
	void readFromFile();
	void writeDO(TaskHandle Task, uInt32 value);
	TaskHandle createTask(char* name);
	CString convertToHexString(int zahl);
	list<string> getConnectedDevices();
	list<string> splitString(string value, char separator, int spacing);
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	afx_msg void OnChangeHexInput();
	afx_msg void OnCbnDropdownMydaqcombo();
	afx_msg void OnCbnSelchangeMydaqcombo();
	afx_msg void OnBnClickedButtontoggle();
	afx_msg void OnBnClickedButtonrotate();
	afx_msg void OnBnClickedButtonshiftl();
	afx_msg void OnBnClickedButtonshiftr();
	afx_msg void OnBnClickedButtonloadfile();
};
