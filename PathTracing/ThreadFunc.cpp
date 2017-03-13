#include "stdafx.h"
#include "PathTracingDlg.h"
#include "ThreadFunc.h"

UINT ThreadProcRender(LPVOID pParam)
{
	CPathTracingDlg* pDlg = (CPathTracingDlg*)pParam;
	
	PathTracer* pPathTracer = pDlg->GetPathTracer();
	if (!pPathTracer){
		return TRUE;
	}

	int width = pPathTracer->GetWidth();
	int height = pPathTracer->GetHeight();

	pPathTracer->ClearBuffer(Color(0.0f, 0.0f, 0.0f));
	int t1, t2;
	t1 = GetTickCount();
#pragma omp parallel for schedule(dynamic, 1) 
	for (int j = 0; j < height; j++){	
		for (int i = 0; i < width; i++){
			pPathTracer->TracePath(i, j);
		}
		pDlg->PostMessage(WM_SET_PROGRESS, j);
	}
	t2 = GetTickCount();
	CString str;
	str.Format(TEXT("%d"), t2 - t1);
	AfxMessageBox(str);

	pDlg->PostMessage(WM_RENDER_COMPLETE);
	return TRUE;
}