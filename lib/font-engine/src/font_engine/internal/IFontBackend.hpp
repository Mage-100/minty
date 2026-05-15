#pragma once
#include <vector>

#include <font_engine/font_utils.hpp>

using font_list = std::vector<font_obj_t>;

class IFontBackend {
public:
    virtual font_list get_font_by_name(const std::string& ) = 0;
    virtual font_list get_font_by_name(const std::string& name, const font_parameters) = 0;
    virtual ~IFontBackend()  = default;
};
