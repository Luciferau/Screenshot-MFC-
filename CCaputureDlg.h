#pragma once
#include "afxdialogex.h"


// CCaputureDlg 对话框

class CCaputureDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCaputureDlg)
	//当前桌面的截图
	CBitmap screenBmp;

	//橡皮筋类
	CRectTracker m_RectTracker;
public:
public:
	CCaputureDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCaputureDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CAPTURE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
//	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};
