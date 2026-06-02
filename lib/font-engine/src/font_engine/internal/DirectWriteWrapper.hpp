#pragma once
#include <font_engine/font_utils.hpp>
#include "./IFontBackend.hpp"

#include <dwrite.h>

class DirectWriteWrapper: public IFontBackend {
    IDWriteFactory* pDWriteFactory = NULL;
    IDWriteFontCollection* pFontCollection = NULL;
    HRESULT hr;

public:
    DirectWriteWrapper();
    ~DirectWriteWrapper();

    std::vector<FontObj> get_font_by_name(const std::string& name) override;
    std::vector<FontObj> get_font_by_name(const std::string& name, const FontParameters) override;
};
