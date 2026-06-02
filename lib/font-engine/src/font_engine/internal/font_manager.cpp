#include <iostream>
#include "font_manager.hpp"
#include "IFontBackend.hpp"
#include "DirectWriteWrapper.hpp"
#include "font_engine/font_utils.hpp"

class FontManager::impl {
    std::unique_ptr<IFontBackend> font_backend;
public:
    impl() {
        std::cout<< "font backend: directwrite" << std::endl;
#ifdef _WIN32
        font_backend = std::make_unique<DirectWriteWrapper>();
#endif
    }

    std::vector<FontObj> get_font_by_name(const std::string& name) {
        std::vector<FontObj> list = font_backend->get_font_by_name(name);
        return list;
    }

    std::vector<FontObj> get_font_by_name(const std::string& name, const FontParameters params) {
        std::vector<FontObj> list = font_backend->get_font_by_name(name, params);
        return list;
    }
};

FontManager::FontManager() : pImpl(std::make_unique<impl>()) {};

std::vector<FontObj> FontManager::get_font_by_name(const std::string& name) {
    return pImpl->get_font_by_name(name);
}

std::vector<FontObj> FontManager::get_font_by_name(const std::string& name, const FontParameters params) {
    return pImpl->get_font_by_name(name, params);
}

FontManager::~FontManager() = default;
