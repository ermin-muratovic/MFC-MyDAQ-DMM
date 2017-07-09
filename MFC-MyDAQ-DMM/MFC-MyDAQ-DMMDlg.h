
// MFC-MyDAQ-DMMDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include <string>
#include <list>

using namespace std;

// CMFCMyDAQDMMDlg dialog
class CMFCMyDAQDMMDlg : public CDialogEx
{
// Construction
public:
	CMFCMyDAQDMMDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCMYDAQDMM_DIALOG };
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
	afx_msg	void OnTimer(UINT TimerVal);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CStatic value_label;
	CStatic unit_label;
	void initGUI();
	void updateDevicesCombobox(list<string> devices);
	CComboBox device_combobox;
	afx_msg void OnCbnDropdownDevicecombo();
	CButton vdc_button;
	CButton vac_button;
	CButton adc_button;
	CButton aac_button;
	CButton ohm_button;
	void updateButtonStyle();
	afx_msg void OnBnClickedButtonvdc();
	afx_msg void OnBnClickedButtonvac();
	afx_msg void OnBnClickedButtonadc();
	afx_msg void OnBnClickedButtonaac();
	afx_msg void OnBnClickedButtonohm();
	CComboBox mode_combobox;
	CComboBox range_combobox;
	CStatic image_container;
	afx_msg void OnCbnSelchangeModecombo();
	afx_msg void OnBnClickedButtonrun();
	void readDMM(string mydaq);
	afx_msg void OnBnClickedButtonstop();
	void saveData();
	CButton run_button;
	CButton stop_button;
};
