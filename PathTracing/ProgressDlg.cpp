// ProgressDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PathTracing.h"
#include "PathTracingDlg.h"
#include "ProgressDlg.h"
#include "afxdialogex.h"


// CProgressDlg 对话框

IMPLEMENT_DYNAMIC(CProgressDlg, CDialogEx)

CProgressDlg::CProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProgressDlg::IDD, pParent)
{

}

CProgressDlg::~CProgressDlg()
{
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
}


BEGIN_MESSAGE_MAP(CProgressDlg, CDialogEx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS1, &CProgressDlg::OnNMCustomdrawProgress1)
	ON_BN_CLICKED(IDCANCEL, &CProgressDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CProgressDlg 消息处理程序


void CProgressDlg::OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
}

BOOL CProgressDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	_opened = true;
	CPathTracingDlg* pDlg = (CPathTracingDlg*)AfxGetMainWnd();
	int height = pDlg->GetPathTracer()->GetHeight();
	m_progress.SetRange(0, height);
	m_progress.SetStep(1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CProgressDlg::Quit()
{
	OnBnClickedCancel();
}

bool CProgressDlg::Opened()
{
	return _opened;
}


void CProgressDlg::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	_opened = false;
	CDialogEx::OnCancel();
}
