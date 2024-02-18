#pragma once
#include "stdafx.hpp"
#include "rage/classes/script/sysMemAllocator.hpp"
#include "rage/classes/texture/grcTexture.hpp"
#include "rage/classes/texture/grcTextureFactory.hpp"
#include "rage/classes/texture/grcTextureStore.hpp"
#include "rage/classes/texture/pgDictionary.hpp"
inline std::string GetDocumentsPathRoot() {
    wchar_t folder[1024];
    HRESULT result = SHGetFolderPathW(0, CSIDL_MYDOCUMENTS, 0, 0, folder);
    if (SUCCEEDED(result)) {
        char string[1024];
        wcstombs(string, folder, 1023);
        return string;
    }
    else return "";
}
namespace Menu {
    struct texture_context {
        std::string m_name;
        Rage::grcTexture* m_texture;
    };

    class textures {
    public:
        textures();
        static void tick();
        void load_image(std::filesystem::path path);
        texture_context get(std::string name);
        bool exists(std::string name) {
            texture_context texture{ get(name) };
            if (!texture.m_name.empty()) {
                return true;
            }
            return false;
        }
    private:
        static textures* get()
        {
            static textures i{};

            return &i;
        }
    public:
        static void initialize() {
            get();
        }
    private:
        std::filesystem::path m_path{ GetDocumentsPathRoot() };
        Rage::pgDictionary* m_dictionary{};
        std::vector<std::string> m_files{};
        std::vector<texture_context> m_textures{};
    };
}