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

	// 初始化成员变量
	m_LButtonDown = FALSE;
	m_pLineCur = NULL;
	m_nPenWidth = 1;
	m_bDrawingShape = false;
	m_eShapeType = ShapeType::FREE_DRAW;
	m_eMoveMode = MoveMode::None;
	m_pMovingPoint = nullptr;
	m_pMovingLine = nullptr;
	m_pt = CPoint(0, 0);
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
	ON_BN_CLICKED(IDC_MOVE_LINE, &CDRAWDlg::OnBnClickedMoveLine)
	ON_BN_CLICKED(IDC_MOVE_POINT, &CDRAWDlg::OnBnClickedMovePoint)

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
	CBrush BrushBk(m_ColorBg);
	CBrush* pOldBrush = pDC->SelectObject(&BrushBk);
	pDC->Rectangle(m_x1, m_y1, m_x2, m_y2);
	pDC->SelectObject(pOldBrush);
	BrushBk.DeleteObject();

	// 画笔设置
	CPen NewPen(PS_SOLID, m_nPenWidth, m_ColorLine);
	CPen* pOldPen = pDC->SelectObject(&NewPen);

	// 绘制所有已保存的线条
	m_Data.ShowLine(pDC, SPoint(0, m_x1, m_y2), m_StateShow);

	// 绘制当前正在画的线
	CPen NewPenCur(PS_SOLID, m_nPenWidth, m_ColorLineCur);
	pDC->SelectObject(&NewPenCur);
	m_Data.ShowCurLine(pDC, SPoint(0, m_x1, m_y2), m_StateShow);

	// 绘制节点
	if (m_StateShow.m_bViewPoint) {
		CPen NewPenPoint(PS_SOLID, 1, m_ColorPoint);
		pDC->SelectObject(&NewPenPoint);
		m_Data.ShowPoint(pDC, SPoint(0, m_x1, m_y2), m_StateShow);

		CPen NewPenPointCur(PS_SOLID, 1, m_ColorPointCur);
		pDC->SelectObject(&NewPenPointCur);
		m_Data.ShowCurPoint(pDC, SPoint(0, m_x1, m_y2), m_StateShow);
	}

	pDC->SelectObject(pOldPen);
	NewPen.DeleteObject();
	NewPenCur.DeleteObject();


	// 绘制形状预览
	if (m_bDrawingShape && m_LButtonDown)
	{
		// 保存当前绘图状态
		int nOldROP2 = pDC->SetROP2(R2_NOTXORPEN);
		CPen previewPen(PS_DOT, 1, RGB(0, 0, 0));
		CBrush* pNullBrush = (CBrush*)CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));

		CPen* pOldPreviewPen = pDC->SelectObject(&previewPen);
		CBrush* pOldPreviewBrush = pDC->SelectObject(pNullBrush);

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

	// 高亮绘制选中的节点
	CPen selectionPen(PS_SOLID, 2, RGB(255, 0, 0)); // 红色，更粗的画笔
	CBrush selectionBrush(RGB(255, 0, 0)); // 红色画刷
	pOldPen = pDC->SelectObject(&selectionPen);
	pOldBrush = pDC->SelectObject(&selectionBrush);
	for (int i = 0; i < m_SelectedPoints.GetSize(); i++)
	{
		CPoint* pPoint = m_SelectedPoints.GetAt(i);
		if (pPoint)
		{
			pDC->Ellipse(pPoint->x - 4, pPoint->y - 4, pPoint->x + 4, pPoint->y + 4);
		}
	}
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
}


//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDRAWDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

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
	// 更新当前鼠标位置
	m_CurrentMousePos = point;

	if (m_LButtonDown && m_bDraw && m_eShapeType == ShapeType::FREE_DRAW)
	{
		if (point.x > m_x1 && point.x < m_x2 && point.y > m_y1 && point.y < m_y2)
		{
			point.x -= m_StateShow.m_dx;
			point.y += m_StateShow.m_dy;
			SPoint tPoint = { 0, (double)point.x, (double)point.y };
			SPoint::XY2xy(tPoint, SPoint(0, m_x1, m_y2), m_StateShow);
			m_Data.AddPoint(tPoint);
			Invalidate();
		}
	}
	else if (m_LButtonDown && !m_bDraw && m_eMoveMode != MoveMode::None)
	{
		if (m_pMovingPoint)
		{
			SPoint tPoint = { 0, (double)point.x, (double)point.y };
			SPoint::XY2xy(tPoint, SPoint(0, m_x1, m_y2), m_StateShow);
			m_pMovingPoint->m_x = tPoint.m_x;
			m_pMovingPoint->m_y = tPoint.m_y;
			Invalidate();
		}
		else if (m_pMovingLine)
		{
			int dx = point.x - m_ptDragStart.x;
			int dy = point.y - m_ptDragStart.y;
			m_ptDragStart = point; // 更新拖动起点

			SPoint delta = { 0, (double)dx, (double)dy };
			// 注意：这里只做简单偏移，没有考虑缩放，后续可优化
			
			for (int i = 0; i < m_pMovingLine->GetNum(); i++)
			{
				SPoint& pt = m_pMovingLine->GetPointForUpdate(i);
				pt.m_x += delta.m_x / m_StateShow.m_r;
				pt.m_y -= delta.m_y / m_StateShow.m_r;
			}
			Invalidate();
		}
	}
	else if (m_LButtonDown && m_bDraw && m_bDrawingShape)
	{
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
	CString s;
	s.Format(CString("坐标: (%4d, %4d)"), m_CurrentMousePos.x, m_CurrentMousePos.y);
	pDC->TextOut(20, m_h - 40, s);

	s.Format(CString("倍率: %5.2f"), m_StateShow.m_r);
	pDC->TextOut(200, m_h - 40, s);
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

void CDRAWDlg::OnBnClickedSel()
{
	m_bDraw = false;
	GetDlgItem(IDC_DRAW)->EnableWindow(true);
	GetDlgItem(IDC_SEL)->EnableWindow(false);

	// 清理内存
	for (int i = 0; i < m_SelectedPoints.GetSize(); i++)
	{
		delete m_SelectedPoints.GetAt(i);
	}
	m_SelectedPoints.RemoveAll();
	Invalidate();
}

void CDRAWDlg::OnBnClickedDelPoint() {
	m_Data.DelPoint();
	Invalidate();
}

void CDRAWDlg::OnBnClickedDelLine() {
	m_Data.DelLine();
	Invalidate();
}

void CDRAWDlg::OnBnClickedMoveLine()
{
	m_bDraw = false;
	m_eMoveMode = MoveMode::MoveLine;
	// 可以添加一些UI反馈，比如让其他按钮失效
}

void CDRAWDlg::OnBnClickedMovePoint()
{
	m_bDraw = false;
	m_eMoveMode = MoveMode::MovePoint;
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

	if (m_bDraw) // 绘制模式
	{
		m_LButtonDown = TRUE;
		point.x -= m_StateShow.m_dx;
		point.y += m_StateShow.m_dy;
		SPoint tPoint = { 0, (double)point.x, (double)point.y };
		SPoint::XY2xy(tPoint, SPoint(0, m_x1, m_y2), m_StateShow);

		if (m_eShapeType == ShapeType::FREE_DRAW) {
			m_Data.AddPoint(tPoint);
		}
		else {
			m_Data.EndLine();
			m_bDrawingShape = true;
			m_ptShapeStart = point;
		}
	}
	else // 选择或移动模式
	{
		m_LButtonDown = TRUE;
		m_ptDragStart = point;

		if (m_eMoveMode == MoveMode::MovePoint)
		{
			m_pMovingPoint = nullptr;
			SPoint tPoint = { 0, (double)point.x, (double)point.y };
			SPoint::XY2xy(tPoint, SPoint(0, m_x1, m_y2), m_StateShow);
			m_pMovingPoint = m_Data.FindPoint(tPoint, 3);
		}
		else if (m_eMoveMode == MoveMode::MoveLine)
		{
			m_pMovingLine = nullptr;
			SPoint tPoint = { 0, (double)point.x, (double)point.y };
			SPoint::XY2xy(tPoint, SPoint(0, m_x1, m_y2), m_StateShow);
			m_pMovingLine = m_Data.FindLine(tPoint, 3);
		}
		else // 选择模式
		{
			// 清理上一次选择的内存
			for (int i = 0; i < m_SelectedPoints.GetSize(); i++)
			{
				delete m_SelectedPoints.GetAt(i);
			}
			m_SelectedPoints.RemoveAll(); // 清除之前的选择
			bool bNodeSelected = false;

			// 检查是否点中节点
			for (int i = 0; i < m_Data.GetNum(); i++)
			{
				CLine* pLine = m_Data.GetLine(i);
				if (!pLine) continue;
				for (int j = 0; j < pLine->GetNum(); j++)
				{
					SPoint nodeS = pLine->GetPoint(j);

					// 将世界坐标转换为屏幕坐标
					SPoint screenNodeS = nodeS;
					SPoint::xy2XY(screenNodeS, SPoint(0, m_x1, m_y2), m_StateShow);
					screenNodeS.m_x += m_StateShow.m_dx;
					screenNodeS.m_y -= m_StateShow.m_dy;
					CPoint screenNode((int)screenNodeS.m_x, (int)screenNodeS.m_y);

					// 计算距离
					int dist = (point.x - screenNode.x) * (point.x - screenNode.x) + (point.y - screenNode.y) * (point.y - screenNode.y);
					if (dist < 25) // 5像素的点击范围
					{
						// 使用 new 创建 CPoint 对象，因为数组存储的是指针
						CPoint* pSelectedPoint = new CPoint(screenNode);
						m_SelectedPoints.Add(pSelectedPoint);
						bNodeSelected = true;
						break; // 点中一个节点后就不用再找这条线上的其他节点
					}
				}
				if (bNodeSelected)
				{
					break; // 点中一个节点后就不用再找其他线
				}
			}

			// 如果没有点中节点，则尝试选择线
			if (!bNodeSelected)
			{
				SPoint tPoint = { 0, (double)point.x, (double)point.y };
				SPoint::XY2xy(tPoint, SPoint(0, m_x1, m_y2), m_StateShow);
				m_Data.SetCurrent(tPoint);
			}
		}
		Invalidate(); // 触发重绘
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

// 鼠标抬起事件
void CDRAWDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_LButtonDown = FALSE;
	m_pMovingPoint = nullptr;
	m_pMovingLine = nullptr;
	m_eMoveMode = MoveMode::None; // 结束移动状态

	if (m_bDraw && m_bDrawingShape)
	{
		if (point.x < m_x1 || point.x > m_x2 || point.y < m_y1 || point.y > m_y2)
			return;

		// 结束形状绘制
		m_bDrawingShape = false;
		CPoint ptStart = m_ptShapeStart;
		CPoint ptEnd = point;

		// 转换坐标
		ptStart.x -= m_StateShow.m_dx; ptStart.y += m_StateShow.m_dy;
		ptEnd.x -= m_StateShow.m_dx; ptEnd.y += m_StateShow.m_dy;

		SPoint spStart = { 0, (double)ptStart.x, (double)ptStart.y };
		SPoint::XY2xy(spStart, SPoint(0, m_x1, m_y2), m_StateShow);

		SPoint spEnd = { 0, (double)ptEnd.x, (double)ptEnd.y };
		SPoint::XY2xy(spEnd, SPoint(0, m_x1, m_y2), m_StateShow);

		switch (m_eShapeType)
		{
		case ShapeType::RECTANGLE:
		{
			m_Data.AddPoint(spStart);
			m_Data.AddPoint(SPoint(0, spEnd.m_x, spStart.m_y));
			m_Data.AddPoint(spEnd);
			m_Data.AddPoint(SPoint(0, spStart.m_x, spEnd.m_y));
			m_Data.AddPoint(spStart);
			break;
		}
		case ShapeType::SQUARE:
		{
			double size = max(abs(spEnd.m_x - spStart.m_x), abs(spEnd.m_y - spStart.m_y));
			double x2 = spStart.m_x + (spEnd.m_x > spStart.m_x ? size : -size);
			double y2 = spStart.m_y + (spEnd.m_y > spStart.m_y ? size : -size);

			m_Data.AddPoint(spStart);
			m_Data.AddPoint(SPoint(0, x2, spStart.m_y));
			m_Data.AddPoint(SPoint(0, x2, y2));
			m_Data.AddPoint(SPoint(0, spStart.m_x, y2));
			m_Data.AddPoint(spStart);
			break;
		}
		case ShapeType::CIRCLE:
		{
			double centerX = (spStart.m_x + spEnd.m_x) / 2.0;
			double centerY = (spStart.m_y + spEnd.m_y) / 2.0;
			double radius = sqrt(pow(spEnd.m_x - spStart.m_x, 2) + pow(spEnd.m_y - spStart.m_y, 2)) / 2.0;
			const int numPoints = 36;
			for (int i = 0; i <= numPoints; i++)
			{
				double angle = 2 * 3.1415926535 / numPoints * i;
				double x = centerX + radius * cos(angle);
				double y = centerY + radius * sin(angle);
				m_Data.AddPoint(SPoint(0, x, y));
			}
			break;
		}
		}

	}
	Invalidate();
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
		m_bDrawingShape = false; // 切换形状时取消预览
		m_LButtonDown = FALSE;

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
	SetButtonTextFromResource(IDC_MOVE_LINE, IDS_STRING_MOVE_LINE);
	SetButtonTextFromResource(IDC_MOVE_POINT, IDS_STRING_MOVE_POINT);
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
