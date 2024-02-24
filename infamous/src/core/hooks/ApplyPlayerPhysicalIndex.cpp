#include "stdafx.hpp"
#include "../hooks.hpp"
#include "cheat/util/notify.hpp"
#include "rage/classes/network/CNetGamePlayer.hpp"
//#include "rage/api/social.hpp"
#include "cheat/menu/submenus/network/network_player_history.hpp"



void Hooks::ApplyPlayerPhysicalIndexHook(uint64_t Rcx, CNetGamePlayer* NetGamePlayer, uint8_t R8b) {
	if (Native::NetworkIsInSession()) {
		if (IsValidPtr(NetGamePlayer) && IsValidPtr(NetGamePlayer->m_player_info)) {
			if (R8b == 0xFF) {
				Menu::Notify::stacked(std::format("{} is leaving", NetGamePlayer->get_name(), R8b).c_str());
			}

			if (R8b != 0xFF) {
				Menu::Notify::stacked(std::format("{} is joining", NetGamePlayer->get_name(), NetGamePlayer->m_player_id).c_str());
			}
			
		}
	}

	if (R8b == 0xFF && !IsValidPtr(NetGamePlayer)) return OgApplyPlayerPhysicalIndexHook(Rcx, NetGamePlayer, R8b);;

	NetworkPlayerHistoryMenuVars::Save(NetGamePlayer->get_name(), NetGamePlayer->m_player_info->m_net_player_data.m_gamer_handle.m_rockstar_id);
	Menu::GetPlayer(R8b == 0xFF ? NetGamePlayer->m_player_id : R8b).Reset(); // Reset player
	OgApplyPlayerPhysicalIndexHook(Rcx, NetGamePlayer, R8b);
}