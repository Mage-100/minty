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

    font_list get_font_by_name(const std::string& name) {
        font_list list = font_backend->get_font_by_name(name);
        return list;
    }

    font_list get_font_by_name(const std::string& name, const font_parameters params) {
        font_list list = font_backend->get_font_by_name(name, params);
        return list;
    }
};

FontManager::FontManager() : pImpl(std::make_unique<impl>()) {};

font_list FontManager::get_font_by_name(const std::string& name) {
    return pImpl->get_font_by_name(name);
}

font_list FontManager::get_font_by_name(const std::string& name, const font_parameters params) {
    return pImpl->get_font_by_name(name, params);
}

FontManager::~FontManager() = default;
