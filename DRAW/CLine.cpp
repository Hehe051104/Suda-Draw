#include "pch.h"
#include "CLine.h"
#include <cmath>
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// SPoint
void SPoint::XY2xy(SPoint& point, SPoint p0, SShowState state) {
    point.m_x = (point.m_x - p0.m_x) / state.m_r;
    point.m_y = (p0.m_y - point.m_y) / state.m_r; // Y is inverted
}

void SPoint::xy2XY(SPoint& point, SPoint p0, SShowState state) {
    point.m_x = point.m_x * state.m_r + p0.m_x;
    point.m_y = p0.m_y - point.m_y * state.m_r; // Y is inverted
}

// CLine
CLine::CLine() : m_sn(0), m_pCurPoint(nullptr) {}

CLine::CLine(int sn, SPoint& p0) : m_sn(sn), m_pCurPoint(nullptr) {
    m_points.push_back(p0);
}

CLine::~CLine() {
}

void CLine::AddPoint(const SPoint& p0) {
    m_points.push_back(p0);
}

bool CLine::DelPoint(const SPoint& p0) {
    auto it = std::remove_if(m_points.begin(), m_points.end(), [&](const SPoint& pt) {
        return pt.m_x == p0.m_x && pt.m_y == p0.m_y;
        });

    if (it != m_points.end()) {
        m_points.erase(it, m_points.end());
        m_pCurPoint = nullptr;
        return true;
    }
    return false;
}

void CLine::Clear()
{
    m_points.clear();
    m_pCurPoint = nullptr;
}

bool CLine::SetCurrent(SPoint p0, int d) {
    for (size_t i = 0; i < m_points.size(); ++i) {
        double dist_sq = pow(m_points[i].m_x - p0.m_x, 2) + pow(m_points[i].m_y - p0.m_y, 2);
        if (dist_sq < pow(d, 2)) {
            m_pCurPoint = &m_points[i];
            return true;
        }
    }
    m_pCurPoint = nullptr;
    return false;
}

bool CLine::IsOnLine(SPoint p0, int d) const {
    if (m_points.size() < 2) return false;

    for (size_t i = 0; i < m_points.size() - 1; ++i) {
        double x1 = m_points[i].m_x, y1 = m_points[i].m_y;
        double x2 = m_points[i + 1].m_x, y2 = m_points[i + 1].m_y;

        double len_sq = pow(x2 - x1, 2) + pow(y2 - y1, 2);
        if (len_sq == 0.0) continue;

        double t = ((p0.m_x - x1) * (x2 - x1) + (p0.m_y - y1) * (y2 - y1)) / len_sq;
        t = max(0, min(1, t));

        double closest_x = x1 + t * (x2 - x1);
        double closest_y = y1 + t * (y2 - y1);

        double dist_sq = pow(p0.m_x - closest_x, 2) + pow(p0.m_y - closest_y, 2);

        if (dist_sq < pow(d, 2)) {
            return true;
        }
    }
    return false;
}

void CLine::ShowLine(CDC* pDC, SPoint p0, SShowState state) const {
    if (m_points.empty()) return;

    SPoint start_point = m_points[0];
    SPoint::xy2XY(start_point, p0, state);
    pDC->MoveTo((int)(start_point.m_x + state.m_dx), (int)(start_point.m_y + state.m_dy));

    for (size_t i = 1; i < m_points.size(); ++i) {
        SPoint end_point = m_points[i];
        SPoint::xy2XY(end_point, p0, state);
        pDC->LineTo((int)(end_point.m_x + state.m_dx), (int)(end_point.m_y + state.m_dy));
    }
}

void CLine::ShowPoint(CDC* pDC, SPoint p0, SShowState state, int d) const {
    if (!state.m_bViewPoint) return;
    for (const auto& pt_const : m_points) {
        SPoint pt = pt_const;
        SPoint::xy2XY(pt, p0, state);
        pDC->Rectangle((int)(pt.m_x + state.m_dx) - d, (int)(pt.m_y + state.m_dy) - d, (int)(pt.m_x + state.m_dx) + d, (int)(pt.m_y + state.m_dy) + d);
    }
}

void CLine::ShowCurPoint(CDC* pDC, SPoint p0, SShowState state, int d) const {
    if (m_pCurPoint) {
        SPoint pt = *m_pCurPoint;
        SPoint::xy2XY(pt, p0, state);
        pDC->Ellipse((int)(pt.m_x + state.m_dx) - d, (int)(pt.m_y + state.m_dy) - d, (int)(pt.m_x + state.m_dx) + d, (int)(pt.m_y + state.m_dy) + d);
    }
}
