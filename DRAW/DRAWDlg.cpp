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

// CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX) {}
void CAboutDlg::DoDataExchange(CDataExchange* pDX) { CDialogEx::DoDataExchange(pDX); }
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDRAWDlg 对话框

CDRAWDlg::CDRAWDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DRAW_DIALOG, pParent)
{
	m_w = 2048; m_h = 1440;
	m_x1 = 220; m_y1 = 10; m_x2 = m_w - 200; m_y2 = m_h - 150;
	m_StateShow.m_r = 1.0; m_StateShow.m_dx = 0; m_StateShow.m_dy = 0;
	m_StateShow.m_bViewPoint = true;
	m_ColorLine = RGB(0, 0, 0); m_ColorPoint = RGB(0, 0, 0); m_ColorBg = RGB(255, 255, 255);
	m_ColorLineCur = RGB(0, 0, 0); m_ColorPointCur = RGB(0, 0, 0); m_ColorBgCur = RGB(255, 255, 255);
	m_bDraw = true;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_ColorDlg.m_cc.Flags |= CC_RGBINIT | CC_FULLOPEN; m_ColorDlg.m_cc.rgbResult = m_ColorBg;
	m_LButtonDown = FALSE; m_pLineCur = NULL; m_nPenWidth = 1;
	m_bDrawingShape = false; m_eShapeType = ShapeType::FREE_DRAW; m_eMoveMode = MoveMode::None;
	m_pMovingPoint = nullptr; m_pMovingLine = nullptr;
	m_pt = CPoint(0, 0);
}

void CDRAWDlg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PEN_WIDTH_COMBO, m_cboPenWidth);
	DDX_Control(pDX, IDC_SHAPE, m_cboShape);
}

BEGIN_MESSAGE_MAP(CDRAWDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
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
	ON_BN_CLICKED(IDC_PEN, &CDRAWDlg::OnBnClickedBtnLineColor)
	ON_BN_CLICKED(IDC_BG, &CDRAWDlg::OnBnClickedBtnBgColor)
	ON_BN_CLICKED(IDC_POINT, &CDRAWDlg::OnBnClickedBtnPointColor)
	ON_CBN_SELCHANGE(IDC_PEN_WIDTH_COMBO, &CDRAWDlg::OnCbnSelchangePenWidthCombo)
	ON_CBN_SELCHANGE(IDC_SHAPE, &CDRAWDlg::OnCbnSelchangeShapeCombo)
END_MESSAGE_MAP()

BOOL CDRAWDlg::OnInitDialog() {
	CDialogEx::OnInitDialog();
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr) {
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty()) {
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	SetIcon(m_hIcon, TRUE); SetIcon(m_hIcon, FALSE);
	InitButtonLabels();
	SetWindowPos(NULL, 0, 0, m_w, m_h, SWP_NOZORDER | SWP_NOMOVE);
	GetDlgItem(IDC_SAVE)->EnableWindow(false);
	UpdateButtonColor(IDC_PEN, m_ColorLine); UpdateButtonColor(IDC_BG, m_ColorBg); UpdateButtonColor(IDC_POINT, m_ColorPoint);
	for (int i = 1; i <= 10; i++) { CString s; s.Format(_T("%d"), i); m_cboPenWidth.AddString(s); }
	m_cboPenWidth.SetCurSel(0); m_nPenWidth = 1;
	if (m_cboShape.GetSafeHwnd()) {
		AddStringFromResource(m_cboShape, IDS_SHAPE_FREEDRAW); AddStringFromResource(m_cboShape, IDS_SHAPE_RECTANGLE);
		AddStringFromResource(m_cboShape, IDS_SHAPE_SQUARE); AddStringFromResource(m_cboShape, IDS_SHAPE_CIRCLE);
		m_cboShape.SetCurSel(0);
	}
	m_CurrentMousePos = CPoint(0, 0);
	return TRUE;
}

void CDRAWDlg::OnSysCommand(UINT nID, LPARAM lParam) {
	if ((nID & 0xFFF0) == IDM_ABOUTBOX) { CAboutDlg().DoModal(); }
	else { CDialogEx::OnSysCommand(nID, lParam); }
}

void CDRAWDlg::OnPaint() {
	if (IsIconic()) {
		CPaintDC dc(this); SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		CRect rect; GetClientRect(&rect);
		int x = (rect.Width() - GetSystemMetrics(SM_CXICON) + 1) / 2;
		int y = (rect.Height() - GetSystemMetrics(SM_CYICON) + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else { CPaintDC dc(this); Draw(&dc); ShowStatus(&dc); CDialogEx::OnPaint(); }
}

void CDRAWDlg::Draw(CDC* pDC) {
	CBrush BrushBk(m_ColorBg); CBrush* pOldBrush = pDC->SelectObject(&BrushBk);
	pDC->Rectangle(m_x1, m_y1, m_x2, m_y2);
	pDC->SelectObject(pOldBrush); BrushBk.DeleteObject();
	CPen NewPen(PS_SOLID, m_nPenWidth, m_ColorLine); CPen* pOldPen = pDC->SelectObject(&NewPen);
	m_Data.Show(pDC, SPoint(0, (double)m_x1, (double)m_y2), m_StateShow);
	m_Data.ShowCur(pDC, SPoint(0, (double)m_x1, (double)m_y2), m_StateShow);
	pDC->SelectObject(pOldPen); NewPen.DeleteObject();
	if (m_bDrawingShape && m_LButtonDown) {
		int nOldROP2 = pDC->SetROP2(R2_NOTXORPEN);
		CPen previewPen(PS_DOT, 1, RGB(0, 0, 0));
		CBrush* pNullBrush = (CBrush*)CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
		CPen* pOldPreviewPen = pDC->SelectObject(&previewPen); CBrush* pOldPreviewBrush = pDC->SelectObject(pNullBrush);
		CPoint ptStart = m_ptShapeStart; CPoint ptEnd = m_CurrentMousePos;
		switch (m_eShapeType) {
		case ShapeType::RECTANGLE: pDC->Rectangle(ptStart.x, ptStart.y, ptEnd.x, ptEnd.y); break;
		case ShapeType::SQUARE: {
			int size = max(abs(ptEnd.x - ptStart.x), abs(ptEnd.y - ptStart.y));
			int x2 = ptStart.x + (ptEnd.x > ptStart.x ? size : -size); int y2 = ptStart.y + (ptEnd.y > ptStart.y ? size : -size);
			pDC->Rectangle(ptStart.x, ptStart.y, x2, y2); break;
		}
		case ShapeType::CIRCLE: {
			double r = sqrt(pow(ptEnd.x - ptStart.x, 2) + pow(ptEnd.y - ptStart.y, 2));
			pDC->Ellipse((int)(ptStart.x - r), (int)(ptStart.y - r), (int)(ptStart.x + r), (int)(ptStart.y + r)); break;
		}
		default: break;
		}
		pDC->SelectObject(pOldPreviewBrush); pDC->SelectObject(pOldPreviewPen); pDC->SetROP2(nOldROP2);
	}
	CPen selectionPen(PS_SOLID, 2, RGB(255, 0, 0)); CBrush selectionBrush(RGB(255, 0, 0));
	pOldPen = pDC->SelectObject(&selectionPen); pOldBrush = pDC->SelectObject(&selectionBrush);
	for (INT_PTR i = 0; i < m_SelectedPoints.GetSize(); i++) {
		SPoint* pPoint = m_SelectedPoints.GetAt(i);
		if (pPoint) {
			SPoint screenPoint = *pPoint; SPoint::xy2XY(screenPoint, SPoint(0, (double)m_x1, (double)m_y2), m_StateShow);
			pDC->Ellipse((int)(screenPoint.m_x + m_StateShow.m_dx) - 4, (int)(screenPoint.m_y + m_StateShow.m_dy) - 4,
				(int)(screenPoint.m_x + m_StateShow.m_dx) + 4, (int)(screenPoint.m_y + m_StateShow.m_dy) + 4);
		}
	}
	pDC->SelectObject(pOldPen); pDC->SelectObject(pOldBrush);
}
HCURSOR CDRAWDlg::OnQueryDragIcon() { return static_cast<HCURSOR>(m_hIcon); }
void CDRAWDlg::OnRButtonDown(UINT nFlags, CPoint point) { m_Data.EndLine(); Invalidate(); CDialogEx::OnRButtonDown(nFlags, point); }
void CDRAWDlg::OnBnClickedOpen() {
	CFileDialog dlg(TRUE, _T("plt"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("HPGL Plotter files (*.PLT)|*.plt|All Files (*.*)|*.*||"));
	if (dlg.DoModal() == IDOK) { m_csFilePath = dlg.GetPathName(); m_Data.ReadPlt(m_csFilePath); GetDlgItem(IDC_SAVE)->EnableWindow(true); Invalidate(); }
}
void CDRAWDlg::OnBnClickedSave() { m_Data.WritePlt(m_csFilePath); AfxMessageBox(_T("文件保存成功!")); }
void CDRAWDlg::OnBnClickedSaveAs() {
	CFileDialog dlg(FALSE, _T("plt"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("HPGL Plotter files (*.plt)|*.plt|All Files (*.*)|*.*||"));
	if (dlg.DoModal() == IDOK) { m_csFilePath = dlg.GetPathName(); m_Data.WritePlt(m_csFilePath); AfxMessageBox(_T("文件保存成功!")); CClientDC dc(this); ShowStatus(&dc); }
}
void CDRAWDlg::OnMouseMove(UINT nFlags, CPoint point) {
	m_CurrentMousePos = point;
	if (m_LButtonDown) {
		if (m_bDraw) {
			if (m_eShapeType == ShapeType::FREE_DRAW) {
				if (point.x > m_x1 && point.x < m_x2 && point.y > m_y1 && point.y < m_y2) {
					SPoint worldPoint = { 0, (double)point.x, (double)point.y };
					worldPoint.m_x -= m_StateShow.m_dx; worldPoint.m_y -= m_StateShow.m_dy;
					SPoint::XY2xy(worldPoint, SPoint(0, (double)m_x1, (double)m_y2), m_StateShow);
					m_Data.AddPointToLine(worldPoint); Invalidate();
				}
			}
			else { Invalidate(); }
		}
		else {
			if (m_pMovingPoint) {
				SPoint worldPoint = { 0, (double)point.x, (double)point.y };
				worldPoint.m_x -= m_StateShow.m_dx; worldPoint.m_y -= m_StateShow.m_dy;
				SPoint::XY2xy(worldPoint, SPoint(0, (double)m_x1, (double)m_y2), m_StateShow);
				m_pMovingPoint->m_x = worldPoint.m_x; m_pMovingPoint->m_y = worldPoint.m_y; Invalidate();
			}
			else if (m_pMovingLine) {
				double dx = (point.x - m_ptDragStart.x) / m_StateShow.m_r;
				double dy = (point.y - m_ptDragStart.y) / m_StateShow.m_r;
				m_ptDragStart = point;
				for (int i = 0; i < m_pMovingLine->GetNum(); i++) {
					SPoint& pt = m_pMovingLine->GetPointForUpdate(i); pt.m_x += dx; pt.m_y -= dy;
				}
				Invalidate();
			}
		}
	}
	CDialogEx::OnMouseMove(nFlags, point);
}
void CDRAWDlg::OnLButtonDown(UINT nFlags, CPoint point) {
	if (point.x < m_x1 || point.x > m_x2 || point.y < m_y1 || point.y > m_y2) return;
	m_LButtonDown = TRUE;
	SPoint worldPoint = { 0, (double)point.x, (double)point.y };
	worldPoint.m_x -= m_StateShow.m_dx; worldPoint.m_y -= m_StateShow.m_dy;
	SPoint::XY2xy(worldPoint, SPoint(0, (double)m_x1, (double)m_y2), m_StateShow);
	if (m_bDraw) {
		if (m_eShapeType == ShapeType::FREE_DRAW) { m_Data.StartNewLine(worldPoint); }
		else { m_bDrawingShape = true; m_ptShapeStart = point; }
	}
	else {
		m_ptDragStart = point;
		if (m_eMoveMode == MoveMode::MovePoint) { m_pMovingPoint = m_Data.FindPoint(worldPoint, 5 / m_StateShow.m_r); }
		else if (m_eMoveMode == MoveMode::MoveLine) { m_pMovingLine = m_Data.FindLine(worldPoint, 5 / m_StateShow.m_r); }
		else {
			m_SelectedPoints.RemoveAll();
			if (SPoint* pSelectedPoint = m_Data.FindPoint(worldPoint, 5 / m_StateShow.m_r)) { m_SelectedPoints.Add(pSelectedPoint); }
			else if (CLine* pSelectedLine = m_Data.FindLine(worldPoint, 5 / m_StateShow.m_r)) { m_Data.SetCurrent(worldPoint); }
		}
	}
	Invalidate(); CDialogEx::OnLButtonDown(nFlags, point);
}
void CDRAWDlg::OnLButtonUp(UINT nFlags, CPoint point) {
	if (!m_LButtonDown) return;
	m_LButtonDown = FALSE;
	if (m_bDraw) {
		if (m_bDrawingShape) {
			m_bDrawingShape = false;
			SPoint spStart = { 0, (double)m_ptShapeStart.x, (double)m_ptShapeStart.y };
			spStart.m_x -= m_StateShow.m_dx; spStart.m_y -= m_StateShow.m_dy;
			SPoint::XY2xy(spStart, SPoint(0, (double)m_x1, (double)m_y2), m_StateShow);
			SPoint spEnd = { 0, (double)point.x, (double)point.y };
			spEnd.m_x -= m_StateShow.m_dx; spEnd.m_y -= m_StateShow.m_dy;
			SPoint::XY2xy(spEnd, SPoint(0, (double)m_x1, (double)m_y2), m_StateShow);
			m_Data.StartNewLine(spStart);
			switch (m_eShapeType) {
			case ShapeType::RECTANGLE:
				m_Data.AddPointToLine(SPoint(0, spEnd.m_x, spStart.m_y)); m_Data.AddPointToLine(spEnd);
				m_Data.AddPointToLine(SPoint(0, spStart.m_x, spEnd.m_y)); m_Data.AddPointToLine(spStart);
				break;
			case ShapeType::SQUARE: {
				double size = max(abs(spEnd.m_x - spStart.m_x), abs(spEnd.m_y - spStart.m_y));
				double x2 = spStart.m_x + (spEnd.m_x > spStart.m_x ? size : -size); double y2 = spStart.m_y + (spEnd.m_y > spStart.m_y ? size : -size);
				m_Data.AddPointToLine(SPoint(0, x2, spStart.m_y)); m_Data.AddPointToLine(SPoint(0, x2, y2));
				m_Data.AddPointToLine(SPoint(0, spStart.m_x, y2)); m_Data.AddPointToLine(spStart);
				break;
			}
			case ShapeType::CIRCLE: {
				double r = sqrt(pow(spEnd.m_x - spStart.m_x, 2) + pow(spEnd.m_y - spStart.m_y, 2));
				const int numPoints = 36;
				for (int i = 0; i <= numPoints; i++) {
					double angle = 2 * 3.1415926535 / numPoints * i;
					m_Data.AddPointToLine(SPoint(0, spStart.m_x + r * cos(angle), spStart.m_y + r * sin(angle)));
				}
				break;
			}
			}
			m_Data.EndLine();
		}
	}
	else { m_pMovingPoint = nullptr; m_pMovingLine = nullptr; m_eMoveMode = MoveMode::None; }
	Invalidate(); CDialogEx::OnLButtonUp(nFlags, point);
}
void CDRAWDlg::OnBnClickedNew() { m_Data.Clear(); Invalidate(); }
void CDRAWDlg::OnBnClickedZoomin() { m_StateShow.m_r *= 1.2; Invalidate(); }
void CDRAWDlg::OnBnClickedZoomout() { m_StateShow.m_r *= 0.8; Invalidate(); }
void CDRAWDlg::OnBnClickedZoomdef() { m_StateShow.m_r = 1.0; Invalidate(); }
void CDRAWDlg::OnBnClickedUp() { m_StateShow.m_dy += (int)(100 / m_StateShow.m_r); Invalidate(); }
void CDRAWDlg::OnBnClickedDown() { m_StateShow.m_dy -= (int)(100 / m_StateShow.m_r); Invalidate(); }
void CDRAWDlg::OnBnClickedMl() { m_StateShow.m_dx -= (int)(100 / m_StateShow.m_r); Invalidate(); }
void CDRAWDlg::OnBnClickedMr() { m_StateShow.m_dx += (int)(100 / m_StateShow.m_r); Invalidate(); }
void CDRAWDlg::ShowStatus(CDC* pDC) {
	CString s; s.Format(_T("坐标: (%4d, %4d)"), m_CurrentMousePos.x, m_CurrentMousePos.y);
	pDC->TextOut(20, m_h - 40, s);
	s.Format(_T("倍率: %5.2f"), m_StateShow.m_r);
	pDC->TextOut(200, m_h - 40, s);
}
void CDRAWDlg::OnBnClickedViewPoint() { m_StateShow.m_bViewPoint = true; Invalidate(); }
void CDRAWDlg::OnBnClickedHidePoint() { m_StateShow.m_bViewPoint = false; Invalidate(); }
void CDRAWDlg::OnBnClickedDraw() { m_bDraw = true; m_eMoveMode = MoveMode::None; }
void CDRAWDlg::OnBnClickedSel() { m_bDraw = false; m_eMoveMode = MoveMode::None; m_SelectedPoints.RemoveAll(); Invalidate(); }
void CDRAWDlg::OnBnClickedDelPoint() {
	if (m_SelectedPoints.GetSize() > 0) {
		for (INT_PTR i = 0; i < m_SelectedPoints.GetSize(); i++) {
			if (SPoint* pPointToDelete = m_SelectedPoints.GetAt(i)) { m_Data.DeletePointAt(*pPointToDelete); }
		}
		m_SelectedPoints.RemoveAll(); Invalidate();
	}
}
void CDRAWDlg::OnBnClickedDelLine() { m_Data.DelLine(); Invalidate(); }
void CDRAWDlg::OnBnClickedMoveLine() { m_bDraw = false; m_eMoveMode = MoveMode::MoveLine; }
void CDRAWDlg::OnBnClickedMovePoint() { m_bDraw = false; m_eMoveMode = MoveMode::MovePoint; }
void CDRAWDlg::OnBnClickedBtnLineColor() {
	m_ColorDlg.m_cc.rgbResult = m_ColorLine;
	if (m_ColorDlg.DoModal() == IDOK) { m_ColorLine = m_ColorDlg.GetColor(); UpdateButtonColor(IDC_PEN, m_ColorLine); Invalidate(); }
}
void CDRAWDlg::OnBnClickedBtnPointColor() {
	m_ColorDlg.m_cc.rgbResult = m_ColorPoint;
	if (m_ColorDlg.DoModal() == IDOK) { m_ColorPoint = m_ColorDlg.GetColor(); UpdateButtonColor(IDC_POINT, m_ColorPoint); Invalidate(); }
}
void CDRAWDlg::OnBnClickedBtnBgColor() {
	m_ColorDlg.m_cc.rgbResult = m_ColorBg;
	if (m_ColorDlg.DoModal() == IDOK) { m_ColorBg = m_ColorDlg.GetColor(); UpdateButtonColor(IDC_BG, m_ColorBg); Invalidate(); }
}
void CDRAWDlg::UpdateButtonColor(UINT nButtonID, COLORREF color) {
	if (auto pButton = GetDlgItem(nButtonID)) {
		CClientDC dc(pButton); CRect rect; pButton->GetClientRect(&rect);
		CBrush brush(color); dc.FillRect(rect, &brush); dc.DrawEdge(rect, EDGE_RAISED, BF_RECT);
	}
}
void CDRAWDlg::OnCbnSelchangePenWidthCombo() {
	int nSel = m_cboPenWidth.GetCurSel();
	if (nSel != CB_ERR) { CString csWidth; m_cboPenWidth.GetLBText(nSel, csWidth); m_nPenWidth = _ttoi(csWidth); Invalidate(); }
}
void CDRAWDlg::OnCbnSelchangeShapeCombo() {
	int nSel = m_cboShape.GetCurSel();
	if (nSel != CB_ERR) { m_bDrawingShape = false; m_LButtonDown = FALSE; m_eShapeType = (ShapeType)nSel; m_bDraw = true; Invalidate(); }
}
void CDRAWDlg::SetButtonTextFromResource(UINT buttonId, UINT stringId) {
	CString buttonText;
	if (buttonText.LoadString(stringId)) { if (auto pButton = GetDlgItem(buttonId)) { pButton->SetWindowText(buttonText); } }
}
void CDRAWDlg::InitButtonLabels() {
	SetButtonTextFromResource(IDC_NEW, IDS_STRING_NEW); SetButtonTextFromResource(IDC_OPEN, IDS_STRING_OPEN);
	SetButtonTextFromResource(IDC_SAVE, IDS_STRING_SAVE); SetButtonTextFromResource(IDC_SAVE_AS, IDS_STRING_SAVE_AS);
	SetButtonTextFromResource(IDC_BG, IDS_STRING_BG); SetButtonTextFromResource(IDC_PEN, IDS_STRING_PEN);
	SetButtonTextFromResource(IDC_POINT, IDS_STRING_POINT); SetButtonTextFromResource(IDC_DRAW, IDS_STRING_DRAW);
	SetButtonTextFromResource(IDC_SEL, IDS_STRING_SEL); SetButtonTextFromResource(IDC_MOVE, IDS_STRING_MOVE);
	SetButtonTextFromResource(IDC_MOVE_LINE, IDS_STRING_MOVE_LINE); SetButtonTextFromResource(IDC_MOVE_POINT, IDS_STRING_MOVE_POINT);
	SetButtonTextFromResource(IDC_DEL_POINT, IDS_STRING_DEL_POINT); SetButtonTextFromResource(DC_DEL_LINE, IDS_STRING_DEL_LINE);
	SetButtonTextFromResource(IDC_UNDO, IDS_STRING_UNDO); SetButtonTextFromResource(IDC_REDO, IDS_STRING_REDO);
	SetButtonTextFromResource(IDC_VIEW_POINT, IDS_STRING_VIEW_POINT); SetButtonTextFromResource(IDC_HIDE_POINT, IDS_STRING_HIDE_POINT);
	SetButtonTextFromResource(IDC_ZOOMIN, IDS_STRING_ZOOMIN); SetButtonTextFromResource(IDC_ZOOMOUT, IDS_STRING_ZOOMOUT);
	SetButtonTextFromResource(IDC_ZOOMDEF, IDS_STRING_ZOOMDEF); SetButtonTextFromResource(IDC_MUP, IDS_STRING_MUP);
	SetButtonTextFromResource(IDC_MDOWN, IDS_STRING_MDOWN); SetButtonTextFromResource(IDC_ML, IDS_STRING_ML);
	SetButtonTextFromResource(IDC_MR, IDS_STRING_MR);
	CString strLabel; strLabel.LoadString(IDS_STRING_PEN_WIDTH); SetDlgItemText(IDC_STATIC_PEN_WIDTH_LABEL, strLabel);
	strLabel.LoadString(IDS_STRING_SHAPE); SetDlgItemText(IDC_STATIC_SHAPE_LABEL, strLabel);
}
void CDRAWDlg::AddStringFromResource(CComboBox& comboBox, UINT stringId) {
	CString str; if (str.LoadString(stringId)) { comboBox.AddString(str); }
}
