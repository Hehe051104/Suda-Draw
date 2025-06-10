#include "pch.h"
#include "CData.h"
#include <algorithm>

CData::CData() : m_pCurLine(nullptr) {}

CData::~CData() {
    Clear();
}

void CData::Clear() {
    m_lines.clear();
    m_pCurLine = nullptr;
}

CLine* CData::GetLine(int nIndex) {
    if (nIndex >= 0 && nIndex < (int)m_lines.size()) {
        return &m_lines[nIndex];
    }
    return nullptr;
}

void CData::AddLine(const CLine& line) {
    m_lines.push_back(line);
}

void CData::StartNewLine(const SPoint& p0) {
    m_lines.emplace_back(m_lines.size(), const_cast<SPoint&>(p0));
    m_pCurLine = &m_lines.back();
}

void CData::AddPointToLine(const SPoint& p0, int lineIndex) {
    if (m_pCurLine) {
        m_pCurLine->AddPoint(p0);
    }
}

bool CData::SetCurrent(SPoint p0, int d) {
    for (auto& line : m_lines) {
        if (line.SetCurrent(p0, d)) {
            m_pCurLine = &line;
            return true;
        }
    }
    m_pCurLine = nullptr;
    return false;
}

bool CData::DelLine() {
    if (!m_pCurLine) return false;

    auto it = std::remove_if(m_lines.begin(), m_lines.end(),
        [this](const CLine& line) {
            return &line == m_pCurLine;
        });

    if (it != m_lines.end()) {
        m_lines.erase(it, m_lines.end());
        m_pCurLine = nullptr;
        return true;
    }
    return false;
}

bool CData::DeletePointAt(const SPoint& p0) {
    for (auto& line : m_lines) {
        if (line.DelPoint(p0)) {
            return true;
        }
    }
    return false;
}

SPoint* CData::FindPoint(SPoint p0, int d) {
    double min_dist_sq = (double)d * d;
    SPoint* found_point = nullptr;

    for (auto& line : m_lines) {
        for (int i = 0; i < line.GetNum(); ++i) {
            SPoint& pt = line.GetPointForUpdate(i);
            double dist_sq = pow(pt.m_x - p0.m_x, 2) + pow(pt.m_y - p0.m_y, 2);
            if (dist_sq < min_dist_sq) {
                min_dist_sq = dist_sq;
                found_point = &pt;
            }
        }
    }
    return found_point;
}

CLine* CData::FindLine(SPoint p0, int d) {
    for (auto& line : m_lines) {
        if (line.IsOnLine(p0, d)) {
            return &line;
        }
    }
    return nullptr;
}

void CData::Show(CDC* pDC, SPoint p0, SShowState state) const {
    for (const auto& line : m_lines) {
        line.ShowLine(pDC, p0, state);
        line.ShowPoint(pDC, p0, state);
    }
}

void CData::ShowCur(CDC* pDC, SPoint p0, SShowState state) const {
    if (m_pCurLine) {
        m_pCurLine->ShowLine(pDC, p0, state);
        m_pCurLine->ShowPoint(pDC, p0, state);
        m_pCurLine->ShowCurPoint(pDC, p0, state);
    }
}

bool CData::ReadPlt(const char* szFname) {
    FILE* plt;
    if (fopen_s(&plt, szFname, "rt")) return false;

    Clear();
    char cBuf[256];
    int x, y;

    while (fgets(cBuf, sizeof(cBuf), plt)) {
        if (strstr(cBuf, "PU")) {
            if (sscanf_s(cBuf, "PU%d,%d", &x, &y) == 2 || sscanf_s(cBuf, "PU %d %d", &x, &y) == 2) {
                StartNewLine(SPoint(0, x / 40.0, y / 40.0));
            }
        }
        else if (strstr(cBuf, "PD")) {
            if (sscanf_s(cBuf, "PD%d,%d", &x, &y) == 2 || sscanf_s(cBuf, "PD %d %d", &x, &y) == 2) {
                AddPointToLine(SPoint(0, x / 40.0, y / 40.0));
            }
        }
    }
    EndLine();
    fclose(plt);
    return true;
}

bool CData::WritePlt(const char* szFname) const {
    FILE* fp;
    if (fopen_s(&fp, szFname, "wt")) return false;

    fprintf(fp, "IN;SP1;\n");
    for (const auto& line : m_lines) {
        if (line.GetNum() == 0) continue;
        const auto& first_point = line.GetPoint(0);
        fprintf(fp, "PU%d,%d;\n", (int)(first_point.m_x * 40), (int)(first_point.m_y * 40));
        for (int j = 1; j < line.GetNum(); ++j) {
            const auto& point = line.GetPoint(j);
            fprintf(fp, "PD%d,%d;\n", (int)(point.m_x * 40), (int)(point.m_y * 40));
        }
    }
    fprintf(fp, "SP0;");
    fclose(fp);
    return true;
}
