#include "stdafx.hpp"
#include "weapon.hpp"
#include "cheat/util/notify.hpp"
#include "cheat/util/control_manager.hpp"
#include "core/patterns.hpp"
#include "rage/engine.hpp"
#include "util/fiber.hpp"
#include "util/fiber_pool.hpp"
#include "cheat/util/pools/pool_manager.hpp"
#include "cheat/util/timer.hpp"
using namespace WeaponMenuVars;

namespace WeaponMenuVars {
	Vars_ m_Vars;

	struct weapons { std::string name; uint32_t id; };
	inline std::vector<weapons> WeaponsList = {
		{ "Pistol", 0x1b06d571 },
		{ "Pistol Mk II", 0xbfe256d4 },
		{ "Combat Pistol", 0x5ef9fec4 },
		{ "AP Pistol", 0x22d8fe39 },
		{ "Stun Gun", 0x3656c8c1 },
		{ "Pistol .50", 0x99aeeb3b },
		{ "SNS Pistol", 0xbfd21232 },
		{ "SNS Pistol Mk II", 0x88374054 },
		{ "Heavy Pistol", 0xd205520e },
		{ "Vintage Pistol", 0x83839c4 },
		{ "Flare Gun", 0x47757124 },
		{ "Marksman Pistol", 0xdc4db296 },
		{ "Heavy Revolver", 0xc1b3c3d1 },
		{ "Heavy Revolver Mk II", 0xcb96392f },
		{ "Double Action Revolver", 0x97ea20b8 },
		{ "Up-n-Atomizer", 0xaf3696a1 },
		{ "Ceramic Pistol", 0x2b5ef5ec },
		{ "Navy Revolver", 0x917f6c8c },
		{ "Gadget Pistol", 0x57a4368c },

		{ "Micro SMG", 0x13532244 },
		{ "SMG", 0x2be6766b },
		{ "SMG Mk II", 0x78a97cd0 },
		{ "Assault SMG", 0xefe7e2df },
		{ "Combat PDW", 0xa3d4d34 },
		{ "Machine Pistol", 0xdb1aa450 },
		{ "Mini SMG", 0xbd248b55 },
		{ "Unholy Hellbringer", 0x476bf155 },

		{ "Pump Shotgun", 0x1d073a89 },
		{ "Pump Shotgun Mk II", 0x555af99a },
		{ "Sawed-Off Shotgun", 0x7846a318 },
		{ "Assault Shotgun", 0xe284c527 },
		{ "Bullpup Shotgun", 0x9d61e50f },
		{ "Musket", 0xa89cb99e },
		{ "Heavy Shotgun", 0x3aabbbaa },
		{ "Double Barrel Shotgun", 0xef951fbb },
		{ "Sweeper Shotgun", 0x12e82d3d },
		{ "Combat Shotgun", 0x5a96ba4 },

		{ "Assault Rifle", 0xbfefff6d },
		{ "Assault Rifle Mk II", 0x394f415c },
		{ "Carbine Rifle", 0x83bf0278 },
		{ "Carbine Rifle Mk II", 0xfad1f1c9 },
		{ "Advanced Rifle", 0xaf113f99 },
		{ "Special Carbine", 0xc0a3098d },
		{ "Special Carbine Mk II", 0x969c3d67 },
		{ "Bullpup Rifle", 0x7f229f94 },
		{ "Bullpup Rifle Mk II", 0x84d6fafd },
		{ "Compact Rifle", 0x624fe830 },
		{ "Military Rifle", 0x9d1f17e6 },
		{ "Heavy Rifle", 0xC78D71B4 },
		{ "Tactical Rifle", 0xD1D5F52B },

		{ "MG", 0x9d07f764 },
		{ "Combat MG", 0x7fd62962 },
		{ "Combat MG Mk II", 0xdbbd7280 },
		{ "Gusenberg Sweeper", 0x61012683 },

		{ "Sniper Rifle", 0x5fc3c11 },
		{ "Heavy Sniper", 0xc472fe2 },
		{ "Heavy Sniper Mk II", 0xa914799 },
		{ "Marksman Rifle", 0xc734385a },
		{ "Marksman Rifle Mk II", 0x6a6c02e0 },
		{ "Precision Rifle", 0x6E7DDDEC },

		{ "RPG", 0xb1ca77b1 },
		{ "Grenade Launcher", 0xa284510b },
		{ "Grenade Launcher Smoke", 0x4dd2dc56 },
		{ "Minigun", 0x42bf8a85 },
		{ "Firework Launcher", 0x7f7497e5 },
		{ "Railgun", 0x6d544c99 },
		{ "Homing Launcher", 0x63ab0442 },
		{ "Compact Grenade Launcher", 0x781fe4a },
		{ "Widowmaker", 0xb62d1f67 },

		{ "Jerry Can", 0x34a67b97 },
		{ "Hazardous Jerry Can", 0xba536372 },
		{ "Parachute", 0xfbab5776 },
		{ "Fire Extinguisher", 0x60ec506 },
		{ "Digi Scanner", 0xfdbadced },

		{ "Grenade", 0x93e220bd },
		{ "BZ Gas", 0xa0973d5e },
		{ "Molotov Cocktail", 0x24b17070 },
		{ "Sticky Bomb", 0x2c3731d9 },
		{ "Proximity Mines", 0xab564b93 },
		{ "Snowballs", 0x787f0bb },
		{ "Pipe Bombs", 0xba45e8b8 },
		{ "Baseball", 0x23c9f95c },
		{ "Tear Gas", 0xfdbc8a50 },
		{ "Flare", 0x497facc3 },

		{ "Antique Cavalry Dagger", 0x92a27487 },
		{ "Baseball Bat", 0x958a4a8f },
		{ "Broken Bottle", 0xf9e6aa4b },
		{ "Crowbar", 0x84bd7bfd },
		{ "Flashlight", 0x8bb05fd7 },
		{ "Golf Club", 0x440e4788 },
		{ "Hammer", 0x4e875f73 },
		{ "Hatchet", 0xf9dcbf2d },
		{ "Brass Knuckles", 0xd8df3c3c },
		{ "Knife", 0x99b507ea },
		{ "Machete", 0xdd5df8d9 },
		{ "Switchblade", 0xdfe37640 },
		{ "Nightstick", 0x678b81b1 },
		{ "Pipe Wrench", 0x19044ee0 },
		{ "Battle Axe", 0xcd274149 },
		{ "Pool Cue", 0x94117305 },
		{ "Stone Hatchet", 0x3813fc08 },
	};

	struct weapon_component {
		std::string name;
		uint32_t hash;
	};

	inline std::vector<weapon_component> WeaponComps = {
		// Barrel
		{"Default", 0xCCFD2AC5},
		{"Suppressor", 0xA73D4664},
		{"Heavy", 0x108AB09E},
		{"Compact", 0x911A1F9B},
		{"Army", 0xB99402D4},
		{"Sergeant", 0xE63E8AAE},
		{"Bullpup", 0xB3688B0F},
		// Sight
		{"Reflex", 0x9FDB5700},
		{"Scope", 0xAA2C45B4},
		{"Night Vision", 0xA0D89C42},
		{"Thermal", 0xC0240885},
		{"Holographic", 0x3FD8A36B},
		// Grip
		{"Grip", 0xC164F53},
		{"Extended Grip", 0x63AB0442},
		{"Tactical Grip", 0x7BC4CDDC},
		{"Light Grip", 0xD2650D57},
		{"Advanced Grip", 0x9D65907A},
		// Magazine
		{"Default", 0x249A17D5},
		{"Extended Clip", 0x7C8BD10E},
		{"Flash Mag", 0x8EB26D8E},
		{"Drum Mag", 0x5C0C6C4A},
		{"Armor Piercing Mag", 0x31C4B7D5},
		// Suppressor
		{"Suppressor", 0x65EA7EBB},
		{"Flash Suppressor", 0x8D34B573},
		{"Heavy Suppressor", 0xA7C4F178},
		{"Screw Suppressor", 0x2E7957A},
		{"Advanced Suppressor", 0xFDCB0C5A},
		// Other Components
		{"Flashlight", 0x7BC4CDDC},
		{"Laser Sight", 0xC34EF234},
		{"Grip Tape", 0xF361A3BA},
		{"Muzzle Brake", 0x2E7957A},
		{"Compensator", 0xD89B9658},
		// Grip Attachments
		{"Vertical Grip", 0xABC532},
		{"Angled Grip", 0x69C6E837},
		{"Stubby Grip", 0xE232C79C},
		{"Folding Grip", 0xC58B6901},
		// Stock Attachments
		{"Fixed Stock", 0xFBBAB08C},
		{"Tactical Stock", 0x56E6AE7F},
		{"Folded Stock", 0xEBA6BBD6},
		{"Precision Stock", 0x6F32A41F},
		// Rail Attachments
		{"RIS Rail", 0x5F7CEC91},
		{"Keymod Rail", 0x9E27D88F},
		{"M-LOK Rail", 0x3ED2F0D5},
		{"Picatinny Rail", 0xA489C51A},
		// Barrel Attachments
		{"Muzzle Device", 0x2F36BB5B},
		{"Choke", 0x5C29DCC9},
		{"Barrel Extension", 0x93B170E1},
		{"Muzzle Shroud", 0xDB5A886A},
		// Magazine Attachments
		{"Magwell", 0x84D6760A},
		{"Extended Magwell", 0x5576BE62},
		{"Magazine Coupler", 0x760C6E1F},
		{"Extended Magazine Coupler", 0x9F0EFE6C},
		// Rail Covers
		{"Rubber Rail Cover", 0x38446EB1},
		{"Polymer Rail Cover", 0x8626CC13},
		{"Carbon Fiber Rail Cover", 0x9F715F4D},
		{"Metal Rail Cover", 0xD2F811A3},
		// Bipods
		{"Standard Bipod", 0xB5A3AF04},
		{"Folding Bipod", 0xD8350EFE},
		{"Swivel Bipod", 0x671313A2},
		{"Quick Detach Bipod", 0x37D872A7},
		// Foregrips
		{"Angled Foregrip", 0x5C640305},
		{"Vertical Foregrip", 0xE97A0F4A},
		{"Handstop", 0x3D4C9DE3},
		{"Bipod Foregrip", 0x5796EFD6},
		// Rail Attachments (continued)
		{"Offset Rail", 0xE8AAABF9},
		{"Angled Rail", 0xA1D86EF2},
		{"Handguard Rail", 0x264A745D},
		{"Top Rail", 0xF53C0DA5},
		// Barrel Attachments (continued)
		{"Threaded Barrel", 0x8BC5D8B8},
		{"Fluted Barrel", 0xC647405F},
		{"Ported Barrel", 0x6ED3BB83},
		{"Bull Barrel", 0xAF8EE8DA},
		// Magazine Attachments (continued)
		{"Magazine Grip", 0xE126E668},
		{"Extended Magazine Grip", 0x51B24FF0},
		{"Magazine Coupler Grip", 0xB2E7D0A1},
		{"Extended Magazine Coupler Grip", 0x7E6E1E9D},
		// Optics
		{"Red Dot Sight", 0xBEC7B19A},
		{"Glow Sight", 0xA7E26B54},
		{"Magnified Scope", 0x813CD34C},
		{"Holographic Sight", 0x92A8D3E9},
		// Stock Attachments (continued)
		{"Retractable Stock", 0xC69DA1D0},
		{"Collapsible Stock", 0x15487555},
		{"Skeleton Stock", 0x8EB0B907},
		{"Foldable Stock", 0x4B1554F3},
		// Grip Attachments (continued)
		{"Rubberized Grip", 0x4E4BC8B4},
		{"Textured Grip", 0xF8A25A30},
		{"Stippled Grip", 0xC3BEBD36},
		{"Patterned Grip", 0x5B0DA2E8},
		// Muzzle Attachments
		{"Muzzle Brake", 0x3DB51CBB},
		{"Flash Hider", 0x7F97E5EB},
		{"Compensator", 0xE61C3E7F},
		{"Choke", 0x98F3857C},
		// Rail Covers (continued)
		{"Plastic Rail Cover", 0x4E82F3C4},
		{"Metallic Rail Cover", 0xD6F3E62E},
		{"Rubberized Rail Cover", 0x58DE2FBC},
		{"Wooden Rail Cover", 0xA40D9EB2},
		// Additional Components
		{ "Bolt", 0x0A6A6361 },
		{ "Charging Handle", 0xBFA62016 },
		{ "Gas Block", 0x6CB141B0 },
		{ "Buffer Tube", 0x2E6DC570 },
		{ "Barrel Nut", 0x1A74C88F },
		{ "Handguard Cap", 0x1B827DA5 },
		{ "Gas Tube", 0x5D7AEE72 },
		{ "Piston", 0x85E482A6 },
		{ "Spring", 0x93B625F2 },
		{ "Firing Pin", 0xC221A157 },
		{ "Extractor", 0x705B6BFA },
		{ "Bolt Carrier", 0x47F75F73 },
		{ "Forward Assist", 0xE82AD6BC },
		{ "Ejection Port Cover", 0x9D9E745C },
		{ "Trigger", 0x6B0D9AB6 },
		{ "Hammer", 0x08FDD631 },
		{ "Disconnector", 0x8B13F370 },
		{ "Selector Switch", 0x5939E1AB },
		{ "Magazine Release", 0xFCA9D965 },
		{ "Bolt Catch", 0x7ADBF5A1 },
		{ "Pistol Grip", 0xB1F40E2C },
		{ "Trigger Guard", 0x224D8EB8 },
		{ "Buffer", 0xD4D874E1 },
		{ "Buffer Spring", 0x7024F6C9 },
		{ "Stock", 0xD92C228C },
		{ "Recoil Pad", 0xAEC42CDB },
		{ "Sling Mount", 0xC6C9B4FC },
		{ "Handstop", 0x3D4C9DE3 },
		{ "Rail Section", 0x8EC5A2D3 },
		{ "Accessory Mount", 0x963F3C7B },
		{ "Picatinny Rail", 0x1C4C69A7 },
		{ "M-Lok Rail", 0x8DD89C6A },
		{ "Keymod Rail", 0x7FD86D0B },
		{ "QD Sling Swivel", 0xA9B0A771 },
		{ "Barrel Extension", 0x180C81B0 },
		{ "Muzzle Device", 0x7C08D6A9 },
		{ "Gas Block", 0x6CB141B0 },
		{ "Sight Base", 0x583486F9 },
		{ "Bipod Adapter", 0xE1D2F655 },
		{ "Suppressor Adapter", 0x97EC4EF1 },
		{ "Rail Adapter", 0x49169A1A },
		{ "Handguard Adapter", 0xCFF79D9D },
		{ "Accessory Adapter", 0x16A1E8C5 },
		{ "Mounting Hardware", 0x877B0D54 },
		{ "Sling", 0x3FBCB0BB },
		{ "Sling Swivel", 0x1CE8A17E },
		{ "Magazine", 0x24CE2A4A },
		{ "Bipod", 0x0B173FFF },
	};

	std::vector<uint32_t> GetHashListFromHashTable(int Count, uint64_t Table) {
		std::vector<uint32_t> Hashes;

		if (Count && Table) {
			uint64_t* TablePtr = (uint64_t*)Table;

			for (int i = 0; i < Count; i++) {
				if (TablePtr[i]) {
					uint32_t Hash = *(uint32_t*)(TablePtr[i] + 0x10);

					if (Hash) {
						Hashes.push_back(Hash);
					}
				}
			}
		}

		return Hashes;
	}
	void UpgradeWeaponComponents(Ped Ped, uint32_t WeaponHash) {
		auto Hashes = GetHashListFromHashTable(*(uint32_t*)(Patterns::Vars::g_GetWeaponHastList)-1, Patterns::Vars::g_GetWeaponHastList);
		for (auto& Component : Hashes) {
			if (Native::DoesWeaponTakeWeaponComponent(WeaponHash, Component)) {
				Native::GiveWeaponComponentToPed(Ped, WeaponHash, Component);
			}
		}
	}

	void RemoveWeaponUpgrades(Ped Ped, uint32_t WeaponHash) {
		auto Hashes = GetHashListFromHashTable(*(uint32_t*)(Patterns::Vars::g_GetWeaponHastList)-1, Patterns::Vars::g_GetWeaponHastList);
		for (auto& Component : Hashes) {
			if (Native::DoesWeaponTakeWeaponComponent(WeaponHash, Component)) {
				Native::RemoveWeaponComponentFromPed(Ped, WeaponHash, Component);
			}
		}
	}

	void GiveWeapon(Ped Ped, uint32_t WeaponHash, bool EquipNow, bool FullyUpgraded) {
		int Ammo = -1;
		Native::GetMaxAmmo(Ped, WeaponHash, &Ammo);
		Native::GiveWeaponToPed(Ped, WeaponHash, Ammo, false, EquipNow);

		if (FullyUpgraded) {
			UpgradeWeaponComponents(Ped, WeaponHash);
		}
	}

	std::uint32_t m_PreviousWeaponHash{};

	std::vector<std::pair<std::uint32_t, float>> m_OriginalRecoilValues{};
	bool IsRecoilValueCached(std::uint32_t hash)
	{
		return std::find_if(m_OriginalRecoilValues.begin(), m_OriginalRecoilValues.end(), [hash](auto const entry)
			{
				return hash == entry.first;
			}) != m_OriginalRecoilValues.end();
	}

	float GetOriginalRecoilValue(std::uint32_t hash)
	{
		return std::find_if(m_OriginalRecoilValues.begin(), m_OriginalRecoilValues.end(), [hash](auto const entry)
			{
				return hash == entry.first;
			})->second;
	}

	float GetRecoilValue(std::uint32_t hash)
	{
		return m_Vars.m_NoRecoil
			? 0.f
			: GetOriginalRecoilValue(hash);
	}

	std::vector<std::pair<std::uint32_t, float>> m_OriginalSpreadValues{};
	bool IsSpreadValueCached(std::uint32_t hash)
	{
		return std::find_if(m_OriginalSpreadValues.begin(), m_OriginalSpreadValues.end(), [hash](auto const entry)
			{
				return hash == entry.first;
			}) != m_OriginalSpreadValues.end();
	}

	float GetOriginalSpeadValue(std::uint32_t hash)
	{
		return std::find_if(m_OriginalSpreadValues.begin(), m_OriginalSpreadValues.end(), [hash](auto const entry)
			{
				return hash == entry.first;
			})->second;
	}

	float GetSpreadValue(std::uint32_t hash)
	{
		return m_Vars.m_NoSpread
			? 0.f
			: GetOriginalSpeadValue(hash);
	}

	std::vector<std::pair<std::uint32_t, float>> m_OriginalDamageValues{};
	bool IsDamageValueCached(std::uint32_t hash)
	{
		return std::find_if(m_OriginalDamageValues.begin(), m_OriginalDamageValues.end(), [hash](auto const entry)
			{
				return hash == entry.first;
			}) != m_OriginalDamageValues.end();
	}

	float GetOriginalDamageValue(std::uint32_t hash)
	{
		return std::find_if(m_OriginalDamageValues.begin(), m_OriginalDamageValues.end(), [hash](auto const entry)
			{
				return hash == entry.first;
			})->second;
	}

	float GetDamageValue(std::uint32_t hash)
	{
		return m_Vars.m_OneShotKill
			? 1000.f
			: GetOriginalDamageValue(hash);
	}

	std::vector<std::pair<std::uint32_t, float>> m_OriginalCooldownValues{};
	bool IsCooldownValueCached(std::uint32_t hash)
	{
		return std::find_if(m_OriginalCooldownValues.begin(), m_OriginalCooldownValues.end(), [hash](auto const entry)
			{
				return hash == entry.first;
			}) != m_OriginalCooldownValues.end();
	}

	float GetOriginalCooldownValue(std::uint32_t hash)
	{
		return std::find_if(m_OriginalCooldownValues.begin(), m_OriginalCooldownValues.end(), [hash](auto const entry)
			{
				return hash == entry.first;
			})->second;
	}

	float GetCooldownValue(std::uint32_t hash)
	{
		return m_Vars.m_NoCooldown
			? 0.f
			: GetOriginalCooldownValue(hash);
	}

	std::vector<std::pair<std::uint32_t, float>> m_OriginalSpinUpValues{};
	bool IsSpinUpValueCached(std::uint32_t hash)
	{
		return std::find_if(m_OriginalSpinUpValues.begin(), m_OriginalSpinUpValues.end(), [hash](auto const entry)
			{
				return hash == entry.first;
			}) != m_OriginalSpinUpValues.end();
	}

	float GetOriginalSpinUpValue(std::uint32_t hash)
	{
		return std::find_if(m_OriginalSpinUpValues.begin(), m_OriginalSpinUpValues.end(), [hash](auto const entry)
			{
				return hash == entry.first;
			})->second;
	}

	float GetSpinUpValue(std::uint32_t hash)
	{
		return m_Vars.m_NoSpinUp
			? 0.f
			: GetOriginalSpinUpValue(hash);
	}

	struct WeaponObject {
		char _0x0000[0x7C];
		float m_Width;
		char _0x0080[0xC];
		float m_Height;
	};

	inline bool IsPedShooting(Ped ped)
	{
		// PED::IS_PED_SHOOTING
		const auto Coords = Native::GetEntityCoords(ped, TRUE);
		return Native::IsPedShootingInArea(ped, Coords.m_X, Coords.m_Y, Coords.m_Z, Coords.m_X, Coords.m_Y, Coords.m_Z, TRUE, TRUE);
	}

	inline bool IsPedAiming(Ped ped)
	{
		return Native::GetPedConfigFlag(ped, 78, TRUE);
	}

	inline Entity GetCurrent(Ped ped)
	{
		return Native::GetCurrentPedWeaponEntityIndex(ped);
	}

	inline bool g_ShapeTestHit;
	inline std::int32_t g_ShapeTestEntity;
	inline Math::Vector3<float> g_ShapeTestDestination;
	inline Math::Vector3<float> g_ShapeTestSurface;
	inline std::int32_t g_ShapeTestRay;

	inline void RunTimed(int* timer, int ms, std::function<void()> callback)
	{
		if (*timer < Native::GetGameTimer()) {
			*timer = Native::GetGameTimer() + ms;
			callback();
		}
	}

	inline int RandomNumberInRange(int min, int max)
	{
		static bool First = true;
		if (First)
		{
			srand(time(NULL));
			First = false;
		}
		return min + rand() % ((max + 1) - min);
	}

	inline bool IsNumberEven(int num)
	{
		if (num % 2 == 0)
			return true;
		else
			return false;
	}

	inline float SmoothGlide(float aimX, float entityX)
	{
		static float Coords;

		if (aimX > 0 && entityX > 0)
			Coords = aimX - (entityX);

		if (aimX < 0 && entityX < 0)
			Coords = aimX - (entityX);

		if (aimX > 0 && entityX < 0)
			Coords = aimX + (entityX);

		if (aimX < 0 && entityX > 0)
			Coords = aimX + (entityX);

		return Coords;
	}

	inline int GetIndex(std::vector<std::int16_t> v, std::int16_t K)
	{
		auto it = find(v.begin(), v.end(), K);

		// If element was found
		if (it != v.end())
		{
			int index = it - v.begin();
			return index;
		}
		else {
			return -1;
		}
	}

	inline  Math::Vector3<float> Multiply(Math::Vector3<float> vector, float inc)
	{
		vector.m_X *= inc;
		vector.m_Y *= inc;
		vector.m_Z *= inc;
		return vector;
	}

	inline  Math::Vector3<float> Add(Math::Vector3<float> vector, Math::Vector3<float> vector2)
	{
		vector.m_X += vector2.m_X;
		vector.m_Y += vector2.m_Y;
		vector.m_Z += vector2.m_Z;
		return vector;
	}

	inline  Math::Vector3<float> Subtract(Math::Vector3<float> vector, Math::Vector3<float> vector2)
	{
		vector.m_X -= vector2.m_X;
		vector.m_Y -= vector2.m_Y;
		vector.m_Z -= vector2.m_Z;
		return vector;
	}

	inline float DistanceBetweenVectors(Math::Vector3<float> a, Math::Vector3<float> b)
	{
		return (float)sqrt(pow((a.m_X - b.m_X), 2) + pow((a.m_Y - b.m_Y), 2) + pow((a.m_Z - b.m_Z), 2));
	}

	inline float DegreeToRadian(float deg)
	{
		const double Radian = (3.14159265359 / 180) * deg;
		return (float)Radian;
	}

	inline  Math::Vector3<float> GetGameplayCameraDirection()
	{
		auto Rotation = Native::GetGameplayCamRot(NULL);
		FLOAT l_tz = Rotation.m_Z * 0.0174532924f;
		FLOAT l_tx = Rotation.m_X * 0.0174532924f;
		FLOAT l_num = abs(cos(l_tx));
		Rotation.m_X = (-sin(l_tz)) * l_num;
		Rotation.m_Y = (cos(l_tz)) * l_num;
		Rotation.m_Z = sin(l_tx);
		return Rotation;
	}

	inline Math::Vector3<float> RotationToDirection(Math::Vector3<float> rotation)
	{
		float x = DegreeToRadian(rotation.m_X);
		float z = DegreeToRadian(rotation.m_Z);

		float num = abs(cos(x));

		return  Math::Vector3<float>
		{
			-sin(z) * num,
			cos(z) * num,
			sin(x)
		};
	}

	inline Math::Vector3<float> CoordsInFrontOfCam(std::float_t distance)
	{
		const Math::Vector3<float> pos = Native::GetGameplayCamCoord();
		const Math::Vector3<float> rot = Native::GetGameplayCamRot(NULL);
		const Math::Vector3<float> dir = RotationToDirection(rot);
		const Math::Vector3<float> dist = Multiply(dir, distance);
		return Add(pos, dist);
	}

	inline bool Empty(const Math::Vector3<float>& vector)
	{
		return (vector.m_X == 0.f && vector.m_Y == 0.f && vector.m_Z == 0.f);
	}

	inline void Raycast()
	{
		if (IsPedShooting(Native::PlayerPedId()))
		{
			const auto Coords = Native::GetOffsetFromEntityInWorldCoords(GetCurrent(Native::PlayerPedId()), 1.5f, 0.f, 0.f);
			const auto AimCoords = CoordsInFrontOfCam(1000.f);
			g_ShapeTestHit = FALSE;
			g_ShapeTestEntity = NULL;
			g_ShapeTestDestination = {};
			g_ShapeTestSurface = {};
			g_ShapeTestRay = NULL;
			g_ShapeTestRay = Native::_StartShapeTestRay(Coords.m_X, Coords.m_Y, Coords.m_Z, AimCoords.m_X, AimCoords.m_Y, AimCoords.m_Z, -1, Native::PlayerPedId(), 7);
			Native::GetShapeTestResult(g_ShapeTestRay, &g_ShapeTestHit, &g_ShapeTestDestination, &g_ShapeTestSurface, &g_ShapeTestEntity);
		}
	}

	inline Math::Vector3<float> GetAimCoords()
	{
		bool Hit;
		Math::Vector3<float> EndCoords;
		Math::Vector3<float> SurfaceNormal;

		Math::Vector3<float> CamCoords = Native::GetGameplayCamCoord();
		Math::Vector3<float> Direction = RotationToDirection(Native::GetGameplayCamRot(2));
		Math::Vector3<float> FarCoords;

		FarCoords.m_X = CamCoords.m_X + Direction.m_X * 1000.f;
		FarCoords.m_Y = CamCoords.m_Y + Direction.m_Y * 1000.f;
		FarCoords.m_Z = CamCoords.m_Z + Direction.m_Z * 1000.f;

		Entity Entity;
		const int Cast = Native::_StartShapeTestRay(CamCoords.m_X, CamCoords.m_Y, CamCoords.m_Z, FarCoords.m_X, FarCoords.m_Y, FarCoords.m_Z, -1, Native::PlayerPedId(), 7);
		Native::GetShapeTestResult(Cast, &Hit, &EndCoords, &SurfaceNormal, &Entity);

		return EndCoords;
	}

	struct AimbotTypeT { const char* Name; int Type; };

	std::vector<AimbotTypeT> AimEntity = {
		{ "Players Only", 0, },
		{ "Peds Only", 1, },
		{ "Everyone", 2 },
	};

	std::vector<AimbotTypeT> AimFilter = {
		{ "Visible", 0, },
		{ "Logical", 1 },
	};

	std::vector<AimbotTypeT> AimType = {
		{ "Snap", 0, },
		{ "Silent", 1 }
	};

	CWeaponInfo* get_weapon_info() {
		CPed* ped = (*Patterns::Vars::g_PedFactory)->m_local_ped;
		if (ped) {
			CPedWeaponManager* weapon_manager = ped->m_weapon_manager;
			if (weapon_manager) {
				return weapon_manager->m_weapon_info;
			}
		}

		return nullptr;
	}

	Math::Vector3<float> get_direction(Math::Vector3<float> rotation) {
		Math::Vector3<float> tmp;
		tmp.m_Y = rotation.m_Z * 0.0174532924f;
		tmp.m_X = rotation.m_X * 0.0174532924f;
		tmp.m_Z = abs(cos(tmp.m_X));

		return Math::Vector3<float>((-sin(tmp.m_Y)) * tmp.m_Z, (cos(tmp.m_Y)) * tmp.m_Z, sin(tmp.m_X));
	}

	const char* typesNames[] = {
		"Object", "Vehicle", "Ped"
	};
	const char* objectNames[] = {
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

	uint32_t objectValues[] = {
		0xDF9841D7,
		0x757C28D,
		0x6B795EBC,
		0xE2FBA8D8,
		0xC079B265,
		0xC79A987E,
		0xE6CB661E,
		0xD5C0BC07,
		0xF046EA37,
		0xB2274905,
		0x6F9939C7,
		0x113FD533,
		0x4B3D240F,
		0x83C07621
	}; int object_id = 0;

	const char* vehicleNames[] = {
	"Hauler",
	"Issi",
	"Kanjo",
	"Rhapsody",
	"Dominator",
	"Hermes",
	"Clique",
	"Slam Van",
	"Dune",
	"Formula",
	"Vagrant",
	"Hellion",
	"Bf Injection",
	"Bifta",
	"Bkazer",
	"Peyote",
	"Hustler",
	"Imperator",
	"Ellie",
	"Sea Shark",
	"Titan"
	};

	uint32_t vehicleValues[] = {
	0x5A82F9AE,
	0x5BA0FF1E,
	0x18619B7E,
	0x322CF98F,
	0xB2E046FB,
	0xE83C17,
	0xA29F78B0,
	0x42BC5E19,
	0x1FD824AF,
	0x8B213907,
	0x2C1FEA99,
	0xEA6A047F,
	0x432AA566,
	0xEB298297,
	0xB44F0582,
	0x9472CD24,
	0x23CA25F2,
	0x619C1B82,
	0xB472D2B5,
	0xC2974024,
	0x761E2AD3
	}; int vehicle_id = 0;


	const char* pedNames[] = {
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


	uint32_t pedValues[] = {
	0x8D8AC8B9,
	0xFCFA9E1E,
	0xD3939DFD,
	0x9CF26183,
	0x6162EC47,
	0x28ABF95,
	0xE7B31432,
	0x795AC7A8,
	0x3CDCA742,
	0xCE2CB751,
	0xDC59940D,
	0xAC4B4506
	}; int ped_id = 0;

	int m_selected_crosshair;

	const char* crosshairs[11] = { "Cross",  "Target", "Arrow", "Holy Cross", "Reticle", "Reticle Zoomed", "Dot","Plus","Middle Finger", "Box","Star" };
}

void WeaponMenu::Run() {
	Framework::addSubmenu("Weapons", "weapon-menu", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::SubmenuOption("Locker")
			.setTarget("weapon_locker"));

		core->addOption(Framework::Options::SubmenuOption("Modifiers")
			.setTarget("weapon_modifiers"));

		core->addOption(Framework::Options::SubmenuOption("Gravity Gun")
			.setTarget("gravity_gun"));

		core->addOption(Framework::Options::SubmenuOption("Entity Gun")
			.setTarget("entity_gun"));

		core->addOption(Framework::Options::SubmenuOption("Paint Gun")
			.setTarget("paint_gun"));

		core->addOption(Framework::Options::SubmenuOption("Bullet Tracers")
			.setTarget("bullet_tracers"));

		core->addOption(Framework::Options::SubmenuOption("Crosshairs")
			.setTarget("weapon_cross"));

		core->addOption(Framework::Options::ToggleOption("Infinite Ammo")
			.addToggle(&m_Vars.m_InfiniteAmmo));

		core->addOption(Framework::Options::ToggleOption("Infinite Clip")
			.addToggle(&m_Vars.m_InfiniteClip));

		core->addOption(Framework::Options::ToggleOption("Explosive Ammo")
			.addToggle(&m_Vars.m_ExplosiveAmmo));

		core->addOption(Framework::Options::ToggleOption("Fire Ammo")
			.addToggle(&m_Vars.m_FireAmmo));

		core->addOption(Framework::Options::ToggleOption("Rapid Fire")
			.addToggle(&m_Vars.m_RapidFire));

		core->addOption(Framework::Options::ToggleOption("Teleport Gun")
			.addToggle(&m_Vars.m_TeleportGun));

		core->addOption(Framework::Options::ToggleOption("Airstrike Gun")
			.addToggle(&m_Vars.m_AirstrikeGun));

		core->addOption(Framework::Options::ToggleOption("Push Gun")
			.addToggle(&m_Vars.m_PushGun));

		core->addOption(Framework::Options::ToggleOption("Ped Gun")
			.addToggle(&m_Vars.m_PedGun));

		core->addOption(Framework::Options::ToggleOption("Delete Gun")
			.addToggle(&m_Vars.m_DeleteGun));

		core->addOption(Framework::Options::ToggleOption("Repair Gun")
			.addToggle(&m_Vars.m_RepairGun));

		core->addOption(Framework::Options::ToggleOption("Revive Gun")
			.addToggle(&m_Vars.m_DefibrillatorGun));

		core->addOption(Framework::Options::ToggleOption("Laser Sight")
			.addToggle(&m_Vars.m_LaserSight));

		core->addOption(Framework::Options::ToggleOption("Shrink Gun")
			.addToggle(&m_Vars.m_ShrinkGun));

		core->addOption(Framework::Options::ToggleOption("Clown Gun")
			.addToggle(&m_Vars.m_ClownGun));
	});

	Framework::addSubmenu("Crosshairs", "weapon_cross", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Toggle")
			.addToggle(&m_Vars.m_toggle_crosshair));

		core->addOption(Framework::Options::scrollOption<const char*, int>("Crosshair")
			.addScroll(&crosshairs).setPosition(&m_selected_crosshair));
	});

	Framework::addSubmenu("Entity Gun", "entity_gun", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Toggle Entity Gun")
			.addToggle(&m_Vars.m_toggle));

		core->addOption(Framework::Options::scrollOption<const char*, int>("Type")
			.addScroll(&typesNames)
			.setPosition(&m_Vars.m_type));

		core->addOption(Framework::Options::NumberOption<int>("Delay")
			.addNumber(&m_Vars.m_wait)
			.addMin(0).addMax(1000).addStep(1));

		core->addOption(Framework::Options::NumberOption<float>("Speed")
			.addNumber(&m_Vars.m_speed)
			.addMin(0).addMax(10000).addStep(100).setPrecision(0));

		core->addOption(Framework::Options::BreakOption("Types"));

		core->addOption(Framework::Options::scrollOption<const char*, int>("Object")
			.addScroll(&objectNames)
			.setPosition(&m_Vars.m_object));

		core->addOption(Framework::Options::scrollOption<const char*, int>("Vehicle")
			.addScroll(&vehicleNames)
			.setPosition(&m_Vars.m_vehicle));

		core->addOption(Framework::Options::scrollOption<const char*, int>("Ped")
			.addScroll(&pedNames)
			.setPosition(&m_Vars.m_ped));
	});

	Framework::addSubmenu("Paint Gun", "paint_gun", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Toggle")
			.addToggle(&m_Vars.m_EnablePaintGun));

		core->addOption(Framework::Options::NumberOption<float>("Size")
			.addNumber(&m_Vars.m_PaintGunSize).addMin(0).addMax(1000).addStep(0.02f).setPrecision(2));

		core->addOption(Framework::Options::NumberOption<float>("Intensity")
			.addNumber(&m_Vars.m_PaintGunIntensity).addMin(0).addMax(10000).addStep(1).setPrecision(0));

		core->addOption(Framework::Options::BreakOption("Color"));

		core->addOption(Framework::Options::NumberOption<int>("[R]ed")
			.addNumber(&m_Vars.m_PaintGunColor.r).addMin(0.f).addMax(255).addStep(1.f).setPrecision(0.f));

		core->addOption(Framework::Options::NumberOption<int>("[G]reen")
			.addNumber(&m_Vars.m_PaintGunColor.g).addMin(0.f).addMax(255).addStep(1.f).setPrecision(0.f));

		core->addOption(Framework::Options::NumberOption<int>("[B]lue")
			.addNumber(&m_Vars.m_PaintGunColor.b).addMin(0.f).addMax(255).addStep(1.f).setPrecision(0.f));
	});

	Framework::addSubmenu("Gravity Gun", "gravity_Gun", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Toggle")
			.addToggle(&m_Vars.m_EnableGravityGun));

		core->addOption(Framework::Options::NumberOption<float>("Distance")
			.addNumber(&m_Vars.m_GravityGunDistance).addMin(100).addMax(180).addStep(1).setPrecision(0.f));

		core->addOption(Framework::Options::NumberOption<float>("Force")
			.addNumber(&m_Vars.m_GravityGunForce).addMin(100).addMax(1000).addStep(1).setPrecision(0.f));
	});


	Framework::addSubmenu("Bullet Tracers", "bullet_tracers", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Toggle")
			.addToggle(&m_Vars.m_EnableBulletTracers));

		core->addOption(Framework::Options::ToggleOption("Rainbow")
			.addToggle(&m_Vars.m_RainbowBulletTracers));

		core->addOption(Framework::Options::NumberOption<int>("Duration")
			.addNumber(&m_Vars.m_BulletTracerDuration).addMin(0).addMax(30000).addStep(1));

		core->addOption(Framework::Options::BreakOption("Color"));

		core->addOption(Framework::Options::NumberOption<int>("[R]ed")
			.addNumber(&m_Vars.m_BulletTracerColor.r).addMin(0.f).addMax(255).addStep(1.f).setPrecision(0.f));

		core->addOption(Framework::Options::NumberOption<int>("[G]reen")
			.addNumber(&m_Vars.m_BulletTracerColor.g).addMin(0.f).addMax(255).addStep(1.f).setPrecision(0.f));

		core->addOption(Framework::Options::NumberOption<int>("[B]lue")
			.addNumber(&m_Vars.m_BulletTracerColor.b).addMin(0.f).addMax(255).addStep(1.f).setPrecision(0.f));
	});

	Framework::addSubmenu("Modifiers", "weapon_modifiers", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("No Recoil")
			.addToggle(&m_Vars.m_NoRecoil)
			.addClick([] {
				if (!m_Vars.m_NoRecoil) {
					if (*Patterns::Vars::g_PedFactory) {
						if ((*Patterns::Vars::g_PedFactory)->m_local_ped) {
							if ((*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager) {
								const auto* WeaponManager = (*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager;
								if (WeaponManager) {
									const auto CurrentWeapon = WeaponManager->m_selected_weapon_hash;
									WeaponManager->m_weapon_info->m_explosion_shake_amplitude = GetRecoilValue(CurrentWeapon);
								}
							}
						}
					}
				}			
			}));

		core->addOption(Framework::Options::ToggleOption("No Spread")
			.addToggle(&m_Vars.m_NoSpread)
			.addClick([] {
				if (!m_Vars.m_NoSpread) {
					if (*Patterns::Vars::g_PedFactory) {
						if ((*Patterns::Vars::g_PedFactory)->m_local_ped) {
							if ((*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager) {
								const auto* WeaponManager = (*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager;
								if (WeaponManager) {
									const auto CurrentWeapon = WeaponManager->m_selected_weapon_hash;
									WeaponManager->m_weapon_info->m_accuracy_spread = GetSpreadValue(CurrentWeapon);
								}
							}
						}
					}
				}
			}));

		core->addOption(Framework::Options::ToggleOption("One Shot Kill")
			.addToggle(&m_Vars.m_OneShotKill)
			.addClick([] {
				if (!m_Vars.m_OneShotKill) {
					if (*Patterns::Vars::g_PedFactory) {
						if ((*Patterns::Vars::g_PedFactory)->m_local_ped) {
							if ((*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager) {
								const auto* WeaponManager = (*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager;
								if (WeaponManager) {
									const auto CurrentWeapon = WeaponManager->m_selected_weapon_hash;
									WeaponManager->m_weapon_info->m_damage = GetDamageValue(CurrentWeapon);
								}
							}
						}
					}
				}
			}));

		core->addOption(Framework::Options::ToggleOption("No Cooldown")
			.addToggle(&m_Vars.m_NoCooldown)
			.addClick([] {
				if (!m_Vars.m_NoCooldown) {
					if (*Patterns::Vars::g_PedFactory) {
						if ((*Patterns::Vars::g_PedFactory)->m_local_ped) {
							if ((*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager) {
								const auto* WeaponManager = (*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager;
								if (WeaponManager) {
									const auto CurrentWeapon = WeaponManager->m_selected_weapon_hash;
									WeaponManager->m_weapon_info->m_time_between_shots = GetCooldownValue(CurrentWeapon);
								}
							}
						}
					}
				}
			}));

		core->addOption(Framework::Options::ToggleOption("No Spin-up")
			.addToggle(&m_Vars.m_NoSpinUp)
			.addClick([] {
				if (!m_Vars.m_NoSpinUp) {
					if (*Patterns::Vars::g_PedFactory) {
						if ((*Patterns::Vars::g_PedFactory)->m_local_ped) {
							if ((*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager) {
								const auto* WeaponManager = (*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager;
								if (WeaponManager) {
									const auto CurrentWeapon = WeaponManager->m_selected_weapon_hash;
									WeaponManager->m_weapon_info->m_spinup_time = GetSpinUpValue(CurrentWeapon);
								}
							}
						}
					}
				}
			}));

		core->addOption(Framework::Options::ToggleOption("Shotgun Mode")
			.addToggle(&m_Vars.m_ShotGunMode)
			.addClick([] {
				if (!m_Vars.m_ShotGunMode) {
					if (*Patterns::Vars::g_PedFactory) {
						if ((*Patterns::Vars::g_PedFactory)->m_local_ped) {
							if ((*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager) {
								const auto* WeaponManager = (*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager;
								if (WeaponManager) {
									auto WeaponInfo = (*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager->m_weapon_info;
									if (WeaponInfo) {
										WeaponInfo->m_batch_spread = 0.1f;
										WeaponInfo->m_bullets_in_batch = 1;
										WeaponInfo->m_force = 200.f;
										WeaponInfo->m_speed = 5000.f;
									}
								}
							}
						}
					}
				}
			}));

		core->addOption(Framework::Options::BreakOption("Scaling"));

		core->addOption(Framework::Options::ToggleNumberOption<float>("Width")
			.addToggle(&m_Vars.m_Width).addNumber(&m_Vars.m_WidthVal)
			.addMin(0.f).addMax(100.f).addStep(0.01f).setPrecision(2));

		core->addOption(Framework::Options::ToggleNumberOption<float>("Height")
			.addToggle(&m_Vars.m_Height).addNumber(&m_Vars.m_HeightVal)
			.addMin(0.f).addMax(100.f).addStep(0.01f).setPrecision(2));
	});

	Framework::addSubmenu("Weapon Locker", "weapon_locker", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ButtonOption("Give All Weapons")
			.addClick([] {
				for (auto& Weapon : WeaponsList) {
					GiveWeapon(Menu::GetLocalPlayer().m_Ped, Weapon.id, false, false);
				}
			}));

		core->addOption(Framework::Options::ButtonOption("Remove All Weapons")
			.addClick([] {
				Native::RemoveAllPedWeapons(Native::PlayerPedId(), 0);
			}));
		
		core->addOption(Framework::Options::ButtonOption("Max Weapon Ammo")
			.addClick([] {
				for (auto& Weapon : WeaponsList) {
					int Ammo = -1;
					Native::GetMaxAmmo(Menu::GetLocalPlayer().m_Ped, Weapon.id, &Ammo);
					Native::AddAmmoToPed(Menu::GetLocalPlayer().m_Ped, Weapon.id, Ammo);
				}
			}));
		
		core->addOption(Framework::Options::BreakOption("Weapons"));
		for (auto& Weapon : WeaponsList) {
			core->addOption(Framework::Options::ButtonOption(Weapon.name.c_str())
				.addClick([=] {
					if (Native::HasPedGotWeapon(Menu::GetLocalPlayer().m_Ped, Weapon.id, false)) {
						Menu::Notify::stacked(std::format("{} Is Already In Your Inventory", Weapon.name).c_str());
					}
					else {
						GiveWeapon(Menu::GetLocalPlayer().m_Ped, Weapon.id, false, false);
					}
				}));
		}
	});
}

void RGBFadee()
{
	if (m_Vars.m_BulletTracerColor.r > 0 && m_Vars.m_BulletTracerColor.b == 0) {
		m_Vars.m_BulletTracerColor.r--;
		m_Vars.m_BulletTracerColor.g++;
	}
	if (m_Vars.m_BulletTracerColor.g > 0 && m_Vars.m_BulletTracerColor.r == 0) {
		m_Vars.m_BulletTracerColor.g--;
		m_Vars.m_BulletTracerColor.b++;
	}
	if (m_Vars.m_BulletTracerColor.b > 0 && m_Vars.m_BulletTracerColor.g == 0) {
		m_Vars.m_BulletTracerColor.r++;
		m_Vars.m_BulletTracerColor.b--;
	}
}
void RainbowPaint()
{
	if (m_Vars.m_PaintGunColor.r > 0 && m_Vars.m_PaintGunColor.b == 0) {
		m_Vars.m_PaintGunColor.r--;
		m_Vars.m_PaintGunColor.g++;
	}
	if (m_Vars.m_PaintGunColor.g > 0 && m_Vars.m_PaintGunColor.r == 0) {
		m_Vars.m_PaintGunColor.g--;
		m_Vars.m_PaintGunColor.b++;
	}
	if (m_Vars.m_PaintGunColor.b > 0 && m_Vars.m_PaintGunColor.g == 0) {
		m_Vars.m_PaintGunColor.r++;
		m_Vars.m_PaintGunColor.b--;
	}
}
void WeaponMenu::Update() {
	Raycast();
	Run();

	if (m_Vars.m_EnablePaintGun) {
		static std::vector<Math::Vector3<float>> a;
		Math::Vector3<float> b;
		if (Native::GetPedLastWeaponImpactCoord(Native::PlayerPedId(), &b)); {
			a.push_back(b);
		}
		for (auto pos : a) {
			Native::DrawLightWithRange(pos.m_X, pos.m_Y, pos.m_Z, m_Vars.m_PaintGunColor.r, m_Vars.m_PaintGunColor.g, m_Vars.m_PaintGunColor.b, m_Vars.m_PaintGunSize, m_Vars.m_PaintGunIntensity);

		}
	}

	Entity g_TargetEntity = 0;
	Math::Vector3<float> g_Rotation = {};

	if (m_Vars.m_EnableGravityGun && IsPedShooting(Native::PlayerPedId()) && !Native::DoesEntityExist(g_TargetEntity))
	{
		if (Native::DoesEntityExist(g_ShapeTestEntity))
		{
			// Safety checks
			if (!Native::IsEntityAnObject(g_ShapeTestEntity) && !Native::IsEntityAPed(g_ShapeTestEntity) && !Native::IsEntityAVehicle(g_ShapeTestEntity) || Native::IsPedAPlayer(g_ShapeTestEntity)) {
				g_TargetEntity = 0;
				return;
			}

			g_TargetEntity = g_ShapeTestEntity;
			if (Native::NetworkIsSessionActive())
				Engine::ChangeNetworkObjectOwner(g_ShapeTestEntity, Engine::GetNetworkGamePlayer(Native::PlayerId()));
			g_Rotation = Native::GetEntityRotation(g_ShapeTestEntity, 2);
			Native::SetEntityCollision(g_ShapeTestEntity, FALSE, FALSE);
		}
	}
	else if (m_Vars.m_EnableGravityGun && Native::DoesEntityExist(g_TargetEntity))
	{
		if (!IsPedAiming(Native::PlayerPedId()))
			g_TargetEntity = 0;

		Native::NetworkRequestControlOfEntity(g_TargetEntity);
		if (Native::NetworkHasControlOfEntity(g_TargetEntity)) {
			const auto Direction = GetGameplayCameraDirection();
			const auto GravityPosition = Native::GetOffsetFromEntityInWorldCoords(Native::PlayerPedId(), 0.4f, 0.f, 0.6f);
			const auto EntityCoords = Native::GetEntityCoords(g_TargetEntity, TRUE);
			Native::SetEntityVelocity(g_TargetEntity,
				((GravityPosition.m_X + (Direction.m_X * m_Vars.m_GravityGunDistance)) - EntityCoords.m_X) * 4.f,
				((GravityPosition.m_Y + (Direction.m_Y * m_Vars.m_GravityGunDistance)) - EntityCoords.m_Y) * 4.f,
				((GravityPosition.m_Z + (Direction.m_Z * m_Vars.m_GravityGunDistance)) - EntityCoords.m_Z) * 4.f);
			Native::SetEntityRotation(g_TargetEntity, g_Rotation.m_X, g_Rotation.m_Y, g_Rotation.m_Z, 2, FALSE);

			if (IsPedShooting(Native::PlayerPedId())) {
				const auto ForcePos = Native::GetOffsetFromEntityGivenWorldCoords(g_TargetEntity,
					(GravityPosition.m_X + (Direction.m_X * m_Vars.m_GravityGunForce)),
					(GravityPosition.m_Y + (Direction.m_Y * m_Vars.m_GravityGunForce)),
					(GravityPosition.m_Z + (Direction.m_Z * m_Vars.m_GravityGunForce)));

				Native::ApplyForceToEntity(g_TargetEntity, 1, ForcePos.m_X, ForcePos.m_Y, ForcePos.m_Z, FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE, TRUE);
				Native::SetEntityCollision(g_TargetEntity, TRUE, TRUE);
				g_TargetEntity = 0;
			}
		}
	}

	if (m_Vars.m_EnableBulletTracers) {
		if (m_Vars.m_RainbowBulletTracers)
		{
			RGBFadee();
		}

		if (IsPedShooting(Native::PlayerPedId()))
		{
			Color BulletTracerColor =
			{
				m_Vars.m_BulletTracerColor.r,
				m_Vars.m_BulletTracerColor.g,
				m_Vars.m_BulletTracerColor.b
			};

			const auto Direction = RotationToDirection(Native::GetGameplayCamRot(0));
			const auto TracerStart = Native::GetWorldPositionOfEntityBone(Native::PlayerPedId(), Native::GetPedBoneIndex(Native::PlayerPedId(), 0x6F06));
			const auto TracerEnd = Add(Native::_GetGameplayCamCoords(), (Multiply(Direction, 100.f)));

			m_Vars.m_BulletTracers.push_back({ TracerStart, TracerEnd, BulletTracerColor, GetTickCount(), 0 });
		}

		if (m_Vars.m_BulletTracers.empty())
			return;

		for (auto& Tracer : m_Vars.m_BulletTracers)
		{
			if (GetTickCount() - Tracer.m_Time < m_Vars.m_BulletTracerDuration) {
				if (Tracer.m_Alpha != 255)
					Tracer.m_Alpha += 5;

				Native::DrawLine(Tracer.m_Start.m_X, Tracer.m_Start.m_Y, Tracer.m_Start.m_Z, Tracer.m_End.m_X, Tracer.m_End.m_Y, Tracer.m_End.m_Z, Tracer.m_Color.r, Tracer.m_Color.g, Tracer.m_Color.b, Tracer.m_Alpha);
			}
			if (GetTickCount() - Tracer.m_Time > m_Vars.m_BulletTracerDuration - 100) {
				if (Tracer.m_Alpha != 0)
					Tracer.m_Alpha -= 5;

				Native::DrawLine(Tracer.m_Start.m_X, Tracer.m_Start.m_Y, Tracer.m_Start.m_Z, Tracer.m_End.m_X, Tracer.m_End.m_Y, Tracer.m_End.m_Z, Tracer.m_Color.r, Tracer.m_Color.g, Tracer.m_Color.b, Tracer.m_Alpha);
			}

			if (GetTickCount() - Tracer.m_Time > m_Vars.m_BulletTracerDuration + 1000) {
				m_Vars.m_BulletTracers.erase(m_Vars.m_BulletTracers.begin());
			}
		}
	}

	if (m_Vars.m_LaserSight) {
		if (IsPedAiming(Native::PlayerPedId()))
		{
			const auto Start = Native::GetWorldPositionOfEntityBone(Native::PlayerPedId(), Native::GetPedBoneIndex(Native::PlayerPedId(), 0x6F06));
			const Math::Vector3<float> Updated = { 0.0f, 200.0f, (Native::Tan(Native::GetGameplayCamRelativePitch()) * 200.0f) };
			const auto End = Native::GetOffsetFromEntityInWorldCoords(Native::PlayerPedId(), Updated.m_X, Updated.m_Y, Updated.m_Z);
			Native::DrawLine(Start.m_X, Start.m_Y, Start.m_Z, End.m_X, End.m_Y, End.m_Z, 219, 75, 75, 255);
		}
	}

	if (m_Vars.m_DefibrillatorGun) {
		Entity Ped;
		if (Native::GetEntityPlayerIsFreeAimingAt(Native::PlayerId(), &Ped))
		{
			if (Native::IsEntityAPed(Ped) && Native::IsEntityDead(Ped, FALSE)) {
				Native::ResurrectPed(Ped);
				Native::ReviveInjuredPed(Ped);
				Native::SetEntityHealth(Ped, 100);
				Native::ClearPedTasks(Ped);
				Native::ClearPedTasksImmediately(Ped);
				Native::ClearPedSecondaryTask(Ped);
			}
		}
	}

	if (m_Vars.m_ShrinkGun) {
		if (Native::DoesEntityExist(g_ShapeTestEntity)) {
			if (Native::IsEntityAPed(g_ShapeTestEntity)) {
				Native::SetPedConfigFlag(g_ShapeTestEntity, 223, TRUE);
			}
		}
	}

	if (m_Vars.m_ClownGun) {
		Utils::GetFiberPool()->Push([] {
			if (IsPedShooting(Native::PlayerPedId()))
			{
				Native::RequestNamedPtfxAsset("scr_rcbarry2");
				while (!Native::HasNamedPtfxAssetLoaded("scr_rcbarry2"))
					Utils::GetFiberManager()->GoToMainFiber();
				Math::Vector3<float> v0, v1;
				Native::GetModelDimensions(Native::GetSelectedPedWeapon(Native::PlayerPedId()), &v0, &v1);
				Native::_UseParticleFxAssetNextCall("scr_rcbarry2");
				Native::_StartParticleFxNonLoopedOnEntity2("muz_clown",GetCurrent(Native::PlayerPedId()), (v0.m_X - v1.m_X) / 2.0f + 0.7f, 0.f, 0.f, 0.f, 180.f, 0.f, 1.f, 1, 1, 1);
			}
			});
	}

	if (m_Vars.m_InfiniteAmmo) {
		Native::SetPedInfiniteAmmo(Native::PlayerPedId(), TRUE, NULL);
	}
	else {
		Native::SetPedInfiniteAmmo(Native::PlayerPedId(), FALSE, NULL);
	}

	if (m_Vars.m_InfiniteClip) {
		Native::SetPedInfiniteAmmoClip(Native::PlayerPedId(), TRUE);
	}
	else {
		Native::SetPedInfiniteAmmoClip(Native::PlayerPedId(), FALSE);
	}

	if (m_Vars.m_RepairGun) {
		if (IsPedShooting(Native::PlayerPedId())) {
			if (Native::DoesEntityExist(g_ShapeTestEntity))
			{
				if (!Native::IsEntityAVehicle(g_ShapeTestEntity))
					return;

				if (Native::NetworkIsSessionActive())
					Engine::ChangeNetworkObjectOwner(g_ShapeTestEntity, Engine::GetNetworkGamePlayer(Native::PlayerId()));

				Native::SetVehicleFixed(g_ShapeTestEntity);
				Native::SetVehicleDeformationFixed(g_ShapeTestEntity);
				Native::SetVehicleDirtLevel(g_ShapeTestEntity, 0.f);
				Native::SetVehicleNumberPlateText(g_ShapeTestEntity, "BALLER");
			}
		}
	}


	if (m_Vars.m_ExplosiveAmmo) {
		const auto BulletCoords = g_ShapeTestDestination;
		if (IsPedShooting(Native::PlayerPedId()) && !Empty(BulletCoords))
			Native::AddExplosion(BulletCoords.m_X, BulletCoords.m_Y, BulletCoords.m_Z, eExplosionTag::GRENADE, 9999.f, TRUE, FALSE, 0.f, FALSE);
	}

	if (m_Vars.m_FireAmmo) {
		const auto BulletCoords = g_ShapeTestDestination;
		if (IsPedShooting(Native::PlayerPedId()) && !Empty(BulletCoords))
			Native::StartScriptFire(BulletCoords.m_X, BulletCoords.m_Y, BulletCoords.m_Z, 24, TRUE);
	}

	bool g_LastPortalGun = false;
	int g_FirstPortal = 0;
	Math::Vector3<float> g_FirstPortalCoords;
	int g_SecondPortal = 0;
	Math::Vector3<float> g_SecondPortalCoords;
	bool g_Transition = false;

	if (m_Vars.m_PortalGun) {
		const auto Ped = Native::PlayerPedId();
		const auto Coords = Native::GetEntityCoords(Ped, TRUE);
		const auto Shooting = IsPedShooting(Ped);
		auto BulletCoords = g_ShapeTestDestination;

		if (Shooting && g_FirstPortal && g_SecondPortal)
		{
			// Cleanup, so the user can create new portals
			Native::DeleteCheckpoint(g_FirstPortal);
			g_FirstPortal = 0;
			Native::DeleteCheckpoint(g_SecondPortal);
			g_SecondPortal = 0;
		}

		// Checkpoints are handled by the game, so you may never put those in a tick
		if (Shooting && !Empty(BulletCoords))
		{
			if (!g_FirstPortal && !g_SecondPortal)
			{
				// Create the first portal checkpoint at the given BulletCoords and set its properties
				g_FirstPortal = Native::CreateCheckpoint(47, BulletCoords.m_X, BulletCoords.m_Y, BulletCoords.m_Z, 0.f, 0.f, 0.f, 2.f, 50, 109, 168, 255, 0);
				Native::SetCheckpointCylinderHeight(g_FirstPortal, 1.5f, 1.5f, 2.f);
				g_FirstPortalCoords = BulletCoords;
			}
			else if (g_FirstPortal && !g_SecondPortal)
			{
				// Create the second portal checkpoint at the given BulletCoords and set its properties
				g_SecondPortal = Native::CreateCheckpoint(47, BulletCoords.m_X, BulletCoords.m_Y, BulletCoords.m_Z, 0.f, 0.f, 0.f, 2.f, 240, 95, 50, 255, 0);
				Native::SetCheckpointCylinderHeight(g_SecondPortal, 1.5f, 1.5f, 2.f);
				g_SecondPortalCoords = BulletCoords;
			}
		}

		// Update
		if (!Empty(g_FirstPortalCoords) && !Empty(g_SecondPortalCoords))
		{
			// Handle teleportation between elements
			if (DistanceBetweenVectors(Coords, g_FirstPortalCoords) <= 1.5f && !g_Transition)
			{
				// First portal to the second 
				Native::SetEntityCoords(Ped, g_SecondPortalCoords.m_X, g_SecondPortalCoords.m_Y, g_SecondPortalCoords.m_Z, FALSE, FALSE, FALSE, FALSE);
				g_Transition = true;
			}


			// Handle teleportation between elements
			if (DistanceBetweenVectors(Coords, g_SecondPortalCoords) <= 1.5f && !g_Transition)
			{
				// Second portal to the first 
				Native::SetEntityCoords(Ped, g_FirstPortalCoords.m_X, g_FirstPortalCoords.m_Y, g_FirstPortalCoords.m_Z, FALSE, FALSE, FALSE, FALSE);
				g_Transition = true;
			}
		}

		// Stop transitioning when out of the element circle
		if ((DistanceBetweenVectors(Coords, g_SecondPortalCoords) >= 1.5f && DistanceBetweenVectors(Coords, g_FirstPortalCoords) >= 1.5f))
			g_Transition = false;
	}
	else if (g_LastPortalGun != m_Vars.m_PortalGun)
	{
		// Cleanup
		Native::DeleteCheckpoint(g_FirstPortal);
		g_FirstPortal = 0;
		Native::DeleteCheckpoint(g_SecondPortal);
		g_SecondPortal = 0;
	}

	g_LastPortalGun = m_Vars.m_PortalGun;

	if (m_Vars.m_TeleportGun) {
		if (IsPedShooting(Native::PlayerPedId()))
		{
			const auto BulletCoord = g_ShapeTestDestination;
			if (!Empty(BulletCoord))
				Native::SetEntityCoords(Native::PlayerPedId(), BulletCoord.m_X, BulletCoord.m_Y, BulletCoord.m_Z, FALSE, TRUE, TRUE, FALSE);
		}
	}

	if (m_Vars.m_AirstrikeGun) {
		Utils::GetFiberPool()->Push([] {
			if (IsPedShooting(Native::PlayerPedId()))
			{
				const auto BulletCoord = g_ShapeTestDestination;
				if (!Empty(BulletCoord))
				{
					const auto Airstrike = Joaat("WEAPON_AIRSTRIKE_ROCKET");
					Native::RequestWeaponAsset(Airstrike, 31, FALSE);
					while (!Native::HasWeaponAssetLoaded(Airstrike))
						Utils::GetFiberManager()->GoToMainFiber();
					Native::ShootSingleBulletBetweenCoords(BulletCoord.m_X, BulletCoord.m_Y, BulletCoord.m_Z + 50.f, BulletCoord.m_X, BulletCoord.m_Y, BulletCoord.m_Z, 250, 1, Airstrike, Native::PlayerPedId(), 1, 0, -1.0);
					Native::RemoveWeaponAsset(Airstrike);
				}
			}
		});
	}

	if (m_Vars.m_PushGun) {
		const auto Ped = Native::PlayerPedId();
		if (IsPedShooting(Ped))
		{
			if (Native::DoesEntityExist(g_ShapeTestEntity))
			{
				if (Native::IsPedAPlayer(g_ShapeTestEntity))
					return;

				if (Native::NetworkIsSessionActive())
					Engine::ChangeNetworkObjectOwner(g_ShapeTestEntity, Engine::GetNetworkGamePlayer(Native::PlayerId()));

				const auto MyCoords = Native::GetEntityCoords(Ped, TRUE);
				const auto Rotation = Native::GetGameplayCamRot(2);
				const auto Pitch = DegreeToRadian(Rotation.m_X);
				const auto Yaw = DegreeToRadian(Rotation.m_Z + 90.f);

				const auto Coords = Native::GetEntityCoords(g_ShapeTestEntity, FALSE);
				Math::Vector3<float> Velocity{};

				Velocity.m_X = MyCoords.m_X + (1000 * cos(Pitch) * cos(Yaw)) - Coords.m_X;
				Velocity.m_Y = MyCoords.m_Y + (1000 * sin(Yaw) * cos(Pitch)) - Coords.m_Y;
				Velocity.m_Z = MyCoords.m_Z + (1000 * sin(Pitch)) - Coords.m_Z;

				Native::SetEntityVelocity(g_ShapeTestEntity, Velocity.m_X, Velocity.m_Y, Velocity.m_Z);
			}
		}
	}

	if (m_Vars.m_RapidFire) {
		Hash WeaponHash;
		Native::GetCurrentPedWeapon(Native::PlayerPedId(), &WeaponHash, FALSE);

		if (WeaponHash == 0x42BF8A85 || WeaponHash == 0xB62D1F67)
			return;

		if (Native::IsControlPressed(1, (int)INPUT_ATTACK))
		{
			const auto Direction = RotationToDirection(Native::GetGameplayCamRot(0));
			const auto EndCoord = Add(Native::_GetGameplayCamCoords(), (Multiply(Direction, 100.f)));
			Native::SetPedShootsAtCoord(Native::PlayerPedId(), EndCoord.m_X, EndCoord.m_Y, EndCoord.m_Z, true);
			Native::_PedSkipNextReloading(Native::PlayerPedId());
		}
	}

	if (m_Vars.m_PedGun) {
		if (IsPedShooting(Native::PlayerPedId())) {
			const auto BulletCoord = g_ShapeTestDestination;
			if (!Empty(BulletCoord))
				Native::CreateRandomPed(BulletCoord.m_X, BulletCoord.m_Y, BulletCoord.m_Z);
		}
	}

	if (m_Vars.m_DeleteGun) {
		if (IsPedShooting(Native::PlayerPedId()))
		{
			if (Native::DoesEntityExist(g_ShapeTestEntity))
			{
				if (Native::NetworkIsSessionActive())
					Engine::ChangeNetworkObjectOwner(g_ShapeTestEntity, Engine::GetNetworkGamePlayer(Native::PlayerId()));

				Native::SetEntityAsMissionEntity(g_ShapeTestEntity, TRUE, TRUE);
				Native::DeleteEntity(&g_ShapeTestEntity);
			}
		}
	}

	if (m_Vars.m_Width) {
		WeaponObject* _Object = Cast<WeaponObject>(Engine::GetEntityAddress(Menu::GetLocalPlayer().m_WeaponEntity));
		if (_Object) {
			_Object->m_Width = m_Vars.m_WidthVal;
		}
	}
	else {
		if (m_Vars.m_WidthVal != 1.f) {
			WeaponObject* _Object = Cast<WeaponObject>(Engine::GetEntityAddress(Menu::GetLocalPlayer().m_WeaponEntity));
			if (_Object) {
				m_Vars.m_WidthVal = _Object->m_Width;
				_Object->m_Width = 1.f;
			}
		}
	}

	if (m_Vars.m_Height) {
		WeaponObject* _Object = Cast<WeaponObject>(Engine::GetEntityAddress(Menu::GetLocalPlayer().m_WeaponEntity));
		if (_Object) {
			_Object->m_Height = m_Vars.m_HeightVal;
		}
	}
	else {
		if (m_Vars.m_HeightVal != 1.f) {
			WeaponObject* _Object = Cast<WeaponObject>(Engine::GetEntityAddress(Menu::GetLocalPlayer().m_WeaponEntity));
			if (_Object) {
				m_Vars.m_HeightVal = _Object->m_Height;
				_Object->m_Height = 1.f;
			}
		}
	}

	if (m_Vars.m_NoRecoil) {
		if (*Patterns::Vars::g_PedFactory) {
			if ((*Patterns::Vars::g_PedFactory)->m_local_ped) {
				if ((*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager) {
					const auto CurrentWeapon = (*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager->m_selected_weapon_hash;
					if (m_PreviousWeaponHash != CurrentWeapon) {
						if (!IsRecoilValueCached(CurrentWeapon))
							m_OriginalRecoilValues.push_back({ CurrentWeapon, (*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager->m_weapon_info->m_explosion_shake_amplitude });

						(*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager->m_weapon_info->m_explosion_shake_amplitude = GetRecoilValue(CurrentWeapon);
					}
				}
			}
		}
	}

	if (m_Vars.m_NoSpread) {
		if (*Patterns::Vars::g_PedFactory) {
			if ((*Patterns::Vars::g_PedFactory)->m_local_ped) {
				if ((*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager) {
					const auto CurrentWeapon = (*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager->m_selected_weapon_hash;
					if (m_PreviousWeaponHash != CurrentWeapon) {
						if (!IsSpreadValueCached(CurrentWeapon))
							m_OriginalSpreadValues.push_back({ CurrentWeapon, (*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager->m_weapon_info->m_accuracy_spread });

						(*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager->m_weapon_info->m_accuracy_spread = GetSpreadValue(CurrentWeapon);
					}
				}
			}
		}
	}

	if (m_Vars.m_OneShotKill) {
		if (*Patterns::Vars::g_PedFactory) {
			if ((*Patterns::Vars::g_PedFactory)->m_local_ped) {
				if ((*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager) {
					const auto CurrentWeapon = (*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager->m_selected_weapon_hash;
					if (m_PreviousWeaponHash != CurrentWeapon) {
						if (!IsDamageValueCached(CurrentWeapon))
							m_OriginalDamageValues.push_back({ CurrentWeapon, (*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager->m_weapon_info->m_damage });

						(*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager->m_weapon_info->m_damage = GetDamageValue(CurrentWeapon);
					}
				}
			}
		}
	}

	if (m_Vars.m_NoCooldown) {
		if (*Patterns::Vars::g_PedFactory) {
			if ((*Patterns::Vars::g_PedFactory)->m_local_ped) {
				if ((*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager) {
					const auto CurrentWeapon = (*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager->m_selected_weapon_hash;
					if (m_PreviousWeaponHash != CurrentWeapon) {
						if (!IsCooldownValueCached(CurrentWeapon))
							m_OriginalCooldownValues.push_back({ CurrentWeapon, (*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager->m_weapon_info->m_time_between_shots });

						(*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager->m_weapon_info->m_time_between_shots = GetCooldownValue(CurrentWeapon);
					}
				}
			}
		}
	}

	if (m_Vars.m_toggle_crosshair) {
		switch (m_selected_crosshair) {
		case 0:
			Framework::Render::DrawSprite({ "srange_gen", "hit_cross" }, { 0.5f, 0.5f }, { 0.02f, 0.03f }, Color(0, 255, 0, 180), 0.0f);
			break;
		case 1:
			Framework::Render::DrawSprite({ "helicopterhud", "hud_target" }, { 0.5f, 0.5f }, { 0.02f, 0.03f }, Color(0, 255, 0, 180), 0.0f);
			break;
		case 2:
			Framework::Render::DrawSprite({ "helicopterhud", "hudarrow" }, { 0.5f, 0.5f }, { 0.02f, 0.03f }, Color(0, 255, 0, 180), 0.0f);
			break;
		case 3:
			Framework::Render::DrawSprite({ "mptattoos3", "tattoo_reach_rank_r_10" }, { 0.5f, 0.5f }, { 0.02f, 0.03f }, Color(0, 255, 0, 180), 0.0f);
			break;
		case 4:
			Framework::Render::DrawSprite({ "darts", "dart_reticules" }, { 0.5f, 0.5f }, { 0.02f, 0.03f }, Color(0, 255, 0, 180), 0.0f);
			break;
		case 5:
			Framework::Render::DrawSprite({ "darts", "dart_reticules_zoomed" }, { 0.5f, 0.5f }, { 0.02f, 0.03f }, Color(0, 255, 0, 180), 0.0f);
			break;
		case 6:
			Framework::Render::DrawSprite({ "shared", "emptydot_32" }, { 0.5f, 0.5f }, { 0.02f, 0.03f }, Color(0, 255, 0, 180), 0.0f);
			break;
		case 7:
			Framework::Render::DrawSprite({ "shared", "menuplus_32" }, { 0.5f, 0.5f }, { 0.02f, 0.03f }, Color(0, 255, 0, 180), 0.0f);
			break;
		case 8:
			Framework::Render::DrawSprite({ "mp_freemode_mc", "mouse" }, { 0.5f, 0.5f }, { 0.02f, 0.03f }, Color(0, 255, 0, 180), 0.0f);
			break;
		case 9:
			Framework::Render::DrawSprite({ "visualflow", "crosshair" }, { 0.5f, 0.5f }, { 0.02f, 0.03f }, Color(0, 255, 0, 180), 0.0f);
			break;
		case 10:
			Framework::Render::DrawSprite({ "shared", "newstar_32" }, { 0.5f, 0.5f }, { 0.02f, 0.03f }, Color(0, 255, 0, 180), 0.0f);
			break;
		}
	}

	if (m_Vars.m_NoSpinUp) {
		if (*Patterns::Vars::g_PedFactory) {
			if ((*Patterns::Vars::g_PedFactory)->m_local_ped) {
				if ((*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager) {
					const auto CurrentWeapon = (*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager->m_selected_weapon_hash;
					if (m_PreviousWeaponHash != CurrentWeapon) {
						if (!IsSpinUpValueCached(CurrentWeapon))
							m_OriginalSpinUpValues.push_back({ CurrentWeapon, (*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager->m_weapon_info->m_spinup_time });

						(*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager->m_weapon_info->m_spinup_time = GetOriginalSpinUpValue(CurrentWeapon);
					}
				}
			}
		}
	}

	if (m_Vars.m_ShotGunMode) {
		if (*Patterns::Vars::g_PedFactory) {
			if ((*Patterns::Vars::g_PedFactory)->m_local_ped) {
				if ((*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager) {
					(*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager->m_weapon_info->m_batch_spread = 0.5f;
					(*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager->m_weapon_info->m_bullets_in_batch = 30;
					(*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager->m_weapon_info->m_force = 200.0f;
					(*Patterns::Vars::g_PedFactory)->m_local_ped->m_weapon_manager->m_weapon_info->m_speed = 5000.0f;
				}
			}
		}
	}

	Native::GetCurrentPedWeapon(Native::PlayerPedId(), &m_Vars.m_weapon_hash, false);

	m_Vars.m_has_weapon_in_hand = (m_Vars.m_weapon_hash && m_Vars.m_weapon_hash != 2725352035);

	if (m_Vars.m_toggle) {
		if (IsPedShooting(Native::PlayerPedId())) {
			static int timer = 0;
			Menu::Timers::RunTimedFunction(&timer, m_Vars.m_wait, [] {
				Entity handle = 0;
				switch (m_Vars.m_type) {
				case 0: // object
					Menu::GetControlManager()->SimpleRequestModel(objectValues[m_Vars.m_object]);
					//*(unsigned short*)patterns::set_this_thread_networked = 0x9090;
					handle = Native::CreateObject(objectValues[m_Vars.m_object],  0.f, 0.f, 0.f , true, true, false);
					//*(unsigned short*)patterns::set_this_thread_networked = 0x0574;
					break;

				case 1: // vehicle
					Menu::GetControlManager()->SimpleRequestModel(vehicleValues[m_Vars.m_vehicle]);
					//*(unsigned short*)patterns::set_this_thread_networked = 0x9090;
					handle = Native::CreateVehicle(vehicleValues[m_Vars.m_vehicle],  0.f, 0.f, 0.f , 0.f, true, true, 0);
					//*(unsigned short*)patterns::set_this_thread_networked = 0x0574;
					break;

				case 2: // ped
					Menu::GetControlManager()->SimpleRequestModel(pedValues[m_Vars.m_ped]);
					//*(unsigned short*)patterns::set_this_thread_networked = 0x9090;
					handle = Native::CreatePed(21, pedValues[m_Vars.m_ped],  0.f, 0.f, 0.f , 0.f, true, false);
					//*(unsigned short*)patterns::set_this_thread_networked = 0x0574;
					break;
				}
				if (Native::DoesEntityExist(handle)) {
					Native::SetEntityAsMissionEntity(handle, true, true);
					Native::SetEntityNoCollisionEntity(Native::PlayerPedId(), handle, true);

					Math::Vector3<float> rotation = Native::GetGameplayCamRot(0);
					Math::Vector3<float> target = Native::GetGameplayCamCoord();// +(get_direction(rotation) * 25.f);

					Native::SetEntityCoords(handle, target.m_X + (get_direction(rotation).m_X * 25.f), target.m_Y + (get_direction(rotation).m_Y * 25.f), target.m_Z + (get_direction(rotation).m_Z * 25.f), false, false, false, false);
					Native::SetEntityRotation(handle, rotation.m_X, 0.f, rotation.m_Z, 0, 1);

					if (Native::IsEntityAVehicle(handle)) Native::SetVehicleOutOfControl(handle, true, true);
					Native::ApplyForceToEntityCenterOfMass(handle, 1,  0.f, m_Vars.m_speed, 0.f , false, true, true, false);
				}
				});
		}
	}
}