
// MFC MyDAQ DigitalWriterDlg.h : header file
//
#include <string>
#include <NIDAQmx.h>

using namespace std;
#pragma once


// CMFCMyDAQDigitalWriterDlg dialog
class CMFCMyDAQDigitalWriterDlg : public CDialogEx
{
// Construction
public:
	CMFCMyDAQDigitalWriterDlg(CWnd* pParent = NULL);	// standard constructor

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
	CString mydaq_name;
	afx_msg void OnChangeMydaqName();
	TaskHandle createTask(char* name);
	void writeDO(TaskHandle Task, uInt32 value);
	afx_msg void OnBnClickedCheck1();
	BOOL box1;
	BOOL box2;
	BOOL box3;
	BOOL box4;
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	CString hex_value;
	afx_msg void OnChangeHexInput();
	char* convertToCharArr(CString string);
	CString convertToHexString(int zahl);
};
