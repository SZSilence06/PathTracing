
// PathTracingDlg.h : ͷ�ļ�
//

#pragma once

#include "PathTracer.h"
#include "ProgressDlg.h"

#define WM_RENDER_COMPLETE               WM_USER + 200
#define WM_SET_PROGRESS                  WM_USER + 201


// CPathTracingDlg �Ի���
class CPathTracingDlg : public CDialogEx
{
// ����
public:
	CPathTracingDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PATHTRACING_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

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

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
