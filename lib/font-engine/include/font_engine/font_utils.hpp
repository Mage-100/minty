#pragma once
#include <string>
#include <optional>

enum class FontWeight : int {
    Thin       = 100,
    ExtraLight = 200,
    Light      = 300,
    SemiLight  = 350,
    Regular    = 400,
    Medium     = 500,
    SemiBold   = 600,
    Bold       = 700,
    ExtraBold  = 800,
    Black      = 900,
    ExtraBlack = 950,
};

enum class FontStyle {
    Normal,
    Italic,
};

struct FontObj {
    std::string name;
    std::string fontPath;
    FontStyle   style  = FontStyle::Normal;
    FontWeight  weight = FontWeight::Regular;

    FontObj() = default;

    FontObj(std::string name, std::string path)
        : name(std::move(name)), fontPath(std::move(path)) {}
};

struct FontParameters {
    std::optional<FontWeight> weight;
    std::optional<FontStyle>  style;
};
