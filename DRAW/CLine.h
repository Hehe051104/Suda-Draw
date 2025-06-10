#pragma once
#include <vector>
#include <afx.h>

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
    SPoint() : m_sn(0), m_x(0), m_y(0) {}
    SPoint(int sn, double x, double y) : m_sn(sn), m_x(x), m_y(y) {}

    static void XY2xy(SPoint& point, SPoint p0, SShowState state);
    static void xy2XY(SPoint& point, SPoint p0, SShowState state);
};

class CLine
{
    int m_sn;
    std::vector<SPoint> m_points;
    SPoint* m_pCurPoint;

public:
    CLine();
    CLine(int sn, SPoint& p0);
    ~CLine();

    int GetNum() const { return (int)m_points.size(); }
    SPoint GetPoint(int i) const { return m_points[i]; }
    SPoint& GetPointForUpdate(int i) { return m_points[i]; }
    const std::vector<SPoint>& GetPoints() const { return m_points; }

    void AddPoint(const SPoint& p0);
    bool DelPoint(const SPoint& p0);
    bool SetCurrent(SPoint p0, int d = 3);
    bool IsOnLine(SPoint p0, int d = 3) const;
    void Clear();

    void ShowLine(CDC* pDC, SPoint p0, SShowState state) const;
    void ShowPoint(CDC* pDC, SPoint p0, SShowState state, int d = 2) const;
    void ShowCurPoint(CDC* pDC, SPoint p0, SShowState state, int d = 3) const;
};
