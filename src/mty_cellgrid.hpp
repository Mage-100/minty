#pragma once

#include "pch.h"

#include <cstdint>


typedef struct cell_t {
    uint32_t glyph_id;
    uint32_t fg_color;
    uint32_t bg_color;
    uint32_t flags;
} cell_t;

typedef struct cell_style_t {
    uint32_t fg_color;
    uint32_t bg_color;
} cell_style_t;

class CellGrid {
    int m_cols, m_rows;
    int m_start = 0;
    std::vector<cell_t> m_buffer;
    std::vector<cell_t> m_staging;

    cell_style_t default_cell_style;
    uint32_t getFGColor() { return default_cell_style.fg_color; };
    uint32_t getBGColor() { return default_cell_style.bg_color; };


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
    void resetAllModes();
    void setForegroundColor(int);
    void setBackgroundColor(int);
};
