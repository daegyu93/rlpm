#pragma once
#include "ChartCtrl.h"
#include "rlpmApi.h"

struct TOpenedDevice
{
	int nDeviceIndex;
	int nChannelInedex;
};

class CScopeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CScopeDlg)

public:
	CScopeDlg(CWnd* pParent , CRlpmApi *_pRlpm);   // 표준 생성자입니다.
	virtual ~CScopeDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SCOPE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	CCheckListBox mListCh;
	DECLARE_MESSAGE_MAP()
public:
	CRlpmApi *pRpmApi;
	int mnDevCount;
	int mnDevChannelCount[MAX_DEVICE_COUNT];

	int mnMinY;
	int mnMaxY;
	unsigned int mnGatheringCnt;

	virtual BOOL OnInitDialog();
	CChartCtrl m_Chart;

	void InitChart(void);
	void DrawChart(void);
	void PrintConsol(void);

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonSetPPY();
};
