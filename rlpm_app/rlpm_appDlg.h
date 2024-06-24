
// rlpm_appDlg.h: 헤더 파일
//

#pragma once


// CrlpmappDlg 대화 상자
class CrlpmappDlg : public CDialogEx
{
// 생성입니다.
public:
	CrlpmappDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RLPM_APP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	bool GetParameter();
	bool SetParameter();
	void GetRegisters();
	afx_msg void OnBnClickedButtonScope();
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonSet();
	afx_msg void OnBnClickedButtonGet();
	afx_msg void OnBnClickedButtonGetRegister();
};
