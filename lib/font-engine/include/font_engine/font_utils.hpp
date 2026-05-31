#pragma once
#include <string>
#include <optional>

enum font_weight {
    FONT_WEIGHT_THIN = 100,
    FONT_WEIGHT_EXTRA_LIGHT = 200,
    FONT_WEIGHT_ULTRA_LIGHT = 200,
    FONT_WEIGHT_LIGHT = 300,
    FONT_WEIGHT_SEMI_LIGHT = 350,
    FONT_WEIGHT_NORMAL = 400,
    FONT_WEIGHT_REGULAR = 400,
    FONT_WEIGHT_MEDIUM = 500,
    FONT_WEIGHT_DEMI_BOLD = 600,
    FONT_WEIGHT_SEMI_BOLD = 600,
    FONT_WEIGHT_BOLD = 700,
    FONT_WEIGHT_EXTRA_BOLD = 800,
    FONT_WEIGHT_ULTRA_BOLD = 800,
    FONT_WEIGHT_BLACK = 900,
    FONT_WEIGHT_HEAVY = 900,
    FONT_WEIGHT_EXTRA_BLACK = 950,
    FONT_WEIGHT_ULTRA_BLACK = 950
};

enum font_style {
    FONT_STYLE_NORMAL,
    FONT_STYLE_ITALIC
};

struct font_obj_t {
    const std::string name;
    const std::string fontPath;
    font_style style;
    font_weight weight;

    font_obj_t(std::string name, std::string path)
        : name(std::move(name)), fontPath(std::move(path)) {}
};

struct font_parameters {
    std::optional<font_weight> weight;
    std::optional<font_style> style;
};
