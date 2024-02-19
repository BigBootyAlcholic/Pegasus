#include "stdafx.hpp"
#include "teleport.hpp"
#include "cheat/util/notify.hpp"
#include "cheat/util/control_manager.hpp"
#include "core/patterns.hpp"
#include "rage/engine.hpp"
#include "util/fiber.hpp"
#include "util/fiber_pool.hpp"
using namespace TeleportMenuVars;

namespace TeleportMenuVars {
	Vars_ m_Vars;
	void PlayerSwitchToCoords(float x, float y, float z, bool keep_veh = true)
	{
		Utils::GetFiberPool()->Push([=] {
			const auto ModelHash = 0x62018559;
			if (!Native::HasModelLoaded(ModelHash)) {
				Native::RequestModel(ModelHash);
				Utils::GetFiberManager()->GoToMainFiber();
			}

			*(unsigned short*)Patterns::Vars::g_SetNetworkedThread = 0x9090;
			auto CreatedPed = Native::CreatePed(1, ModelHash, x, y, z, 0.f, TRUE, FALSE);
			*(unsigned short*)Patterns::Vars::g_SetNetworkedThread = 0x0574;
			Native::StartPlayerSwitch(Native::PlayerPedId(), CreatedPed, 0, 0);
			Native::SetEntityAsMissionEntity(CreatedPed, TRUE, TRUE);
			Native::DeleteEntity(&CreatedPed);
			Native::SetModelAsNoLongerNeeded(ModelHash);
			if (Native::IsPlayerSwitchInProgress())
			{
				while (Native::GetPlayerSwitchState() != 8)
				{
					Utils::GetFiberManager()->GoToMainFiber();
					if (Native::GetPlayerSwitchState() == 12)
						break;
					keep_veh ? Native::SetPedCoordsKeepVehicle(Native::PlayerPedId(), x, y, z) : Native::SetEntityCoords(Native::PlayerPedId(), x, y, z, FALSE, FALSE, FALSE, TRUE);
				}
			}
		});
	}

	bool LoadGround(Math::Vector3<float>& location)
	{
		float GroundZ;
		const std::uint8_t Attempts = 10;

		for (std::uint8_t i = 0; i < Attempts; i++)
		{
			// Only request a collision after the first try failed because the location might already be loaded on first attempt
			for (std::uint16_t z = 0; i && z < 1000; z += 100)
			{
				Native::RequestCollisionAtCoord(location.m_X, location.m_Y, (float)z);

				Utils::GetFiberManager()->GoToMainFiber();
			}

			if (Native::GetGroundZFor3DCoord(location.m_X, location.m_Y, 1000.f, &GroundZ, FALSE))
			{
				location.m_Z = GroundZ + 1.f;

				return true;
			}

			Utils::GetFiberManager()->GoToMainFiber();
		}

		location.m_Z = 1000.f;

		return false;
	}

	bool GetBlipLocation(Math::Vector3<float>& location, int sprite, int color = -1)
	{
		Blip Blip{};
		for (Blip = Native::GetFirstBlipInfoId(sprite); Native::DoesBlipExist(Blip) && color != -1 && Native::GetBlipColour(Blip) != color; Blip = Native::GetNextBlipInfoId(sprite))
			;

		if (!Native::DoesBlipExist(Blip) || (color != -1 && Native::GetBlipColour(Blip) != color))
			return false;

		location = Native::GetBlipCoords(Blip);

		return true;
	}

	enum class eBlipColors
	{
		None,
		Red,
		Green,
		Blue,
		PlayerColor,
		YellowMission,
		FriendlyVehicle = 0x26,
		MichaelColor = 0x2A,
		FranklinColor,
		TravorColor,
		BlipColorEnemy = 0x31,
		MissionVehicle = 0x36,
		RedMission = 0x3B,
		YellowMission2 = 0x3C,
		Mission = 0x42,
		WaypointColor = 0x54
	};

	enum class eBlipIcons
	{
		Null,
		Circle,
		Empty,
		Cop,
		CopSearchZone,
		Square,
		PlayerArrow,
		North,
		Waypoint,
		BigCircle,
		BigCircleOutline,
		ArrowUpOutlined,
		ArrowDownOutlined,
		ArrowUp,
		ArrowDown,
		PoliceHelicopter,
		Jet,
		Number1,
		Number2,
		Number3,
		Number4,
		Number5,
		Number6,
		Number7,
		Number8,
		Number9,
		Number10,
		GTAOCrew,
		GTAOFriend,
		CableCar = 36,
		RaceFinish = 38,
		Safehouse = 40,
		Cop2,
		CopCar,
		Helicopter,
		ChatBubble = 47,
		Garage = 50,
		Drugs,
		ConvenienceStore,
		PoliceCar = 56,
		Circle2,
		CircleWithStar,
		ConvenienceStore2,
		ShieldWithStar,
		Asterisk,
		Helicopter2 = 64,
		StrangersAndFreaks = 66,
		ArmoredTruck,
		TowTruck,
		Barber = 71,
		LosSantosCustoms,
		Clothes,
		TattooParlor = 75,
		Simeon,
		Lester,
		Michael,
		Trevor,
		Heist1,
		Rampage = 84,
		VinewoodTours,
		Franklin,
		Franklin2 = 88,
		Chinese,
		FlightSchool,
		Bar = 93,
		ParachuteJump,
		Heist2 = 96,
		CarWash = 100,
		ComedyClub = 102,
		Darts,
		FIB = 106,
		Heist3,
		DollarSign,
		Golf,
		AmmuNation,
		Exile = 112,
		ShootingRange = 119,
		Solomon,
		StripClub,
		Tennis,
		Triathlon = 126,
		OffRoadRaceFinish,
		Key = 134,
		MovieTheater,
		Music,
		Marijuana = 140,
		Hunting,
		ArmsTraffickingGround = 147,
		Nigel = 149,
		AssaultRifle,
		Bat,
		Grenade,
		Health,
		Knife,
		Molotov,
		Pistol,
		RPG,
		Shotgun,
		SMG,
		Sniper,
		SonicWave,
		PointOfInterest,
		GTAOPassive,
		GTAOUsingMenu,
		Link = 171,
		Minigun = 173,
		GrenadeLauncher,
		Armor,
		Castle,
		CameraIcon = 184,
		Handcuffs = 188,
		Yoga = 197,
		Cab,
		Number11,
		Number12,
		Number13,
		Number14,
		Number15,
		Number16,
		Shrink,
		Epsilon,
		PersonalVehicleCar = 225,
		PersonalVehicleBike,
		Custody = 237,
		ArmsTraffickingAir = 251,
		Fairground = 266,
		PropertyManagement,
		Altruist = 269,
		Enemy,
		Chop = 273,
		Dead,
		Hooker = 279,
		Friend,
		BountyHit = 303,
		GTAOMission,
		GTAOSurvival,
		CrateDrop,
		PlaneDrop,
		Sub,
		Race,
		Deathmatch,
		ArmWrestling,
		AmmuNationShootingRange = 313,
		RaceAir,
		RaceCar,
		RaceSea,
		GarbageTruck = 318,
		Motorcycle = 348,
		SafehouseForSale = 350,
		Package,
		MartinMadrazo,
		EnemyHelicopter,
		Boost,
		Devin,
		Marina,
		Garage2,
		GolfFlag,
		Hangar,
		Helipad,
		JerryCan,
		Masks,
		HeistSetup,
		Incapacitated,
		PickupSpawn,
		BoilerSuit,
		Completed,
		Rockets,
		GarageForSale,
		HelipadForSale,
		MarinaForSale,
		HangarForSale,
		Business = 374,
		BusinessForSale,
		RaceBike,
		Parachute,
		TeamDeathmatch,
		RaceFoot,
		VehicleDeathmatch,
		Barry,
		Dom,
		MaryAnn,
		Cletus,
		Josh,
		Minute,
		Omega,
		Tonya,
		Paparazzo,
		Crosshair,
		Creator = 398,
		CreatorDirection,
		Abigail,
		Blimp,
		Repair,
		Testosterone,
		Dinghy,
		Fanatic,
		GangAttack,
		Information,
		CaptureBriefcase,
		LastTeamStanding,
		Boat,
		CaptureHouse,
		JerryCan2 = 415,
		RPLogo,
		GTAOPlayerSafehouse,
		GTAOPlayerSafehouseBounty,
		CaptureAmericanFlag,
		CaptureFlag,
		Tank,
		PlayerHelicopter,
		PlayerPlane,
		PlayerJet,
		PlayerArrowNoColor,
		PlayerLAPV,
		PlayerBoat,
		HeistRoom,
		Stopwatch = 430,
		DollarSignCircled,
		Crosshair2,
		Crosshair3,
		DollarSignSquared,
		RaceFlagWithArrow,
		Fire,
		DoubleDeathmatchSkulls,
		CastleIcon,
		Crown,
		SupplyBag,
		QuadirectionalArrows,
		Fox,
		Triangle,
		ThreeBars,
		LamarDavis,
		Mechanic,
		LamarDavisOne,
		LamarDavisTwo,
		LamarDavisThree,
		LamarDavisFour,
		LamarDavisFive,
		LamarDavisSix,
		LamarDavisSeven,
		LamarDavisEight,
		Yacht,
		SmallQuestionMark,
		GymBag,
		CrosshairWithPersonInMiddle,
		PlayerSpeaking,
		Taxi,
		ShieldWithX,
		YachtMinimap,
		Fox2,
		Hourglass,
		SmallQuestionMark2,
		Stopwatch2,
		Restart,
		SunWithEye,
		Marijuana2,
		Shotgun2,
		Jetski,
		EyeWithLineThroughIt,
		Garage3,
		GarageForSale2,
		Building,
		BuildingForSale,
		Truck,
		Crate,
		Trailer,
		Person,
		Cargobob,
		BigOutlineSquare,
		Speedometer,
		Ghost,
		RemoteControl,
		Bomb,
		Shield,
		StuntRace,
		Heart,
		StuntRaceMoney,
		TeamDeathMatch2,
		HouseWithGarage,
		QuadirectionalArrowsWithMotorcycle,
		CircleWithMotorcycle,
		MotorcycleRace,
		Weed,
		Cocaine,
		ForgedDocuments,
		Meth,
		CounterfeitMoney,
		DrugPickup,
		BigOne,
		BigTwo,
		BigThree,
		BigFour,
		BigFive,
		BigSix,
		BigSeven,
		BigEight,
		BigNine,
		BigTen,
		ATVQuad,
		Bus,
		BagMaybe,
		UpArrowOnSquare,
		FourPlayers,
		EightPlayers,
		TenPlayers,
		TwelvePlayers,
		SixteenPlayers,
		Laptop,
		Supercycle,
		Supercar,
		GarageWithCar,
		PaperDocument,
		ShieldWithBlackStar,
		CraneClaw,
		PhantomWedge,
		BoxvilleArmored,
		Ruiner2000,
		RampBuggy,
		Wastelander,
		RocketVoltic,
		TechnicalAqua,
		LetterA,
		LetterB,
		LetterC,
		LetterD,
		LetterE,
		LetterF,
		LetterG,
		LetterH,
		SkullWithTriangleTop,
		Wrench,
		SteeringWheel,
		Trolphy,
		Rocket,
		Missile,
		Bullets,
		Parachute2,
		Number5WithCircle,
		Number10WithCircle,
		Number15WithCircle,
		Number20WithCircle,
		Number30WithCircle,
		Bullets2,
		Bunker,
		InsurgentPickup,
		Opressor,
		TechnicalCustom,
		DuneFAV,
		HalfTrack,
		AntiAircraftTrailer,
		Satelite,
		BunkerWithPeople,
		ScrewdriverAndHammer,
		UpgradedWeaponWorkbench,
		CrateWithStraps,
		AircraftHanger,
		Icosikaitetragon,
		RaceflagWithQuestionmark,
		AlphaZ1,
		Bombushka,
		Havok,
		HowardNX25,
		Hunter,
		Ultralight,
		Mogul,
		Molotok,
		Nokota,
		Pyro,
		Rogue,
		Starling,
		Starbreeze,
		Tula,
		GymBagWithLetterH,
		LockedCrate,
		SateliteIcon,
		Avenger,
		IAAFacility,
		DoomsdayHeist,
		SAMTurret,
		HackingPuzzleRed,
		HackingPuzzleDiode,
		Stromburg,
		Deluxo,
		ThrusterJetpack,
		TM02KhanjaliTank,
		RCV,
		Volatol,
		Barrage,
		Akula,
		Chernobog,
		SpeakerMaybe,
		YellowishBrownMoneySign,
		ComputerTerminal,
		HackingPuzzleMirror,
		HackingPuzzleReflector,
		HackingPuzzleRotator,
		RaceFlagWithCrosshair,
		RaceFlagWithSteeringWheel,
		SeaSparrow,
		Caracara,
		Nightclub,
		CrateB,
		VanB,
		Diamond,
		BullionStacks,
		CalculatorMaybe,
		CarWithLightningBolt,
		HeartIcon,
		StarWithAdd,
		StarWithSubtract,
		BombWithAdd,
		BombWithSubtract,
		CrosshairDJ,
		Drone,
		Cashregister,
		SpeakerMaybeBigger,
		Skull,
		FestivalBus,
		Terrorbyte,
		Menacer,
		Scramjet,
		PounderCustom,
		MuleCustom,
		SpeedoCustom,
		BlimpIcon,
		OpressorMKII,
		B11StrikeForce,
		GearWithCar,
		GearWithMoney,
		GearWithWrench,
		RaceFlagWithGear,
		CrosshairOnCar,
		RCBandito,
		RCRemote,
		FlameSign,
		ArrowSign,
		GearSign,
		RepeatSign,
		DownArrowSign,
		ExplosionSign,
		WheelExplosionSign,
		ElevatorSign,
		ElevatorSign2,
		EjectorPlatformSign,
		Bruiser,
		Brutus,
		Cerberus,
		Deathbike,
		Dominator,
		Impaler,
		Imperator,
		Issi,
		Sasquatch,
		Scarab,
		Slamvan,
		ZR380
	};

	template <typename t1, typename t2>
	struct conextpr_pair {
		t1 first{};
		t2 second{};
	};
	static constexpr std::array<conextpr_pair<int, int>, 6> BlipColors
	{
		{
			{ static_cast<int>(eBlipIcons::Circle), static_cast<int>(eBlipColors::YellowMission) },
			{ static_cast<int>(eBlipIcons::Circle), static_cast<int>(eBlipColors::YellowMission2) },
			{ static_cast<int>(eBlipIcons::Circle), static_cast<int>(eBlipColors::Mission) },
			{ static_cast<int>(eBlipIcons::RaceFinish), static_cast<int>(eBlipColors::None) },
			{ static_cast<int>(eBlipIcons::Circle), static_cast<int>(eBlipColors::Green) },
			{ static_cast<int>(eBlipIcons::Circle), static_cast<int>(eBlipColors::Blue) }
		}
	};

	bool GetObjectiveLocation(Math::Vector3<float>& location)
	{
		for (const auto& [icon, color] : BlipColors)
		{
			if (GetBlipLocation(location, icon, color))
				return true;
		}

		static const int Blips[] = { 1, 57, 128, 129, 130, 143, 144, 145, 146, 271, 286, 287, 288 };
		for (const auto& Blip : Blips)
		{
			if (GetBlipLocation(location, Blip, 5))
				return true;
		}

		return false;
	}

	bool ToBlip(Entity ent, int sprite, int color = -1)
	{
		Math::Vector3<float> Coords{};

		if (!GetBlipLocation(Coords, sprite, color))
			return false;

		if (sprite == (int)eBlipIcons::Waypoint)
			LoadGround(Coords);

		Native::SetEntityCoords(ent, Coords.m_X, Coords.m_Y, Coords.m_Z, FALSE, FALSE, FALSE, FALSE);
		return true;
	}

	bool ToWaypoint(Entity ent)
	{
		if (!ToBlip(ent, (int)eBlipIcons::Waypoint))
			return false;
		return true;
	}

	bool ToObjective(Entity ent)
	{
		Math::Vector3<float> Coords{};

		if (!GetObjectiveLocation(Coords))
			return false;

		Native::SetEntityCoords(ent, Coords.m_X, Coords.m_Y, Coords.m_Z, FALSE, FALSE, FALSE, FALSE);
		return true;
	}

	void LocalToCoords(float x, float y, float z)
	{
		if (m_Vars.m_Animation) {
			PlayerSwitchToCoords(x, y, z, m_Vars.m_KeepVehicle);
		}
		else {
			m_Vars.m_KeepVehicle ? Native::SetPedCoordsKeepVehicle(Native::PlayerPedId(), x, y, z) : Native::SetEntityCoords(Native::PlayerPedId(), x, y, z, FALSE, FALSE, FALSE, TRUE);
		}	
	}

	void LocalToCoords2(Math::Vector3<float> Coords)
	{
		if (m_Vars.m_Animation) {
			PlayerSwitchToCoords(Coords.m_X, Coords.m_Y, Coords.m_Z, m_Vars.m_KeepVehicle);
		}
		else {
			m_Vars.m_KeepVehicle ? Native::SetPedCoordsKeepVehicle(Native::PlayerPedId(), Coords.m_X, Coords.m_Y, Coords.m_Z) : Native::SetEntityCoords(Native::PlayerPedId(), Coords.m_X, Coords.m_Y, Coords.m_Z, FALSE, FALSE, FALSE, TRUE);
		}
	}

	bool LocalToObjective()
	{
		Math::Vector3<float> Coords{};

		if (!GetObjectiveLocation(Coords))
			return false;

		LocalToCoords(Coords.m_X, Coords.m_Y, Coords.m_Z);
		return true;
	}

	bool LocalToWaypoint()
	{
		Math::Vector3<float> Coords{};

		if (!GetBlipLocation(Coords, (int)eBlipIcons::Waypoint, -1))
			return false;

		LoadGround(Coords);

		LocalToCoords(Coords.m_X, Coords.m_Y, Coords.m_Z);
		return true;
	}


	void TeleportDirection(int Direction) {
		Math::Vector3<float> Coords = { 0.f, 0.f, 0.f };

		switch (Direction) {
		case 0:
			Coords.m_Y = m_Vars.m_Directions[0] + 1.f;
			break;
		case 1:
			Coords.m_Y = (m_Vars.m_Directions[1] * -1.f) + 1.f;
			break;
		case 2:
			Coords.m_Z = m_Vars.m_Directions[2] + 1.f;
			break;
		case 3:
			Coords.m_Z = (m_Vars.m_Directions[3] * -1.f) + 1.f;
			break;
		case 4:
			Coords.m_X = (m_Vars.m_Directions[4] * -1.f) + 1.f;
			break;
		case 5:
			Coords.m_X = m_Vars.m_Directions[5] + 1.f;
			break;
		}

		Math::Vector3<float> Velocity;
		if (Menu::GetLocalPlayer().m_InVehicle) {
			Velocity = Native::GetEntityVelocity(Menu::GetLocalPlayer().m_Vehicle);
		}
		else {
			Coords.m_Z -= 1.f;
		}

		auto C = Native::GetOffsetFromEntityInWorldCoords(Menu::GetLocalPlayer().m_Entity, Coords.m_X, Coords.m_Y, Coords.m_Z);
		Native::SetEntityCoordsNoOffset(Menu::GetLocalPlayer().m_Entity, C.m_X, C.m_Y, C.m_Z, false, false, false);

		if (Menu::GetLocalPlayer().m_InVehicle && Native::IsVehicleOnAllWheels(Menu::GetLocalPlayer().m_Vehicle)) {
			Native::SetEntityVelocity(Menu::GetLocalPlayer().m_Vehicle, Velocity.m_X, Velocity.m_Y, Velocity.m_Z);
			Native::SetVehicleEngineOn(Menu::GetLocalPlayer().m_Vehicle, true, true, true);
		}
	}
	struct teleport_location { std::string str; float x; float y; float z; };
	std::vector<teleport_location> locations_underwater = {
	   { "Dead Sea Monster", -3373.726f, 504.714f, -24.656f },
	   { "Humane Labs Tunnel", 3619.749f, 2742.740f, 28.690f },
	   { "Sunken Body", -3161.078f, 3001.998f, -37.974f },
	   { "Sunken Cargo Ship", 3199.748f, -379.018f, -22.500f },
	   { "Sunken Plane", -942.350f, 6608.752f, -20.912f },
	   { "Underwater Hatch", 4273.950f, 2975.714f, -170.746f },
	   { "Underwater UFO", 762.426f, 7380.371f, -111.377f },
	   { "Underwater Tank", 4201.633f, 3643.821f, -39.016f }
	};

	std::vector<teleport_location> locations_inside = {
	   { "Strip Club DJ Booth", 126.135f, -1278.583f, 29.270f },
	   { "Blaine County Savings Bank", -109.299f, 6464.035f, 31.627f },
	   { "Police Station", 436.491f, -982.172f, 30.699f },
	   { "Humane Labs Entrance", 3619.749f, 2742.740f, 28.690f },
	   { "Humane Labs Tunnel", 3525.495f, 3705.301f, 20.992f },
	   { "Ammunation Office", 12.494f, -1110.130f, 29.797f },
	   { "Ammunation Gun Range", 22.153f, -1072.854f, 29.797f },
	   { "Meth Lab", 1391.773f, 3608.716f, 38.942f },
	   { "Pacific Standard Bank Vault", 255.851f, 217.030f, 101.683f },
	   { "FIB Building", 135.733f, -749.216f, 258.152f },
	   { "IAA Office", 117.220f, -620.938f, 206.047f },
	   { "Torture Room", 147.170f, -2201.804f, 4.688f },
	   { "Zancudo Control Tower", -2356.094f, 3248.645f, 101.451f },
	   { "Solomon's Office", -1003.96f, -477.95f, 50.03f }
	};

	std::vector<teleport_location> locations_outside = {
	   { "LS Customs", -365.425f, -131.809f, 37.873f },
	   { "IAA Roof", 134.085f, -637.859f, 262.851f },
	   { "FIB Roof", 150.126f, -754.591f, 262.865f },
	   { "Maze Bank Roof", -75.015f, -818.215f, 326.176 },
	   { "Mt Chilad", 450.718f, 5566.614f, 806.183f },
	   { "Hippy Camp", 2476.712f, 3789.645f, 41.226f },
	   { "Marlowe Vineyards", -1868.971f, 2095.674f, 139.115f },
	   { "Abandoned Mine", -595.342f, 3608.716f, 38.942f },
	   { "Merryweather Dock", 486.417f, -3339.692f, 6.070f },
	   { "Cargo Ship", 899.678f, -2882.191f, 19.013f },
	   { "Del Perro Pier", -1850.127f, -1231.751f, 13.017f },
	   { "Play Boy Mansion", -1475.234, 167.088f, 55.841f }
	};

	std::vector<teleport_location> locations_pop = {
	   { "Airport Airstrip", -1336.000f, -3044.000f, 13.900f },
	   { "Construction Crane", -119.860f, -976.439f, 306.339f },
	   { "Altruist Radio Tower", -1002.000f, 4842.000f, 280.000f },
	   { "Galileo Observatory Roof", -438.796f, 1075.821f, 353.000f },
	   { "Black UFO", -2052.662f, 3236.953f, 1466.315f },
	   { "Maza Bank Roof", -75.015f, -818.215f, 326.176f },
	   { "Mors Mutual Impound", -222.198f, -1185.850, 23.029f }
	};

	std::vector<teleport_location> locations_airstrips = {
	   { "Airport Runway", -1336.000f, -3044.000f, 13.900f },
	   { "Grapeseed Runway", 2136.51f, 4810.95f, 41.20f },
	   { "Sandy Shores Runway", 1707.08f, 3251.56f, 41.02f },
	   { "Fort Zancudo Runway", -2022.53f, 2867.f, 32.91f },
	   { "Airport Helipad", -1159.51f, -2889.13f, 13.95f },
	   { "Sandy Shores Helipad", 1781.56f, 3249.81f, 42.12f },
	   { "Fort Zancudo Helipad", -1859.f, 2816.f, 32.81f }
	};
}


void TeleportMenu::Run() {
	Framework::addSubmenu("Teleport", "teleport-menu", [](Framework::Options::Core* core) {
		core->addOption(Framework::Options::ToggleOption("Keep Vehicle")
			.addToggle(&m_Vars.m_KeepVehicle));

		core->addOption(Framework::Options::ToggleOption("Animation")
			.addToggle(&m_Vars.m_Animation));

		core->addOption(Framework::Options::ButtonOption("Teleport To Waypoint")
			.addClick([] { LocalToWaypoint(); }));

		core->addOption(Framework::Options::ButtonOption("Teleport To Objective")
			.addClick([] { LocalToObjective(); }));

		core->addOption(Framework::Options::NumberOption<float>("Forwards")
			.addNumber(&m_Vars.m_Directions[0]).addMin(1.f).addMax(100.f).addStep(1.f).setPrecision(0.f)
			.addClick([] { TeleportDirection(0); }));

		core->addOption(Framework::Options::NumberOption<float>("Backwards")
			.addNumber(&m_Vars.m_Directions[1]).addMin(1.f).addMax(100.f).addStep(1.f).setPrecision(0.f)
			.addClick([] { TeleportDirection(1); }));

		core->addOption(Framework::Options::NumberOption<float>("Above")
			.addNumber(&m_Vars.m_Directions[2]).addMin(1.f).addMax(100.f).addStep(1.f).setPrecision(0.f)
			.addClick([] { TeleportDirection(2); }));

		core->addOption(Framework::Options::NumberOption<float>("Below")
			.addNumber(&m_Vars.m_Directions[3]).addMin(1.f).addMax(100.f).addStep(1.f).setPrecision(0.f)
			.addClick([] { TeleportDirection(3); }));

		core->addOption(Framework::Options::NumberOption<float>("Left")
			.addNumber(&m_Vars.m_Directions[4]).addMin(1.f).addMax(100.f).addStep(1.f).setPrecision(0.f)
			.addClick([] { TeleportDirection(4); }));

		core->addOption(Framework::Options::NumberOption<float>("Right")
			.addNumber(&m_Vars.m_Directions[5]).addMin(1.f).addMax(100.f).addStep(1.f).setPrecision(0.f)
			.addClick([] { TeleportDirection(5); }));;

		core->addOption(Framework::Options::SubmenuOption("Popular")
			.setTarget("tp_pop"));

		core->addOption(Framework::Options::SubmenuOption("Airstrips")
			.setTarget("tp_airstrips"));

		core->addOption(Framework::Options::SubmenuOption("Indoors")
			.setTarget("tp_inddors"));

		core->addOption(Framework::Options::SubmenuOption("Outdoors")
			.setTarget("tp_ooutdoors"));

		core->addOption(Framework::Options::SubmenuOption("Underwater")
			.setTarget("tp_underwater"));

	});

	Framework::addSubmenu("Airstrips", "tp_airstrips", [](Framework::Options::Core* core) {
		for (int i = 0; i < locations_airstrips.size(); i++) {
			Math::Vector3<float> coords;
			coords.m_X = locations_airstrips[i].x;
			coords.m_Y = locations_airstrips[i].y;
			coords.m_Z = locations_airstrips[i].z;

			core->addOption(Framework::Options::ButtonOption(locations_airstrips[i].str.c_str())
				.addClick([=] { LocalToCoords2(coords); }));
		}
		});
	Framework::addSubmenu("Popular", "tp_pop", [](Framework::Options::Core* core) {
		for (int i = 0; i < locations_pop.size(); i++) {
			Math::Vector3<float> coords;
			coords.m_X = locations_pop[i].x;
			coords.m_Y = locations_pop[i].y;
			coords.m_Z = locations_pop[i].z;

			core->addOption(Framework::Options::ButtonOption(locations_pop[i].str.c_str())
				.addClick([=] { LocalToCoords2(coords); }));
		}
		});
	Framework::addSubmenu("Outdoors", "tp_ooutdoors", [](Framework::Options::Core* core) {
		for (int i = 0; i < locations_outside.size(); i++) {
			Math::Vector3<float> coords;
			coords.m_X = locations_outside[i].x;
			coords.m_Y = locations_outside[i].y;
			coords.m_Z = locations_outside[i].z;

			core->addOption(Framework::Options::ButtonOption(locations_outside[i].str.c_str())
				.addClick([=] { LocalToCoords2(coords); }));
		}
		});
	Framework::addSubmenu("Indoors", "tp_inddors", [](Framework::Options::Core* core) {
		for (int i = 0; i < locations_inside.size(); i++) {
			Math::Vector3<float> coords;
			coords.m_X = locations_inside[i].x;
			coords.m_Y = locations_inside[i].y;
			coords.m_Z = locations_inside[i].z;

			core->addOption(Framework::Options::ButtonOption(locations_inside[i].str.c_str())
				.addClick([=] { LocalToCoords2(coords); }));
		}
		});
	Framework::addSubmenu("Underwater", "tp_underwater", [](Framework::Options::Core* core) {
		for (int i = 0; i < locations_underwater.size(); i++) {
			Math::Vector3<float> coords;
			coords.m_X = locations_underwater[i].x;
			coords.m_Y = locations_underwater[i].y;
			coords.m_Z = locations_underwater[i].z;

			core->addOption(Framework::Options::ButtonOption(locations_underwater[i].str.c_str())
				.addClick([=] { LocalToCoords2(coords); }));
		}
		});

}


void TeleportMenu::Update() {
	Run();

}