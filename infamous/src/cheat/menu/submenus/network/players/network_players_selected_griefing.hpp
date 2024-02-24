#pragma once
#include "framework/framework.hpp"
#include "framework/options/core.hpp"
#include "framework/options/button.hpp"
#include "framework/options/toggle.hpp"
#include "framework/options/submenu_option.hpp"
#include "framework/options/scroll.hpp"
#include "framework/options/break.hpp"
#include "core/core.hpp"
#include "framework/options/number.hpp"
class NetworkPlayersSelectedGriefingMenu {
public:
	void Run();
	void Update();
};

static NetworkPlayersSelectedGriefingMenu* GetNetworkPlayersSelectedGriefingMenu() {
	static NetworkPlayersSelectedGriefingMenu instance;
	return &instance;
}

namespace NetworkPlayersSelectedGriefingMenuVars {
	struct Vars_ {
		bool m_AlwaysWanted;
		bool m_DisableTasks;
		bool m_Taze;
		bool m_Ragdoll;
		bool m_KickFromVehicle;
		bool m_GlitchPhysics;
		bool m_Forcefield;
		bool m_RainRockets;
		int m_Explode;
		int m_CEO;


		bool m_FakeMoneyDrop;
		std::unordered_map<int, int> m_Sounds;

		bool g_explosion_loop{ false };
		bool g_visible{ false };
		float g_camera_shake{ 1.0f };
		int g_radius{ 100 };
		bool g_audible{ true };



	};

	void GiveWantedLevel(Menu::PlayerVars Player, int Level);
	void SetOnFire(Menu::PlayerVars Player);
	void Explode(Menu::PlayerVars Player, int Type);
	void CEOBan(Menu::PlayerVars Player);
	void CEOKick(Menu::PlayerVars Player);
	void Blame(Menu::PlayerVars Player);
	void Rape(Menu::PlayerVars Player);
	void BlackScreen(Menu::PlayerVars Player);
	void GlitchPhysics(Menu::PlayerVars Player);
	void TazePlayer(Menu::PlayerVars Player);
	void KickFromVehiclePlayer(Menu::PlayerVars Player);
	void RagdollPlayer(Menu::PlayerVars Player);
	void DumpInfo(Menu::PlayerVars Player);
	void SendPlayerTo(Menu::PlayerVars Player);

	extern Vars_ m_Vars;
}