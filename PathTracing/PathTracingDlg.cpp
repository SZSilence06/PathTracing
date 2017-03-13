
// PathTracingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PathTracing.h"
#include "PathTracingDlg.h"
#include "afxdialogex.h"
#include "ThreadFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPathTracingDlg 对话框

#define DLG_WIDTH      640
#define DLG_HEIGHT     480
#define GL_WIDTH       400
#define GL_HEIGHT      400


CPathTracingDlg::CPathTracingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPathTracingDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_PathTracer = NULL;
}

void CPathTracingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPathTracingDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CPathTracingDlg::OnBnClickedButton1)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_RENDER_COMPLETE, &CPathTracingDlg::OnRenderComplete)
	ON_MESSAGE(WM_SET_PROGRESS, &CPathTracingDlg::OnSetProgress)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CPathTracingDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON2, &CPathTracingDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CPathTracingDlg 消息处理程序

BOOL CPathTracingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	RECT rc1, rc2;
	GetClientRect(&rc1);
	GetWindowRect(&rc2);
	MoveWindow(0, 0, DLG_WIDTH + ((rc2.right - rc2.left) - (rc1.right - rc1.left)), 
		DLG_HEIGHT + ((rc2.bottom - rc2.top) - (rc1.bottom - rc1.top)));
	CenterWindow();

	m_BitmapDC.CreateCompatibleDC(m_pDC);
	
	srand(time(NULL));
	InitGLContext();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPathTracingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		//Render();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPathTracingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CPathTracingDlg::InitGLContext()
{
	m_pDC = new CClientDC(this);
	//Failure to Get DC
	if (m_pDC == NULL)
	{
		//::MessageBox("Error Obtaining DC");
		return FALSE;
	}
	return TRUE;
}

BOOL CPathTracingDlg::SetupPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR), // size of this pfd
		1,                              // version number
		PFD_DRAW_TO_WINDOW |            // support window
		PFD_SUPPORT_OPENGL |            // support OpenGL
		PFD_DOUBLEBUFFER,                // double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		24,                             // 24-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		16,                             // 16-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored 
	};

	int m_nPixelFormat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);
	if (m_nPixelFormat == 0)
	{
		return FALSE;
	}
	if (::SetPixelFormat(m_pDC->GetSafeHdc(), m_nPixelFormat, &pfd) == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

PathTracer* CPathTracingDlg::GetPathTracer()
{
	return m_PathTracer;
}

void CPathTracingDlg::BuildScene(int type)
{
	if (m_PathTracer){
		delete m_PathTracer;
		m_PathTracer = NULL;
	}
	switch (type){
	case 1:
	{
		m_PathTracer = new PathTracer(GL_WIDTH, GL_HEIGHT);
		m_PathTracer->SetCamera(Vector3f(0, 4.5f, -16.0f), Vector3f(0, 1, 0), Vector3f(0, 4.5f, 0.0f), 54, 250.0 / 200.0, 0.1f, 100.0f);
		Scene* scene = m_PathTracer->GetScene();

		Material red;
		red.diffuse = Color(0.75f, 0.25f, 0.25f);
		red.specular = Color(0.0f, 0.0f, 0.0f);
		red.transmissive = Color(0.0f, 0.0f, 0.0f);
		red.refractivity = 1.0f;
		red.roughness = 0.0f;

		Material blue;
		blue.diffuse = Color(0.25f, 0.25f, 0.75f);
		blue.specular = Color(0.0f, 0.0f, 0.0f);
		blue.transmissive = Color(0.0f, 0.0f, 0.0f);
		blue.refractivity = 1.0f;
		blue.roughness = 0.0f;

		Material mirror;
		mirror.diffuse = Color(0.0f, 0.0f, 0.0f);
		mirror.specular = Color(1.0f, 1.0f, 1.0f);
		mirror.transmissive = Color(0.0f, 0.0f, 0.0f);
		mirror.refractivity = 1.0f;
		mirror.roughness = 0.0f;

		Material glass;
		glass.diffuse = Color(0.0f, 0.0f, 0.0f);
		glass.specular = Color(0.25f, 0.25f, 0.25f);
		glass.transmissive = Color(0.75f, 0.75f, 0.75f);
		glass.refractivity = 1.8f;
		glass.roughness = 0.0f;

		Material white;
		white.diffuse = Color(0.75f, 0.75f, 0.75f);
		white.specular = Color(0.0f, 0.0f, 0.0f);
		white.transmissive = Color(0.0f, 0.0f, 0.0f);
		white.refractivity = 1.0f;
		white.roughness = 0.0f;

		Sphere* sphere1 = new Sphere(Vector3f(-2.4f, 1.6f, 1.6f), 1.6f);
		sphere1->SetMaterial(mirror);
		scene->AddRenderObject(sphere1);

		Sphere* sphere2 = new Sphere(Vector3f(2.24f, 1.6f, -1.9f), 1.6f);
		sphere2->SetMaterial(glass);
		scene->AddRenderObject(sphere2);

		//back
		Rect* back = new Rect(Vector3f(-5.75f, 0.0f, 5.0f), Vector3f(-5.75f, 10.0f, 5.0f),
			Vector3f(5.75f, 10.0f, 5.0f), Vector3f(5.75f, 0.0f, 5.0f));
		back->SetMaterial(white);
		scene->AddRenderObject(back);

		//floor
		Rect* floor = new Rect(Vector3f(-5.75f, 0.0f, -5.0f), Vector3f(-5.75f, 0.0f, 5.0f),
			Vector3f(5.75f, 0.0f, 5.0f), Vector3f(5.75f, 0.0f, -5.0f));
		floor->SetMaterial(white);
		scene->AddRenderObject(floor);

		//up
		Rect* up = new Rect(Vector3f(-5.75f, 10.0f, -5.0f), Vector3f(-5.75f, 10.0f, 5.0f),
			Vector3f(5.75f, 10.0f, 5.0f), Vector3f(5.75f, 10.0f, -5.0f));
		up->SetMaterial(white);
		scene->AddRenderObject(up);

		//left
		Rect* left = new Rect(Vector3f(-5.75f, 0.0f, -5.0f), Vector3f(-5.75f, 10.0f, -5.0f),
			Vector3f(-5.75f, 10.0f, 5.0f), Vector3f(-5.75f, 0.0f, 5.0f));
		left->SetMaterial(red);
		scene->AddRenderObject(left);

		//right
		Rect* right = new Rect(Vector3f(5.75f, 0.0f, -5.0f), Vector3f(5.75f, 10.0f, -5.0f),
			Vector3f(5.75f, 10.0f, 5.0f), Vector3f(5.75f, 0.0f, 5.0f));
		right->SetMaterial(blue);
		scene->AddRenderObject(right);

		AreaLight* light = new AreaLight(Color(12.0f, 12.0f, 12.0f));
		vector<RenderObject*> faces;
		faces.push_back(new Rect(Vector3f(-2.0f, 10.0f, -2.0f), Vector3f(2.0f, 10.0f, -2.0f),
			Vector3f(2.0f, 10.0f, 2.0f), Vector3f(-2.0f, 10.0f, 2.0f)));
		light->SetFaces(faces);
		scene->AddLight(light);
		break;
	}

	case 2:
	{
		m_PathTracer = new PathTracer(GL_WIDTH, GL_HEIGHT);
		m_PathTracer->SetCamera(Vector3f(0, 4.5f, -18.0f), Vector3f(0, 1, 0), Vector3f(0, 4.5f, 0.0f), 90, 250.0 / 200.0, 0.1f, 100.0f);
		Scene* scene = m_PathTracer->GetScene();

		Material grey;
		grey.diffuse = Color(1.0f, 1.0f, 1.0f);
		grey.specular = Color(0.0f, 0.0f, 0.0f);
		grey.transmissive = Color(0.0f, 0.0f, 0.0f);
		grey.refractivity = 1.0f;
		grey.roughness = 0.0f;

		Material board1;
		board1.diffuse = Color(0.0f, 0.0f, 0.0f);
		board1.specular = Color(0.25f, 0.25f, 0.25f);
		board1.transmissive = Color(0.0f, 0.0f, 0.0f);
		board1.refractivity = 1.0f;
		board1.roughness = 0.0f;

		Material board2;
		board2.diffuse = Color(0.0f, 0.0f, 0.0f);
		board2.specular = Color(0.25f, 0.25f, 0.25f);
		board2.transmissive = Color(0.0f, 0.0f, 0.0f);
		board2.refractivity = 1.0f;
		board2.roughness = 0.05f;

		Material board3;
		board3.diffuse = Color(0.0f, 0.0f, 0.0f);
		board3.specular = Color(0.25f, 0.25f, 0.25f);
		board3.transmissive = Color(0.0f, 0.0f, 0.0f);
		board3.refractivity = 1.0f;
		board3.roughness = 0.1f;

		Material board4;
		board4.diffuse = Color(0.0f, 0.0f, 0.0f);
		board4.specular = Color(0.25f, 0.25f, 0.25f);
		board4.transmissive = Color(0.0f, 0.0f, 0.0f);
		board4.refractivity = 1.0f;
		board4.roughness = 0.15f;

		//back
		Rect* back = new Rect(Vector3f(-24.0f, 0.0f, 0.0f), Vector3f(-24.0f, 25.0f, 0.0f),
			Vector3f(24.0f, 25.0f, 0.0f), Vector3f(24.0f, 0.0f, 0.0f));
		back->SetMaterial(grey);
		scene->AddRenderObject(back);

		//floor
		Rect* floor = new Rect(Vector3f(-24.0f, 0.0f, -21.0f), Vector3f(-24.0f, 0.0f, 0.0f),
			Vector3f(24.0f, 0.0f, 0.0f), Vector3f(24.0f, 0.0f, -21.0f));
		floor->SetMaterial(grey);
		scene->AddRenderObject(floor);

		//plane1
		Rect* plane1 = new Rect(Vector3f(-8.0f, 3.56f, -3.9f), Vector3f(8.0f, 3.56f, -3.9f),
			Vector3f(8.0f, 4.43f, -2.1f), Vector3f(-8.0f, 4.43f, -2.1f));
		plane1->SetMaterial(board1);
		scene->AddRenderObject(plane1);

		//plane2
		Rect* plane2 = new Rect(Vector3f(-8.0f, 2.7f, -7.0f), Vector3f(8.0f, 2.7f, -7.0f),
			Vector3f(8.0f, 3.3f, -5.0f), Vector3f(-8.0f, 3.3f, -5.0f));
		plane2->SetMaterial(board2);
		scene->AddRenderObject(plane2);

		//plane3
		Rect* plane3 = new Rect(Vector3f(-8.0f, 1.84f, -10.0f), Vector3f(8.0f, 1.84f, -10.0f),
			Vector3f(8.0f, 2.15f, -8.0f), Vector3f(-8.0f, 2.15f, -8.0f));
		plane3->SetMaterial(board3);
		scene->AddRenderObject(plane3);

		//plane4
		Rect* plane4 = new Rect(Vector3f(-8.0f, 1.0f, -13.0f), Vector3f(8.0f, 1.0f, -13.0f),
			Vector3f(8.0f, 1.0f, -11.0f), Vector3f(-8.0f, 1.0f, -11.0f));
		plane4->SetMaterial(board4);
		scene->AddRenderObject(plane4);
		
		AreaLight* light1 = new AreaLight(Color(6.0f, 6.0f, 6.0f));
		vector<RenderObject*> faces1;
		faces1.push_back(new Circle(Vector3f(-9.0f, 8.0f, 0.0f), 0.5f, Vector3f(0, 0, -1)));
		light1->SetFaces(faces1);
		scene->AddLight(light1);

		AreaLight* light2 = new AreaLight(Color(5.0f, 5.0f, 5.0f));
		vector<RenderObject*> faces2;
		faces2.push_back(new Circle(Vector3f(-3.0f, 8.0f, 0.0f), 1.0f, Vector3f(0, 0, -1)));
		light2->SetFaces(faces2);
		scene->AddLight(light2);

		AreaLight* light3 = new AreaLight(Color(4.0f, 4.0f, 4.0f));
		vector<RenderObject*> faces3;
		faces3.push_back(new Circle(Vector3f(3.0f, 8.0f, 0.0f), 1.5f, Vector3f(0, 0, -1)));
		light3->SetFaces(faces3);
		scene->AddLight(light3);

		AreaLight* light4 = new AreaLight(Color(3.0f, 3.0f, 3.0f));
		vector<RenderObject*> faces4;
		faces4.push_back(new Circle(Vector3f(9.0f, 8.0f, 0.0f), 2.0f, Vector3f(0, 0, -1)));
		light4->SetFaces(faces4);
		scene->AddLight(light4);

		AreaLight* light5 = new AreaLight(Color(1.0f, 1.0f, 1.0f));
		vector<RenderObject*> faces5;
		faces5.push_back(new Rect(Vector3f(-100.0f, 0.0f, -20.0f), Vector3f(100.0f, 0.0f, -20.0f),
			Vector3f(100.0f, 100.0f, -20.0f), Vector3f(-100.0f, 100.0f, -20.0f)));
		light5->SetFaces(faces5);
		scene->AddLight(light5);

		Rect* lightSphere = new Rect(Vector3f(-100.0f, 0.0f, -20.0f), Vector3f(100.0f, 0.0f, -20.0f),
			Vector3f(100.0f, 100.0f, -20.0f), Vector3f(-100.0f, 100.0f, -20.0f));
		lightSphere->SetMaterial(grey);
		scene->AddRenderObject(lightSphere);

		break;
	}

	}
}

void CPathTracingDlg::Render()
{

}

void CPathTracingDlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	BuildScene(1);

	(GetDlgItem(IDC_BUTTON1))->EnableWindow(FALSE);
	(GetDlgItem(IDC_BUTTON2))->EnableWindow(FALSE);
	m_hThreadRender = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ThreadProcRender, this, 0, NULL);

	m_progressDlg.DoModal();
	TerminateThread(m_hThreadRender, 0);
	(GetDlgItem(IDC_BUTTON1))->EnableWindow(TRUE);
	(GetDlgItem(IDC_BUTTON2))->EnableWindow(TRUE);
}


void CPathTracingDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO:  在此处添加消息处理程序代码

	TerminateThread(m_hThreadRender, 0);
	if (m_pDC)
	{
		delete m_pDC;
		m_pDC = NULL;
	}

	if (m_PathTracer){
		delete m_PathTracer;
	}

}


afx_msg LRESULT CPathTracingDlg::OnRenderComplete(WPARAM wParam, LPARAM lParam)
{
	m_progressDlg.Quit();

	int width = m_PathTracer->GetWidth();
	int height = m_PathTracer->GetHeight();
	COLORREF* pixel = new COLORREF[width*height];
	for (int j = 0; j < height; j++){
		for (int i = 0; i < width; i++){
			int r, g, b;
			Color col = m_PathTracer->GetBuffer(i, j);
			r = col.r * 255;
			g = col.g * 255;
			b = col.b * 255;
			*(pixel + j*width + i) = RGB(b, g, r);
		}
	}
	
	m_bitmap.DeleteObject();
	m_bitmap.CreateCompatibleBitmap(m_pDC, width, height);
	m_BitmapDC.SelectObject(m_bitmap);
	m_bitmap.SetBitmapBits(4 * width * height, pixel);
	m_pDC->BitBlt(0, 0, width, height, &m_BitmapDC, 0, 0, SRCCOPY);

	delete[] pixel;
	return 0;
}


afx_msg LRESULT CPathTracingDlg::OnSetProgress(WPARAM wParam, LPARAM lParam)
{
	if (m_progressDlg.Opened()){
		m_progressDlg.m_progress.StepIt();
	}

	int width = m_PathTracer->GetWidth();
	int height = wParam;
	COLORREF* pixel = new COLORREF[width*height];
	for (int j = 0; j < height; j++){
		for (int i = 0; i < width; i++){
			int r, g, b;
			Color col = m_PathTracer->GetBuffer(i, j);
			r = col.r * 255;
			g = col.g * 255;
			b = col.b * 255;
			*(pixel + j*width + i) = RGB(b, g, r);
		}
	}

	m_bitmap.DeleteObject();
	m_bitmap.CreateCompatibleBitmap(m_pDC, width, height);
	m_BitmapDC.SelectObject(m_bitmap);
	m_bitmap.SetBitmapBits(4 * width * height, pixel);
	m_pDC->BitBlt(0, 0, width, height, &m_BitmapDC, 0, 0, SRCCOPY);

	delete[] pixel;
	return 0;
}

void CPathTracingDlg::OnBnClickedButtonSave()
{
	// TODO:  在此添加控件通知处理程序代码
	CFileDialog fileDlg(FALSE, NULL, NULL, NULL, TEXT("*.jpg"));
	if (fileDlg.DoModal() != IDOK){
		return;
	}
	CString filePath = fileDlg.GetPathName();
	CImage image;
	image.Attach((HBITMAP)m_bitmap.GetSafeHandle());
	image.Save(filePath);
}


void CPathTracingDlg::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码
	// TODO:  在此添加控件通知处理程序代码
	BuildScene(2);
	
	(GetDlgItem(IDC_BUTTON1))->EnableWindow(FALSE);
	(GetDlgItem(IDC_BUTTON2))->EnableWindow(FALSE);

	m_hThreadRender = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ThreadProcRender, this, 0, NULL);

	m_progressDlg.DoModal();
	TerminateThread(m_hThreadRender, 0);
	(GetDlgItem(IDC_BUTTON1))->EnableWindow(TRUE);
	(GetDlgItem(IDC_BUTTON2))->EnableWindow(TRUE);
}
