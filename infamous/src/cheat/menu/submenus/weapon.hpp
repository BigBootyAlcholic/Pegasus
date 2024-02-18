#pragma once
#include "framework/framework.hpp"
#include "framework/options/core.hpp"
#include "framework/options/button.hpp"
#include "framework/options/toggle.hpp"
#include "framework/options/submenu_option.hpp"
#include "framework/options/scroll.hpp"
#include "framework/options/break.hpp"
#include "framework/options/number.hpp"
#include "core/core.hpp"

class WeaponMenu {
public:
	void Run();
	void Update();
};

static WeaponMenu* GetWeaponMenu() {
	static WeaponMenu instance;
	return &instance;
}

namespace WeaponMenuVars {
	struct Vars_ {
		bool m_NoRecoil = false;
		bool m_NoSpread = false;
		bool m_ShotGunMode = false;
		bool m_OneShotKill = false;
		bool m_NoCooldown = false;
		bool m_NoSpinUp = false;

		bool m_Width;
		bool m_Height;
		float m_WidthVal = 1.f;
		float m_HeightVal = 1.f;

		bool m_InfiniteAmmo = false;
		bool m_InfiniteClip = false;
		bool m_ExplosiveAmmo = false;
		bool m_FireAmmo = false;
		bool m_PortalGun = false;
		bool m_TeleportGun = false;
		bool m_SkydiveGun = false;
		bool m_AirstrikeGun = false;
		bool m_PushGun = false;
		bool m_PullGun = false;
		bool m_PedGun = false;
		bool m_RapidFire = false;
		bool m_DeleteGun = false;
		bool m_HijackGun = false;
		bool m_RopeGun = false;
		bool m_ValkyrieGun = false;
		bool m_DeadEyes = false;
		bool m_LaserSight = false;
		bool m_DefibrillatorGun = false;
		bool m_HashGun = false;
		bool m_ShrinkGun = false;
		bool m_ClownGun = false;
		bool m_RepairGun = false;
		bool m_RecolorGun = false;
		bool m_InstrumentOfDeath = false;
		bool m_VortexGunPeds = true;
		int m_VortexGunPedCapacity = 10;
		bool m_VortexGunVehicles = true;
		int m_VortexGunVehicleCapacity = 10;
		bool m_VortexGunObjects = true;
		int m_VortexGunObjectCapacity = 10;
		float m_VortexGunDistance = 20.f;
		float m_VortexGunForce = 500.f;
		bool m_VortexGunCooldown = false;
		bool m_VortexGunSphere = true;
		bool m_VortexGunEnable = false;
		float m_TotemGunDistance = 10.f;
		float m_TotemGunSpacing = 5.f;
		bool m_EnableTotemGun = false;
		int m_BulletTracerDuration = 5000;
		Color m_BulletTracerColor;
		bool m_RainbowBulletTracers = false;
		bool m_EnableBulletTracers = false;
		float m_GravityGunDistance = 20.f;
		float m_GravityGunForce = 500.f;
		bool m_EnableGravityGun = false;
		const char* const m_BulletChangerType[14]
		{
			"Firework",
			"Grenade",
			"Molotov",
			"Vehicle Rocket",
			"RPG",
			"Tank Rocket",
			"Red Laser",
			"Green Laser",
			"Ball",
			"Snowball",
			"Smoke Grenade",
			"Flare",
			"Sticky Bomb",
			"Proximity Mine"
		};
		int m_BulletChangerTypePos = 0;
		float m_BulletChangerSpeed = 100.f;
		bool m_BulletChangerAudible = true;
		bool m_BulletChangerVisible = true;
		bool m_EnableBulletChanger = false;
		const char* const m_ImpactGunType[84]
		{
			"Grenade",
			"Grenade Launcher",
			"Sticky Bomb",
			"Molotov",
			"Rocket",
			"Tank Shell",
			"Hi Octane",
			"Car",
			"Plane",
			"Petrol Pump",
			"Bike",
			"Water",
			"Flame",
			"Water Big",
			"Flame Big",
			"Boat",
			"Ship Destroy",
			"Truck",
			"Bullet",
			"Smoke Grenade Launcher",
			"Smoke Grenade",
			"Tear Gas",
			"Flare",
			"Gas Canister",
			"Extinguisher",
			"Unknown",
			"Train",
			"Barrel",
			"Propane",
			"Blimp",
			"Flame Explode",
			"Tanker",
			"Plane Rocket",
			"Vehicle Bullet",
			"Gas Tank",
			"Bird Crap",
			"Rail Gun",
			"Blimp 2",
			"Firework",
			"Snowball",
			"Proximity Mine",
			"Valkyrie Cannon",
			"Air Defence",
			"Pipebomb",
			"Vehicle Mine",
			"Explosive Ammo",
			"APC Shell",
			"Bomb Cluster",
			"Bomb Gas",
			"Bomb Incendiary",
			"Bomb Standard",
			"Torpedo",
			"Torpedo Underwater",
			"Bombushka Cannon",
			"Bomb Cluster Secondary",
			"Hunter Barrage",
			"Hunter Cannon",
			"Rogue Cannon",
			"Mine Underwater",
			"Orbital Cannon",
			"Bomb Standard Wide",
			"Explosive Ammo Shotgun",
			"Oppressor Mk2 Cannon",
			"Mortar Kinetic",
			"Vehicle Mine Kinetic",
			"Vehicle Mine EMP",
			"Vehicle Mine Spike",
			"Vehicle Mine Slick",
			"Vehicle Mine Tar",
			"Script Drone",
			"Raygun",
			"Buried Mine",
			"Script Missile",
			"RC Tank Rocket",
			"Bomb Water",
			"Bomb Water Secondary",
			"Unknown 2",
			"Unknown 3",
			"Flash Grenade",
			"Stun Grenade",
			"Unknown 4",
			"Large Script Missile",
			"Submarine",
			"EMP Launcher"
		};
		int m_ImpactGunTypePos = 0;
		float m_ImpactGunDamageScale = 9999.f;
		float m_ImpactGunCameraShake = 0.f;
		bool m_ImpactGunAudible = true;
		bool m_ImpactGunVisible = true;
		bool m_EnableImpactGun = false;
		Color m_PaintGunColor;
		bool m_RainbowPaintGun = false;
		float m_PaintGunSize = 0.5f;
		float m_PaintGunIntensity = 1000.f;
		int m_PaintGunDuration = 5000;
		bool m_EnablePaintGun = false;
		const char* const m_ParticleGunEffect[20]
		{
			"Purple Cloud 1",
			"Purple Cloud 2",
			"Purple Cloud 3",
			"Firework Starburst",
			"Firework Fountain",
			"Firework Shotburst",
			"Xmas Firework Burst",
			"Teleport",
			"Disintegration",
			"Drip",
			"Flower Jet",
			"Flash",
			"Blue Flash",
			"Burnout",
			"Splash",
			"Blood",
			"Sparks",
			"Shock",
			"Money Cloud",
			"Tire Smoke"
		};
		int m_ParticleGunEffectPos = 0;
		float m_ParticleGunScale = 1.f;
		bool m_EnableParticleGun = false;
		const char* const m_EntityGunType[3]
		{
			"Object",
			"Vehicle",
			"Ped"
		};
		int m_EntityGunTypePos = 0;
		const char* const m_EntityGunObject[14]
		{
			"Meteor",
			"Stairs",
			"Egg",
			"Barbell",
			"Campfire",
			"Volleyball",
			"Dildo",
			"Guitar",
			"Gold",
			"Missile",
			"Toilet",
			"Money Bag",
			"Wheelchair",
			"Pumpkin"
		};
		int m_EntityGunObjectPos = 0;
		const char* const m_EntityGunVehicle[23]
		{
			"Jobuilt Hauler",
			"Weeny Issi",
			"Dinka Blista Kanjo",
			"Declasse Rhapsody",
			"Vapid Dominator",
			"Albany Hermes",
			"Vapid Clique",
			"Vapid Slamvan",
			"Space Dune",
			"Ocelot R88",
			"Maxwell Vagrant",
			"Annis Hellion",
			"BF Injection",
			"BF Bifta",
			"Nagasaki Hot Rod Blazer",
			"Vapid Peyote",
			"Vapid Hustler",
			"Vapid Imperator",
			"Vapid Ellie",
			"Speedophile Seashark",
			"Titan",
			"Truffade Adder",
			"Progen T20"
		};
		int m_EntityGunVehiclePos = 0;
		const char* const m_EntityGunPed[12]
		{
			"Whale",
			"Cow",
			"Seagull",
			"Topless Girl",
			"Bride",
			"Hooker",
			"Astronaut",
			"Human Statue",
			"Mime",
			"Jesus",
			"Pogo",
			"Zombie"
		};
		int m_EntityGunPedPos = 0;
		int m_EntityGunDelay = 100;
		float m_EntityGunForce = 900.f;
		bool m_EnableEntityGun = false;
		float m_WeaponWidth = 1.f;
		bool m_EnableWeaponWidth = false;
		float m_WeaponHeight = 1.f;
		bool m_EnableWeaponHeight = false;
		float m_WeaponScale = 1.f;
		bool m_EnableWeaponScale = false;
		float m_PositionX = 1.f;
		bool m_EnablePositionX = false;
		float m_PositionY = 1.f;
		bool m_EnablePositionY = false;
		bool m_InteriorWeapons = false;
		bool m_PassiveWeapons = false;
		bool m_AllowWeaponsInVehicle = false;
		bool m_FriendlyFire = false;
		bool m_InstantLockOn = false;
		bool m_BypassC4Limit = false;
		bool m_BypassFlareLimit = false;
		const char* const m_TriggerBotBone[7]
		{
			"Head",
			"Neck",
			"Core",
			"Left Hand",
			"Right Hand",
			"Left Foot",
			"Right Foot"
		};
		int m_TriggerBotBonePos = 0;
		bool m_TriggerBotPolice = true;
		bool m_TriggerBotCivilians = true;
		bool m_TriggerBotEnemies = true;
		bool m_TriggerBotPlayers = true;
		bool m_EnableTriggerBot = false;
		const char* const m_AimBotBone[7]
		{
			"Head",
			"Neck",
			"Core",
			"Left Hand",
			"Right Hand",
			"Left Foot",
			"Right Foot"
		};
		int m_AimBotBonePos = 0;
		float m_AimBotLegitSpeed = 2.f;
		float m_AimBotFOV = 90.f;
		float m_AimBotDistance = 200.f;
		bool m_AimBotPolice = true;
		bool m_AimBotCivilians = true;
		bool m_AimBotEnemies = true;
		bool m_AimBotPlayers = true;
		bool m_LegitBot = true;
		bool m_EnableAimBot = false;

		struct BulletTracer
		{
			Math::Vector3<float> m_Start{};
			Math::Vector3<float> m_End{};
			Color m_Color{};
			DWORD m_Time{};
			int m_Alpha{};
		}; std::vector<BulletTracer> m_BulletTracers;
	};

	extern Vars_ m_Vars;
}
