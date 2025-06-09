#include "pch.h"
#include "CLine.h"

CLine::CLine(int sn, SPoint& p0) {
    m_pPoint = new SPoint[MAX_POINT];
    m_pPoint[0] = p0;
    m_pPoint[0].m_sn = 1;
    m_sn = sn;
    m_nPoint = 1;
    m_pCurPoint = NULL;
}

CLine::~CLine() {
    delete[] m_pPoint;
}

int CLine::AddPoint(SPoint& p0) {
    m_pPoint[m_nPoint] = p0;
    m_pPoint[m_nPoint].m_sn = m_nPoint - 1;
    m_pCurPoint = &m_pPoint[m_nPoint];
    m_nPoint++;
    return m_nPoint;
}

bool CLine::DelPoint() {
    if (!m_pCurPoint)
        return false;
    for (int i = 0; i < m_nPoint; i++) {
        if (m_pCurPoint == &m_pPoint[i]) {
            m_pCurPoint = NULL;
            for (int j = i; j < m_nPoint; j++) {
                m_pPoint[j] = m_pPoint[j + 1];
            }
            m_nPoint--;
            return true;
        }
    }
    return false;
}

bool CLine::SetCurrent(SPoint p0, int d) {
    for (int i = 0; i < m_nPoint; i++) {
        if (m_pPoint[i].m_x >= p0.m_x - d && m_pPoint[i].m_y > p0.m_y - d
            && m_pPoint[i].m_x <= p0.m_x + d && m_pPoint[i].m_y <= p0.m_y + d) {
            m_pCurPoint = &m_pPoint[i];
            return true;
        }
    }
    // 若循环结束未找到，返回 false 表示未找到
    return false;
}


void CLine::ShowLine(CDC* pDC, SPoint p0, SShowState state) {
    if (m_nPoint >= 2) {
        SPoint tPoint = m_pPoint[0];
        SPoint::xy2XY(tPoint, p0, state);
        pDC->MoveTo(tPoint.m_x + state.m_dx, tPoint.m_y - state.m_dy);
        for (int i = 1; i < m_nPoint; i++) {
            tPoint = m_pPoint[i];
            SPoint::xy2XY(tPoint, p0, state);
            pDC->LineTo(tPoint.m_x + state.m_dx, tPoint.m_y - state.m_dy);
        }
    }
}


void CLine::ShowPoint(CDC* pDC, SPoint p0, SShowState state, int d) {
    SPoint tPoint;
    for (int i = 0; i < m_nPoint; i++) {
        tPoint = m_pPoint[i];
        SPoint::xy2XY(tPoint, p0, state);
        pDC->Rectangle(tPoint.m_x + state.m_dx - d, tPoint.m_y - state.m_dy - d,
            tPoint.m_x + state.m_dx + d, tPoint.m_y - state.m_dy + d);
    }
}

void CLine::ShowCurPoint(CDC* pDC, SPoint p0, SShowState state, int d) {
    if (m_pCurPoint) {
        SPoint tPoint;
        tPoint = *m_pCurPoint;
        SPoint::xy2XY(tPoint, p0, state);
        pDC->Rectangle(tPoint.m_x + state.m_dx - d, tPoint.m_y - state.m_dy - d,
            tPoint.m_x + state.m_dx + d, tPoint.m_y - state.m_dy + d);
    }
}


