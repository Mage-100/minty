#pragma once
#include <cstdint>
#include <memory>
#include "font.hpp"

typedef struct cell_t {
    uint32_t glyph_id;
    uint32_t fg_color;
    uint32_t bg_color;
    uint32_t flags;
} cell_t;

class MintyState {
private:
    int cols, rows;
    int framebuffer_w, framebuffer_h;
    bool isCellBufferDirty = true;

    Font* font;
    std::vector<cell_t> cellBuffer;

public:
    MintyState();
    ~MintyState();

    void setFont(Font *);
    void initCellBuffer();
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
    int getCellWidth();
    int getCellHeight();

    void setFramebufferSize(int, int);

    const std::vector<cell_t>& getCellBuffer() const { return cellBuffer; };
};
