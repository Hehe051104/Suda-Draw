#pragma once
#include "CLine.h"
#include <vector>

class CData {
    std::vector<CLine> m_lines;
    std::vector<CLine> m_redoStack;
    CLine* m_pCurLine;

public:
    CData();
    ~CData();
    int GetNum() const { return (int)m_lines.size(); }
    CLine* GetLine(int nIndex);
    const std::vector<CLine>& GetLines() const { return m_lines; }

    void AddLine(const CLine& line);
    void AddPointToLine(const SPoint& p0, int lineIndex = -1);
    void StartNewLine(const SPoint& p0);

    bool SetCurrent(SPoint p0, int d = 3);
    void EndLine() { m_pCurLine = nullptr; }

    bool DelLine();
    bool DeletePointAt(const SPoint& p0);

    SPoint* FindPoint(SPoint p0, int d);
    CLine* FindLine(SPoint p0, int d);

    void Undo();
    void Redo();
    bool CanUndo() const;
    bool CanRedo() const;

    void Show(CDC* pDC, SPoint p0, SShowState state) const;
    void ShowCur(CDC* pDC, SPoint p0, SShowState state) const;

    bool ReadPlt(const char* szFname);
    bool WritePlt(const char* szFname) const;
    void Clear();
};
