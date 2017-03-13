
// PathTracingDlg.h : 头文件
//

#pragma once

#include "PathTracer.h"
#include "ProgressDlg.h"

#define WM_RENDER_COMPLETE               WM_USER + 200
#define WM_SET_PROGRESS                  WM_USER + 201


// CPathTracingDlg 对话框
class CPathTracingDlg : public CDialogEx
{
// 构造
public:
	CPathTracingDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PATHTRACING_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// Coder-added attributes:
protected:
	CDC* m_pDC;
	HGLRC m_hRC;
	PathTracer* m_PathTracer;
	HANDLE m_hThreadRender;
	CProgressDlg m_progressDlg;
	CDC m_BitmapDC;
	CBitmap m_bitmap;

// Coder-added methods:
public:
	void BuildScene(int type);
	void Render();
	BOOL InitGLContext();
	BOOL SetupPixelFormat();
	PathTracer* GetPathTracer();

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
protected:
	afx_msg LRESULT OnRenderComplete(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetProgress(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButton2();
};
