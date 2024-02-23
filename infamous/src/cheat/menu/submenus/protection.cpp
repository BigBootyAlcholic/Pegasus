#include "stdafx.hpp"
#include "protection.hpp"

using namespace ProtectionMenuVars;

namespace ProtectionMenuVars {
	Vars_ m_Vars;

	std::pair<std::string, eNetObjectTypes> Types[] = {
		{ "Automobile", NetObjectAutomobile },
		{ "Bike", NetObjectBike },
		{ "Boat", NetObjectBoat },
		{ "Helicopter", NetObjectHeli },
		{ "Object", NetObjectObject },
		{ "Ped", NetObjectPed },
		{ "Pickup", NetObjectPickup },
		{ "Plane", NetObjectPlane },
		{ "Submarine", NetObjectSubmarine },
		{ "Player", NetObjectPlayer },
		{ "Trailer", NetObjectTrailer },
		{ "Train", NetObjectTrain }
	};
}

void ProtectionMenu::Run() {

	std::pair<std::string, ProtectionEventContext*> Items[] = {
	{ "Freeze", &m_Vars.m_Freeze },
	{ "Explosion", &m_Vars.m_Explosion },
	{ "Fire", &m_Vars.m_Fire },
	{ "Give Weapon", &m_Vars.m_GiveWeapon },
	{ "Remove Weapon", &m_Vars.m_RemoveWeapon },
	{ "Remove All Weapons", &m_Vars.m_RemoveAllWeapons },
	{ "Ragdoll", &m_Vars.m_Ragdoll },
	{ "PTFX", &m_Vars.m_PTFX },
	{ "Sound", &m_Vars.m_Sound },
	{ "Control", &m_Vars.m_Control },
	};

	std::pair<std::string, ProtectionEventContext*> Scripts[] = {
	{ "Crash Event", &m_Vars.m_Freeze },
	{ "Kick Event", &m_Vars.m_Explosion },
	{ "CEO Kick", &m_Vars.m_CeoKick },
	{ "CEO Money", &m_Vars.m_CeoMoney },
	{ "Start Mission", &m_Vars.m_ForceToActivity },
	{ "Send To Clubhouse", &m_Vars.m_SendToClubHouse },
	{ "Send To Island", &m_Vars.m_SendToIsland },
	{ "Teleport To Warehouse", &m_Vars.m_TeleportToWarehouse },
	{ "Give Collectible", &m_Vars.m_GiveCollectible },
	{ "Game Banner", &m_Vars.m_Banner },
	{ "Send To Cutscene", &m_Vars.m_ForceToCutscene },
	{ "Transaction Error", &m_Vars.m_TransactionError },
	{ "Kick From Vehicle", &m_Vars.m_VehicleKick },
	{ "Kick From Interior", &m_Vars.m_KickFromInterior },
	{ "Rotate Camera", &m_Vars.m_RotateCam },
	{ "Spectate Message", &m_Vars.m_SpectateMessage },
	{ "Notification", &m_Vars.m_Notification },
	{ "Force To Mission", &m_Vars.m_ForceToMission },
	{ "Clear Wanted Level", &m_Vars.m_ClearWantedLevel },
	{ "Remote Off Radar", &m_Vars.m_RemoteOffRadar },
	{ "Insurance Message", &m_Vars.m_InsuranceMessage },
	{ "Destroy Personal Vehicle", &m_Vars.m_DestroyPersonalVehicle },
	{ "Teleport", &m_Vars.m_Teleport },
	{ "Bounty", &m_Vars.m_Bounty },
	};



	Framework::addSubmenu("Protections", "protections", [=](Framework::Options::Core* core) {		
		core->addOption(Framework::Options::SubmenuOption("Events")
			.setTarget("protection_events"));

		core->addOption(Framework::Options::SubmenuOption("Reports")
			.setTarget("protection_reports"));

		core->addOption(Framework::Options::SubmenuOption("Syncs")
			.setTarget("entity-events"));

		core->addOption(Framework::Options::SubmenuOption("Flags")
			.setTarget("protection-flags"));

		core->addOption(Framework::Options::ToggleOption("Block Join")
			.addToggle(&m_Vars.m_BlockJoin));

		core->addOption(Framework::Options::ToggleOption("Block All Script Events")
			.addToggle(&m_Vars.m_BlockAllScriptEvents));

	});


	Framework::addSubmenu("Flags", "protection-flags", [=](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Godmode")
			.addToggle(&m_Vars.m_Godmode)
			.addTooltip("Disables modders seeing you're in godmode"));

		core->addOption(Framework::Options::ToggleOption("Super Jump")
			.addToggle(&m_Vars.m_SuperJump)
			.addTooltip("Disables modders seeing you're using super jump"));

		core->addOption(Framework::Options::ToggleOption("Health")
			.addToggle(&m_Vars.m_Health)
			.addTooltip("Disables modders seeing you have abnormal health"));

		core->addOption(Framework::Options::ToggleOption("Armor")
			.addToggle(&m_Vars.m_Armor)
			.addTooltip("Disables modders seeing you have abnormal armor"));

		core->addOption(Framework::Options::ToggleOption("Spectate")
			.addToggle(&m_Vars.m_Spectate)
			.addTooltip("Disables modders seeing you're spectating someone"));
	});


	Framework::addSubmenu("Syncs", "entity-events", [=](Framework::Options::Core* core) {
		core->addOption(Framework::Options::SubmenuOption("Entity Blocking")
			.setTarget("entity-blocking"));
		
		core->addOption(Framework::Options::ToggleOption("Block Incoming Syncs")
			.addToggle(&m_Vars.m_BlockIncomingSyncs));

		core->addOption(Framework::Options::ToggleOption("Block Outgoing Syncs")
			.addToggle(&m_Vars.m_BlockOutGoingSyncs));

		core->addOption(Framework::Options::ToggleOption("Block Modder Sync")
			.addToggle(&m_Vars.m_BlockModderSync));

		core->addOption(Framework::Options::ToggleOption("Block Modder Create")
			.addToggle(&m_Vars.m_BlockModderCreate));
	});

	Framework::addSubmenu("Entity Blocking", "entity-blocking", [=](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Automobile")
			.addToggle(&m_Vars.m_EntityLimits[0]));

		core->addOption(Framework::Options::ToggleOption("Bike")
			.addToggle(&m_Vars.m_EntityLimits[1]));

		core->addOption(Framework::Options::ToggleOption("Boat")
			.addToggle(&m_Vars.m_EntityLimits[2]));

		core->addOption(Framework::Options::ToggleOption("Door")
			.addToggle(&m_Vars.m_EntityLimits[3]));

		core->addOption(Framework::Options::ToggleOption("Heli")
			.addToggle(&m_Vars.m_EntityLimits[4]));

		core->addOption(Framework::Options::ToggleOption("Object")
			.addToggle(&m_Vars.m_EntityLimits[5]));

		core->addOption(Framework::Options::ToggleOption("Ped")
			.addToggle(&m_Vars.m_EntityLimits[6]));

		core->addOption(Framework::Options::ToggleOption("Pickup")
			.addToggle(&m_Vars.m_EntityLimits[7]));

		core->addOption(Framework::Options::ToggleOption("Pickup Placement")
			.addToggle(&m_Vars.m_EntityLimits[8]));

		core->addOption(Framework::Options::ToggleOption("Plane")
			.addToggle(&m_Vars.m_EntityLimits[9]));

		core->addOption(Framework::Options::ToggleOption("Submarine")
			.addToggle(&m_Vars.m_EntityLimits[10]));

		core->addOption(Framework::Options::ToggleOption("Player")
			.addToggle(&m_Vars.m_EntityLimits[11]));

		core->addOption(Framework::Options::ToggleOption("Trailer")
			.addToggle(&m_Vars.m_EntityLimits[12]));

		core->addOption(Framework::Options::ToggleOption("Train")
			.addToggle(&m_Vars.m_EntityLimits[13]));
	});


	Framework::addSubmenu("Events", "protection_events", [=](Framework::Options::Core* core) {
		for (auto& Script : Scripts) {
			core->addOption(Framework::Options::SubmenuOption(Script.first.c_str())
				.setTarget(Script.first.c_str()));
		}

		for (auto& Item : Items) {
			core->addOption(Framework::Options::SubmenuOption(Item.first.c_str())
				.setTarget(Item.first.c_str()));
		}
	});
}

void ProtectionMenu::Reports() {
	Framework::addSubmenu("Reports", "protection_reports", [=](Framework::Options::Core* core) {
		core->addOption(Framework::Options::SubmenuOption("Metrics")
			.addTooltip("All Reports Are Automatically Blocked And Are Prevented From Being Sent").setTarget("reports_metric"));

		core->addOption(Framework::Options::SubmenuOption("Bonus")
			.addTooltip("All Reports Are Automatically Blocked And Are Prevented From Being Sent").setTarget("reports_bonus"));

		core->addOption(Framework::Options::SubmenuOption("Arxan")
			.addTooltip("All Reports Are Automatically Blocked And Are Prevented From Being Sent").setTarget("reports_arxan"));

		core->addOption(Framework::Options::SubmenuOption("RAC")
			.addTooltip("All Reports Are Automatically Blocked And Are Prevented From Being Sent").setTarget("reports_rac"));
	});



	Framework::addSubmenu("Arxan", "reports_arxan", [=](Framework::Options::Core* core) {

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_ArxanNotify));


		core->addOption(Framework::Options::ToggleOption("Log")
			.addToggle(&m_Vars.m_ArxanNotify));
	});

	Framework::addSubmenu("Bonus", "reports_bonus", [=](Framework::Options::Core* core) {

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_BonusNotify));


		core->addOption(Framework::Options::ToggleOption("Log")
			.addToggle(&m_Vars.m_BonusNotify));
	});

	Framework::addSubmenu("RAC", "reports_rac", [=](Framework::Options::Core* core) {

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_RacNotify));


		core->addOption(Framework::Options::ToggleOption("Log")
			.addToggle(&m_Vars.m_RacLog));
		});

	Framework::addSubmenu("Metrics", "reports_metric", [=](Framework::Options::Core* core) {

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_MetricNotify));


		core->addOption(Framework::Options::ToggleOption("Log")
			.addToggle(&m_Vars.m_MetricLog));
	});
}

void ProtectionMenu::Events() {
	Framework::addSubmenu("Bounty", "Bounty", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Block")
			.addToggle(&m_Vars.m_Bounty.m_Block));

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_Bounty.m_Notify));

		if (m_Vars.m_Bounty.m_CanRedirect) {
			core->addOption(Framework::Options::ToggleOption("Redirect")
				.addToggle(&m_Vars.m_Bounty.m_Redirect));
		}
	});

	Framework::addSubmenu("Teleport", "Teleport", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Block")
			.addToggle(&m_Vars.m_Teleport.m_Block));

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_Teleport.m_Notify));

		if (m_Vars.m_Teleport.m_CanRedirect) {
			core->addOption(Framework::Options::ToggleOption("Redirect")
				.addToggle(&m_Vars.m_Teleport.m_Redirect));
		}
	});

	Framework::addSubmenu("Destroy Personal Vehicle", "Destroy Personal Vehicle", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Block")
			.addToggle(&m_Vars.m_DestroyPersonalVehicle.m_Block));

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_DestroyPersonalVehicle.m_Notify));

		if (m_Vars.m_DestroyPersonalVehicle.m_CanRedirect) {
			core->addOption(Framework::Options::ToggleOption("Redirect")
				.addToggle(&m_Vars.m_DestroyPersonalVehicle.m_Redirect));
		}
	});
	
	Framework::addSubmenu("Insurance Message", "Insurance Message", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Block")
			.addToggle(&m_Vars.m_InsuranceMessage.m_Block));

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_InsuranceMessage.m_Notify));

		if (m_Vars.m_InsuranceMessage.m_CanRedirect) {
			core->addOption(Framework::Options::ToggleOption("Redirect")
				.addToggle(&m_Vars.m_InsuranceMessage.m_Redirect));
		}
	});

	Framework::addSubmenu("Remote Off Radar", "Remote Off Radar", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Block")
			.addToggle(&m_Vars.m_RemoteOffRadar.m_Block));

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_RemoteOffRadar.m_Notify));

		if (m_Vars.m_RemoteOffRadar.m_CanRedirect) {
			core->addOption(Framework::Options::ToggleOption("Redirect")
				.addToggle(&m_Vars.m_RemoteOffRadar.m_Redirect));
		}
	});

	Framework::addSubmenu("Clear Wanted Level", "Clear Wanted Level", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Block")
			.addToggle(&m_Vars.m_ClearWantedLevel.m_Block));

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_ClearWantedLevel.m_Notify));

		if (m_Vars.m_ClearWantedLevel.m_CanRedirect) {
			core->addOption(Framework::Options::ToggleOption("Redirect")
				.addToggle(&m_Vars.m_ClearWantedLevel.m_Redirect));
		}
	});

	Framework::addSubmenu("Force To Mission", "Force To Mission", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Block")
			.addToggle(&m_Vars.m_ForceToMission.m_Block));

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_ForceToMission.m_Notify));

		if (m_Vars.m_ForceToMission.m_CanRedirect) {
			core->addOption(Framework::Options::ToggleOption("Redirect")
				.addToggle(&m_Vars.m_ForceToMission.m_Redirect));
		}
	});

	Framework::addSubmenu("Notification", "Notification", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Block")
			.addToggle(&m_Vars.m_Notification.m_Block));

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_Notification.m_Notify));

		if (m_Vars.m_Notification.m_CanRedirect) {
			core->addOption(Framework::Options::ToggleOption("Redirect")
				.addToggle(&m_Vars.m_Notification.m_Redirect));
		}
	});

	Framework::addSubmenu("Spectate Message", "Spectate Message", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Block")
			.addToggle(&m_Vars.m_SpectateMessage.m_Block));

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_SpectateMessage.m_Notify));

		if (m_Vars.m_SpectateMessage.m_CanRedirect) {
			core->addOption(Framework::Options::ToggleOption("Redirect")
				.addToggle(&m_Vars.m_SpectateMessage.m_Redirect));
		}
	});

	Framework::addSubmenu("Rotate Camera", "Rotate Camera", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Block")
			.addToggle(&m_Vars.m_RotateCam.m_Block));

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_RotateCam.m_Notify));

		if (m_Vars.m_RotateCam.m_CanRedirect) {
			core->addOption(Framework::Options::ToggleOption("Redirect")
				.addToggle(&m_Vars.m_RotateCam.m_Redirect));
		}
	});
	
	Framework::addSubmenu("Kick From Interior", "Kick From Interior", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Block")
			.addToggle(&m_Vars.m_KickFromInterior.m_Block));

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_KickFromInterior.m_Notify));

		if (m_Vars.m_KickFromInterior.m_CanRedirect) {
			core->addOption(Framework::Options::ToggleOption("Redirect")
				.addToggle(&m_Vars.m_KickFromInterior.m_Redirect));
		}
	});
	
	Framework::addSubmenu("Kick From Vehicle", "Kick From Vehicle", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Block")
			.addToggle(&m_Vars.m_VehicleKick.m_Block));

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_VehicleKick.m_Notify));

		if (m_Vars.m_VehicleKick.m_CanRedirect) {
			core->addOption(Framework::Options::ToggleOption("Redirect")
				.addToggle(&m_Vars.m_VehicleKick.m_Redirect));
		}
	});

	Framework::addSubmenu("Transaction Error", "Transaction Error", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Block")
			.addToggle(&m_Vars.m_TransactionError.m_Block));

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_TransactionError.m_Notify));

		if (m_Vars.m_TransactionError.m_CanRedirect) {
			core->addOption(Framework::Options::ToggleOption("Redirect")
				.addToggle(&m_Vars.m_TransactionError.m_Redirect));
		}
	});

	Framework::addSubmenu("Send To Cutscene", "Send To Cutscene", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Block")
			.addToggle(&m_Vars.m_ForceToCutscene.m_Block));

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_ForceToCutscene.m_Notify));

		if (m_Vars.m_ForceToCutscene.m_CanRedirect) {
			core->addOption(Framework::Options::ToggleOption("Redirect")
				.addToggle(&m_Vars.m_ForceToCutscene.m_Redirect));
		}
	});

	Framework::addSubmenu("Game Banner", "Game Banner", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Block")
			.addToggle(&m_Vars.m_Banner.m_Block));

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_Banner.m_Notify));

		if (m_Vars.m_Banner.m_CanRedirect) {
			core->addOption(Framework::Options::ToggleOption("Redirect")
				.addToggle(&m_Vars.m_Banner.m_Redirect));
		}
	});

	Framework::addSubmenu("Give Collectible", "Give Collectible", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Block")
			.addToggle(&m_Vars.m_GiveCollectible.m_Block));

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_GiveCollectible.m_Notify));

		if (m_Vars.m_GiveCollectible.m_CanRedirect) {
			core->addOption(Framework::Options::ToggleOption("Redirect")
				.addToggle(&m_Vars.m_GiveCollectible.m_Redirect));
		}
	});
	
	Framework::addSubmenu("Teleport To Warehouse", "Teleport To Warehouse", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Block")
			.addToggle(&m_Vars.m_TeleportToWarehouse.m_Block));

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_TeleportToWarehouse.m_Notify));

		if (m_Vars.m_TeleportToWarehouse.m_CanRedirect) {
			core->addOption(Framework::Options::ToggleOption("Redirect")
				.addToggle(&m_Vars.m_TeleportToWarehouse.m_Redirect));
		}
	});
	
	Framework::addSubmenu("Send To Island", "Send To Island", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Block")
			.addToggle(&m_Vars.m_SendToIsland.m_Block));

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_SendToIsland.m_Notify));

		if (m_Vars.m_SendToIsland.m_CanRedirect) {
			core->addOption(Framework::Options::ToggleOption("Redirect")
				.addToggle(&m_Vars.m_SendToIsland.m_Redirect));
		}
	});

	Framework::addSubmenu("Send To Clubhouse", "Send To Clubhouse", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Block")
			.addToggle(&m_Vars.m_SendToClubHouse.m_Block));

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_SendToClubHouse.m_Notify));

		if (m_Vars.m_SendToClubHouse.m_CanRedirect) {
			core->addOption(Framework::Options::ToggleOption("Redirect")
				.addToggle(&m_Vars.m_SendToClubHouse.m_Redirect));
		}
	});

	Framework::addSubmenu("Start Mission", "Start Mission", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Block")
			.addToggle(&m_Vars.m_ForceToActivity.m_Block));

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_ForceToActivity.m_Notify));

		if (m_Vars.m_ForceToActivity.m_CanRedirect) {
			core->addOption(Framework::Options::ToggleOption("Redirect")
				.addToggle(&m_Vars.m_ForceToActivity.m_Redirect));
		}
	});

	Framework::addSubmenu("CEO Money", "CEO Money", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Block")
			.addToggle(&m_Vars.m_CeoMoney.m_Block));

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_CeoMoney.m_Notify));

		if (m_Vars.m_CeoMoney.m_CanRedirect) {
			core->addOption(Framework::Options::ToggleOption("Redirect")
				.addToggle(&m_Vars.m_CeoMoney.m_Redirect));
		}
	});

	Framework::addSubmenu("CEO Kick", "CEO Kick", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Block")
			.addToggle(&m_Vars.m_CeoKick.m_Block));

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_CeoKick.m_Notify));

		if (m_Vars.m_CeoKick.m_CanRedirect) {
			core->addOption(Framework::Options::ToggleOption("Redirect")
				.addToggle(&m_Vars.m_CeoKick.m_Redirect));
		}
	});

	Framework::addSubmenu("Kick Event", "Kick Event", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Block")
			.addToggle(&m_Vars.m_KickEvent.m_Block));

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_KickEvent.m_Notify));

		if (m_Vars.m_KickEvent.m_CanRedirect) {
			core->addOption(Framework::Options::ToggleOption("Redirect")
				.addToggle(&m_Vars.m_KickEvent.m_Redirect));
		}
	});

	Framework::addSubmenu("Crash Event", "Crash Event", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Block")
			.addToggle(&m_Vars.m_CrashEvent.m_Block));

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_CrashEvent.m_Notify));

		if (m_Vars.m_CrashEvent.m_CanRedirect) {
			core->addOption(Framework::Options::ToggleOption("Redirect")
				.addToggle(&m_Vars.m_CrashEvent.m_Redirect));
		}
	});

	Framework::addSubmenu("Ragdoll", "Ragdoll", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Block")
			.addToggle(&m_Vars.m_Ragdoll.m_Block));

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_Ragdoll.m_Notify));

		if (m_Vars.m_Ragdoll.m_CanRedirect) {
			core->addOption(Framework::Options::ToggleOption("Redirect")
				.addToggle(&m_Vars.m_Ragdoll.m_Redirect));
		}
	});

	Framework::addSubmenu("Sound", "Sound", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Block")
			.addToggle(&m_Vars.m_Sound.m_Block));

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_Sound.m_Notify));

		if (m_Vars.m_Sound.m_CanRedirect) {
			core->addOption(Framework::Options::ToggleOption("Redirect")
				.addToggle(&m_Vars.m_Sound.m_Redirect));
		}
	});

	Framework::addSubmenu("Control", "Control", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Block")
			.addToggle(&m_Vars.m_Control.m_Block));

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_Control.m_Notify));

		if (m_Vars.m_Control.m_CanRedirect) {
			core->addOption(Framework::Options::ToggleOption("Redirect")
				.addToggle(&m_Vars.m_Control.m_Redirect));
		}
	});

	Framework::addSubmenu("Freeze", "Freeze", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Block")
			.addToggle(&m_Vars.m_Freeze.m_Block));

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_Freeze.m_Notify));

		if (m_Vars.m_Freeze.m_CanRedirect) {
			core->addOption(Framework::Options::ToggleOption("Redirect")
				.addToggle(&m_Vars.m_Freeze.m_Redirect));
		}
	});

	Framework::addSubmenu("Remove All Weapon", "Remove All Weapon", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Block")
			.addToggle(&m_Vars.m_RemoveAllWeapons.m_Block));

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_RemoveAllWeapons.m_Notify));

		if (m_Vars.m_RemoveAllWeapons.m_CanRedirect) {
			core->addOption(Framework::Options::ToggleOption("Redirect")
				.addToggle(&m_Vars.m_RemoveAllWeapons.m_Redirect));
		}
	});

	Framework::addSubmenu("Remove Weapon", "Remove Weapon", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Block")
			.addToggle(&m_Vars.m_RemoveWeapon.m_Block));

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_RemoveWeapon.m_Notify));

		if (m_Vars.m_RemoveWeapon.m_CanRedirect) {
			core->addOption(Framework::Options::ToggleOption("Redirect")
				.addToggle(&m_Vars.m_RemoveWeapon.m_Redirect));
		}
	});

	Framework::addSubmenu("Give Weapon", "Give Weapon", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Block")
			.addToggle(&m_Vars.m_GiveWeapon.m_Block));

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_GiveWeapon.m_Notify));

		if (m_Vars.m_GiveWeapon.m_CanRedirect) {
			core->addOption(Framework::Options::ToggleOption("Redirect")
				.addToggle(&m_Vars.m_GiveWeapon.m_Redirect));
		}
	});

	Framework::addSubmenu("PTFX", "PTFX", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Block")
			.addToggle(&m_Vars.m_PTFX.m_Block));

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_PTFX.m_Notify));

		if (m_Vars.m_PTFX.m_CanRedirect) {
			core->addOption(Framework::Options::ToggleOption("Redirect")
				.addToggle(&m_Vars.m_PTFX.m_Redirect));
		}
	});

	Framework::addSubmenu("Fire", "Fire", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Block")
			.addToggle(&m_Vars.m_Fire.m_Block));

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_Fire.m_Notify));

		if (m_Vars.m_Fire.m_CanRedirect) {
			core->addOption(Framework::Options::ToggleOption("Redirect")
				.addToggle(&m_Vars.m_Fire.m_Redirect));
		}
	});

	Framework::addSubmenu("Explosion", "Explosion", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Block")
			.addToggle(&m_Vars.m_Explosion.m_Block));

		core->addOption(Framework::Options::ToggleOption("Notify")
			.addToggle(&m_Vars.m_Explosion.m_Notify));

		if (m_Vars.m_Explosion.m_CanRedirect) {
			core->addOption(Framework::Options::ToggleOption("Redirect")
				.addToggle(&m_Vars.m_Explosion.m_Redirect));
		}
	});
}

void ProtectionMenu::Update() {
	Run();
	Events();
	Reports();
}