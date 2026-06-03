#include <dwrite.h>
#include <iostream>
#include <string>
#include <winerror.h>
#include <winnt.h>

#include "DirectWriteWrapper.hpp"
#include "font_engine/font_utils.hpp"

// TODO: Handle the errors gracefully,
//       set correct defaults if a font is not found.

// SafeRelease inline function
template <class T> inline void SafeRelease(T **ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

//
FontStyle _mapFontStyle(DWRITE_FONT_STYLE style) {
    if (style == DWRITE_FONT_STYLE_ITALIC) {
        return FontStyle::Italic;
    }
    return FontStyle::Normal;
}

// Converts UTF-16 string to UTF-8
std::string wideToString(const wchar_t* wstr) {
    int size_needed = WideCharToMultiByte(
        CP_UTF8,
        0,
        wstr,
        -1,
        nullptr,
        0,
        nullptr,
        nullptr
    );

    std::string str(size_needed - 1, 0);

    WideCharToMultiByte(
        CP_UTF8,
        0,
        wstr,
        -1,
        &str[0],
        size_needed,
        nullptr,
        nullptr
    );

    return str;
}

DirectWriteWrapper::DirectWriteWrapper() {
    hr = DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(&pDWriteFactory)
    );
    if (FAILED(hr)) {
        std::cerr << "Error: Failed to create DWriteFactory" << std::endl;
    }

    if (SUCCEEDED(hr)) {
        hr = pDWriteFactory->GetSystemFontCollection(&pFontCollection);

        if (FAILED(hr)) {
            std::cerr << "Error: Failed to get System Font Collection" << std::endl;
        }
    }
}

std::vector<FontObj> DirectWriteWrapper::get_font_by_name(const std::string& name) {
    std::vector<FontObj> list;
    std::wstring familyName(name.begin(), name.end());
    uint32_t index;
    BOOL exists;

    // This code finds the font family by taking in name parameter,
    // and sets the index and exists values above
    if (SUCCEEDED(hr)) {
        hr = pFontCollection->FindFamilyName(familyName.c_str(), &index, &exists);

        if (FAILED(hr)) {
            std::cerr << "Error: Failed to fetch family name: " << name << std::endl;
        }
    }

    // Based the index value , this code fetches the FontFamily
    IDWriteFontFamily* pFontFamily = nullptr;
    if (exists) {
        hr = pFontCollection->GetFontFamily(index, &pFontFamily);

        if (FAILED(hr)) {
            std::cerr << "Error: Failed to GetFontFamily: " << name << std::endl;
        }
    }

    if (SUCCEEDED(hr)) {
        uint32_t fontCount = pFontFamily->GetFontCount();

        for (uint32_t i = 0; i < fontCount; i++) {
            IDWriteFont* pFont = nullptr;
            hr = pFontFamily->GetFont(i, &pFont);
            if (FAILED(hr)) continue;

            // Get Face name
            IDWriteLocalizedStrings* names = nullptr;
            hr = pFont->GetFaceNames(&names);

            std::wstring faceName;

            if (SUCCEEDED(hr)) {
                std::uint32_t index;
                BOOL exists;
                hr = names->FindLocaleName(L"en-us", &index, &exists);

                if (!exists) {
                    index = 0;
                }

                std::uint32_t length;
                names->GetStringLength(index, &length);

                faceName.resize(length + 1);
                names->GetString(index, faceName.data(), length+1);
                faceName.pop_back(); // remove '\0'
            }

            SafeRelease(&names);

            // Get weight, style, stretch
            DWRITE_FONT_WEIGHT  weight  = pFont->GetWeight();   // e.g. 400 = Regular, 700 = Bold
            DWRITE_FONT_STYLE   style   = pFont->GetStyle();    // Normal, Italic, Oblique
            DWRITE_FONT_STRETCH stretch = pFont->GetStretch();  // Condensed, Normal, Expanded etc.

            // Discarding aritficial slanted fonts
            if (style != DWRITE_FONT_STYLE_OBLIQUE) {

                IDWriteFontFace* pFontFace = nullptr;
                hr = pFont->CreateFontFace(&pFontFace);

                // this code fetches the number of files associated per face
                uint32_t numberOfFiles;
                hr = pFontFace->GetFiles(&numberOfFiles, nullptr);

                IDWriteFontFile** ppFontFiles = new IDWriteFontFile*[numberOfFiles];
                hr = pFontFace->GetFiles(&numberOfFiles, ppFontFiles);

                /*
                    This is the order of DirectWrite api calls needed to the get path of a font face
                    pFontFile->GetReferenceKey(...)
                    pFontFile->GetLoader(...);
                    pLoader->QueryInterface(...);
                    pLocalLoader->GetFilePathLengthFromKey(...);
                    pLocalLoader->GetFilePathFromKey(...);

                    The following does this in a verbose way
                    Refer: https://learn.microsoft.com/en-us/windows/win32/DirectWrite
                 */

                for (uint32_t f = 0; f < numberOfFiles; f++) {
                    IDWriteFontFile* pFontFile = ppFontFiles[f];

                    const void* referenceKey = nullptr;
                    uint32_t referenceKeySize = 0;
                    hr = pFontFile->GetReferenceKey(&referenceKey, &referenceKeySize);

                    IDWriteFontFileLoader* pLoader = nullptr;
                    hr = pFontFile->GetLoader(&pLoader);

                    IDWriteLocalFontFileLoader* pLocalLoader = nullptr;
                    hr = pLoader->QueryInterface(__uuidof(IDWriteLocalFontFileLoader), (void**)&pLocalLoader);

                    if (SUCCEEDED(hr) && pLocalLoader) {
                        uint32_t pathLen = 0;
                        hr = pLocalLoader->GetFilePathLengthFromKey(referenceKey, referenceKeySize, &pathLen);

                        wchar_t* path = new wchar_t[pathLen + 1];
                        hr = pLocalLoader->GetFilePathFromKey(referenceKey, referenceKeySize, path, pathLen + 1);

                        FontObj font_obj(
                            name + " - " + wideToString(faceName.data()),
                            wideToString(path)
                        );
                        font_obj.weight = static_cast<FontWeight>(weight);
                        font_obj.style = _mapFontStyle(style);
                        list.push_back(font_obj);

                        delete[] path;
                    }

                    // Cleanup for pFontFile, pLoader and pLocalLoader per loop
                    SafeRelease(&pLocalLoader);
                    SafeRelease(&pLoader);
                }
            }

            SafeRelease(&pFont);
        }
    }

    SafeRelease(&pFontFamily);
    return list;
}


std::vector<FontObj> DirectWriteWrapper::get_font_by_name(const std::string& name, const FontParameters params) {
    std::vector<FontObj> list;
    std::wstring familyName(name.begin(), name.end());
    uint32_t index;
    BOOL exists;

    if (SUCCEEDED(hr)) {
        hr = pFontCollection->FindFamilyName(familyName.c_str(), &index, &exists);

        if (FAILED(hr)) {
            std::cerr << "Error: Failed to fetch family name: " << name << std::endl;
        }
    }

    IDWriteFontFamily* pFontFamily = nullptr;
    if (exists) {
        hr = pFontCollection->GetFontFamily(index, &pFontFamily);

        if (FAILED(hr)) {
            std::cerr << "Error: Failed to GetFontFamily: " << name << std::endl;
        }
    }

    IDWriteFont* pFont = nullptr;
    DWRITE_FONT_WEIGHT fontWeight = static_cast<DWRITE_FONT_WEIGHT>(
        params.weight.value_or(FontWeight::Regular)
    );
    DWRITE_FONT_STRETCH fontStretch = DWRITE_FONT_STRETCH_NORMAL;
    DWRITE_FONT_STYLE fontStyle;

    if (params.style.has_value()) {
        if (params.style == FontStyle::Normal) fontStyle = DWRITE_FONT_STYLE_NORMAL;
        else if (params.style == FontStyle::Italic) fontStyle = DWRITE_FONT_STYLE_ITALIC;
    }

    if (SUCCEEDED(hr)) {
        hr = pFontFamily->GetFirstMatchingFont(
            fontWeight,
            fontStretch,
            fontStyle,
            &pFont
        );
    }

    IDWriteFontFace* pFontFace = nullptr;
    hr = pFont->CreateFontFace(&pFontFace);

    uint32_t numberOfFiles;
    hr = pFontFace->GetFiles(&numberOfFiles, nullptr);

    IDWriteFontFile** ppFontFiles = new IDWriteFontFile*[numberOfFiles];
    hr = pFontFace->GetFiles(&numberOfFiles, ppFontFiles);

    for (uint32_t f = 0; f < numberOfFiles; f++) {
        IDWriteFontFile* pFontFile = ppFontFiles[f];

        const void* referenceKey = nullptr;
        uint32_t referenceKeySize = 0;
        hr = pFontFile->GetReferenceKey(&referenceKey, &referenceKeySize);

        IDWriteFontFileLoader* pLoader = nullptr;
        hr = pFontFile->GetLoader(&pLoader);

        IDWriteLocalFontFileLoader* pLocalLoader = nullptr;
        hr = pLoader->QueryInterface(__uuidof(IDWriteLocalFontFileLoader), (void**)&pLocalLoader);

        if (SUCCEEDED(hr) && pLocalLoader) {
            uint32_t pathLen = 0;
            hr = pLocalLoader->GetFilePathLengthFromKey(referenceKey, referenceKeySize, &pathLen);

            wchar_t* path = new wchar_t[pathLen + 1];
            hr = pLocalLoader->GetFilePathFromKey(referenceKey, referenceKeySize, path, pathLen + 1);

            FontObj font_obj(name, wideToString(path));
            font_obj.weight = static_cast<FontWeight>(fontWeight);
            font_obj.style = _mapFontStyle(fontStyle);
            list.push_back(font_obj);

            delete[] path;
            pLocalLoader->Release();

        }

        // Cleanup for pFontFile, pLoader and pLocalLoader per loop
        SafeRelease(&pLocalLoader);
        SafeRelease(&pLoader);
    }

    // Cleanup
    // Clearning font files per font family if exists
    for(std::uint32_t i = 0; i < numberOfFiles; i++) {
        SafeRelease(&ppFontFiles[i]);
    }
    delete[] ppFontFiles;

    SafeRelease(&pFontFace);
    SafeRelease(&pFont);
    SafeRelease(&pFontFamily);

    return list;
}

DirectWriteWrapper::~DirectWriteWrapper() {
    SafeRelease(&pFontCollection);
    SafeRelease(&pDWriteFactory);
}
