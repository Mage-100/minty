#pragma once

#include "pch.h"

#include <cstdint>


typedef struct cell_t {
    uint32_t glyph_id;
    uint32_t fg_color;
    uint32_t bg_color;
    uint32_t flags;
} cell_t;

class CellGrid {
    int m_cols, m_rows;
    int m_start = 0;
    std::vector<cell_t> m_buffer;
    std::vector<cell_t> m_staging;

    void clearCell(int, int);
public:
    CellGrid(int, int);
    ~CellGrid();

    int rows() { return m_rows; };
    int cols() { return m_cols; };
    void resize(int, int);
    const cell_t* data();

    void scroll_up(int);
    int getStart() const {return m_start;};

    void printChar(int ,int , char );
    void eraseEntireScreen();
    void eraseLine(int, int, int);
};
