// DRAWDlg.h: 头文件
//

#pragma once
#include "CData.h"
#include <afxdlgs.h> // 包含颜色对话框头文件
#include <cmath>	 // 用于计算距离
#include <afxwin.h>  // 包含MFC基础类
#include <afxcmn.h>  // 包含组合框控件类
#include <afxext.h>  // MFC 扩展
#include <vector>


// CDRAWDlg 对话框

class CDRAWDlg : public CDialogEx
{
// 构造
public:
	CDRAWDlg(CWnd* pParent = nullptr);	// 标准构造函数
	
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DRAW_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	void InitButtonLabels();
	void SetButtonTextFromResource(UINT buttonId, UINT stringId);

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	// 核心数据和状态
	int m_w;                     //对话框窗口宽度
	int m_h;                     //对话框窗口高度
	int m_x1;                    //绘图区域左上角物理坐标x
	int m_y1;                    //绘图区域左上角物理坐标y
	int m_x2;                    //绘图区域右下角物理坐标x
	int m_y2;                    //绘图区域右下角物理坐标y
	SShowState m_StateShow;      //当前显示状态
	bool m_bDraw;                //true 绘图, false 选择
	CData m_Data;                //数据处理对象
	CString m_csFilePath;        //当前文件路径
	CFont m_Font;                //当前字体

	// 颜色相关
	COLORREF m_ColorLine;        //画线颜色
	COLORREF m_ColorPoint;       //画点颜色
	COLORREF m_ColorLineCur;     //当前线颜色
	COLORREF m_ColorPointCur;    //当前点颜色
	COLORREF m_ColorBg;           // 背景颜色
	COLORREF m_ColorBgCur;    // 当前背景颜色
	CColorDialog m_ColorDlg;     // 颜色对话框对象

	// 绘图状态变量
	CPoint m_pt;
	BOOL m_LButtonDown;
	CLine* m_pLineCur;
	int m_nPenWidth; // 笔画宽度（像素）
	CPoint m_CurrentMousePos;

	// 形状绘制相关
	enum class ShapeType {
		FREE_DRAW,    // 自由绘制
		RECTANGLE,    // 矩形（包括正方形）
		SQUARE,
		CIRCLE,        // 圆形
		TRIANGLE
	};
	
	enum class MoveMode {
		None,
		MovePoint,
		MoveLine
	};

	CPoint m_ptShapeStart;
	bool m_bDrawingShape;
	ShapeType m_eShapeType;
	MoveMode m_eMoveMode;

	// 控件变量
	CComboBox m_cboShape;        // 形状选择组合框
	CComboBox m_cboPenWidth;

	// 选择功能相关
	CArray<CPoint*, CPoint*> m_SelectedPoints;
	SPoint* m_pMovingPoint;
	CLine* m_pMovingLine;
	CPoint m_ptDragStart;

public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);    //鼠标左键弹起消息
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);  //鼠标右键按下消息
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);    //鼠标移动消息
	afx_msg void OnBnClickedNew();                           //新建图形
	afx_msg void OnBnClickedOpen();                          //打开文件
	afx_msg void OnBnClickedSave();                          //保存文件
	afx_msg void OnBnClickedSaveAs();                        //文件另存为
	afx_msg void OnBnClickedZoomin();                        //显示放大
	afx_msg void OnBnClickedZoomout();                       //显示缩小
	afx_msg void OnBnClickedZoomdef();                       //显示1:1
	afx_msg void OnBnClickedUp();                            //显示上移
	afx_msg void OnBnClickedDown();                          //显示下移
	afx_msg void OnBnClickedMl();                            //显示左移
	afx_msg void OnBnClickedMr();                            //显示右移
	afx_msg void OnBnClickedViewPoint();                     //显示结点
	afx_msg void OnBnClickedHidePoint();                     //隐藏结点
	afx_msg void OnBnClickedDraw();                          //绘图状态
	afx_msg void OnBnClickedSel();                           //选择状态
	afx_msg void OnBnClickedDelPoint();                      //删除图结
	afx_msg void OnBnClickedDelLine();                       //删除线
	void ShowStatus(CDC* pDC);                               //显示状态信息
	void Draw(CDC* pDC);

	// 新增
	afx_msg void OnBnClickedBtnLineColor();  // 线条颜色按钮事件
	afx_msg void OnBnClickedBtnPointColor();  // 节点颜色按钮事件
	afx_msg void OnBnClickedBtnBgColor();    // 背景颜色按钮事件
	void UpdateButtonColor(UINT nButtonID, COLORREF color); // 更新按钮显示

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point); // 声明消息响应函数

	afx_msg void OnCbnSelchangePenWidthCombo(); // 笔画粗细

	afx_msg void OnCbnSelchangeShapeCombo(); // 选择形状

	afx_msg void OnBnClickedMoveLine();
	afx_msg void OnBnClickedMovePoint();
};
