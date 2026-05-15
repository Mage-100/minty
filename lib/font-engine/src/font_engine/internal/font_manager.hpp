#pragma once
#include <string>
#include <memory>
#include <vector>
#include <font_engine/font_utils.hpp>

using font_list = std::vector<font_obj_t>;

class FontManager {
public:
    FontManager();
    ~FontManager();
    font_list get_font_by_name(const std::string&);
    font_list get_font_by_name(const std::string&, const font_parameters);
private:
    class impl;
    std::unique_ptr<impl> pImpl;
};
