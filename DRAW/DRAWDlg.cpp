// DRAWDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "DRAW.h"
#include "DRAWDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma once
#include "CData.h"

// 用于应用程序"关于"菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDRAWDlg 对话框


// 初始化
CDRAWDlg::CDRAWDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DRAW_DIALOG, pParent)
{
	m_w = 2048;
	m_h = 1440;
	m_x1 = 200; // 左边框
	m_y1 = 10;
	m_x2 = m_w - 200; // 右边框
	m_y2 = m_h - 150; // 下边框
	m_StateShow.m_r = 1.0;          //放大倍率
	m_StateShow.m_dx = 0;           //水平移动
	m_StateShow.m_dy = 0;           //垂直移动
	m_StateShow.m_bViewPoint = true;//显示结点
	m_ColorLine = RGB(0, 0, 0);   //黑色
	m_ColorPoint = RGB(0, 0, 0);    
	m_ColorLineCur = RGB(0, 0, 0); 
	m_ColorPointCur = RGB(0, 0, 0);
	m_bDraw = true;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_ColorBg = RGB(255, 255, 255); // 初始背景色设为白色，可根据需求修改
	m_ColorBgCur = RGB(255, 255, 255);

	// 设置颜色对话框标志（CC_FULLOPEN | CC_RGBINIT）
	m_ColorDlg.m_cc.Flags |= CC_RGBINIT | CC_FULLOPEN;
	m_ColorDlg.m_cc.rgbResult = m_ColorBg;

}	

void CDRAWDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PEN_WIDTH_COMBO, m_cboPenWidth);
	DDX_Control(pDX, IDC_SHAPE, m_cboShape);

}

BEGIN_MESSAGE_MAP(CDRAWDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	
	ON_WM_TIMER()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()

	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN() // 绑定鼠标按下事件
	

	// 按钮控件
	ON_BN_CLICKED(IDC_NEW, &CDRAWDlg::OnBnClickedNew)
	ON_BN_CLICKED(IDC_OPEN, &CDRAWDlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDC_SAVE, &CDRAWDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_SAVE_AS, &CDRAWDlg::OnBnClickedSaveAs)
	ON_BN_CLICKED(IDC_ZOOMIN, &CDRAWDlg::OnBnClickedZoomin)
	ON_BN_CLICKED(IDC_ZOOMOUT, &CDRAWDlg::OnBnClickedZoomout)
	ON_BN_CLICKED(IDC_ZOOMDEF, &CDRAWDlg::OnBnClickedZoomdef)
	ON_BN_CLICKED(IDC_MUP, &CDRAWDlg::OnBnClickedUp)
	ON_BN_CLICKED(IDC_MDOWN, &CDRAWDlg::OnBnClickedDown)
	ON_BN_CLICKED(IDC_ML, &CDRAWDlg::OnBnClickedMl)
	ON_BN_CLICKED(IDC_MR, &CDRAWDlg::OnBnClickedMr)
	ON_BN_CLICKED(IDC_VIEW_POINT, &CDRAWDlg::OnBnClickedViewPoint)
	ON_BN_CLICKED(IDC_HIDE_POINT, &CDRAWDlg::OnBnClickedHidePoint)
	ON_BN_CLICKED(IDC_DEL_POINT, &CDRAWDlg::OnBnClickedDelPoint)
	ON_BN_CLICKED(DC_DEL_LINE, &CDRAWDlg::OnBnClickedDelLine)
	ON_BN_CLICKED(IDC_DRAW, &CDRAWDlg::OnBnClickedDraw)
	ON_BN_CLICKED(IDC_SEL, &CDRAWDlg::OnBnClickedSel)

	// 颜色设置
	ON_BN_CLICKED(IDC_PEN, &CDRAWDlg::OnBnClickedBtnLineColor)
	ON_BN_CLICKED(IDC_BG, &CDRAWDlg::OnBnClickedBtnBgColor)
	ON_BN_CLICKED(IDC_POINT, &CDRAWDlg::OnBnClickedBtnPointColor)

	// 笔画粗细
	ON_CBN_SELCHANGE(IDC_PEN_WIDTH_COMBO, &CDRAWDlg::OnCbnSelchangePenWidthCombo)

	// 添加组合框选择变更事件处理
	ON_CBN_SELCHANGE(IDC_SHAPE, &CDRAWDlg::OnCbnSelchangeShapeCombo)

END_MESSAGE_MAP()


// CDRAWDlg 消息处理程序

BOOL CDRAWDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将"关于..."菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	InitButtonLabels();

	SetWindowPos(NULL, 0, 0, m_w, m_h, SWP_NOZORDER | SWP_NOMOVE);
	m_Font.CreatePointFont(105, "黑体", NULL);
	GetDlgItem(IDC_SAVE)->EnableWindow(false);

	// 初始化线条颜色按钮
	UpdateButtonColor(IDC_PEN, m_ColorLine);
	// 初始化背景颜色按钮
	UpdateButtonColor(IDC_BG, m_ColorBg);
	// 初始化节点颜色按钮
	UpdateButtonColor(IDC_POINT, m_ColorPoint);

	// 填充笔画粗细选项
	for (int i = 1; i <= 10; i++) {
		CString strWidth;  // 创建CString对象
		strWidth.Format(_T("%d"), i);  // 通过对象调用Format
		m_cboPenWidth.AddString(strWidth);
	}
	m_cboPenWidth.SetCurSel(0); // 默认选择1像素
	m_nPenWidth = 1;

	
	// 填充形状选项
	if (m_cboShape.GetSafeHwnd()) // 确保控件句柄有效
	{
		m_cboShape.AddString(_T("自由绘制"));
		m_cboShape.AddString(_T("矩形"));
		m_cboShape.AddString(_T("正方形"));
		m_cboShape.AddString(_T("圆形"));
		m_cboShape.SetCurSel(0); // 设置默认选项
	}
	// 初始化鼠标位置
	m_CurrentMousePos = CPoint(0, 0);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDRAWDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDRAWDlg::OnPaint()
{
	if (IsIconic()) // 最小化时的处理
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
		CPaintDC dc(this);
		Draw(&dc);
		ShowStatus(&dc);
		CDialogEx::OnPaint();
	}
}

void CDRAWDlg::Draw(CDC* pDC) {
	// 背景设置
	CBrush* pOldBrush, BrushBk(m_ColorBg);
	pOldBrush = pDC->SelectObject(&BrushBk);
	pDC->Rectangle(m_x1, m_y1, m_x2, m_y2);
	pDC->SelectObject(pOldBrush);

	// 画笔设置
	CPen NewPen(PS_SOLID, m_nPenWidth, m_ColorLine);
	CPen* pOldPen = pDC->SelectObject(&NewPen);
	m_Data.ShowLine(pDC, SPoint(0, m_x1, m_y2), m_StateShow);

	CPen NewPenCur(PS_SOLID, m_nPenWidth, m_ColorLineCur);
	pDC->SelectObject(&NewPenCur);
	m_Data.ShowCurLine(pDC, SPoint(0, m_x1, m_y2), m_StateShow);

	if (m_StateShow.m_bViewPoint) {
		CPen NewPenPoint(PS_SOLID, 1, m_ColorPoint);
		pDC->SelectObject(&NewPenPoint);
		m_Data.ShowPoint(pDC, SPoint(0, m_x1, m_y2), m_StateShow);

		CPen NewPenPointCur(PS_SOLID, 1, m_ColorPointCur);
		pDC->SelectObject(&NewPenPointCur);
		m_Data.ShowCurPoint(pDC, SPoint(0, m_x1, m_y2), m_StateShow);
	}

	pDC->SelectObject(pOldPen);

	// 绘制形状预览
	if (m_bDrawingShape)
	{
		// 保存当前绘图状态
		int nOldROP2 = pDC->SetROP2(R2_XORPEN);
		CPen previewPen(PS_DOT, 1, RGB(0, 0, 0));
		CBrush nullBrush;
		nullBrush.CreateStockObject(NULL_BRUSH);

		CPen* pOldPreviewPen = pDC->SelectObject(&previewPen);
		CBrush* pOldPreviewBrush = pDC->SelectObject(&nullBrush);

		// 转换坐标
		CPoint ptStart = m_ptShapeStart;
		CPoint ptEnd = m_CurrentMousePos;

		switch (m_eShapeType)
		{
		case ShapeType::RECTANGLE:
			pDC->Rectangle(ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
			break;

		case ShapeType::SQUARE:
		{
			int size = max(abs(ptEnd.x - ptStart.x), abs(ptEnd.y - ptStart.y));
			int x2 = ptStart.x + (ptEnd.x > ptStart.x ? size : -size);
			int y2 = ptStart.y + (ptEnd.y > ptStart.y ? size : -size);
			pDC->Rectangle(ptStart.x, ptStart.y, x2, y2);
			break;
		}

		case ShapeType::CIRCLE:
		{
			double centerX = (ptStart.x + ptEnd.x) / 2.0;
			double centerY = (ptStart.y + ptEnd.y) / 2.0;
			double radius = sqrt(pow(ptEnd.x - ptStart.x, 2) + pow(ptEnd.y - ptStart.y, 2)) / 2.0;
			pDC->Ellipse(
				(int)(centerX - radius),
				(int)(centerY - radius),
				(int)(centerX + radius),
				(int)(centerY + radius)
			);
			break;
		}
		default:
			break;
		}

		// 恢复绘图状态
		pDC->SelectObject(pOldPreviewBrush);
		pDC->SelectObject(pOldPreviewPen);
		pDC->SetROP2(nOldROP2);
	}
}


//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDRAWDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*
void CDRAWDlg::OnLButtonUp(UINT nFlags, CPoint point) {
	if (point.x <= m_x1 || point.x >= m_x2 || point.y <= m_y1 || point.y >= m_y2)
		return;
	point.x -= m_StateShow.m_dx;
	point.y += m_StateShow.m_dy;
	SPoint tPoint = { 0, (double)point.x, (double)point.y };
	SPoint::XY2xy(tPoint, SPoint(0, m_x1, m_y2), m_StateShow);
	if (m_bDraw)
		m_Data.AddPoint(tPoint);
	else
		m_Data.SetCurrent(tPoint);
	CClientDC dc(this);
	Draw(&dc);
	ShowStatus(&dc);
	CDialogEx::OnLButtonUp(nFlags, point);

}
*/
void CDRAWDlg::OnRButtonDown(UINT nFlags, CPoint point) {
	m_Data.EndLine();
	CDialogEx::OnRButtonDown(nFlags, point);
}

void CDRAWDlg::OnBnClickedOpen() {
	char szFilter[] = "HPGL Plotter files (*.PLT)|*.plt|Text files (*.txt)|*.txt||";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	if (dlg.DoModal() == IDOK) {
		m_csFilePath = dlg.GetPathName();
		m_Data.ReadPlt(dlg.GetPathName());
		GetDlgItem(IDC_SAVE)->EnableWindow(true);
		Invalidate();
	}
}

void CDRAWDlg::OnBnClickedSave() {
	m_Data.WritePlt(m_csFilePath);
	AfxMessageBox("文件保存成功!");
}

void CDRAWDlg::OnBnClickedSaveAs() {
	char szFilter[] = "HPGL Plotter files(*.plt)|*.plt|Text files(*.txt)|*.txt||";
	CFileDialog dlg(FALSE, "plt", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	if (dlg.DoModal() == IDOK) {
		m_csFilePath = dlg.GetPathName();
		m_Data.WritePlt(dlg.GetPathName());
		AfxMessageBox("文件保存成功!");
		CClientDC dc(this);
		ShowStatus(&dc);
	}
}

void CDRAWDlg::OnMouseMove(UINT nFlags, CPoint point) { 
	// 输出光标坐标
	CString csString;
	SPoint tPoint(0, point.x, point.y);
	SPoint::XY2xy(tPoint, SPoint(0, 105, m_h - 70), m_StateShow);
	csString.Format("x=% 04d,y=% 04d", (int)tPoint.m_x - m_StateShow.m_dx, (int)tPoint.m_y - m_StateShow.m_dy);
	CClientDC dc(this);
	dc.TextOut(m_w - 350, m_h - 130, csString);

	// 更新当前鼠标位置
	m_CurrentMousePos = point;

	// 如果正在绘制形状，触发重绘
	if (m_bDrawingShape) {
		Invalidate();
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

void CDRAWDlg::OnBnClickedNew() {
	m_csFilePath = "";
	GetDlgItem(IDC_SAVE)->EnableWindow(false);
	m_Data.Clear();
	Invalidate();
	CClientDC dc(this);
	ShowStatus(&dc);
}

void CDRAWDlg::ShowStatus(CDC* pDC) {
		CString csString;
		csString.Format("图形总数: % 04d; 放大倍率: % 0.2f; 横向偏移: % 04d; 纵向偏移: % 04d; 文件路径: % s",
			m_Data.GetNum(), m_StateShow.m_r, m_StateShow.m_dx, m_StateShow.m_dy, m_csFilePath);
		
		/*
		pDC->SetTextColor(RGB(0, 0, 0)); // 黑色文字
		pDC->SetBkMode(TRANSPARENT);     // 透明背景
		*/

		CFont* pOldFont = pDC->SelectObject(&m_Font);
		pDC->TextOut(200, m_h - 130, csString);
		pDC->SelectObject(pOldFont);

}

void CDRAWDlg::OnBnClickedZoomin() {
	m_StateShow.m_r *= 1.2;
	Invalidate();
}

void CDRAWDlg::OnBnClickedZoomout() {
	m_StateShow.m_r *= 0.8;
	Invalidate();
}

void CDRAWDlg::OnBnClickedZoomdef() {
	m_StateShow.m_r = 1.0;
	Invalidate();
}

void CDRAWDlg::OnBnClickedUp() {
	m_StateShow.m_dy += 100;
	Invalidate();
}

void CDRAWDlg::OnBnClickedDown() {
	m_StateShow.m_dy -= 100;
	Invalidate();
}

void CDRAWDlg::OnBnClickedMl() {
	m_StateShow.m_dx -= 100;
	Invalidate();
}

void CDRAWDlg::OnBnClickedMr() {
	m_StateShow.m_dx += 100;
	Invalidate();
}

void CDRAWDlg::OnBnClickedViewPoint() {
	GetDlgItem(IDC_VIEW_POINT)->EnableWindow(false);
	GetDlgItem(IDC_HIDE_POINT)->EnableWindow(true);
	m_StateShow.m_bViewPoint = true;
	Invalidate();
}

void CDRAWDlg::OnBnClickedHidePoint() {
	GetDlgItem(IDC_HIDE_POINT)->EnableWindow(false);
	GetDlgItem(IDC_VIEW_POINT)->EnableWindow(true);
	m_StateShow.m_bViewPoint = false;
	Invalidate();
}

void CDRAWDlg::OnBnClickedDraw() {
	GetDlgItem(IDC_DRAW)->EnableWindow(false);
	GetDlgItem(IDC_SEL)->EnableWindow(true);
	m_bDraw = true;
}

void CDRAWDlg::OnBnClickedSel() {
	GetDlgItem(IDC_SEL)->EnableWindow(false);
	GetDlgItem(IDC_DRAW)->EnableWindow(true);
	m_bDraw = false;
}

void CDRAWDlg::OnBnClickedDelPoint() {
	m_Data.DelPoint();
	Invalidate();
}

void CDRAWDlg::OnBnClickedDelLine() {
	m_Data.DelLine();
	Invalidate();
}

//----------------新增颜色选择-------------
// 线条颜色选择
void CDRAWDlg::OnBnClickedBtnLineColor()
{
	m_ColorDlg.m_cc.rgbResult = m_ColorLineCur; // 设置初始颜色
	m_ColorDlg.m_cc.rgbResult = m_ColorLine; // 设置初始颜色

	if (m_ColorDlg.DoModal() == IDOK)
	{
		m_ColorLineCur = m_ColorDlg.GetColor();
		m_ColorLine = m_ColorDlg.GetColor();
		UpdateButtonColor(IDC_PEN, m_ColorLineCur);
		UpdateButtonColor(IDC_PEN, m_ColorLine);
		Invalidate();
	}
}

// 节点颜色选择
void CDRAWDlg::OnBnClickedBtnPointColor()
{
	m_ColorDlg.m_cc.rgbResult = m_ColorPoint; // 设置初始颜色
	if (m_ColorDlg.DoModal() == IDOK)
	{
		m_ColorPoint = m_ColorDlg.GetColor();
		UpdateButtonColor(IDC_POINT, m_ColorPoint);
		Invalidate();
	}
}

// 背景颜色选择
void CDRAWDlg::OnBnClickedBtnBgColor()
{
	m_ColorDlg.m_cc.rgbResult = m_ColorBg; // 设置初始颜色
	if (m_ColorDlg.DoModal() == IDOK)
	{
		m_ColorBg = m_ColorDlg.GetColor();
		UpdateButtonColor(IDC_BG, m_ColorBg);
		Invalidate();
	}
}

// 更新按钮颜色显示
void CDRAWDlg::UpdateButtonColor(UINT nButtonID, COLORREF color)
{
	if (auto pButton = GetDlgItem(nButtonID))
	{
		CClientDC dc(pButton);
		CRect rect;
		pButton->GetClientRect(&rect);

		// 填充颜色
		CBrush brush(color);
		dc.FillRect(rect, &brush);

		// 绘制边框（可选）
		dc.DrawEdge(rect, EDGE_RAISED, BF_RECT);
	}
}


// 鼠标按下事件
void CDRAWDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (point.x < m_x1 || point.x > m_x2 || point.y < m_y1 || point.y > m_y2)
        return;

    // 根據組合框選擇的形狀類型處理
    if (m_eShapeType == ShapeType::FREE_DRAW) {
        // 自由繪製模式
        point.x -= m_StateShow.m_dx;
        point.y += m_StateShow.m_dy;
        SPoint tPoint = { 0, (double)point.x, (double)point.y };
        SPoint::XY2xy(tPoint, SPoint(0, m_x1, m_y2), m_StateShow);

        if (m_bDraw)
            m_Data.AddPoint(tPoint);
        else
            m_Data.SetCurrent(tPoint);
    }
    else {
        // 規則形狀模式
        m_Data.EndLine(); // 只在開始新形狀時結束前一個線段
        m_bDrawingShape = true;
        m_ptShapeStart = point;
    }

    Invalidate();
    CDialogEx::OnLButtonDown(nFlags, point);
}

// 鼠标抬起事件
void CDRAWDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (point.x < m_x1 || point.x > m_x2 || point.y < m_y1 || point.y > m_y2)
        return;

    if (m_bDrawingShape)
    {
        // 轉換坐標
        CPoint ptStart = m_ptShapeStart;
        CPoint ptEnd = point;

        // 根據形狀類型添加點
        switch (m_eShapeType)
        {
        case ShapeType::RECTANGLE:
        {
            // 添加矩形的四個頂點
            SPoint p1 = { 0, (double)(ptStart.x - m_StateShow.m_dx), (double)(ptStart.y + m_StateShow.m_dy) };
            SPoint p2 = { 0, (double)(ptEnd.x - m_StateShow.m_dx), (double)(ptStart.y + m_StateShow.m_dy) };
            SPoint p3 = { 0, (double)(ptEnd.x - m_StateShow.m_dx), (double)(ptEnd.y + m_StateShow.m_dy) };
            SPoint p4 = { 0, (double)(ptStart.x - m_StateShow.m_dx), (double)(ptEnd.y + m_StateShow.m_dy) };
            
            // 轉換坐標
            SPoint::XY2xy(p1, SPoint(0, m_x1, m_y2), m_StateShow);
            SPoint::XY2xy(p2, SPoint(0, m_x1, m_y2), m_StateShow);
            SPoint::XY2xy(p3, SPoint(0, m_x1, m_y2), m_StateShow);
            SPoint::XY2xy(p4, SPoint(0, m_x1, m_y2), m_StateShow);
            
            m_Data.AddPoint(p1);
            m_Data.AddPoint(p2);
            m_Data.AddPoint(p3);
            m_Data.AddPoint(p4);
            m_Data.AddPoint(p1); // 閉合矩形
            break;
        }

        case ShapeType::SQUARE:
        {
            int size = max(abs(ptEnd.x - ptStart.x), abs(ptEnd.y - ptStart.y));
            int x2 = ptStart.x + (ptEnd.x > ptStart.x ? size : -size);
            int y2 = ptStart.y + (ptEnd.y > ptStart.y ? size : -size);
            
            SPoint p1 = { 0, (double)(ptStart.x - m_StateShow.m_dx), (double)(ptStart.y + m_StateShow.m_dy) };
            SPoint p2 = { 0, (double)(x2 - m_StateShow.m_dx), (double)(ptStart.y + m_StateShow.m_dy) };
            SPoint p3 = { 0, (double)(x2 - m_StateShow.m_dx), (double)(y2 + m_StateShow.m_dy) };
            SPoint p4 = { 0, (double)(ptStart.x - m_StateShow.m_dx), (double)(y2 + m_StateShow.m_dy) };
            
            // 轉換坐標
            SPoint::XY2xy(p1, SPoint(0, m_x1, m_y2), m_StateShow);
            SPoint::XY2xy(p2, SPoint(0, m_x1, m_y2), m_StateShow);
            SPoint::XY2xy(p3, SPoint(0, m_x1, m_y2), m_StateShow);
            SPoint::XY2xy(p4, SPoint(0, m_x1, m_y2), m_StateShow);
            
            m_Data.AddPoint(p1);
            m_Data.AddPoint(p2);
            m_Data.AddPoint(p3);
            m_Data.AddPoint(p4);
            m_Data.AddPoint(p1); // 閉合正方形
            break;
        }

        case ShapeType::CIRCLE:
        {
            double centerX = (ptStart.x + ptEnd.x) / 2.0;
            double centerY = (ptStart.y + ptEnd.y) / 2.0;
            double radius = sqrt(pow(ptEnd.x - ptStart.x, 2) + pow(ptEnd.y - ptStart.y, 2)) / 2.0;
            
            const int numPoints = 32;
            for (int i = 0; i <= numPoints; i++)
            {
                double angle = 2 * 3.14159 * i / numPoints;
                double x = centerX + radius * cos(angle);
                double y = centerY + radius * sin(angle);
                SPoint p = { 0, x - m_StateShow.m_dx, y + m_StateShow.m_dy };
                SPoint::XY2xy(p, SPoint(0, m_x1, m_y2), m_StateShow);
                m_Data.AddPoint(p);
            }
            break;
        }
        }

        m_bDrawingShape = false;
        Invalidate();
    }

    CDialogEx::OnLButtonUp(nFlags, point);
}

// 笔画粗细
void CDRAWDlg::OnCbnSelchangePenWidthCombo() {
	int nSel = m_cboPenWidth.GetCurSel(); // 获取选中索引
	if (nSel != CB_ERR) {
		CString csWidth;
		m_cboPenWidth.GetLBText(nSel, csWidth); // 获取选中项文本
		m_nPenWidth = _ttoi(csWidth); // 转换为整数
		Invalidate(); // 刷新界面应用新宽度
	}
}

// 添加组合框选择变更事件处理函数
void CDRAWDlg::OnCbnSelchangeShapeCombo()
{
	int nSel = m_cboShape.GetCurSel();
	if (nSel != CB_ERR)
	{
		switch (nSel)
		{
		case 0: // 自由绘制
			m_eShapeType = ShapeType::FREE_DRAW;
			break;

		case 1: // 矩形
			m_eShapeType = ShapeType::RECTANGLE;
			break;

		case 2: // 正方形
			m_eShapeType = ShapeType::SQUARE;
			break;

		case 3: // 圆形
			m_eShapeType = ShapeType::CIRCLE;
			break;
		}

		m_bDraw = true; // 确保处于绘制模式
		Invalidate();   // 刷新界面
	}
}

void CDRAWDlg::SetButtonTextFromResource(UINT buttonId, UINT stringId)
{
	CString buttonText;
	if (buttonText.LoadString(stringId))
	{
		CWnd* pButton = GetDlgItem(buttonId);
		if (pButton)
		{
			pButton->SetWindowText(buttonText);
		}
	}
}

void CDRAWDlg::InitButtonLabels()
{
	SetButtonTextFromResource(IDC_NEW, IDS_STRING_NEW);
	SetButtonTextFromResource(IDC_OPEN, IDS_STRING_OPEN);
	SetButtonTextFromResource(IDC_SAVE, IDS_STRING_SAVE);
	SetButtonTextFromResource(IDC_SAVE_AS, IDS_STRING_SAVE_AS);
	SetButtonTextFromResource(IDC_BG, IDS_STRING_BG);
	SetButtonTextFromResource(IDC_PEN, IDS_STRING_PEN);
	SetButtonTextFromResource(IDC_POINT, IDS_STRING_POINT);
	SetButtonTextFromResource(IDC_DRAW, IDS_STRING_DRAW);
	SetButtonTextFromResource(IDC_SEL, IDS_STRING_SEL);
	SetButtonTextFromResource(IDC_MOVE, IDS_STRING_MOVE);
	//SetButtonTextFromResource(IDC_MOVE_LINE, IDS_STRING_MOVE_LINE);
	//SetButtonTextFromResource(IDC_MOVE_POINT, IDS_STRING_MOVE_POINT);
	SetButtonTextFromResource(IDC_DEL_POINT, IDS_STRING_DEL_POINT);
	SetButtonTextFromResource(DC_DEL_LINE, IDS_STRING_DEL_LINE);
	SetButtonTextFromResource(IDC_UNDO, IDS_STRING_UNDO);
	SetButtonTextFromResource(IDC_REDO, IDS_STRING_REDO);
	SetButtonTextFromResource(IDC_VIEW_POINT, IDS_STRING_VIEW_POINT);
	SetButtonTextFromResource(IDC_HIDE_POINT, IDS_STRING_HIDE_POINT);
	SetButtonTextFromResource(IDC_ZOOMIN, IDS_STRING_ZOOMIN);
	SetButtonTextFromResource(IDC_ZOOMOUT, IDS_STRING_ZOOMOUT);
	SetButtonTextFromResource(IDC_ZOOMDEF, IDS_STRING_ZOOMDEF);
	SetButtonTextFromResource(IDC_MUP, IDS_STRING_MUP);
	SetButtonTextFromResource(IDC_MDOWN, IDS_STRING_MDOWN);
	SetButtonTextFromResource(IDC_ML, IDS_STRING_ML);
	SetButtonTextFromResource(IDC_MR, IDS_STRING_MR);
}
