#include "font.hpp"
#include "mty_state.hpp"

MintyState::MintyState() {}

void MintyState::setFont(Font *f) {
    font = f;
}

void MintyState::initCellBuffer() {
    cellBuffer = std::vector<cell_t>(rows * cols);
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            int index = y * cols + x;

            cellBuffer[index].glyph_id = 'A' - 32;
            cellBuffer[index].fg_color = 0xFFFFFFFF;
            cellBuffer[index].bg_color = 0xFF000000;
            cellBuffer[index].flags    = 0;
        }
    }
}

void MintyState::resizeCellBuffer() {
    cellBuffer.resize(cols * rows);
    std::cout << "cellBuffer: " << cellBuffer.size() << std::endl;
    setCellBufferDirty();
}

void MintyState::setFramebufferSize(int w, int h) {
    framebuffer_w = w;
    framebuffer_h = h;
    cols = w / font->get_advanceX();
    rows = h / font->get_line_height();
}

int MintyState::getWindowWidth() {
    return 800;
}

int MintyState::getWindowHeight() {
    return 600;
}

int MintyState::getFramebufferWidth() {
    return framebuffer_w;
}
int MintyState::getFramebufferHeight() {
    return framebuffer_h;
}

int MintyState::getCols(void) {
    if (cols) return cols;
    return 0;
}

int MintyState::getRows(void) {
    if (rows) return rows;
    return 0;
}

float MintyState::getAdvanceX(void) {
    return font->get_advanceX();
}

float MintyState::getLineHeight(void) {
    return font->get_line_height();
}

int getCellWidth();
int getCellHeight();


MintyState::~MintyState() {

}
