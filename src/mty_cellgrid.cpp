#include "mty_cellgrid.hpp"
#include <algorithm>
#include <cstring>

CellGrid::CellGrid(int cols, int rows) : m_cols(cols), m_rows(rows) {
    m_buffer = std::vector<cell_t>(m_cols * m_rows);
    m_staging.reserve(m_cols * m_rows);

    for (int y = 0; y < m_rows; y++) {
        for (int x = 0; x < m_cols; x++) {
            int index = x + y * m_cols;

            int ch = 32 + (index % (126 - 32));
            m_buffer[index].glyph_id = 0;
            m_buffer[index].bg_color = 0x00000000;
            m_buffer[index].fg_color = 0xFFFFFFFF;
            m_buffer[index].flags = 0;
        }
    }
}

void CellGrid::scroll_up(int lines = 1) {
    lines = std::min(lines, m_rows);
    for (int i = 0; i < lines; i++) {
        for (int x = 0; x < m_cols; x++) {
            int index = x + m_start * m_cols;
            m_buffer[index].glyph_id = ' ' - 32;
            m_buffer[index].bg_color = 0x00000000;
            m_buffer[index].fg_color = 0xFFFFFFFF;
            m_buffer[index].flags = 0;
        }
        m_start = (m_start + 1) % m_rows;
    }
}

const cell_t* CellGrid::data() {
    m_staging.resize(m_cols * m_rows);

    for (int logical_row = 0; logical_row < m_rows; logical_row++) {
        int physical_row = (m_start + logical_row) % m_rows;

        const cell_t* src = m_buffer.data() + physical_row * m_cols;
        cell_t* dst = m_staging.data() + logical_row * m_cols;
        std::memcpy(dst, src, m_cols * sizeof(cell_t));
    }

    return m_staging.data();
}

void CellGrid::resize(int new_cols, int new_rows) {
    std::vector<cell_t> new_buffer(new_cols * new_rows);

    int copy_cols = std::min(m_cols, new_cols);
    int copy_rows = std::min(m_rows, new_rows);

    for (int row = 0; row < copy_rows; ++row)
        for (int col = 0; col < copy_cols; ++col)
            new_buffer[row * new_cols + col] = m_buffer[row * m_cols + col];

    m_buffer = std::move(new_buffer);
    m_cols = new_cols;
    m_rows = new_rows;
    m_start = 0;
}

void CellGrid::clearCell(int col, int row) {
    if (col < 0 || col >= m_cols || row < 0 || row >= m_rows) return;
    int physical_row = (m_start + row) % m_rows;
    int index = col + physical_row * m_cols;

    m_buffer[index].glyph_id = ' ' - 32;
    m_buffer[index].bg_color = 0x00000000;
    m_buffer[index].fg_color = 0xFFFFFFFF;
    m_buffer[index].flags = 0;
}

void CellGrid::printChar(int col, int row, char ch) {
    if (col < 0 || col >= m_cols || row < 0 || row >= m_rows) return;
    int physical_row = (m_start + row) % m_rows;
    int index = col + physical_row * m_cols;

    m_buffer[index].glyph_id = ch - 32;
    m_buffer[index].bg_color = 0x00000000;
    m_buffer[index].fg_color = 0xFFFFFFFF;
    m_buffer[index].flags = 0;
}

void CellGrid::eraseEntireScreen() {
    for (int y = 0; y < m_rows; y++) {
        for (int x = 0; x < m_cols; x++) {
            clearCell(x, y);
        }
    }
}

void CellGrid::eraseLine(int col, int row, int param) {
    if (param == 0) {
        for (int x = col; x < m_cols; x++) {
            clearCell(x, row);
        }
    } else if (param == 1) {
        for (int x = 0; x < col; x++) {
            clearCell(x, row);
        }
    } else if (param == 2) {
        for (int x = 0; x < m_cols; x++) {
            clearCell(x, row);
        }
    }
}

CellGrid::~CellGrid() {}
