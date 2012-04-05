// ChildView.cpp : CChildView 类的实现
//

#include "stdafx.h"
#include "Demo.h"
#include "ChildView.h"
#include ".\childview.h"
#include "gldemo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
//	ON_WM_CREATE()
ON_WM_CREATE()
ON_WM_SIZE()
//ON_WM_SIZING()
ON_WM_ERASEBKGND()
ON_WM_KEYDOWN()
END_MESSAGE_MAP()



// CChildView 消息处理程序

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{ 
	if (!CWnd::PreCreateWindow(cs))
		return FALSE; 

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);


	return TRUE;
}



void CChildView::OnPaint() 
{
	CPaintDC dc(this); // 用于绘制的设备上下文
	
	// TODO: 在此处添加消息处理程序代码
	// 不要为绘制消息而调用 CWnd::OnPaint()
	if (m_rotate == FALSE) {
		return;
	}
	GLPaint();
	Invalidate(FALSE);
}




int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_pDC = new CClientDC(this);
	m_rotate = TRUE;

	GLCreate(m_pDC->GetSafeHdc());
	return 0;
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	GLResize(cx, cy);
	// TODO: 在此处添加消息处理程序代码
}


BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nChar == VK_LEFT) {
		GLChangeFilter();
	}
	if (nChar == VK_RIGHT) {
		GLChangeEnvMode();
	}
	if (nChar == VK_SPACE) {
		GLChangeDemo();
	}
	Invalidate(FALSE);

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}
