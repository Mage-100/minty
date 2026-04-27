#pragma once
#include <cstdint>

#include "font.hpp"
#include "mty_cellgrid.hpp"


class MintyState {
private:
    int cols, rows;
    int framebuffer_w, framebuffer_h;
    bool isCellBufferDirty = true;

    Font* font;
    std::unique_ptr<CellGrid> cell_grid;
    std::vector<cell_t> cellBuffer;

public:
    MintyState();
    ~MintyState();

    void setFont(Font *);
    void initCellBuffer();
    void initCellGrid();
    void resizeCellBuffer();
    void setCellBufferClean() { isCellBufferDirty = false; };
    void setCellBufferDirty() { isCellBufferDirty = true;  };
    bool getCellBufferStatus() { return isCellBufferDirty; };
    int getCols(void);
    int getRows(void);
    int getWindowWidth();
    int getWindowHeight();
    int getFramebufferWidth();
    int getFramebufferHeight();
    float getAdvanceX(void);
    float getLineHeight(void);
    float getCellWidth();
    float getCellHeight();

    void setFramebufferSize(int, int);

    const std::vector<cell_t>& getCellBuffer() const { return cellBuffer; };
    CellGrid* getCellGrid() { return cell_grid.get(); };
};
