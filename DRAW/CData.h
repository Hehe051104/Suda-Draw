#pragma once
#include "CLine.h"
#define MAX_LINE 1024  //一个图形最多有1024个曲线

class CData {
    CLine* m_pLine[MAX_LINE];
    int m_nLine;
    CLine* m_pCurLine;  //当前线
public:
    CData();
    ~CData();
    int GetNum() { return m_nLine; }
    int AddLine(SPoint p0);  //添加一条新线返回该线序号
    int AddPoint(SPoint p0, int sn = 0);  //为指定线添加一个点，sn=0表示默认当前线
    bool SetCurrent(SPoint p0, int d = 3);  //选择当前线、点

    void EndLine() { m_pCurLine = nullptr; }
    bool DelLine();
    bool DelPoint();

    void ShowLine(CDC* pDC, SPoint p0, SShowState state);
    void ShowCurLine(CDC* pDC, SPoint p0, SShowState state);

    void ShowPoint(CDC* pDC, SPoint p0, SShowState state, int d = 2);
    void ShowCurPoint(CDC* pDC, SPoint p0, SShowState state, int d = 3);

    bool ReadPlt(const char* szFname);  //从PLT文件读入图形
    bool WritePlt(const char* szFname);  //将图形数据写入PLT文件
    void Clear();  //清空数据
};

