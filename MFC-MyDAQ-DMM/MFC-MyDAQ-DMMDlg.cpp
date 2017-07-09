
// MFC-MyDAQ-DMMDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC-MyDAQ-DMM.h"
#include "MFC-MyDAQ-DMMDlg.h"
#include "afxdialogex.h"
#include "MyDAQ.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <list>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

HBITMAP voltBmp;
HBITMAP ampBmp;
MyDAQ myDAQ = MyDAQ::MyDAQ();
string mydaqName;
bool isRunning = false;

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


// CMFCMyDAQDMMDlg dialog


CMFCMyDAQDMMDlg::CMFCMyDAQDMMDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCMYDAQDMM_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCMyDAQDMMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VALUELABEL, value_label);
	DDX_Control(pDX, IDC_UNITLABEL, unit_label);
	DDX_Control(pDX, IDC_DEVICECOMBO, device_combobox);
	DDX_Control(pDX, IDC_BUTTONVDC, vdc_button);
	DDX_Control(pDX, IDC_BUTTONVAC, vac_button);
	DDX_Control(pDX, IDC_BUTTONADC, adc_button);
	DDX_Control(pDX, IDC_BUTTONAAC, aac_button);
	DDX_Control(pDX, IDC_BUTTONOHM, ohm_button);
	DDX_Control(pDX, IDC_MODECOMBO, mode_combobox);
	DDX_Control(pDX, IDC_RANGECOMBO, range_combobox);
	DDX_Control(pDX, IDC_IMAGECONTAINER, image_container);
	DDX_Control(pDX, IDC_BUTTONRUN, run_button);
	DDX_Control(pDX, IDC_BUTTONSTOP, stop_button);
}

BEGIN_MESSAGE_MAP(CMFCMyDAQDMMDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_DROPDOWN(IDC_DEVICECOMBO, &CMFCMyDAQDMMDlg::OnCbnDropdownDevicecombo)
	ON_BN_CLICKED(IDC_BUTTONVDC, &CMFCMyDAQDMMDlg::OnBnClickedButtonvdc)
	ON_BN_CLICKED(IDC_BUTTONVAC, &CMFCMyDAQDMMDlg::OnBnClickedButtonvac)
	ON_BN_CLICKED(IDC_BUTTONADC, &CMFCMyDAQDMMDlg::OnBnClickedButtonadc)
	ON_BN_CLICKED(IDC_BUTTONAAC, &CMFCMyDAQDMMDlg::OnBnClickedButtonaac)
	ON_BN_CLICKED(IDC_BUTTONOHM, &CMFCMyDAQDMMDlg::OnBnClickedButtonohm)
	ON_CBN_SELCHANGE(IDC_MODECOMBO, &CMFCMyDAQDMMDlg::OnCbnSelchangeModecombo)
	ON_BN_CLICKED(IDC_BUTTONRUN, &CMFCMyDAQDMMDlg::OnBnClickedButtonrun)
	ON_BN_CLICKED(IDC_BUTTONSTOP, &CMFCMyDAQDMMDlg::OnBnClickedButtonstop)
END_MESSAGE_MAP()


// CMFCMyDAQDMMDlg message handlers

BOOL CMFCMyDAQDMMDlg::OnInitDialog()
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
	initGUI();
	updateDevicesCombobox(myDAQ.getConnectedDevices());
	updateButtonStyle();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCMyDAQDMMDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCMyDAQDMMDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCMyDAQDMMDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCMyDAQDMMDlg::initGUI()
{
	// modify font
	CFont *f1 = new CFont;
	string font = "Arial Bold";
	f1->CreatePointFont(260, (LPCTSTR)font.c_str());
	value_label.SetFont(f1);
	unit_label.SetFont(f1);

	// init mode combobox
	mode_combobox.AddString((LPCTSTR)(CA2T)"Auto");
	mode_combobox.AddString((LPCTSTR)(CA2T)"Specify range");
	mode_combobox.SetCurSel(0);

	range_combobox.EnableWindow(FALSE);

	string voltImagePath = ".\\res\\jack-connection-description-volt.bmp";
	string ampImagePath = ".\\res\\jack-connection-description-ampere.bmp";

	CRect rect;
	image_container.GetWindowRect(&rect);
	voltBmp = (HBITMAP)LoadImage(0, (LPCWSTR)(CA2T)voltImagePath.c_str(), IMAGE_BITMAP, rect.Width(), rect.Height(), LR_LOADFROMFILE);
	ampBmp = (HBITMAP)LoadImage(0, (LPCWSTR)(CA2T)ampImagePath.c_str(), IMAGE_BITMAP, rect.Width(), rect.Height(), LR_LOADFROMFILE);
	image_container.ModifyStyle(0xF, SS_BITMAP, SWP_NOSIZE);
}


void CMFCMyDAQDMMDlg::updateDevicesCombobox(list<string> devices)
{
	device_combobox.ResetContent();
	if (devices.size() > 0) {
		list<string>::iterator it;
		for (it = devices.begin(); it != devices.end(); it++) {
			device_combobox.AddString((LPCTSTR)(CA2T)(*it).c_str());
		}
		run_button.EnableWindow(true);
		stop_button.EnableWindow(false);
	}
	else {
		device_combobox.AddString((LPCTSTR)(CA2T)"No device connected");
		run_button.EnableWindow(false);
		stop_button.EnableWindow(false);
	}
	device_combobox.SetCurSel(0);
}


void CMFCMyDAQDMMDlg::OnCbnDropdownDevicecombo()
{
	updateDevicesCombobox(myDAQ.getConnectedDevices());
}


void CMFCMyDAQDMMDlg::updateButtonStyle()
{
	vdc_button.SetState(false);
	vac_button.SetState(false);
	adc_button.SetState(false);
	aac_button.SetState(false);
	ohm_button.SetState(false);

	switch (myDAQ.mode) {
	case VDC:
		image_container.SetBitmap(voltBmp);
		vdc_button.SetState(true);
		unit_label.SetWindowTextW(_T("V"));
		break;
	case ADC:
		image_container.SetBitmap(ampBmp);
		adc_button.SetState(true);
		unit_label.SetWindowTextW(_T("A"));
		break;
	case OHM:
		image_container.SetBitmap(voltBmp);
		ohm_button.SetState(true);
		unit_label.SetWindowTextW(_T("Ω"));
		break;
	}

}


void CMFCMyDAQDMMDlg::OnBnClickedButtonvdc()
{
	myDAQ.mode = VDC;
	updateButtonStyle();
}


void CMFCMyDAQDMMDlg::OnBnClickedButtonvac()
{
	AfxMessageBox(_T("This function is not implemented!"));
}


void CMFCMyDAQDMMDlg::OnBnClickedButtonadc()
{
	myDAQ.mode = ADC;
	updateButtonStyle();
}


void CMFCMyDAQDMMDlg::OnBnClickedButtonaac()
{
	AfxMessageBox(_T("This function is not implemented!"));
}


void CMFCMyDAQDMMDlg::OnBnClickedButtonohm()
{
	myDAQ.mode = OHM;
	updateButtonStyle();
}


void CMFCMyDAQDMMDlg::OnCbnSelchangeModecombo()
{
	int sel = mode_combobox.GetCurSel();
	if (sel == 1) {
		AfxMessageBox(_T("This function is not implemented!"));
	}
	mode_combobox.SetCurSel(0);
}

UINT run(LPVOID Param) {
	ASSERT(Param != NULL);
	CMFCMyDAQDMMDlg* dlg = reinterpret_cast<CMFCMyDAQDMMDlg*>(Param);

	while (isRunning) {
		dlg->readDMM(mydaqName);
	}

	return true;
}

void CMFCMyDAQDMMDlg::OnBnClickedButtonrun()
{
	myDAQ.clearValues();
	CString mydaqSelection;
	device_combobox.GetLBText(device_combobox.GetCurSel(), mydaqSelection);
	mydaqName = string((CT2CA)mydaqSelection);

	device_combobox.EnableWindow(false);
	mode_combobox.EnableWindow(false);
	run_button.EnableWindow(false);
	vdc_button.EnableWindow(false);
	vac_button.EnableWindow(false);
	adc_button.EnableWindow(false);
	aac_button.EnableWindow(false);
	ohm_button.EnableWindow(false);
	stop_button.EnableWindow(true);
	run_button.SetState(true);

	isRunning = true;
	AfxBeginThread(run, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
}

void CMFCMyDAQDMMDlg::readDMM(string mydaq)
{
	float value;

	switch (myDAQ.mode) {
		case VDC:
			value = myDAQ.readDCVolt(mydaq);
			if (value < 0.1) {
				value = value * 1000;
				unit_label.SetWindowTextW(_T("mV"));
			}
			else {
				unit_label.SetWindowTextW(_T("V"));
			}
			break;
		case ADC:
			value = myDAQ.readDCCur(mydaq);
			if (value < 0.1) {
				value = value * 1000;
				unit_label.SetWindowTextW(_T("mA"));
			}
			else {
				unit_label.SetWindowTextW(_T("A"));
			}
			break;
		case OHM:
			value = myDAQ.readResist(mydaq);
			if (value > 1000000) {
				value = value / 1000000;
				unit_label.SetWindowTextW(_T("MΩ"));
			} else if (value > 2000) {
				value = value / 1000;
				unit_label.SetWindowTextW(_T("kΩ"));
			} else {
				unit_label.SetWindowTextW(_T("Ω"));
			}
			break;
	}

	stringstream s;
	s << fixed << setprecision(2) << value;
	value_label.SetWindowTextW((LPCTSTR)(CA2T)s.str().c_str());
}


void CMFCMyDAQDMMDlg::OnBnClickedButtonstop()
{
	isRunning = false;
	device_combobox.EnableWindow(true);
	mode_combobox.EnableWindow(true);
	vdc_button.EnableWindow(true);
	vac_button.EnableWindow(true);
	adc_button.EnableWindow(true);
	aac_button.EnableWindow(true);
	ohm_button.EnableWindow(true);
	run_button.EnableWindow(true);
	stop_button.EnableWindow(false);
	run_button.SetState(false);
	saveData();
}

void CMFCMyDAQDMMDlg::saveData() {
	FILE *pFile;
	errno_t err = 0;
	err = fopen_s(&pFile, "data.txt", "w+");
	list<float>::iterator it;
	for (it = myDAQ.values.begin(); it != myDAQ.values.end(); it++) {
		

		switch (myDAQ.mode) {
		case VDC:
			fprintf(pFile, "%.4f V\n", (*it));
			break;
		case ADC:
			fprintf(pFile, "%.4f A\n", (*it));
			break;
		case OHM:
			fprintf(pFile, "%.4f Ohm\n", (*it));
			break;
		}
	}
	fclose(pFile);
	AfxMessageBox(_T("Read values have been saved in data.txt!"));
}