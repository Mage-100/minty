#pragma once
#include <vector>

#include <font_engine/font_utils.hpp>

class IFontBackend {
public:
    virtual std::vector<FontObj> get_font_by_name(const std::string& ) = 0;
    virtual std::vector<FontObj> get_font_by_name(const std::string& name, const FontParameters) = 0;
    virtual ~IFontBackend()  = default;
};
