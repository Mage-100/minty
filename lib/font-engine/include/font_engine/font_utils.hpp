#pragma once
#include <string>
#include <optional>

struct font_obj_t {
    const std::string name;
    const std::string fontPath;

    font_obj_t(std::string name, std::string path)
        : name(std::move(name)), fontPath(std::move(path)) {}
};

enum font_weight {
    FONT_WEIGHT_REGULAR,
    FONT_WEIGHT_BOLD
};

enum font_style {
    FONT_STYLE_NORMAL,
    FONT_STYLE_ITALIC
};

struct font_parameters {
    std::optional<font_weight> weight;
    std::optional<font_style> style;
};
