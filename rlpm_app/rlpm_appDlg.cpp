
// rlpm_appDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "rlpm_app.h"
#include "rlpm_appDlg.h"
#include "afxdialogex.h"
#include "ScopeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CrlpmappDlg 대화 상자



CrlpmappDlg::CrlpmappDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RLPM_APP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CrlpmappDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CrlpmappDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SCOPE, &CrlpmappDlg::OnBnClickedButtonScope)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CrlpmappDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_SET, &CrlpmappDlg::OnBnClickedButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_GET, &CrlpmappDlg::OnBnClickedButtonGet)
	ON_BN_CLICKED(IDC_BUTTON_GET_REGISTER, &CrlpmappDlg::OnBnClickedButtonGetRegister)
END_MESSAGE_MAP()


// CrlpmappDlg 메시지 처리기

BOOL CrlpmappDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.
	CListCtrl *ListRegister = (CListCtrl *)GetDlgItem(IDC_LIST_REGISTER);
	CRect rt;
	ListRegister->GetWindowRect(&rt);
	ListRegister->InsertColumn(0, TEXT("Name"), LVCFMT_CENTER, rt.Width()*0.6);
	ListRegister->InsertColumn(1, TEXT("Value"), LVCFMT_CENTER, rt.Width()*0.35);
	return TRUE;  
}

void CrlpmappDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CrlpmappDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CrlpmappDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CrlpmappDlg::OnBnClickedButtonScope()
{
	CRlpmApi *pRlpm = &((CrlpmappApp*)AfxGetApp())->mRlpmApi;

	CScopeDlg* pDlg = new CScopeDlg(this, pRlpm);
	INT_PTR nResponse = pDlg->DoModal();
	delete pDlg;
}


void CrlpmappDlg::OnBnClickedButtonConnect()
{
	CRlpmApi *pRlpm = &((CrlpmappApp*)AfxGetApp())->mRlpmApi;
	CListBox* pDeviceterminal = (CListBox*)theApp.m_pMainWnd->GetDlgItem(IDC_LIST_TERMINAL);
	if (pRlpm->Open())
	{
		/*
		CListBox* pDeviceList = (CListBox*)theApp.m_pMainWnd->GetDlgItem(IDC_LIST_DEVICE_LIST);
		for (int i = 0; i < pRlpm->Device_GetCount(); i++)
			pDeviceList->InsertString(-1, CA2W(pRlpm->Device_GetName(i)));
		*/
		GetParameter();
		GetRegisters();
		pDeviceterminal->InsertString(-1, _T("Connect Success"));
	}
	else
		pDeviceterminal->InsertString(-1,_T("Connect Fail"));
}


bool CrlpmappDlg::SetParameter()
{
	CRlpmApi *pRlpm = &((CrlpmappApp*)AfxGetApp())->mRlpmApi;
	if (!pRlpm->IsOpen())
		return false;

	UpdateData(true);
	unsigned int nFMAX_Min = GetDlgItemInt(IDC_EDIT_FMAX_MIN);
	unsigned int nFMAX_Max = GetDlgItemInt(IDC_EDIT_FMAX_MAX);
	unsigned int nLatency_Min = GetDlgItemInt(IDC_EDIT_LT_MIN);
	unsigned int nLatency_Max = GetDlgItemInt(IDC_EDIT_LT_MAX);
	unsigned int nFWHDuty_Min = GetDlgItemInt(IDC_EDIT_FWHM_MIN);
	unsigned int nFWHDuty_Max = GetDlgItemInt(IDC_EDIT_FWHM_MAX);

	unsigned int bEnable = GetDlgItemInt(IDC_EDIT_GATE_ENABLE);
	short nOffset = GetDlgItemInt(IDC_EDIT_ADC_OFFSET);
	unsigned int nDelay = GetDlgItemInt(IDC_EDIT_CAPTURE_DELAY);

	pRlpm->SetFmaxThreshold(nFMAX_Min, nFMAX_Max);
	pRlpm->SetLatencyThreshold(nLatency_Min, nLatency_Max);
	pRlpm->SetFWHMDutyThreshold(nFWHDuty_Min, nFWHDuty_Max);
	
	pRlpm->SetGateEnable(bEnable);
	pRlpm->SetADCOffset(nOffset);
	pRlpm->SetADCCaptureDelay(nDelay);

	ERLPM_MODE eRlpmMode;
	int nUseChannel;
	ESYNC_MODE eSyncMode;

	CComboBox *RlpmMode = (CComboBox *)GetDlgItem(IDC_COMBO_RLPM_MODE);
	CComboBox *UseCh = (CComboBox *)GetDlgItem(IDC_COMBO_USE_CHANNEL);
	CComboBox *SyncMode = (CComboBox *)GetDlgItem(IDC_COMBO_SYNC_MODE);

	eRlpmMode = (ERLPM_MODE)RlpmMode->GetCurSel();
	nUseChannel = UseCh->GetCurSel();
	eSyncMode = (ESYNC_MODE)SyncMode->GetCurSel();

	pRlpm->SetRlpmMode(eRlpmMode, nUseChannel);
	pRlpm->SetSyncMode(eSyncMode);
	
	return true;
}
bool CrlpmappDlg::GetParameter()
{
	CRlpmApi *pRlpm = &((CrlpmappApp*)AfxGetApp())->mRlpmApi;

	if (!pRlpm->IsOpen())
		return false;

	unsigned int nFMAX_Min;		
	unsigned int nFMAX_Max;		
	unsigned int nLatency_Min;	
	unsigned int nLatency_Max;	
	unsigned int nFWHDuty_Min;	
	unsigned int nFWHDuty_Max;	

	bool bEnable;
	short nOffset;
	unsigned int nDelay;

	pRlpm->GetFmaxThreshold(&nFMAX_Min, &nFMAX_Max);
	pRlpm->GetLatencyThreshold(&nLatency_Min, &nLatency_Max);
	pRlpm->GetFWHMDutyThreshold(&nFWHDuty_Min, &nFWHDuty_Max);

	pRlpm->GetGateEnable(&bEnable);
	pRlpm->GetADCOffset(&nOffset);
	pRlpm->GetADCCaptureDelay(&nDelay);

	ERLPM_MODE eRlpmMode;
	int nUseChannel;
	ESYNC_MODE eSyncMode;

	pRlpm->GetRlpmMode(&eRlpmMode, &nUseChannel);
	pRlpm->GetSyncMode(&eSyncMode);

	CComboBox *RlpmMode = (CComboBox *)GetDlgItem(IDC_COMBO_RLPM_MODE);
	CComboBox *UseCh = (CComboBox *)GetDlgItem(IDC_COMBO_USE_CHANNEL);
	CComboBox *SyncMode = (CComboBox *)GetDlgItem(IDC_COMBO_SYNC_MODE);

	RlpmMode->SetCurSel(eRlpmMode);
	UseCh->SetCurSel(nUseChannel);
	SyncMode->SetCurSel(eSyncMode);

	SetDlgItemInt(IDC_EDIT_FMAX_MIN, nFMAX_Min);
	SetDlgItemInt(IDC_EDIT_FMAX_MAX, nFMAX_Max);
	SetDlgItemInt(IDC_EDIT_LT_MIN, nLatency_Min);
	SetDlgItemInt(IDC_EDIT_LT_MAX, nLatency_Max);
	SetDlgItemInt(IDC_EDIT_FWHM_MIN, nFWHDuty_Min);
	SetDlgItemInt(IDC_EDIT_FWHM_MAX, nFWHDuty_Max);

	SetDlgItemInt(IDC_EDIT_GATE_ENABLE, bEnable);
	SetDlgItemInt(IDC_EDIT_ADC_OFFSET, nOffset);
	SetDlgItemInt(IDC_EDIT_CAPTURE_DELAY, nDelay);
	UpdateData(false);

	return true;
}

#include "rlpm_reg.h"

void CrlpmappDlg::GetRegisters()
{
	CRlpmApi *pRlpm = &((CrlpmappApp*)AfxGetApp())->mRlpmApi;
	int regcnt = sizeof(rlpm_regmaps) / sizeof(CRlpmReg);

	CListCtrl *ListRegister = (CListCtrl *)GetDlgItem(IDC_LIST_REGISTER);
	ListRegister->DeleteAllItems();
	int nCalcRegisterIndex = 4; //임시

	unsigned int value = 1234;
	int nItem;
	char str[32];
	for (int i = regcnt - 1; i >= 0; i--)
	{
		
		if(pRlpm->GetRegister(EDEVICE_PARA, rlpm_regmaps[i].offset, &value))
		{
			nItem = ListRegister->InsertItem(0, CA2W(rlpm_regmaps[i].name));
			sprintf_s(str, "0x%08x", value);
			ListRegister->SetItemText(nItem,1, CA2W(str));
		}
	}
}

void CrlpmappDlg::OnBnClickedButtonSet()
{
	SetParameter();
}


void CrlpmappDlg::OnBnClickedButtonGet()
{
	GetParameter();
}


void CrlpmappDlg::OnBnClickedButtonGetRegister()
{
	GetRegisters();
}