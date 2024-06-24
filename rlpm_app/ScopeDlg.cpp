// ScopeDlg.cpp: 구현 파일
//

#include "pch.h"
#include "rlpm_app.h"
#include "ScopeDlg.h"
#include "afxdialogex.h"
#include "ChartLineSerie.h"
#include "log.h"

// CScopeDlg 대화 상자
#define new DEBUG_NEW



IMPLEMENT_DYNAMIC(CScopeDlg, CDialogEx)

CScopeDlg::CScopeDlg(CWnd* pParent, CRlpmApi *_pRlpm)
	: CDialogEx(IDD_DIALOG_SCOPE, pParent)
{
	pRpmApi = _pRlpm;
}

CScopeDlg::~CScopeDlg()
{
}

void CScopeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM_CHART, m_Chart);
	DDX_Text(pDX, IDC_EDIT_MIN_Y, mnMinY);
	DDX_Text(pDX, IDC_EDIT_MAX_Y, mnMaxY);
	DDX_Text(pDX, IDC_EDIT_GATHERING_CNT, mnGatheringCnt);
}

BEGIN_MESSAGE_MAP(CScopeDlg, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_START, &CScopeDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CScopeDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_SET_PPY, &CScopeDlg::OnBnClickedButtonSetPPY)
END_MESSAGE_MAP()

BOOL CScopeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	mListCh.SubclassDlgItem(IDC_LIST_SET_CH, this);
	/*
	if (!pRpmApi->IsOpen())
	{
		AfxMessageBox(_T("ERROR : context not connect "));
		OnCancel();
	}
	*/
	mnMinY = -200;
	mnMaxY = 30000;
	mnGatheringCnt = 1024;
	InitChart();
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(false);
	//pRpmApi->Init();
	Invalidate(false);
	UpdateData(false);
	return TRUE;  
}

void CScopeDlg::InitChart(void) {
	CChartStandardAxis* pLeftAxis = m_Chart.CreateStandardAxis(CChartCtrl::LeftAxis);
	CChartStandardAxis* pBottomAxis = m_Chart.CreateStandardAxis(CChartCtrl::BottomAxis);
	pLeftAxis->SetMinMax(mnMinY, mnMaxY);
	pBottomAxis->SetMinMax(0, mnGatheringCnt*2);
	CChartCrossHairCursor* pCrossHair = m_Chart.CreateCrossHairCursor();
}

void CScopeDlg::DrawChart(void)
{
	CChartXYSerie* pSeries = nullptr;

	if (m_Chart.GetSeriesCount() > 0)
		m_Chart.RemoveAllSeries();

	int nCount = pRpmApi->Wave_RefillBuffer();


	nCount = nCount / sizeof(int);
	int *pVal;
	
	if (nCount > 4095)
		return;

	double *pXVal = new double[nCount];
	double *pYVal = new double[nCount];

	pVal = (int *)pRpmApi->Wave_GetBufferData();

	for (int i = 0; i < nCount; i++)
	{
		pXVal[i] = i*2;
		//pYVal[i] = (pVal[i] >> 16);
		pYVal[i] = (short)(pVal[i] & 0xFFFF);
	}
	pSeries = m_Chart.CreateLineSerie();
	pSeries->SetPoints(pXVal, pYVal, nCount);
	
	pVal = NULL;
	delete[] pXVal;
	delete[] pYVal;
}

void CScopeDlg::PrintConsol(void)
{
	int nCount = pRpmApi->Calc_RefillBuffer();

	nCount = nCount / sizeof(UINT16);



	UINT16 *pVal;// = new UINT16[nCount];
	pVal = (UINT16 *)pRpmApi->Calc_GetBufferData();

	if (nCount > 4095)
		return;

	for (int i = 0; i < nCount; i++)
	{
		printf("%d\t", pVal[i]);
		if ((i + 1) % 8 == 0)
			printf("\n");
	}
	pVal = NULL;
}

void CScopeDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case 1: {}
		DrawChart();
		PrintConsol();
	}
		
	CDialogEx::OnTimer(nIDEvent);
}

void CScopeDlg::OnBnClickedButtonStart()
{
	GetDlgItem(IDC_BUTTON_SET_PPY)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_GATHERING_CNT)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_MIN_Y)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_MAX_Y)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(true);
	pRpmApi->Wave_CreateBuffer(mnGatheringCnt);
	pRpmApi->Calc_CreateBuffer(mnGatheringCnt);
	SetTimer(1, 10, NULL);
}

void CScopeDlg::OnBnClickedButtonStop()
{
	KillTimer(1);

	GetDlgItem(IDC_BUTTON_SET_PPY)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_GATHERING_CNT)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_MIN_Y)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_MAX_Y)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(false);
	pRpmApi->Wave_DistroyBuffer();
	pRpmApi->Calc_DistroyBuffer();
}

void CScopeDlg::OnBnClickedButtonSetPPY()
{
	UpdateData(TRUE);

	if (mnMinY >= mnMaxY)
		AfxMessageBox(_T("The max value must be greater than the min value."));
	else
	{
		m_Chart.RemoveAllSeries();
		CChartStandardAxis* pLeftAxis = m_Chart.CreateStandardAxis(CChartCtrl::LeftAxis);
		CChartStandardAxis* pBottomAxis = m_Chart.CreateStandardAxis(CChartCtrl::BottomAxis);
		pLeftAxis->SetMinMax(mnMinY, mnMaxY);
		pBottomAxis->SetMinMax(0, mnGatheringCnt);
	}
}
