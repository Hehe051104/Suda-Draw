#pragma once

struct SShowState {
    double m_r;
    int m_dx;
    int m_dy;
    bool m_bViewPoint;
};

struct SPoint {
    int m_sn;
    double m_x;
    double m_y;
    SPoint() {
        m_sn = 0;
        m_x = 0;
        m_y = 0;
    }
    SPoint(int sn, double x, double y) {
        m_sn = sn;
        m_x = x;
        m_y = y;
    }
    static void XY2xy(SPoint& point, SPoint p0, SShowState state);
    static void xy2XY(SPoint& point, SPoint p0, SShowState state);
};

#define MAX_POINT 8192
class CLine
{
    int m_sn;
    SPoint* m_pPoint;
    int m_nPoint;
    SPoint* m_pCurPoint;

public:
    CLine(int sn, SPoint& p0);
    ~CLine();
    int GetNum() { return m_nPoint; }
    SPoint GetPoint(int i) { return m_pPoint[i]; }
    SPoint& GetPointForUpdate(int i) { return m_pPoint[i]; }

    int AddPoint(SPoint& p0);
    bool DelPoint();
    bool SetCurrent(SPoint p0, int d = 3);
    bool IsOnLine(SPoint p0, int d = 3);

    void ShowLine(CDC* pDC, SPoint p0, SShowState state);
    void ShowPoint(CDC* pDC, SPoint p0, SShowState state, int d = 2);
    void ShowCurPoint(CDC* pDC, SPoint p0, SShowState state, int d = 3);

};