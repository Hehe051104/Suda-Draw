#pragma once
#include "CData.h"

// CDRAWDlg 对话框
class CDRAWDlg : public CDialogEx
{
	// 构造
public:
	CDRAWDlg(CWnd* pParent = nullptr);	// 标准构造函数
	~CDRAWDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DRAW_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	int m_w, m_h;
	int m_x1, m_y1, m_x2, m_y2;
	CData m_Data;
	SShowState m_StateShow;
	BOOL m_bDraw;
	CString m_csFilePath;
	CFont m_Font;
	CPoint m_CurrentMousePos;

	COLORREF m_ColorLine, m_ColorPoint, m_ColorBg;
	COLORREF m_ColorLineCur, m_ColorPointCur, m_ColorBgCur;
	CColorDialog m_ColorDlg;

	BOOL m_LButtonDown;
	CPoint m_pt, m_ptDragStart, m_ptShapeStart;
	CLine* m_pLineCur;
	CComboBox m_cboPenWidth;
	CComboBox m_cboShape;
	int m_nPenWidth;

	enum class ShapeType { FREE_DRAW, RECTANGLE, SQUARE, CIRCLE } m_eShapeType;
	bool m_bDrawingShape;

	enum class MoveMode { None, MovePoint, MoveLine } m_eMoveMode;
	SPoint* m_pMovingPoint;
	CLine* m_pMovingLine;

	CPtrArray m_SelectedPoints;

	void Draw(CDC* pDC);
	void ShowStatus(CDC* pDC);
	void SetButtonTextFromResource(UINT buttonId, UINT stringId);
	void InitButtonLabels();
	void UpdateButtonColor(UINT nButtonID, COLORREF color);
	void AddStringFromResource(CComboBox& comboBox, UINT stringId);
	void UpdateUndoRedoButtons();

public:
	afx_msg void OnBnClickedNew();
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedSaveAs();
	afx_msg void OnBnClickedZoomin();
	afx_msg void OnBnClickedZoomout();
	afx_msg void OnBnClickedZoomdef();
	afx_msg void OnBnClickedUp();
	afx_msg void OnBnClickedDown();
	afx_msg void OnBnClickedMl();
	afx_msg void OnBnClickedMr();
	afx_msg void OnBnClickedViewPoint();
	afx_msg void OnBnClickedHidePoint();
	afx_msg void OnBnClickedDraw();
	afx_msg void OnBnClickedSel();
	afx_msg void OnBnClickedDelPoint();
	afx_msg void OnBnClickedDelLine();
	afx_msg void OnBnClickedMoveLine();
	afx_msg void OnBnClickedMovePoint();
	afx_msg void OnBnClickedUndo();
	afx_msg void OnBnClickedRedo();
	afx_msg void OnBnClickedBtnLineColor();
	afx_msg void OnBnClickedBtnPointColor();
	afx_msg void OnBnClickedBtnBgColor();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCbnSelchangePenWidthCombo();
	afx_msg void OnCbnSelchangeShapeCombo();
};