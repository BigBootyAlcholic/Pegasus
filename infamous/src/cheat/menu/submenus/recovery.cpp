#include "stdafx.hpp"
#include "recovery.hpp"
#include "cheat/util/input.hpp"
#include "core/patterns.hpp"
#include "cheat/util/global.hpp"
#include "cheat/util/control_manager.hpp"
#include "util/fiber.hpp"
#include "util/fiber_pool.hpp"
#include "cheat/util/shop_manager.hpp"
#include "cheat/util/stats.hpp"
using namespace RecoveryMenuVars;

namespace RecoveryMenuVars {
	Vars_ m_Vars;


	const char* CharNames[] = {
		"PLACEHOLDER",
		"PLACEHOLDER"
	};

	int GetCharacterCount() {
		const char* CharacterTwo = Native::StatGetString(0xD2AB0EC6, -1);
		if (!CharacterTwo || strlen(CharacterTwo) < 1) return 1;
		return 2;
	}

	void fleeca_job() {
		Menu::NetShoppingManager::ProcessBasketTransaction(joaat("CATEGORY_SERVICE_WITH_THRESHOLD"), 1445302971, 4,
			{
				{ (int)joaat("SERVICE_EARN_JOB_BONUS"), 1, 15000000, 0, 1},
			});
	}

	void bend_job() {
		Menu::NetShoppingManager::ProcessBasketTransaction(joaat("CATEGORY_SERVICE_WITH_THRESHOLD"), 1445302971, 4,
			{
				{ (int)joaat("SERVICE_EARN_BEND_JOB"), 1, 15000000, 0, 1},
			});
	}

	void gang_ops() {
		Menu::NetShoppingManager::ProcessBasketTransaction(joaat("CATEGORY_SERVICE_WITH_THRESHOLD"), 1445302971, 4,
			{
				{ (int)joaat("SERVICE_EARN_GANGOPS_AWARD_MASTERMIND_4"), 1, 7000000, 0, 1},
			});
	}

	void premium_job() {
		Menu::NetShoppingManager::ProcessBasketTransaction(joaat("CATEGORY_SERVICE_WITH_THRESHOLD"), 1445302971, 4,
			{
				{ (int)joaat("SERVICE_EARN_CASINO_HEIST_FINALE"), 1, 3619000, 0, 1},
			});
	}

	void job_bonus() {
		Menu::NetShoppingManager::ProcessBasketTransaction(joaat("CATEGORY_SERVICE_WITH_THRESHOLD"), 1445302971, 4,
			{
				{ (int)joaat("SERVICE_EARN_JOB_BONUS_HEIST_AWARD"), 1, 2000000, 0, 1 },
			});
	}

	void tuner_robbery_finale() {
		Menu::NetShoppingManager::ProcessBasketTransaction(joaat("CATEGORY_SERVICE_WITH_THRESHOLD"), 1445302971, 4,
			{
				{ (int)joaat("SERVICE_EARN_TUNER_ROBBERY_FINALE"), 1, 2000000, 0, 1 },
			});
	}

	void UnlockMisc() {
		Stats::Interface<int>::Set("CHAR_HEIST_1_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_ABILITY_1_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_ABILITY_2_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_ABILITY_3_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_FM_HEALTH_1_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_FM_HEALTH_2_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_CREWUNLOCK_1_UNLCK", -1);
	}

	void UnlockHeistVehicles() {
		Stats::Interface<int>::Set("CHAR_FM_VEHICLE_1_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_FM_VEHICLE_2_UNLCK", -1);
	}

	void UnlockWeapons() {
		Stats::Interface<int>::Set("CHAR_WEAP_UNLOCKED", -1);
		Stats::Interface<int>::Set("CHAR_WEAP_UNLOCKED2", -1);
		Stats::Interface<int>::Set("CHAR_FM_WEAP_UNLOCKED", -1);
		Stats::Interface<int>::Set("CHAR_FM_WEAP_UNLOCKED2", -1);
		Stats::Interface<int>::Set("CHAR_FM_WEAP_UNLOCKED3", -1);
		Stats::Interface<int>::Set("CHAR_WEAP_ADDON_1_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_WEAP_ADDON_2_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_WEAP_ADDON_3_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_WEAP_ADDON_4_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_FM_WEAP_ADDON_1_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_FM_WEAP_ADDON_2_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_FM_WEAP_ADDON_3_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_FM_WEAP_ADDON_4_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_FM_WEAP_ADDON_5_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_FM_WEAP_ADDON_6_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_FM_WEAP_ADDON_7_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_FM_WEAP_ADDON_8_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_FM_WEAP_ADDON_9_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_FM_WEAP_ADDON_10_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_FM_WEAP_ADDON_11_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_FM_WEAP_ADDON_12_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_FM_WEAP_ADDON_13_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_FM_WEAP_ADDON_14_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_FM_WEAP_ADDON_15_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_FM_WEAP_ADDON_16_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_FM_WEAP_ADDON_17_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_FM_WEAP_ADDON_18_UNLCK", -1);
		Stats::Interface<int>::Set("GRENADE_ENEMY_KILLS", 50);
		Stats::Interface<int>::Set("AWD_20_KILLS_MELEE", 50);
		Stats::Interface<int>::Set("CMBTPISTOL_ENEMY_KILLS", 600);
		Stats::Interface<int>::Set("PISTOL50_ENEMY_KILLS", 600);
		Stats::Interface<int>::Set("APPISTOL_ENEMY_KILLS", 600);
		Stats::Interface<int>::Set("MICROSMG_ENEMY_KILLS", 600);
		Stats::Interface<int>::Set("SMG_ENEMY_KILLS", 600);
		Stats::Interface<int>::Set("ASLTSMG_ENEMY_KILLS", 600);
		Stats::Interface<int>::Set("ASLTRIFLE_ENEMY_KILLS", 600);
		Stats::Interface<int>::Set("CRBNRIFLE_ENEMY_KILLS", 600);
		Stats::Interface<int>::Set("ADVRIFLE_ENEMY_KILLS", 600);
		Stats::Interface<int>::Set("MG_ENEMY_KILLS", 600);
		Stats::Interface<int>::Set("CMBTMG_ENEMY_KILLS", 600);
		Stats::Interface<int>::Set("ASLTMG_ENEMY_KILLS", 600);
		Stats::Interface<int>::Set("PUMP_ENEMY_KILLS", 600);
		Stats::Interface<int>::Set("SAWNOFF_ENEMY_KILLS", 600);
		Stats::Interface<int>::Set("BULLPUP_ENEMY_KILLS", 600);
		Stats::Interface<int>::Set("ASLTSHTGN_ENEMY_KILLS", 600);
		Stats::Interface<int>::Set("SNIPERRFL_ENEMY_KILLS", 600);
		Stats::Interface<int>::Set("HVYSNIPER_ENEMY_KILLS", 600);
		Stats::Interface<int>::Set("GRNLAUNCH_ENEMY_KILLS", 600);
		Stats::Interface<int>::Set("RPG_ENEMY_KILLS", 600);
		Stats::Interface<int>::Set("MINIGUNS_ENEMY_KILLS", 600);
		Stats::Interface<int>::Set("SMKGRENADE_ENEMY_KILLS", 600);
		Stats::Interface<int>::Set("STKYBMB_ENEMY_KILLS", 600);
		Stats::Interface<int>::Set("MOLOTOV_ENEMY_KILLS", 600);
	}

	void UnlockClothes() {
		Stats::Interface<int>::Set("CHAR_FM_CLOTHES_1_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_FM_CLOTHES_2_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_FM_CLOTHES_3_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_FM_CLOTHES_4_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_FM_CLOTHES_5_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_FM_CLOTHES_6_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_FM_CLOTHES_7_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_FM_CLOTHES_8_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_FM_CLOTHES_9_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_FM_CLOTHES_10_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_FM_CLOTHES_11_UNLCK", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_FEET_1", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_JBIB", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_JBIB_1", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_JBIB_2", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_JBIB_3", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_JBIB_4", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_JBIB_5", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_JBIB_6", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_JBIB_7", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_LEGS", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_LEGS_1", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_LEGS_2", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_LEGS_3", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_LEGS_4", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_LEGS_5", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_LEGS_6", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_LEGS_7", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_FEET", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_FEET_1", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_FEET_2", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_FEET_3", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_FEET_4", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_FEET_5", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_FEET_6", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_FEET_7", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_BERD", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_BERD_1", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_BERD_2", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_BERD_3", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_BERD_4", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_BERD_5", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_BERD_6", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_BERD_7", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_PROPS", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_PROPS_1", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_PROPS_2", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_PROPS_3", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_PROPS_4", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_PROPS_5", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_PROPS_6", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_PROPS_7", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_PROPS_8", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_PROPS_9", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_PROPS_10", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_OUTFIT", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_HAIR", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_HAIR_1", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_HAIR_2", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_HAIR_3", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_HAIR_4", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_HAIR_5", -1);;
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_HAIR_6", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_HAIR_7", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_JBIB", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_JBIB_1", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_JBIB_2", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_JBIB_3", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_JBIB_4", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_JBIB_5", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_JBIB_6", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_JBIB_7", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_LEGS", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_LEGS_1", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_LEGS_2", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_LEGS_3", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_LEGS_4", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_LEGS_5", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_LEGS_6", -1);;
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_LEGS_7", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_FEET", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_FEET_1", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_FEET_2", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_FEET_3", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_FEET_4", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_FEET_5", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_FEET_6", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_FEET_7", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_BERD", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_BERD_1", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_BERD_2", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_BERD_3", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_BERD_4", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_BERD_5", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_BERD_6", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_BERD_7", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_PROPS", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_PROPS_1", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_PROPS_2", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_PROPS_3", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_PROPS_4", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_PROPS_5", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_PROPS_6", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_PROPS_7", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_PROPS_8", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_PROPS_9", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_PROPS_10", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_OUTFIT", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_TORSO", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_SPECIAL", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_SPECIAL_1", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_SPECIAL_2", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_SPECIAL_3", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_SPECIAL_4", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_SPECIAL_5", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_SPECIAL_6", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_SPECIAL_7", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_SPECIAL2", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_SPECIAL2_1", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_DECL", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_TEETH", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_TEETH_1", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_TEETH_2", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_TORSO", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_SPECIAL", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_SPECIAL_1", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_SPECIAL_2", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_SPECIAL_3", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_SPECIAL_4", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_SPECIAL_5", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_SPECIAL_6", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_SPECIAL_7", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_SPECIAL2", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_SPECIAL2_1", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_DECL", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_TEETH", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_TEETH_1", -1);
		Stats::Interface<int>::Set("CLTHS_ACQUIRED_TEETH_2", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_0", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_1", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_2", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_3", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_4", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_5", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_6", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_7", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_8", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_9", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_10", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_11", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_12", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_13", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_14", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_15", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_16", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_17", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_18", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_19", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_21", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_22", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_23", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_24", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_24", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_25", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_26", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_27", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_28", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_29", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_30", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_31", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_32", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_33", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_34", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_35", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_36", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_37", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_38", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_39", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_40", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_41", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_42", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_43", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_44", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_45", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_46", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_47", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_48", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_49", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_50", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_51", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_52", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_53", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_54", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_55", -1);
		Stats::Interface<int>::Set("DLC_APPAREL_ACQUIRED_56", -1);
		Stats::Interface<int>::Set("ADMIN_CLOTHES_GV_BS_1", -1);
		Stats::Interface<int>::Set("ADMIN_CLOTHES_GV_BS_2", -1);
		Stats::Interface<int>::Set("ADMIN_CLOTHES_GV_BS_3", -1);
		Stats::Interface<int>::Set("ADMIN_CLOTHES_GV_BS_4", -1);
		Stats::Interface<int>::Set("ADMIN_CLOTHES_GV_BS_5", -1);
		Stats::Interface<int>::Set("ADMIN_CLOTHES_GV_BS_6", -1);
		Stats::Interface<int>::Set("ADMIN_CLOTHES_GV_BS_7", -1);
		Stats::Interface<int>::Set("ADMIN_CLOTHES_GV_BS_8", -1);
		Stats::Interface<int>::Set("ADMIN_CLOTHES_GV_BS_9", -1);
		Stats::Interface<int>::Set("ADMIN_CLOTHES_GV_BS_10", -1);
		Stats::Interface<int>::Set("ADMIN_CLOTHES_GV_BS_11", -1);
		Stats::Interface<int>::Set("ADMIN_CLOTHES_GV_BS_12", -1);
		Stats::Interface<int>::Set("ADMIN_CLOTHES_GV_BS_13", -1);
		Stats::Interface<int>::Set("ADMIN_CLOTHES_GV_BS_1", -1);
		Stats::Interface<int>::Set("ADMIN_CLOTHES_RM_BS_10", -1);
		Stats::Interface<int>::Set("ADMIN_CLOTHES_GV_BS_10", -1);
		Stats::Interface<int>::Set("ADMIN_CLOTHES_RM_BS_11", -1);
		Stats::Interface<int>::Set("ADMIN_CLOTHES_GV_BS_11", -1);
		Stats::Interface<int>::Set("ADMIN_CLOTHES_RM_BS_12", -1);
		Stats::Interface<int>::Set("ADMIN_CLOTHES_GV_BS_12", -1);
		Stats::Interface<int>::Set("AWD_FMHORDWAVESSURVIVE", 10);
		Stats::Interface<int>::Set("AWD_FMPICKUPDLCCRATE1ST", 1);
		Stats::Interface<int>::Set("AWD_WIN_CAPTURE_DONT_DYING", 25);
		Stats::Interface<int>::Set("AWD_DO_HEIST_AS_MEMBER", 25);
		Stats::Interface<int>::Set("AWD_PICKUP_CAP_PACKAGES", 100);
		Stats::Interface<bool>::Set("AWD_FINISH_HEIST_NO_DAMAGE", 1);
		Stats::Interface<int>::Set("AWD_WIN_GOLD_MEDAL_HEISTS", 25);
		Stats::Interface<int>::Set("AWD_KILL_TEAM_YOURSELF_LTS", 25);
		Stats::Interface<int>::Set("AWD_KILL_PSYCHOPATHS", 100);
		Stats::Interface<int>::Set("AWD_DO_HEIST_AS_THE_LEADER", 25);
		Stats::Interface<bool>::Set("AWD_STORE_20_CAR_IN_GARAGES", 1);
	}

	void UnlockHaircuts() {
		Stats::Interface<int>::Set("CHAR_HAIR_UNLCK1", -1);
		Stats::Interface<int>::Set("CHAR_HAIR_UNLCK2", -1);
		Stats::Interface<int>::Set("CHAR_HAIR_UNLCK3", -1);
		Stats::Interface<int>::Set("CHAR_HAIR_UNLCK4", -1);
		Stats::Interface<int>::Set("CHAR_HAIR_UNLCK5", -1);
		Stats::Interface<int>::Set("CHAR_HAIR_UNLCK6", -1);
		Stats::Interface<int>::Set("CHAR_HAIR_UNLCK7", -1);
		Stats::Interface<int>::Set("CHAR_HAIR_UNLCK8", -1);
		Stats::Interface<int>::Set("CHAR_HAIR_UNLCK9", -1);
		Stats::Interface<int>::Set("CHAR_HAIR_UNLCK10", -1);
		Stats::Interface<int>::Set("CHAR_HAIR_UNLCK11", -1);
		Stats::Interface<int>::Set("CHAR_HAIR_UNLCK12", -1);
		Stats::Interface<int>::Set("CHAR_HAIR_UNLCK13", -1);
		Stats::Interface<int>::Set("CHAR_HAIR_UNLCK14", -1);
		Stats::Interface<int>::Set("CHAR_HAIR_UNLCK15", -1);
		Stats::Interface<int>::Set("CHAR_HAIR_UNLCK16", -1);
		Stats::Interface<int>::Set("CHAR_FM_HAIRCUT_1_UNLCK", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_HAIR", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_HAIR1", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_HAIR2", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_HAIR3", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_HAIR4", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_HAIR5", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_HAIR6", -1);
		Stats::Interface<int>::Set("CLTHS_AVAILABLE_HAIR7", -1);
	}

	void UnlockTattoos() {
		Stats::Interface<bool>::Set("AWD_FMATTGANGHQ", true);
		Stats::Interface<bool>::Set("AWD_FMKILLSTREAKSDM", true);
		Stats::Interface<bool>::Set("AWD_FMWINEVERYGAMEMODE", true);
		Stats::Interface<bool>::Set("AWD_FMRACEWORLDRECHOLDER", true);
		Stats::Interface<bool>::Set("AWD_FMFULLYMODDEDCAR", true);
		Stats::Interface<bool>::Set("AWD_FMMOSTKILLSSURVIVE", true);
		Stats::Interface<bool>::Set("AWD_FMKILL3ANDWINGTARACE", true);
		Stats::Interface<int>::Set("AWD_FM_DM_WINS", 50);
		Stats::Interface<int>::Set("AWD_FM_TDM_MVP", 50);
		Stats::Interface<int>::Set("AWD_FM_DM_TOTALKILLS", 500);
		Stats::Interface<int>::Set("AWD_FMBBETWIN", 50000);
		Stats::Interface<int>::Set("AWD_FMKILLBOUNTY", 25);
		Stats::Interface<int>::Set("AWD_FMREVENGEKILLSDM", 50);
		Stats::Interface<int>::Set("AWD_HOLD_UP_SHOPS", 20);
		Stats::Interface<int>::Set("AWD_LAPDANCES", 25);
		Stats::Interface<int>::Set("AWD_SECURITY_CARS_ROBBED", 25);
		Stats::Interface<int>::Set("AWD_RACES_WON", 50);
		Stats::Interface<int>::Set("AWD_CAR_BOMBS_ENEMY_KILLS", 25);
		Stats::Interface<int>::Set("PLAYER_HEADSHOTS", 500);
		Stats::Interface<int>::Set("DB_PLAYER_KILLS", 1000);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_0", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_1", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_2", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_3", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_4", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_5", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_6", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_7", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_8", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_9", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_10", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_11", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_12", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_13", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_14", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_15", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_16", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_17", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_18", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_19", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_20", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_21", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_22", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_23", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_24", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_25", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_26", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_27", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_28", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_29", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_30", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_31", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_32", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_33", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_34", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_35", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_36", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_37", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_38", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_39", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_40", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_41", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_42", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_43", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_44", -1);
		Stats::Interface<int>::Set("TATTOO_FM_UNLOCKS_45", -1);
	}

	void UnlockCamoAndParachutes() {
		Stats::Interface<int>::Set("CHAR_KIT_1_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_2_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_3_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_4_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_5_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_6_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_7_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_8_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_9_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_10_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_11_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_12_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_13_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_14_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_15_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_16_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_17_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_18_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_19_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_20_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_21_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_22_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_23_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_24_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_25_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_26_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_27_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_28_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_29_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_30_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_31_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_32_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_33_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_34_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_35_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_36_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_37_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_38_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_39_FM_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_KIT_40_FM_UNLCK", -1);
	}

	void UnlockLosSantosCustoms() {
		Stats::Interface<int>::Set("RACES_WON", 50);
		Stats::Interface<int>::Set("CHAR_FM_CARMOD_1_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_FM_CARMOD_2_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_FM_CARMOD_3_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_FM_CARMOD_4_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_FM_CARMOD_5_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_FM_CARMOD_6_UNLCK", -1);
		Stats::Interface<int>::Set("CHAR_FM_CARMOD_7_UNLCK", -1);
		Stats::Interface<int>::Set("NUMBER_SLIPSTREAMS_IN_RACE", 110);
		Stats::Interface<int>::Set("NUMBER_TURBO_STARTS_IN_RACE", 90);
		Stats::Interface<int>::Set("USJS_FOUND", 50);
		Stats::Interface<int>::Set("USJS_COMPLETED", 50);
		Stats::Interface<int>::Set("AWD_FMRALLYWONDRIVE", 2);
		Stats::Interface<int>::Set("AWD_FMRALLYWONNAV", 2);
		Stats::Interface<int>::Set("AWD_FMWINSEARACE", 2);
		Stats::Interface<int>::Set("AWD_FMWINAIRRACE", 2);
		Stats::Interface<int>::Set("AWD_FM_RACES_FASTEST_LAP", 101);
		Stats::Interface<int>::Set("AWD_WIN_CAPTURES", 25);
		Stats::Interface<int>::Set("AWD_DROPOFF_CAP_PACKAGES", 100);
		Stats::Interface<int>::Set("AWD_KILL_CARRIER_CAPTURE", 100);
		Stats::Interface<int>::Set("AWD_FINISH_HEISTS", 50);
		Stats::Interface<int>::Set("AWD_FINISH_HEIST_SETUP_JOB", 50);
		Stats::Interface<int>::Set("AWD_NIGHTVISION_KILLS", 100);
		Stats::Interface<int>::Set("AWD_WIN_LAST_TEAM_STANDINGS", 50);
		Stats::Interface<int>::Set("AWD_ONLY_PLAYER_ALIVE_LTS", 50);
		Stats::Interface<int>::Set("AWD_ENEMYDRIVEBYKILLS", 600);
		Stats::Interface<int>::Set("DB_PLAYER_KILLS", 1000);
		Stats::Interface<int>::Set("KILLS_PLAYERS", 1000);
		Stats::Interface<int>::Set("AWD_FMHORDWAVESSURVIVE", 21);
		Stats::Interface<int>::Set("AWD_CAR_BOMBS_ENEMY_KILLS", 25);
		Stats::Interface<int>::Set("AWD_FM_TDM_MVP", 60);
		Stats::Interface<int>::Set("AWD_HOLD_UP_SHOPS", 20);
		Stats::Interface<int>::Set("AWD_RACES_WON", 101);
		Stats::Interface<int>::Set("AWD_NO_ARMWRESTLING_WINS", 21);
		Stats::Interface<int>::Set("AWD_FMBBETWIN", 50000);
		Stats::Interface<int>::Set("AWD_FM_DM_WINS", 51);
		Stats::Interface<int>::Set("AWD_FM_DM_TOTALKILLS", 500);
		Stats::Interface<int>::Set("MPPLY_DM_TOTAL_DEATHS", 412);
		Stats::Interface<int>::Set("MPPLY_TIMES_FINISH_DM_TOP_3", 36);
		Stats::Interface<int>::Set("PLAYER_HEADSHOTS", 623);
		Stats::Interface<int>::Set("AWD_FM_DM_WINS", 63);
		Stats::Interface<int>::Set("AWD_FM_TDM_WINS", 13);
		Stats::Interface<int>::Set("AWD_FM_GTA_RACES_WON", 12);
		Stats::Interface<int>::Set("AWD_FM_GOLF_WON", 2);
		Stats::Interface<int>::Set("AWD_FM_SHOOTRANG_TG_WON", 2);
		Stats::Interface<int>::Set("AWD_FM_SHOOTRANG_RT_WON", 2);
		Stats::Interface<int>::Set("AWD_FM_SHOOTRANG_CT_WON", 2);
		Stats::Interface<int>::Set("AWD_FM_SHOOTRANG_GRAN_WON", 2);
		Stats::Interface<int>::Set("AWD_FM_TENNIS_WON", 2);
		Stats::Interface<int>::Set("MPPLY_TENNIS_MATCHES_WON", 2);
		Stats::Interface<int>::Set("MPPLY_TOTAL_TDEATHMATCH_WON", 63);
		Stats::Interface<int>::Set("MPPLY_TOTAL_RACES_WON", 101);
		Stats::Interface<int>::Set("MPPLY_TOTAL_DEATHMATCH_LOST", 23);
		Stats::Interface<int>::Set("MPPLY_TOTAL_RACES_LOST", 36);
		Stats::Interface<int>::Set("AWD_25_KILLS_STICKYBOMBS", 50);
		Stats::Interface<int>::Set("AWD_50_KILLS_GRENADES", 50);
		Stats::Interface<int>::Set("GRENADE_ENEMY_KILLS", 50);
		Stats::Interface<int>::Set("AWD_20_KILLS_MELEE", 50);
		Stats::Interface<int>::Set("ARENAWARS_AP_TIER", 1000);
	}

	void UnlockClubPropularity() {
		Stats::Interface<int>::Set("CLUB_POPULARITY", 1000);
	}

	void UnlockBunkerResearch() {
		std::map<int, int> map({ { 0, 15381 },{ 1, 15382 },{ 2, 15428 },{ 3, 15429 },{ 4, 15430 },{ 5, 15431 },{ 6, 15491 },{ 7, 15432 },{ 8, 15433 },{ 9, 15434 },{ 10, 15435 },{ 11, 15436 },{ 12, 15437 },{ 13, 15438 },{ 14, 15439 },{ 15, 15447 },{ 16, 15448 },{ 17, 15449 },{ 18, 15450 },{ 19, 15451 },{ 20, 15452 },{ 21, 15453 },{ 22, 15454 },{ 23, 15455 },{ 24, 15456 },{ 25, 15457 },{ 26, 15458 },{ 27, 15459 },{ 28, 15460 },{ 29, 15461 },{ 30, 15462 },{ 31, 15463 },{ 32, 15464 },{ 33, 15465 },{ 34, 15466 },{ 35, 15467 },{ 36, 15468 },{ 37, 15469 },{ 38, 15470 },{ 39, 15471 },{ 40, 15472 },{ 41, 15473 },{ 42, 15474 },{ 43, 15492 },{ 44, 15493 },{ 45, 15494 },{ 46, 15495 },{ 47, 15496 },{ 48, 15497 },{ 49, 15498 },{ 50, 15499 } });

		for (auto entry : map) {
			const auto hash = Native::_GetNgstatBoolHash(entry.second - 15369, 0, 1, m_Vars.m_SelectedCharacter, "_DLCGUNPSTAT_BOOL");
			const auto mask = ((entry.second - 15369) - Native::_0xF4D8E7AC2A27758C(entry.second - 15369) * 64);
			Native::StatSetBoolMasked(hash, true, mask, true);
		}

		Stats::Interface<int>::Set("GUNR_RESEARCH_ITEM", -1);
	}

	void UnlockMaxAbilities() {
		Stats::Interface<int>::Set("SCRIPT_INCREASE_STAM", 100);
		Stats::Interface<int>::Set("SCRIPT_INCREASE_STRN", 100);
		Stats::Interface<int>::Set("SCRIPT_INCREASE_LUNG", 100);
		Stats::Interface<int>::Set("SCRIPT_INCREASE_DRIV", 100);
		Stats::Interface<int>::Set("SCRIPT_INCREASE_FLY", 100);
		Stats::Interface<int>::Set("SCRIPT_INCREASE_SHO", 100);
		Stats::Interface<int>::Set("SCRIPT_INCREASE_STL", 100);
	}

	void UnlockMaxSnacks() {
		Stats::Interface<int>::Set("NO_BOUGHT_YUM_SNACKS", 30);
		Stats::Interface<int>::Set("NO_BOUGHT_HEALTH_SNACKS", 15);
		Stats::Interface<int>::Set("NO_BOUGHT_EPIC_SNACKS", 5);
		Stats::Interface<int>::Set("NUMBER_OF_CHAMP_BOUGHT", 5);
		Stats::Interface<int>::Set("NUMBER_OF_ORANGE_BOUGHT", 10);
		Stats::Interface<int>::Set("NUMBER_OF_BOURGE_BOUGHT", 10);
		Stats::Interface<int>::Set("CIGARETTES_BOUGHT", 20);
	}

	void UnlockMaxArmor() {
		Stats::Interface<int>::Set("MP_CHAR_ARMOUR_1_COUNT", 10);
		Stats::Interface<int>::Set("MP_CHAR_ARMOUR_2_COUNT", 10);
		Stats::Interface<int>::Set("MP_CHAR_ARMOUR_3_COUNT", 10);
		Stats::Interface<int>::Set("MP_CHAR_ARMOUR_4_COUNT", 10);
		Stats::Interface<int>::Set("MP_CHAR_ARMOUR_5_COUNT", 10);
	}

	void UnlockMaxFireworks() {
		Stats::Interface<int>::Set("FIREWORK_TYPE_1_WHITE", 2000000);
		Stats::Interface<int>::Set("FIREWORK_TYPE_1_RED", 2000000);
		Stats::Interface<int>::Set("FIREWORK_TYPE_1_BLUE", 2000000);
		Stats::Interface<int>::Set("FIREWORK_TYPE_2_WHITE", 2000000);
		Stats::Interface<int>::Set("FIREWORK_TYPE_2_RED", 2000000);
		Stats::Interface<int>::Set("FIREWORK_TYPE_2_BLUE", 2000000);
		Stats::Interface<int>::Set("FIREWORK_TYPE_3_WHITE", 2000000);
		Stats::Interface<int>::Set("FIREWORK_TYPE_3_RED", 2000000);
		Stats::Interface<int>::Set("FIREWORK_TYPE_3_BLUE", 2000000);
		Stats::Interface<int>::Set("FIREWORK_TYPE_4_WHITE", 2000000);
		Stats::Interface<int>::Set("FIREWORK_TYPE_4_RED", 2000000);
		Stats::Interface<int>::Set("FIREWORK_TYPE_4_BLUE", 2000000);
	}

	void UnlockAll() {
		UnlockMisc();
		UnlockHeistVehicles();
		UnlockWeapons();
		UnlockClothes();
		UnlockHaircuts();
		UnlockTattoos();
		UnlockCamoAndParachutes();
		UnlockLosSantosCustoms();
		UnlockClubPropularity();
		UnlockBunkerResearch();
		UnlockMaxAbilities();
		UnlockMaxSnacks();
		UnlockMaxArmor();
		UnlockMaxFireworks();
	}
}



void RecoveryMenu::Run() {
	Framework::addSubmenu("Recovery", "recovery-menu", [](Framework::Options::Core* core) {
		bool init = false;
		if (!init) {
			int CurrentChar = 0;
			Native::StatGetInt(0x2F2F120F, &CurrentChar, -1);

			m_Vars.m_SelectedCharacter = CurrentChar ? 1 : 0;



			init = true;
		}

		core->addOption(Framework::Options::scrollOption<const char*, int>("Selected Character")
			.addScroll(&CharNames).setPosition(&m_Vars.m_SelectedCharacter));

		core->addOption(Framework::Options::SubmenuOption("Money")
			.setTarget("recovery-money"));

		core->addOption(Framework::Options::SubmenuOption("Unlocks")
			.setTarget("recovery-unlocks"));

		core->addOption(Framework::Options::ButtonOption("Redesign Character")
			.addClick([] { Stats::Interface<bool>::Set("FM_CHANGECHAR_ASKED", false); }));
	});

	Framework::addSubmenu("Unlocks", "recovery-unlocks", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ButtonOption("Unlock All")
			.addClick(UnlockAll));

		core->addOption(Framework::Options::ButtonOption("Tattoos")
			.addClick(UnlockTattoos));

		core->addOption(Framework::Options::ButtonOption("Clothes")
			.addClick(UnlockClothes));

		core->addOption(Framework::Options::ButtonOption("Haircuts")
			.addClick(UnlockHaircuts));

		core->addOption(Framework::Options::ButtonOption("Weapons")
			.addClick(UnlockWeapons));

		core->addOption(Framework::Options::ButtonOption("Camo and Parachutes")
			.addClick(UnlockCamoAndParachutes));

		core->addOption(Framework::Options::ButtonOption("Los Santos Customs")
			.addClick(UnlockLosSantosCustoms));

		core->addOption(Framework::Options::ButtonOption("Heist Vehicles")
			.addClick(UnlockHeistVehicles));

		core->addOption(Framework::Options::ButtonOption("Club Propularity")
			.addClick(UnlockClubPropularity));

		core->addOption(Framework::Options::ButtonOption("Bunker Research")
			.addClick(UnlockBunkerResearch));

		core->addOption(Framework::Options::ButtonOption("Abilities")
			.addClick(UnlockMaxAbilities));

		core->addOption(Framework::Options::ButtonOption("Snacks")
			.addClick(UnlockMaxSnacks));

		core->addOption(Framework::Options::ButtonOption("Armor")
			.addClick(UnlockMaxArmor));

		core->addOption(Framework::Options::ButtonOption("Fireworks")
			.addClick(UnlockMaxFireworks));

		core->addOption(Framework::Options::ButtonOption("Misc")
			.addClick(UnlockMisc));
	});

	Framework::addSubmenu("Recovery", "recovery-money", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("$1m Loop")
			.addToggle(&m_Vars.m_MillionsLoop));

		core->addOption(Framework::Options::ToggleOption("$50k Loop")
			.addToggle(&m_Vars.m_KLoop));

		core->addOption(Framework::Options::BreakOption("Transactions"));

		core->addOption(Framework::Options::ButtonOption("$15m (Bend Job)")
			.addClick([] { bend_job(); }));

		core->addOption(Framework::Options::ButtonOption("$15m (Fleeca Job)")
			.addClick([] { fleeca_job(); }));

		core->addOption(Framework::Options::ButtonOption("$7m (Gang Operations)")
			.addClick([] { gang_ops(); }));

		core->addOption(Framework::Options::ButtonOption("$3.61m (Casino Heist Finale)")
			.addClick([] { premium_job(); }));

		core->addOption(Framework::Options::ButtonOption("$2m (Job Bonus)")
			.addClick([] { job_bonus(); }));

		core->addOption(Framework::Options::ButtonOption("$2m (Tuner Robbery Finale)")
			.addClick([] { tuner_robbery_finale(); }));
	});
}

void RecoveryMenu::Update() {
	Run();

	static bool once = false;
	if (!once) {
		CharNames[0] = (Native::StatGetString(0x4A211FC8, -1));
		CharNames[1] = (Native::StatGetString(0xD2AB0EC6, -1));

		if (!Native::NetworkIsInSession()) {
			CharNames[0] = ("~c~ Unavailable");
			CharNames[1] = ("~c~ Unavailable");
		}
		once = true;
	}

	if (m_Vars.m_MillionsLoop) {
		static Timer LoopTimer(650ms);
		if (LoopTimer.Update()) {
			Menu::NetShoppingManager::ProcessServiceTransaction(joaat("CATEGORY_SERVICE_WITH_THRESHOLD"), 1633116913, joaat("NET_SHOP_ACTION_EARN"), 1000000, 4);
		}
	}

	if (m_Vars.m_MillionsLoopV2) {
		static Timer LoopTimer(650ms);
		if (LoopTimer.Update()) {
			Menu::NetShoppingManager::ProcessServiceTransaction(joaat("CATEGORY_SERVICE_WITH_THRESHOLD"), 312105838, joaat("NET_SHOP_ACTION_EARN"), 10000000, 4);
		}
	}

	if (m_Vars.m_KLoop) {
		static Timer LoopTimer(650ms);
		if (LoopTimer.Update()) {
			Menu::NetShoppingManager::ProcessServiceTransaction(joaat("CATEGORY_SERVICE_WITH_THRESHOLD"), 1628412596, joaat("NET_SHOP_ACTION_EARN"), 50000, 4);
		}
	}
}