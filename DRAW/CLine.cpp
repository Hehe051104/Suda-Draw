#include "pch.h"
#include "CLine.h"
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// SPoint
void SPoint::XY2xy(SPoint& point, SPoint p0, SShowState state) {
    point.m_x = (point.m_x - p0.m_x) / state.m_r + state.m_dx;
    point.m_y = (point.m_y - p0.m_y) / state.m_r + state.m_dy;
}

void SPoint::xy2XY(SPoint& point, SPoint p0, SShowState state) {
    point.m_x = (point.m_x - state.m_dx) * state.m_r + p0.m_x;
    point.m_y = (point.m_y - state.m_dy) * state.m_r + p0.m_y;
}


// CLine
CLine::CLine(int sn, SPoint& p0) {
    m_sn = sn;
    m_pPoint = new SPoint[MAX_POINT];
    m_pPoint[0] = p0;
    m_nPoint = 1;
    m_pCurPoint = &m_pPoint[0];
}

CLine::~CLine() {
    delete[] m_pPoint;
}

int CLine::AddPoint(SPoint& p0) {
    if (m_nPoint < MAX_POINT) {
        m_pPoint[m_nPoint] = p0;
        m_nPoint++;
        return m_nPoint;
    }
    return 0;
}

bool CLine::DelPoint() {
    if (!m_pCurPoint) return false;
    if (m_nPoint <= 1) return false;

    for (int i = 0; i < m_nPoint; ++i) {
        if (&m_pPoint[i] == m_pCurPoint) {
            for (int j = i; j < m_nPoint - 1; ++j) {
                m_pPoint[j] = m_pPoint[j + 1];
            }
            m_nPoint--;
            m_pCurPoint = nullptr;
            return true;
        }
    }
    return false;
}

bool CLine::SetCurrent(SPoint p0, int d) {
    // We need to check against the screen coordinates, but p0 is in world coordinates.
    // This function can't do the conversion without more context (p0_origin, state).
    // For now, I'll assume p0 is close enough to world coordinates for picking.
    for (int i = 0; i < m_nPoint; ++i) {
        double dist_sq = pow(m_pPoint[i].m_x - p0.m_x, 2) + pow(m_pPoint[i].m_y - p0.m_y, 2);
        if (dist_sq < pow(d, 2)) {
            m_pCurPoint = &m_pPoint[i];
            return true;
        }
    }
    m_pCurPoint = nullptr;
    return false;
}

bool CLine::IsOnLine(SPoint p0, int d) {
    // This check should also be done in screen coordinates. Assuming world coordinates for now.
    for (int i = 0; i < m_nPoint - 1; ++i) {
        double x1 = m_pPoint[i].m_x, y1 = m_pPoint[i].m_y;
        double x2 = m_pPoint[i+1].m_x, y2 = m_pPoint[i+1].m_y;
        
        double len_sq = pow(x2 - x1, 2) + pow(y2 - y1, 2);
        if (len_sq == 0.0) continue; // It's a point, not a line segment

        double t = ((p0.m_x - x1) * (x2 - x1) + (p0.m_y - y1) * (y2 - y1)) / len_sq;
        t = max(0, min(1, t)); // clamp to segment
        
        double closest_x = x1 + t * (x2 - x1);
        double closest_y = y1 + t * (y2 - y1);

        double dist_sq = pow(p0.m_x - closest_x, 2) + pow(p0.m_y - closest_y, 2);

        if (dist_sq < pow(d, 2)) {
            return true;
        }
    }
    return false;
}


void CLine::ShowLine(CDC* pDC, SPoint p0, SShowState state) {
    if (m_nPoint <= 0) return;

    SPoint start_point = m_pPoint[0];
    SPoint::xy2XY(start_point, p0, state);
    pDC->MoveTo((int)start_point.m_x, (int)start_point.m_y);

    for (int i = 1; i < m_nPoint; ++i) {
        SPoint end_point = m_pPoint[i];
        SPoint::xy2XY(end_point, p0, state);
        pDC->LineTo((int)end_point.m_x, (int)end_point.m_y);
    }
}

void CLine::ShowPoint(CDC* pDC, SPoint p0, SShowState state, int d) {
    if (!state.m_bViewPoint) return;
    for (int i = 0; i < m_nPoint; ++i) {
        SPoint pt = m_pPoint[i];
        SPoint::xy2XY(pt, p0, state);
        pDC->Rectangle((int)pt.m_x - d, (int)pt.m_y - d, (int)pt.m_x + d, (int)pt.m_y + d);
    }
}

void CLine::ShowCurPoint(CDC* pDC, SPoint p0, SShowState state, int d) {
    if (m_pCurPoint) {
        SPoint pt = *m_pCurPoint;
        SPoint::xy2XY(pt, p0, state);
        pDC->Ellipse((int)pt.m_x - d, (int)pt.m_y - d, (int)pt.m_x + d, (int)pt.m_y + d);
    }
} 