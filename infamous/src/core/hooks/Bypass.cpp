#include "stdafx.hpp"
#include "../hooks.hpp"
#include "util/caller.hpp"
#include "cheat/menu/submenus/protection.hpp"
#include "cheat/util/notify.hpp"
#include "rage/classes/network/Network.hpp"
class game_skeleton_update_group;
class game_skeleton_item;

#pragma pack(push, 8)
struct game_skeleton_update_mode
{
	int m_type;                           // 0x00
	game_skeleton_update_group* m_groups; // 0x08
	game_skeleton_update_mode* m_next;    // 0x10
};
static_assert(sizeof(game_skeleton_update_mode) == 0x18);

struct game_skeleton_update_group
{
	virtual ~game_skeleton_update_group() = default;
	virtual void run() = 0; // 0x08

	char pad[0x10];                     // 0x08
	game_skeleton_update_group* m_next; // 0x18	
	game_skeleton_item* m_items;        // 0x20
};
static_assert(sizeof(game_skeleton_update_group) == 0x28);

struct game_skeleton_item
{
	virtual ~game_skeleton_item() = default;
	virtual void run() = 0; // 0x08

	char m_pad[0x8];            // 0x08
	uint32_t m_hash;            // 0x10
	game_skeleton_item* m_next; // 0x18
};
static_assert(sizeof(game_skeleton_item) == 0x20);
#pragma pack(pop)


//helpers
std::vector<std::string> blocked_metrics = {
  "REPORTER",
		"REPORT_INVALIDMODEL",
		"MEM_NEW",
		"DEBUGGER_ATTACH",
		"DIG",
		"XP_LOSS",
		"AWARD_XP",
		"CF",
		"CC",
		"CNR",
		"SCRIPT",
		"CHEAT",
		"AUX_DEUX",
		"WEATHER",
		"HARDWARE_OS",
		"HARDWARE_GPU",
		"HARDWARE_MOBO",
		"HARDWARE_MEM",
		"HARDWARE_CPU",
		"PCSETTINGS",
		"CASH_CREATED",
		"DR_PS",
		"UVC",
		"W_L",
		"ESVCS",
		"IDLEKICK",
		"GSCB",
		"GSINV",
		"GSCW",
		"GSINT",
		"EARN",
		"GARAGE_TAMPER",
		"DUPE_DETECT",
		"LAST_VEH",
		"FAIL_SERV",
		"CCF_UPDATE",
		"CODE_CRC",
		"COLLECTIBLE",
		"FIRST_VEH",
		"MM",
		"RDEV",
		"RQA",
		"RANK_UP",
};


namespace Hooks {
	bool SendMetricHook(Rage::rlMetric* metric, bool unk) {
		//LOG_WARN("METRIC: %s, %d, %d, %d", metric->get_name(), metric->get_name_hash(), metric->get_size(), metric->_0x18());

		if (std::find(begin(blocked_metrics), end(blocked_metrics), metric->get_name()) != end(blocked_metrics)) {
			
			if (ProtectionMenuVars::m_Vars.m_MetricLog) {
				LOG_WARN("Blocking Metric: %s", metric->get_name());
			}
			if (ProtectionMenuVars::m_Vars.m_MetricNotify) {
				//LOG_WARN("Blocking Metric: %s", metric->get_name());
				Menu::Notify::stacked(std::format("Blocking Metric: {}", metric->get_name()).c_str());
			}
			return false;
		}

		return OgSendMetricHook(metric, unk);
	}

	bool SendHTTPRequestHook(void* request, const char* uri) {
		if (strstr(uri, "Bonus")) {
			if (ProtectionMenuVars::m_Vars.m_BonusLog) {
				LOG_WARN("Blocking Bonus Report");
			}
			if (ProtectionMenuVars::m_Vars.m_BonusNotify) {
				Menu::Notify::stacked("Blocking Bonus Report");
			}
			uri = "https://0.0.0.0/";
		}
		return OgSendHTTPRequestHook(request, uri);
	}

	bool inline is_address_in_game_region(int64_t address)
	{
		if (!address)
			return false;
		static int64_t moduleBase = NULL;
		static int64_t moduleSize = NULL;
		if (!moduleBase || !moduleSize)
		{
			MODULEINFO info;
			if (!GetModuleInformation(GetCurrentProcess(), GetModuleHandle(0), &info, sizeof(info)))
			{
				LOG_ERROR("Failed to get module handle");
				return true;
			}
			else
			{
				moduleBase = (int64_t)GetModuleHandle(0);
				moduleSize = (int64_t)info.SizeOfImage;
			}
		}
		return address > moduleBase && address < (moduleBase + moduleSize);
	}

	struct ac_verifier
	{
		virtual ~ac_verifier() = 0;
		virtual bool run() = 0;
		Rage::Obf32 m_last_time; // 0x8 
		Rage::Obf32 m_delay; // 0x18
	};

	bool is_unwanted_dependency(int64_t cb)
	{
		int64_t f1 = *reinterpret_cast<int64_t*>(cb + 0x60);
		int64_t f2 = *reinterpret_cast<int64_t*>(cb + 0x100);
		int64_t f3 = *reinterpret_cast<int64_t*>(cb + 0x1A0);

		if (!is_address_in_game_region(f1) || !is_address_in_game_region(f2) || !is_address_in_game_region(f3))
			return false;

		if (*reinterpret_cast<uint8_t*>(f1) != 0xE9)
			return false;

		return true;
	}

	static bool nullsub()
	{
		return true; // returning false would cause the dependency to requeue
	}

	int QueueDependencyHook(void* a1, int a2, int64_t dependency) {
		if (is_unwanted_dependency(dependency))
		{

			if (ProtectionMenuVars::m_Vars.m_RacLog) {
				LOG_WARN("Blocking RAC Report");
			}
			if (ProtectionMenuVars::m_Vars.m_RacNotify) {
				Menu::Notify::stacked("Blocking RAC Report");
			}

			ac_verifier* verifier = reinterpret_cast<ac_verifier*>(dependency - 0x30);
			verifier->m_delay = INT_MAX; // makes it so these won't queue in the future
			*reinterpret_cast<void**>(dependency + 0x60) = nullsub;
			*reinterpret_cast<void**>(dependency + 0x100) = nullsub;
			*reinterpret_cast<void**>(dependency + 0x1A0) = nullsub;
		}
		return OgQueueDependencyHook(a1, a2, dependency);
	}

	void UpdateGameSkeletonHook(__int64 skeleton, int type) {
		for (auto mode = *(game_skeleton_update_mode**)(skeleton + 0x140); mode; mode = mode->m_next) {
			if (mode && mode->m_type == type) {
				for (auto group = mode->m_groups; group; group = group->m_next) {
					for (auto item = group->m_items; item; item = item->m_next) {
						if (item->m_hash != 0xA0F39FB6) {
							item->run();
						}
					}
				}

				break;
			}
		}
	}

	void CreateNetworkEventHook(uint64_t net_table, uint64_t event) {
		if (event) {
			short type = *(short*)(event + 8);

			if (type == 83u || type == 84u || type == 78u) {
				LOG_CUSTOM_WARN("AC", "network event - %i", type);

				uint64_t table = *(uint64_t*)event;
				Caller::Call<int>(*(uint64_t*)table, event, 1); // Deallocate event
				return;
			}
		}

		return OgCreateNeverEventHook(net_table, event);
	}

}