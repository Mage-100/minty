#pragma once
#include <functional>

struct VTCallbacks {
    std::function<void(int col, int row, unsigned int ch)> onPrintChar;
    std::function<void(void)>                              onEraseEntireScreen;
    std::function<void(int, int, int)>                     onEraseLine;
    std::function<void(int, int)>                          onPromptStart;
    std::function<void(int, int)>                          onPromptEnd;
    std::function<void(int)>                               onScrollUp;
    std::function<void(void)>                              onResetAllModes; // Resets styles and colors
    std::function<void(int)>                               onSetForegroundColor;
    std::function<void(int)>                               onSetBackgroundColor;
};
