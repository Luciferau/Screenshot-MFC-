// CCaputureDlg.cpp: 实现文件
//

#include "pch.h"
#include "screenCutMFC.h"
#include "afxdialogex.h"
#include "CCaputureDlg.h"


// CCaputureDlg 对话框

IMPLEMENT_DYNAMIC(CCaputureDlg, CDialogEx)

CCaputureDlg::CCaputureDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CAPTURE, pParent)
{
	//对桌面拍照 显示到此窗口 
	int screenwidth = GetSystemMetrics(SM_CXSCREEN);
	int screenhight = GetSystemMetrics(SM_CYSCREEN);

	//获取到了桌面的设备DC 一张纸而已
	CDC* pDc = CDC::FromHandle(::GetDC(nullptr));

	//内存DC 承载桌面DC
	CDC  memDc;
	memDc.CreateCompatibleDC(pDc);//创建兼容DC 兼容于pDc

	//拷贝里面的图片

	CBitmap* pOLDBmp = nullptr;
	//CBitmap screenBmp;
	screenBmp.CreateCompatibleBitmap(pDc, screenwidth, screenhight);

	 pOLDBmp =  memDc.SelectObject(&screenBmp);


	 memDc.BitBlt(0, 0, screenwidth, screenhight, pDc,/*从这里copy*/0, 0, SRCCOPY);//进行图片的拷贝 

	 memDc.SelectObject(pOLDBmp);//把模撕下来

	 memDc.DeleteDC();

	 ::ReleaseDC(nullptr, pDc->m_hDC);
	  
}

CCaputureDlg::~CCaputureDlg()
{
}

void CCaputureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCaputureDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
//	ON_WM_MBUTTONDBLCLK()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CCaputureDlg 消息处理程序

//重新初始化窗口
BOOL CCaputureDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	 
	int screenwidth = GetSystemMetrics(SM_CXSCREEN);
	int screenhight = GetSystemMetrics(SM_CYSCREEN);

	//调整大小
	MoveWindow(0, 0, screenwidth, screenhight);

	m_RectTracker.m_nStyle = CRectTracker::resizeInside | CRectTracker::dottedLine;

	m_RectTracker.m_rect.SetRect(0, 0, 0, 0);
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


//防止刷新的clear
BOOL CCaputureDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDC memDC;//虚拟的内存DC

	memDC.CreateCompatibleDC(pDC);//关联pDc

	memDC.SelectObject(&screenBmp);//贴膜

	CRect rect;//矩形

	GetClientRect(rect);

	//贴膜
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.DeleteDC();
	return TRUE;
	return CDialogEx::OnEraseBkgnd(pDC);
}


void CCaputureDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//橡皮筋类一个矩形的东西

	//是不是点选在橡皮筋内
	if (m_RectTracker.HitTest(point) == CRectTracker::hitNothing) {

		m_RectTracker.TrackRubberBand(this, point, TRUE);

	}
	else
	{
		m_RectTracker.Track(this, TRUE);
		m_RectTracker.m_rect.NormalizeRect();

	}

	Invalidate(TRUE);

	 
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CCaputureDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()

	CDC* pDC = GetDC();

	m_RectTracker.Draw(pDC);

	ReleaseDC(pDC);
}






//void CCaputureDlg::OnMButtonDblClk(UINT nFlags, CPoint point)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//
//	CDialogEx::OnMButtonDblClk(nFlags, point);
//}


void CCaputureDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值


	if (m_RectTracker.HitTest(point) != CRectTracker::hitMiddle)
	{
		return;
	}

	CDC* pDc = GetDC();
	CDC memDC;
	memDC.CreateCompatibleDC(pDc);
	memDC.SelectObject(&screenBmp);

	CRect rect;
	rect = m_RectTracker.m_rect;
	CBitmap mBmp, * pOldBmp;
	mBmp.CreateCompatibleBitmap(pDc, rect.Width(), rect.Height());

	CDC dstDC;
	dstDC.CreateCompatibleDC(pDc);

	pOldBmp = dstDC.SelectObject(&mBmp);

	dstDC.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, rect.left, rect.top, SRCCOPY);

	CImage img;
	img.Attach(mBmp);
	img.Save(L"./lucifer.png");

	mBmp.DeleteObject();
	memDC.DeleteDC();
	dstDC.DeleteDC();
	ReleaseDC(pDc);

	CDialog::OnCancel();

	CDialogEx::OnLButtonDblClk(nFlags, point);
}
