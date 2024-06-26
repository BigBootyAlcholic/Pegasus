#include "stdafx.hpp"
#include "pattern.hpp"

namespace Memory {
	void Batch::Add(std::pair<const char*, Pattern> entry, std::function<void(Ptr)> callback, bool& out) {
		m_entries.emplace_back(entry.first, std::move(entry.second), std::move(callback), std::move(out));
	}

	void Batch::Run(Range region) {
		bool all_found = true;
		for (auto& entry : m_entries) {
			if (auto result = region.Scan(entry.m_pattern)) {
				if (entry.m_callback) {
					std::invoke(std::move(entry.m_callback), result);

					if (entry.m_out) {
						LOG_CUSTOM_SUCCESS("Found", "%s", entry.m_name.c_str());
					}

				}
				else {
					entry.m_out = false;
					all_found = false;
					LOG_ERROR(std::format(("Failed To Find {}"), entry.m_name).c_str());
				}
			}
			else {
				entry.m_out = false;
				all_found = false;
				LOG_ERROR(std::format(("Failed To Find {}"), entry.m_name).c_str());
			}
		}

		m_entries.clear();
		if (!all_found) {
			throw std::runtime_error("Failed to find some patterns.");
		}
	}
}