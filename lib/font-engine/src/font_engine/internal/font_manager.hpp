#pragma once
#include <string>
#include <memory>
#include <vector>
#include <font_engine/font_utils.hpp>

class FontManager {
public:
    FontManager();
    ~FontManager();
    std::vector<FontObj> get_font_by_name(const std::string&);
    std::vector<FontObj> get_font_by_name(const std::string&, const FontParameters);
private:
    class impl;
    std::unique_ptr<impl> pImpl;
};
