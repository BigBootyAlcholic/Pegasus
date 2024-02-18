#include "stdafx.hpp"
#include "player.hpp"
#include "cheat/util/input.hpp"
#include "core/patterns.hpp"
#include "cheat/util/global.hpp"
#include "cheat/util/control_manager.hpp"
#include "util/fiber.hpp"
#include "util/fiber_pool.hpp"
using namespace PlayerMenuVars;

namespace PlayerMenuVars {
	Vars_ m_Vars;

	void start_particle(const char* dict, const char* texture, bool toggle) {
		if (toggle) {
			Native::RequestNamedPtfxAsset(dict);
			Native::_UseParticleFxAssetNextCall(dict);
			Native::StartParticleFxNonLoopedOnEntity(texture, Native::PlayerPedId(),  0, 0, 0 ,  0, 0, 0, 0.5f, 0, 0, 0);
		}

	}

	struct particle { const char* name; const char* dict; const char* texture; };

	std::vector<particle> ParticleList = {
		{"Purple Cloud 1", "scr_rcbarry2", "scr_clown_appears" },
		{"Purple Cloud 2", "scr_rcbarry2", "scr_clown_death" },
		{"Purple Cloud 3", "scr_rcbarry2", "scr_exp_clown" },
		{"Firework Starburst", "scr_indep_fireworks", "scr_indep_firework_starburst" },
		{"Firework Fountain", "scr_indep_fireworks", "scr_indep_firework_fountain" },
		{"Firework Shotburst", "scr_indep_fireworks", "scr_indep_firework_shotburst" },
		{"Xmas Firework Burst", "proj_xmas_firework", "scr_firework_xmas_spiral_burst_rgw" },
		{"Teleport", "scr_rcbarry1", "scr_alien_teleport" },
		{"Disintegration", "scr_rcbarry1", "scr_alien_disintegrate" },
		{"Drip", "scr_agencyheist", "scr_fbi_mop_drips" },
		{"Flower Jet", "scr_rcbarry2", "scr_clown_bul" },
		{"Flash", "scr_rcpaparazzo1", "scr_rcpap1_camera" },
		{"Blue Flash", "scr_rcbarry1", "scr_alien_impact_bul" },
		{"Burnout", "scr_recartheft", "scr_wheel_burnout" },
		{"Splash", "scr_trevor1", "scr_trev1_trailer_splash" },
		{"Blood", "scr_solomon3", "scr_trev4_747_blood_impact" },
		{"Sparks", "scr_family4", "scr_fam4_trailer_sparks" },
		{"Shock", "scr_trevor1", "scr_trev1_trailer_boosh" },
		{"Money Cloud", "scr_ornate_heist", "scr_heist_ornate_banknotes" },
		{"Tire Smoke", "scr_carsteal4", "scr_carsteal4_wheel_burnout" },
		{"Clouds",  "wpn_projectile", "proj_missile_trail" },
		{"Golf Trail", "scr_minigamegolf", "scr_golf_ball_trail" },
		{"Tennis Trail", "scr_minigametennis", "scr_tennis_ball_trail" },
		{"Water Trail",  "core", "trail_splash_water" },
		{"Parachute Trail",  "core", "ped_parachute_trail" },
		{"Flare Trail",  "core", "proj_flare_trail" },
		{"Blood Trail",  "core", "trail_splash_blood" },
		{"Grenade Trail",  "wpn_projectile", "proj_grenade_trail" },
		{"Flaregun Trail",  "wpn_projectile", "proj_flare_trail" },
	};


	inline const char* g_ped_model_popular[8] = {
		"s_m_m_movalien_01",
		"s_m_m_movspace_01",
		"mp_f_deadhooker",
		"u_m_y_pogo_01",
		"s_f_y_stripper_01",
		"s_m_y_swat_01",
		"a_f_y_topless_01",
		"u_m_y_zombie_01"
	}; inline int g_ped_model_popular_id;

	inline const char* g_ped_model_popular_names[8] = {
		"Alien",
		"Astronaut",
		"Hooker",
		"Pogo",
		"Stripper",
		"Swat",
		"Topless",
		"Zombie"
	};

	inline const char* g_ped_model_story[25] = {
		"player_zero",
		"player_one",
		"player_two",
		"ig_amandatownley",
		"ig_ashley",
		"ig_brad",
		"ig_chef",
		"ig_chrisformage",
		"ig_clay",
		"ig_cletus",
		"ig_davenorton",
		"ig_denise",
		"ig_fabien",
		"ig_floyd",
		"ig_jay_norris",
		"ig_jimmydisanto",
		"ig_lamardavis",
		"ig_lestercrest",
		"ig_mrsphillips",
		"ig_stretch",
		"ig_taocheng",
		"ig_tenniscoach",
		"ig_tracydisanto"
	}; inline int g_ped_model_story_id;


	inline const char* g_ped_model_story_names[25] = {
		"Michael",
		"Franklin",
		"Trevor",
		"Amanda",
		"Ashley",
		"Brad",
		"Chef",
		"Chris Formage",
		"Clay",
		"Cletus",
		"Dave Norton",
		"Denise",
		"Fabien",
		"Floyd",
		"Jay Norris",
		"Jimmy",
		"Lamar",
		"Lester",
		"Mrs Phillips",
		"Stretch",
		"Tao Cheng",
		"Tennis Coach",
		"Tracy"
	};

	inline const char* g_ped_model_animal[18] = {
		"a_c_boar",
		"a_c_chimp",
		"a_c_chop",
		"a_c_cow",
		"a_c_coyote",
		"a_c_crow",
		"a_c_deer",
		"a_c_fish",
		"a_c_chickenhawk",
		"a_c_hen",
		"a_c_husky",
		"a_c_mtlion",
		"a_c_pig",
		"a_c_retriever",
		"a_c_rhesus",
		"a_c_rottweiler",
		"a_c_seagull",
		"a_c_shepherd"
	}; inline int g_ped_model_animal_id;

	inline const char* g_ped_model_animal_names[18] = {
		"Boar",
		"Chimp",
		"Chop",
		"Cow",
		"Coyote",
		"Crow",
		"Deer",
		"Fish",
		"Chicken Hawk",
		"Hen",
		"Husky",
		"Mountain Lion",
		"Pig",
		"Retriever",
		"Rhesus",
		"Rottweiler",
		"Seagull",
		"Shepherd"
	};

	inline const char* g_ped_model_emergency[12] = {
		"s_m_y_blackops_01",
		"s_m_y_cop_01",
		"csb_cop",
		"s_m_y_fireman_01",
		"u_m_m_prolsec_01",
		"s_m_y_hwaycop_01",
		"s_m_y_marine_03",
		"s_m_m_paramedic_01",
		"s_m_y_ranger_01",
		"s_m_m_security_01",
		"s_m_y_sheriff_01",
		"s_m_m_snowcop_01"
	}; inline int g_ped_model_emergency_id;

	inline const char* g_ped_model_emergency_names[12] = {
		"Black Ops",
		"City Cop",
		"County Cop",
		"Fireman",
		"Security Guard",
		"Highway Cop",
		"Marine",
		"Paramedic",
		"Ranger",
		"Security Guard",
		"Sheriff",
		"Snow Cop"
	};

	inline const char* g_ped_model_role[14] = {
		"s_m_m_bouncer_01",
		"ig_bride",
		"s_m_y_pestcont_01",
		"s_m_m_doctor_01",
		"s_m_y_factory_01",
		"s_m_y_garbage",
		"ig_groom",
		"ig_hunter",
		"u_m_m_jesus_01",
		"s_m_y_mime",
		"ig_priest",
		"s_m_y_prisoner_01",
		"s_m_m_scientist_01",
		"u_f_y_spyactress"
	}; inline int g_ped_model_role_id;

	inline const char* g_ped_model_role_names[14] = {
		"Bouncer",
		"Bride",
		"Pest Control",
		"Doctor",
		"Factory Worker",
		"Garbage Man",
		"Groom",
		"Hunter",
		"Jesus",
		"Mime",
		"Priest",
		"Prisoner",
		"Scientist",
		"Spy"
	};

	inline const char* g_ped_model_ambient[6] = {
		"u_m_y_babyd",
		"u_m_y_justin",
		"u_m_y_guido_01",
		"s_m_m_strperf_01",
		"u_m_y_mani",
		"u_m_m_griff_01"
	}; inline int g_ped_model_ambient_id;

	inline const char* g_ped_model_ambient_names[6] = {
		"Body Builder",
		"Nonce",
		"Guido",
		"Statue",
		"Mani",
		"Griff"
	};

	void ChangeModel(uint32_t Hash) {
		if (Native::IsModelInCdimage(Hash)) {
			if (Native::IsModelValid(Hash)) {
				Menu::GetControlManager()->RequestModel(Hash, [](uint32_t ModelHash) {
					Native::SetPlayerModel(Native::PlayerId(), ModelHash);
					m_Vars.m_LastModel = ModelHash;

					Utils::WaitFiber(2000);
					//Global::Vars::g_ReapplyWeapons = true;
					});
			}
		}
	}

	void CopsTurnBlindEye() {
		if (m_Vars.m_CopsTurnBlindEye) {
			Menu::Global(2738587).At(4661).As<int>() = 5;
			Menu::Global(2738587).At(4664).As<int>() = Native::GetNetworkTime() + 638000;
		}
		else {
			Menu::Global(2738587).At(4661).As<int>() = 0;
			Menu::Global(2738587).At(4664).As<int>() = Native::GetNetworkTime();
		}
	}

	void OffTheRadar() {
		if (m_Vars.m_OffTheRadar) {
			Menu::Global(2657921).At(Native::PlayerId(), 463).At(210).As<int>() = 1;
			Menu::Global(2672741).At(58).As<int>() = Native::GetNetworkTime() + 758000;
		}
		else {
			Menu::Global(2657921).At(Native::PlayerId(), 463).At(210).As<int>() = 0;
			Menu::Global(2672741).At(58).As<int>() = Native::GetNetworkTime();
		}
	}

	void RevealHiddenPlayers() {
		if (m_Vars.m_RevealHiddenPlayers) {
			Menu::Global(2657921).At(Native::PlayerId(), 463).At(213).As<int>() = 1;
			Menu::Global(2672524).At(71).As<int>() = Native::GetNetworkTime() + 758000;
		}
		else {
			Menu::Global(2657921).At(Native::PlayerId(), 463).At(213).As<int>() = 0;
			Menu::Global(2672524).At(71).As<int>() = Native::GetNetworkTime();
		}
	}

	void NoRagdoll() {
		Native::SetPedCanRagdoll(Native::PlayerPedId(), !m_Vars.m_DisableRagdoll);
		Native::SetPedCanRagdollFromPlayerImpact(Native::PlayerPedId(), !m_Vars.m_DisableRagdoll);
		Native::SetPedCanBeKnockedOffVehicle(Native::PlayerPedId(), !m_Vars.m_DisableRagdoll);
	}

	double GetAnalogRange(int Stick) {
		return (((double)Native::GetControlValue(0, Stick)) - 127) / 127;
	}

	void NoClip() {
		if (m_Vars.m_Noclip) {
			Math::Vector3<float> pos;
			pos.m_X = GetAnalogRange(ControlFrontendAxisX) * m_Vars.m_NoClipSpeed;
			pos.m_Y = GetAnalogRange(ControlFrontendAxisY) * m_Vars.m_NoClipSpeed * -1.f;

			Math::Vector3<float> apos = Native::GetOffsetFromEntityInWorldCoords(Native::PlayerPedId(), pos.m_X, pos.m_Y, pos.m_Z);
			auto Input = Native::GetControlNormal(0, ControlAttack) - Native::GetControlNormal(0, ControlAim);

			if (Menu::Util::GetInput()->IsPressed(false, ControlFrontendRt) || Input > 0.f) {
				apos.m_Z += m_Vars.m_NoClipSpeed;
			}

			if (Menu::Util::GetInput()->IsPressed(false, ControlFrontendLt) || Input < 0.f) {
				apos.m_Z -= m_Vars.m_NoClipSpeed;
			}

			Native::SetEntityCoordsNoOffset(Native::PlayerPedId(), apos.m_X, apos.m_Y, apos.m_Z, false, false, true);

			Math::Vector3<float> rot = Native::GetGameplayCamRot(2);
			Native::SetEntityRotation(Native::PlayerPedId(), 0.f, rot.m_Y, rot.m_Z, 2, true);

			Native::FreezeEntityPosition(Native::PlayerPedId(), true);
			Native::SetEntityCollision(Native::PlayerPedId(), false, false);

			Native::DisableControlAction(0, ControlFrontendLt, true);
			Native::DisableControlAction(0, ControlFrontendRt, true);
			Native::DisableControlAction(0, ControlFrontendCancel, true);
		}
		else {
			Native::SetEntityCollision(Native::PlayerPedId(), true, true);
			Native::FreezeEntityPosition(Native::PlayerPedId(), false);
			Native::EnableAllControlActions(0);
			Native::DisableControlAction(0, 27, true);
		}
	}

	struct animation_struct {
		const char* name;
		std::pair<const char*, const char*> values;
	};

	std::vector<animation_struct> sexual = {
		{ "Doggystyle 1", { "rcmpaparazzo_2", "shag_loop_poppy" } },
		{ "Doggystyle 2", { "rcmpaparazzo_2", "shag_loop_a" } },
		{ "Shaking Ass", { "switch@trevor@mocks_lapdance", "001443_01_trvs_28_idle_stripper" } },
		{ "Slow Humping", { "misscarsteal2pimpsex", "shagloop_pimp" } }
	};

	const char* sexualNames[] = {
		"Doggystyle 1",
		"Doggystyle 2",
		"Shaking Ass",
		"Slow Humping"
	};

	std::vector<animation_struct> animals = {
		{"Monkey", { "missfbi5ig_30monkeys", "monkey_b_freakout_loop" } },
		{"Chop Hump", { "missfra0_chop_find", "hump_loop_chop" } },
		{"Chop Swim", { "creatures@rottweiler@swim@", "swim" } }
	};

	const char* animalsNames[] = {
		"Monkey",
		"Chop Hump",
		"Chop Swim"
	};

	std::vector<animation_struct> actions = {
		{"Air Guitar", { "anim@mp_player_intcelebrationfemale@air_guitar", "air_guitar" } },
		{"Blow Kiss", { "anim@mp_player_intcelebrationfemale@blow_kiss", "blow_kiss" } },
		{"Bro Hug", { "anim@mp_player_intcelebrationpaired@f_m_bro_hug", "bro_hug_right" } },
		{"Challenge", { "misscommon@response", "face_palm" } },
		{"Face Palm", { "anim@mp_player_intcelebrationmale@face_palm", "" } },
		{"Finger", { "anim@mp_player_intcelebrationmale@finger", "finger" } },
		{"Hands Up", { "mp_pol_bust_out", "guard_handsup_loop" } },
		{"Hump Air", { "anim@mp_player_intcelebrationfemale@air_shagging", "air_shagging" } },
		{"Jazz Hands", { "anim@mp_player_intcelebrationmale@jazz_hands", "jazz_hands" } },
		{"Nose Pick", { "anim@mp_player_intcelebrationmale@nose_pick", "nose_pick" } },
		{"Photographer", { "anim@mp_player_intcelebrationmale@photography", "photography" } },
		{"Salute", { "anim@mp_player_intcelebrationmale@salute", "salute" } },
		{"Shush", { "anim@mp_player_intcelebrationmale@shush", "shush" } },
		{"Slow Clap", { "anim@mp_player_intcelebrationmale@slow_clap", "slow_clap" } },
		{"Smoke", { "anim@mp_player_intcelebrationmale@smoke_flick", "smoke_flick" } },
		{"Surrender", { "anim@mp_player_intcelebrationmale@surrender", "surrender" } },
		{"Synth", { "anim@mp_player_intcelebrationfemale@air_synth", "air_synth" } },
		{"Thumbs Up", { "anim@mp_player_intcelebrationmale@thumbs_up", "thumbs_up" } },
		{"Wank", { "mp_player_intwank", "mp_player_int_wank" } }
	};

	const char* actionsNames[] = {
	"Air Guitar",
	"Blow Kiss",
	"Bro Hug",
	"Challenge",
	"Face Palm",
	"Finger",
	"Hands Up",
	"Hump Air",
	"Jazz Hands",
	"Nose Pick",
	"Photographer",
	"Salute",
	"Shush",
	"Slow Clap",
	"Smoke",
	"Surrender",
	"Synth",
	"Thumbs Up",
	"Wank"
	};

	std::vector<animation_struct> dance = {
		{"Casual", { "rcmnigel1bnmt_1b", "dance_loop_tyler" } },
		{"Clown", { "rcm_barry2", "clown_idle_6" } },
		{"Pole", { "mini@strip_club@pole_dance@pole_dance3", "pd_dance_03" } },
		{"Private", { "mini@strip_club@private_dance@part2", "priv_dance_p2" } },
		{"Receive", { "mp_am_stripper", "lap_dance_player" } },
		{"Sexual", { "mini@strip_club@pole_dance@pole_a_2_stage", "pole_a_2_stage" } },
		{"Yacht", { "oddjobs@assassinate@multi@yachttarget@lapdance", "yacht_ld_f" } }
	};

	std::vector<animation_struct> misc = {
		{"Electrocute", { "ragdoll@human", "electrocute" } },
		{"Hover", { "swimming@base", "dive_idle" } },
		{"Jump", { "move_jump", "jump_launch_l_to_skydive" } },
		{"Meditate", { "rcmcollect_paperleadinout@", "meditiate_idle" } },
		{"Party", { "rcmfanatic1celebrate", "celebrate" } },
		{"Pissing", { "misscarsteal2peeing", "peeing_loop" } },
		{"Push Ups", { "rcmfanatic3", "ef_3_rcm_loop_maryann" } },
		{"Run", { "move_m@alien", "alien_run" } },
		{"Shitting", { "missfbi3ig_0", "shit_loop_trev" } },
		{"Showering", { "mp_safehouseshower@male@", "male_shower_idle_b" } },
		{"Swim", { "swimming@scuba", "dive_idle" } },
		{"Vomit", { "missfam5_blackout", "vomit" } },
		{"Wave Forward", { "friends@frj@ig_1", "wave_d" } },
		{"Wave Hands High", { "random@prisoner_lift", "arms_waving" } },
		{"Wave One Arm", { "random@shop_gunstore", "_greeting" } }
	};

	const char* danceNames[] = {
	"Casual",
	"Clown",
	"Pole",
	"Private",
	"Receive",
	"Sexual",
	"Yacht"
	};

	const char* miscNames[] = {
		"Electrocute",
		"Hover",
		"Jump",
		"Meditate",
		"Party",
		"Pissing",
		"Push Ups",
		"Run",
		"Shitting",
		"Showering",
		"Swim",
		"Vomit",
		"Wave Forward",
		"Wave Hands High",
		"Wave One Arm"
	};

	std::size_t sexual_id = 0;
	std::size_t animal_id = 0;
	std::size_t actions_id = 0;
	std::size_t dance_id = 0;
	std::size_t misc_id = 0;

	void start_animation(std::pair<const char*, const char*> anim) {
		Utils::GetFiberPool()->Push([=] {
			int flag = AnimationRepeat;
			if (m_Vars.m_controllable) flag |= AnimationEnablePlayerControl | AnimationUpperBody;
			if (m_Vars.m_contort) flag |= AnimationCreature;

			while (!Native::HasAnimDictLoaded(anim.first)) {
				Native::RequestAnimDict(anim.first);
				Utils::GetFiberManager()->Sleep(100);
			}

			Native::ClearPedTasksImmediately(Native::PlayerPedId());
			Native::TaskPlayAnim(Native::PlayerPedId(), anim.first, anim.second, 8.f, 8.f, -1, flag, 0.f, false, false, false);
			});
	}

}

/*
* movement
* services
* appearance
* particles
* animation
* wardrobe
*/

void PlayerMenu::Run() {
	
	Framework::addSubmenu("Player", "player-menu", [](Framework::Options::Core* core) {

		core->addOption(Framework::Options::SubmenuOption("Movement")
			.setTarget("player_movement"));

		core->addOption(Framework::Options::SubmenuOption("Appearance")
			.setTarget("player_appearance"));

		core->addOption(Framework::Options::SubmenuOption("Particles")
			.setTarget("player_particles"));

		core->addOption(Framework::Options::SubmenuOption("Animation")
			.setTarget("player_anim"));

		core->addOption(Framework::Options::ToggleOption("Godmode")
			.addToggle(&m_Vars.m_GodMode).addClick([] {
				if (!m_Vars.m_GodMode) {
					Native::SetEntityInvincible(Native::PlayerPedId(), false);
				}
				}));

		core->addOption(Framework::Options::ToggleOption("Disable Police")
			.addToggle(&m_Vars.m_DisablePolice).addClick([] {
				if (!m_Vars.m_DisablePolice) {
					Native::SetMaxWantedLevel(5);
				}
				}));

		core->addOption(Framework::Options::ToggleOption("Always Wanted")
			.addToggle(&m_Vars.m_AlwaysWanted));

		core->addOption(Framework::Options::ToggleOption("Disable Ragdoll")
			.addToggle(&m_Vars.m_DisableRagdoll));

		core->addOption(Framework::Options::ToggleOption("Off the Radar")
			.addToggle(&m_Vars.m_OffTheRadar));

		core->addOption(Framework::Options::ToggleOption("Cops Turn Blind Eye")
			.addToggle(&m_Vars.m_CopsTurnBlindEye));

		/*core->addOption(Framework::Options::ToggleOption("Reveal Hidden Players")
			.addToggle(&m_Vars.m_RevealHiddenPlayers));*/

		core->addOption(Framework::Options::ToggleOption("Invisibility")
			.addToggle(&m_Vars.m_Invisibility).addClick([] {
				if (!m_Vars.m_Invisibility) {
					Native::SetEntityVisible(Native::PlayerPedId(), true, true);
				}
				}));

		core->addOption(Framework::Options::ToggleOption("Superman")
			.addToggle(&m_Vars.m_SuperMan));

		core->addOption(Framework::Options::ToggleOption("Reduced Collision")
			.addToggle(&m_Vars.m_ReducedCollision));

		core->addOption(Framework::Options::ToggleOption("Dragon")
			.addToggle(&m_Vars.m_Dragon).addClick([] {
				if (!m_Vars.m_Dragon) {
					Native::RemoveParticleFx(m_Vars.m_BreatheFireHandle, false);
				}
				}));

		core->addOption(Framework::Options::ToggleOption("Badsport")
			.addToggle(&m_Vars.m_BadSport));

		core->addOption(Framework::Options::ButtonOption("Skyfall")
			.addClick([] {
				if (Native::GetEntityHeightAboveGround(Native::PlayerPedId()) < 50.f) {
					Native::SetEntityCoords(Native::PlayerPedId(), Native::GetEntityCoords(Native::PlayerPedId(), 1).m_X, Native::GetEntityCoords(Native::PlayerPedId(), 1).m_Y, Native::GetEntityCoords(Native::PlayerPedId(), 1).m_Z + 1000.f, true, true, true, false);
					Native::TaskSkyDive(Native::PlayerPedId(), TRUE);
				}
				}));
		core->addOption(Framework::Options::ButtonOption("Clone")
			.addClick([] {
				Native::ClonePed(Native::PlayerPedId(), Native::GetEntityHeading(Native::PlayerPedId()), true, false);
				}));

		core->addOption(Framework::Options::ButtonOption("Suicide")
			.addClick([] {
				Native::SetEntityHealth(Native::PlayerPedId(), 0);
				}));

		core->addOption(Framework::Options::ButtonOption("Regen Health")
			.addClick([] {
				Native::SetEntityHealth(Native::PlayerPedId(), Native::GetEntityMaxHealth(Native::PlayerPedId()));
				}));

		core->addOption(Framework::Options::ButtonOption("Regen Armor")
			.addClick([] {
				Native::SetPedArmour(Native::PlayerPedId(), 100);
				}));
		});


	Framework::addSubmenu("Animation", "player_anim", [](Framework::Options::Core* core) {

		core->addOption(Framework::Options::ButtonOption("Stop Animation")
			.addClick([] { Native::ClearPedTasksImmediately(Native::PlayerPedId()); }));


		core->addOption(Framework::Options::ToggleOption("Controllable")
			.addToggle(&m_Vars.m_controllable));

		core->addOption(Framework::Options::ToggleOption("Contort")
			.addToggle(&m_Vars.m_contort));

		core->addOption(Framework::Options::BreakOption("Animations"));

		core->addOption(Framework::Options::scrollOption<const char*, std::size_t>("Sexual")
			.addScroll(&sexualNames)
			.setPosition(&sexual_id)
			.addClick([] {
				start_animation(std::make_pair(sexual[sexual_id].values.first, sexual[sexual_id].values.second));
				}));

		core->addOption(Framework::Options::scrollOption<const char*, std::size_t>("Animals")
			.addScroll(&animalsNames)
			.setPosition(&animal_id)
			.addClick([] {
				start_animation(std::make_pair(animals[animal_id].values.first, animals[animal_id].values.second));
				}));

		core->addOption(Framework::Options::scrollOption<const char*, std::size_t>("Actions")
			.addScroll(&actionsNames)
			.setPosition(&actions_id)
			.addClick([] {
				start_animation(std::make_pair(actions[actions_id].values.first, actions[actions_id].values.second));
				}));


		core->addOption(Framework::Options::scrollOption<const char*, std::size_t>("Dance")
			.addScroll(&danceNames)
			.setPosition(&dance_id)
			.addClick([] {
				start_animation(std::make_pair(dance[dance_id].values.first, dance[dance_id].values.second));
				}));


		core->addOption(Framework::Options::scrollOption<const char*, std::size_t>("Misc")
			.addScroll(&miscNames)
			.setPosition(&misc_id)
			.addClick([] {
				start_animation(std::make_pair(misc[misc_id].values.first, misc[misc_id].values.second));
				}));
	});

	Framework::addSubmenu("Particles", "player_particles", [](Framework::Options::Core* core) {

		for (const auto particle : ParticleList) {
			core->addOption(Framework::Options::ToggleOption(particle.name)
				.addToggle(&m_Vars.m_particleToggles[particle.name])
				.addClick([=] {
					if (m_Vars.m_particleToggles[particle.name]) {
						start_particle(particle.dict, particle.texture, m_Vars.m_particleToggles[particle.name]);
					}
					}));
		}
	});

	Framework::addSubmenu("Appearance", "player_appearance", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ButtonOption("Clean Model")
			.addClick([] {
				Native::ResetPedVisibleDamage(Native::PlayerPedId());
				Native::ClearPedBloodDamage(Native::PlayerPedId());
				Native::ClearPedDecorations(Native::PlayerPedId());
				}));

		core->addOption(Framework::Options::ButtonOption("Refresh Cache")
			.addClick([] {
				m_Vars.m_LastModel = 0;
				}));

		core->addOption(Framework::Options::SubmenuOption("Popular")
			.setTarget("model_pop"));

		core->addOption(Framework::Options::SubmenuOption("Story")
			.setTarget("model_anim"));

		core->addOption(Framework::Options::SubmenuOption("Emergency")
			.setTarget("model_emer"));

		core->addOption(Framework::Options::SubmenuOption("Ambient")
			.setTarget("model_amb"));

		core->addOption(Framework::Options::SubmenuOption("Gangsters")
			.setTarget("model_gang"));
		});

	Framework::addSubmenu("Popular", "model_pop", [](Framework::Options::Core* core) {
		for (int i = 0; i < 8; ++i) {
			core->addOption(Framework::Options::ButtonOption(g_ped_model_popular_names[i])
				.addClick([=] {
					ChangeModel(Rage::LowerCaseJoaat(g_ped_model_popular[i]));
					}));
		}
		});

	Framework::addSubmenu("Story", "model_anim", [](Framework::Options::Core* core) {
		for (int i = 0; i < 25; ++i) {
			core->addOption(Framework::Options::ButtonOption(g_ped_model_story_names[i])
				.addClick([=] {
					ChangeModel(Rage::LowerCaseJoaat(g_ped_model_story[i]));
					}));
		}
		});

	Framework::addSubmenu("Emergency", "model_emer", [](Framework::Options::Core* core) {
		for (int i = 0; i < 12; ++i) {
			core->addOption(Framework::Options::ButtonOption(g_ped_model_emergency_names[i])
				.addClick([=] {
					ChangeModel(Rage::LowerCaseJoaat(g_ped_model_emergency[i]));
					}));
		}
		});

	Framework::addSubmenu("Ambient", "model_amb", [](Framework::Options::Core* core) {
		for (int i = 0; i < 6; ++i) {
			core->addOption(Framework::Options::ButtonOption(g_ped_model_ambient_names[i])
				.addClick([=] {
					ChangeModel(Rage::LowerCaseJoaat(g_ped_model_ambient[i]));
					}));
		}
		});

	Framework::addSubmenu("Gangsters", "model_gang", [](Framework::Options::Core* core) {
		for (int i = 0; i < 14; ++i) {
			core->addOption(Framework::Options::ButtonOption(g_ped_model_role_names[i])
				.addClick([=] {
					ChangeModel(Rage::LowerCaseJoaat(g_ped_model_role[i]));
					}));
		}
		});


	Framework::addSubmenu("Movement", "player_movement", [](Framework::Options::Core* core) {

		core->addOption(Framework::Options::ToggleOption("Super Jump")
			.addToggle(&m_Vars.m_SuperJump));

		core->addOption(Framework::Options::ToggleOption("Beast Jump")
			.addToggle(&m_Vars.m_BeastJump));

		core->addOption(Framework::Options::ToggleOption("Always Swim")
			.addToggle(&m_Vars.m_AlwaySwim));

		core->addOption(Framework::Options::ToggleOption("No Fall Animation")
			.addToggle(&m_Vars.m_NoFallAnim));

		core->addOption(Framework::Options::ToggleNumberOption<float>("No Clip")
			.addToggle(&m_Vars.m_Noclip).addNumber(&m_Vars.m_NoClipSpeed)
			.addMin(0.f).addMax(10.f).addStep(0.1f).setPrecision(2));

		core->addOption(Framework::Options::ToggleNumberOption<float>("Run Multiplier")
			.addToggle(&m_Vars.m_SuperRun).addNumber(&m_Vars.m_SuperRunSpeed)
			.addMin(0.f).addMax(100.f).addStep(0.01f).setPrecision(2));

		core->addOption(Framework::Options::ToggleNumberOption<float>("Swim Multiplier")
			.addToggle(&m_Vars.m_SuperSwim).addNumber(&m_Vars.m_SuperSwimSpeed)
			.addMin(0.f).addMax(100.f).addStep(0.01f).setPrecision(2));

		core->addOption(Framework::Options::ToggleNumberOption<float>("Super Run")
			.addToggle(&m_Vars.m_SuperRunAlternative).addNumber(&m_Vars.m_SuperRunAlternativeSpeed)
			.addMin(0.f).addMax(100.f).addStep(0.01f).setPrecision(2));
	});
}

void PlayerMenu::Update() {

	Run();
	NoClip();

	if (m_Vars.m_SuperRun) {
		(*Patterns::Vars::g_PedFactory)->m_local_ped->m_player_info->m_run_speed = m_Vars.m_SuperRunSpeed;
	}
	else {
		(*Patterns::Vars::g_PedFactory)->m_local_ped->m_player_info->m_run_speed = 1.f;
	}

	if (m_Vars.m_SuperSwim) {
		(*Patterns::Vars::g_PedFactory)->m_local_ped->m_player_info->m_swim_speed = m_Vars.m_SuperSwimSpeed;
	}
	else {
		(*Patterns::Vars::g_PedFactory)->m_local_ped->m_player_info->m_swim_speed = 1.f;
	}

	if (m_Vars.m_SuperRunAlternative) {
		if (Native::IsDisabledControlPressed(0, ControlSprint)) {
			Native::ApplyForceToEntity(Native::PlayerPedId(), 1, 0.f, m_Vars.m_SuperRunAlternativeSpeed, 0.f, 0.f, 0.f, 0.f, 0, 1, 1, 1, 0, 1);
		}
	}

	if (m_Vars.m_AlwaySwim) {
		Native::SetPedConfigFlag(Native::PlayerPedId(), 65, true);
	}
	else {
		Native::SetPedConfigFlag(Native::PlayerPedId(), 65, false);
	}

	if (m_Vars.m_GodMode) {
		Native::SetEntityInvincible(Native::PlayerPedId(), true);
	}

	if (m_Vars.m_DisablePolice) {
		Native::SetMaxWantedLevel(0);
		Native::ClearPlayerWantedLevel(Native::PlayerPedId());
	}

	if (m_Vars.m_AlwaysWanted) {
		Native::SetMaxWantedLevel(5);
		Native::SetWantedLevelDifficulty(Native::PlayerPedId(), 5);
		Native::SetPlayerWantedLevelNow(Native::PlayerPedId(), 5);
		Native::SetPlayerWantedLevelNoDrop(Native::PlayerPedId(), 5, 0);
	}

	if (m_Vars.m_DisableRagdoll) {
		NoRagdoll();
	}

	if (m_Vars.m_Invisibility) {
		Native::SetEntityVisible(Native::PlayerPedId(), false, false);
	}

	if (m_Vars.m_ReducedCollision) {
		Native::SetPedCapsule(Native::PlayerPedId(), 0.001f);
	}

	if (m_Vars.m_Dragon) {
		if (Native::HasNamedPtfxAssetLoaded("core")) {
			Native::_UseParticleFxAssetNextCall("core");
			if (!Native::DoesParticleFxLoopedExist(m_Vars.m_BreatheFireHandle)) {
				m_Vars.m_BreatheFireHandle = Native::StartParticleFxLoopedOnEntityBone("ent_sht_flame", Native::PlayerPedId(), -0.02f, 0.2f, 0.0f, 90.0f, 100.0f, 90.0f, Native::GetPedBoneIndex(Native::PlayerPedId(), 31086), 1.0f, false, false, false);
			}
		}
		else {
			Native::RequestNamedPtfxAsset("core");
		}
	}

	if (m_Vars.m_BadSport) {

	}

	if (m_Vars.m_OffTheRadar) {
		OffTheRadar();
	}

	if (m_Vars.m_RevealHiddenPlayers) {
		RevealHiddenPlayers();
	}

	if (m_Vars.m_CopsTurnBlindEye) {
		CopsTurnBlindEye();
	}


	if (m_Vars.m_SuperMan) {
		int LeftRight = Native::GetControlValue(2, 188);
		int UpDown = Native::GetControlValue(2, 189);

		if (Native::GetEntityHeightAboveGround(Native::PlayerPedId()) < 3.f) {
			Native::GiveWeaponToPed(Native::PlayerPedId(), 0xFBAB5776, -1, true, true);

			Math::Vector3<float> Coords = Native::GetEntityCoords(Native::PlayerPedId(), true);
			Coords.m_Z += 100.f;
			Native::SetEntityCoords(Native::PlayerPedId(), Coords.m_X, Coords.m_Y, Coords.m_Z, false, false, false, false);
		}
		else {
			if (UpDown == 254 || Menu::Util::GetInput()->IsPressed(true, 'S'))
				Native::ApplyForceToEntity(Native::PlayerPedId(), 1, 0.f, 3.f, 10.f, 0.f, 0.f, 0.f, false, true, true, true, false, true);

			if (UpDown == 0 || Menu::Util::GetInput()->IsPressed(true, 'W'))
				Native::ApplyForceToEntity(Native::PlayerPedId(), 1, 0.f, 3.f, 0.f, 0.f, 0.f, 0.f, false, true, true, true, false, true);

			if (Menu::Util::GetInput()->IsPressed(true, VK_SHIFT) || Menu::Util::GetInput()->IsPressed(false, ControlFrontendRt))
				Native::ApplyForceToEntity(Native::PlayerPedId(), 1, 0.f, 15.f, .3f, 0.f, 0.f, 0.f, false, true, true, true, false, true);

			if (LeftRight == 254 || Menu::Util::GetInput()->IsPressed(true, 'D'))
				Native::ApplyForceToEntity(Native::PlayerPedId(), 1, 1.20f, 0.f, 0.f, 0.f, 0.1f, 0.f, false, true, true, true, false, true);

			if (LeftRight == 0 || Menu::Util::GetInput()->IsPressed(true, 'A'))
				Native::ApplyForceToEntity(Native::PlayerPedId(), 1, -1.20f, 0.f, 0.f, 0.f, 0.1f, 0.f, false, true, true, true, false, true);
		}
	}

	for (const auto particle : ParticleList) {
		start_particle(particle.dict, particle.texture, m_Vars.m_particleToggles[particle.name]);
	}
}