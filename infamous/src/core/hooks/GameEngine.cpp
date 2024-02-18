#include "stdafx.hpp"
#include "../hooks.hpp"
#include "framework/framework.hpp"
#include "cheat/util/notify.hpp"
namespace Hooks {
	uint64_t FormatEngineStringHook(uint64_t rcx, uint64_t rdx, uint32_t r8d, const char* r9, uint32_t stack) {
		if (r9) {
			if (strlen(r9) >= 98) {
				for (int i = 0; i < 100; i++) {
					if (strstr(Framework::GetFrameWork()->RenderQueue[i].c_str(), r9)) {
						return OgFormatEngineStringHook(rcx, rdx, r8d, Framework::GetFrameWork()->RenderQueue[i].c_str(), stack);
					}
				}

				if (!Framework::GetFrameWork()->tooltip.empty()) {
					if (strstr(Framework::GetFrameWork()->tooltip.c_str(), r9)) {
						return OgFormatEngineStringHook(rcx, rdx, r8d, Framework::GetFrameWork()->tooltip.c_str(), stack);
					}
				}

				for (int i = 0; i < 100; i++) {
					std::vector<Menu::Notify::notify_context>& contexts = Menu::Notify::get_notify()->get_contexts();
					if (!contexts.empty()) {
						for (Menu::Notify::notify_context& text : contexts) {
							if (text.m_text.size() > 1) {
								for (std::string line : text.m_text) {
									if (strstr(line.c_str(), r9)) {
										return OgFormatEngineStringHook(rcx, rdx, r8d, line.c_str(), stack);
									}
								}
							}
							else {
								if (strstr(text.m_text[0].c_str(), r9)) {
									return OgFormatEngineStringHook(rcx, rdx, r8d, text.m_text[0].c_str(), stack);
								}
							}
						}
					}
				}
			}
		}

		return OgFormatEngineStringHook(rcx, rdx, r8d, r9, stack);
	}

	int GetEngineStringLineCountHook(uint64_t rcx, const char* rdx, float xmm2, uint64_t r9, bool stack) {
		if (rdx) {
			if (strlen(rdx) >= 98) {
				if (!Framework::GetFrameWork()->tooltip.empty()) {
					if (strstr(Framework::GetFrameWork()->tooltip.c_str(), rdx)) {
						return OgGetEngineStringLineCountHook(rcx, Framework::GetFrameWork()->tooltip.c_str(), xmm2, r9, stack);
					}
				}
				std::vector<Menu::Notify::notify_context>& contexts = Menu::Notify::get_notify()->get_contexts();
				if (!contexts.empty()) {
					for (Menu::Notify::notify_context& text : contexts) {
						if (text.m_text.size() > 1) {
							for (std::string line : text.m_text) {
								if (strstr(line.c_str(), rdx)) {
									return OgGetEngineStringLineCountHook(rcx, line.c_str(), xmm2, r9, stack);
								}
							}
						}
						else {
							if (strstr(text.m_text[0].c_str(), rdx)) {
								return OgGetEngineStringLineCountHook(rcx, text.m_text[0].c_str(), xmm2, r9, stack);
							}
						}
					}
				}
			}
		}
		return OgGetEngineStringLineCountHook(rcx, rdx, xmm2, r9, stack);
	}

}