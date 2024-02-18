#include "stdafx.hpp"
#include "texture.hpp"
#include "core/patterns.hpp"
#include "framework/d3d.hpp"
#define VALID_TEXTURE_EXT(ext) ext == ".png" || ext == ".jpg" || ext == ".jpeg" || ext == ".bmp" || ext == ".gif")

using namespace Patterns::Vars;

namespace Menu {

	//WE LOVE VALI :SPEAK: :FIRE:
	void add_dictionary_to_pool(Rage::pgDictionary* dictionary, std::string name) {
		if (auto grcTextureStore = g_TextureStore; grcTextureStore) {
			uint32_t hash{ Rage::LowerCaseJoaat(name) };
			uint32_t output{};
			uint32_t index{ *g_SetupStoreItem(grcTextureStore, &output, &hash) };
			g_AddDictionaryToPool(grcTextureStore, index, dictionary);
		}
	}

	Rage::pgDictionary* create_dictionary(size_t size) {
		Rage::pgDictionary* customDictionary{ (Rage::pgDictionary*)Rage::sysMemAllocator::get()->Allocate(64, 8, NULL) };
		if (customDictionary) {
			customDictionary = g_SetupDictionary(customDictionary, size);
			return customDictionary;
		}
		return nullptr;
	}

	Rage::grcTexture* create_texture(std::string filepath) {
		Rage::grcTexture* texture{};
		if (auto grcTextureFactory = *g_TextureFactory; grcTextureFactory) {
			texture = grcTextureFactory->CreateTexture(filepath.c_str(), NULL);
		}
		texture->Set(filepath, d3d::create_texture(filepath));
		return texture;
	}
	textures::textures() {
		m_path.append("Pegasus").append("Textures");
		if (std::filesystem::exists(m_path)) {
			std::filesystem::directory_iterator it{ m_path };
			for (auto&& entry : it) {
				if (entry.is_regular_file()) {
					auto path{ entry.path() };
					if (path.has_filename() && (path.extension() == ".png" || path.extension() == ".jpg" || path.extension() == ".jpeg" || path.extension() == ".bmp" || path.extension() == ".gif")) {
						m_files.push_back({ path.filename().string() });
					}
				}
			}

			m_dictionary = create_dictionary(m_files.size() * 2);

			if (m_dictionary) {
				add_dictionary_to_pool(m_dictionary, "textures");
				if (!m_files.empty()) {
					for (auto& file : m_files) {
						load_image(std::filesystem::path(m_path).append(file));
						//LOG("Loaded %s", file.c_str());
					}

				}
			}
		}
	}

	void textures::tick() {
		Native::RequestStreamedTextureDict("textures", FALSE);
		if (std::filesystem::exists(textures::get()->m_path)) {
			std::filesystem::directory_iterator it{ textures::get()->m_path };
			for (auto&& entry : it) {
				if (entry.is_regular_file()) {
					auto path{ entry.path() };
					if (path.has_filename() && path.has_extension()) {
						std::string name{ path.stem().string() };
						if (VALID_TEXTURE_EXT(path.extension()) {
							if (!textures::get()->exists(name)) {
								//loadImage(fs::path(m_path).append(name));
							}
						}
					}
				}
			}
		}
	}

	void textures::load_image(std::filesystem::path path) {
		if (Rage::grcTextureFactory* factory{ *g_TextureFactory }; factory) {
			std::string name{ path.stem().string() };
			Rage::grcTexture* texture{ factory->CreateTexture(name.c_str(), 0) };
			texture->Set(name, d3d::create_texture(path));
			g_AddTextureToDictionary(m_dictionary, Rage::LowerCaseJoaat(name), texture);


		}
	}

	texture_context textures::get(std::string name) {
		for (auto& texture : m_textures) {
			if (texture.m_name == name)
				return texture;
		}
		return {};
	}
}