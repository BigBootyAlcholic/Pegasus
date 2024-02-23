#pragma once
#include "stdafx.hpp"
#include "rage/classes/rage/joaat.hpp"
#include "rage/classes/netsync/netSyncTree.hpp"
#include "util/caller.hpp"
#include "rage/classes/enums/enums.hpp"
#include "rage/classes/netsync/data_nodes.hpp"
#include "rage/classes/base/CObject.hpp"
#include "rage/classes/vehicle/CVehicleModelInfo.hpp"
#include "cheat/util/notify.hpp"

namespace SyncData {
	struct GetSyncNode
	{
		Hash id;
		const char* name;

		constexpr GetSyncNode()
		{
			id = 0;
			name = "INVALID NODE ID!";
		}

		template<size_t N>
		constexpr GetSyncNode(char const (&pp)[N])
		{
			id = Rage::consteval_joaat(pp);
			name = pp;
		}

		// implicit conversion
		constexpr operator Hash() const
		{
			return id;
		}
	};

	using sync_node_vft_to_ids = std::unordered_map<uint64_t, GetSyncNode>;
	using sync_tree_node_array_index_to_node_id_t = std::vector<GetSyncNode>;

	struct sync_node_finder_t
	{
		static constexpr size_t sync_tree_count = size_t(eNetObjType::NET_OBJ_TYPE_TRAIN) + 1;

		std::array<sync_node_vft_to_ids, sync_tree_count> sync_trees_sync_node_addr_to_ids;

		std::array<sync_tree_node_array_index_to_node_id_t, sync_tree_count> sync_trees_node_array_index_to_node_id = {
			{
				// AUTOMOBILE
				{
					{"CVehicleCreationDataNode"},
					{"CAutomobileCreationDataNode"},

					{"CGlobalFlagsDataNode"},
					{"CDynamicEntityGameStateDataNode"},
					{"CPhysicalGameStateDataNode"},
					{"CVehicleGameStateDataNode"},

					{"CEntityScriptGameStateDataNode"},
					{"CPhysicalScriptGameStateDataNode"},
					{"CVehicleScriptGameStateDataNode"},
					{"CEntityScriptInfoDataNode"},

					{"CPhysicalAttachDataNode"},
					{"CVehicleAppearanceDataNode"},
					{"CVehicleDamageStatusDataNode"},
					{"CVehicleComponentReservationDataNode"},
					{"CVehicleHealthDataNode"},
					{"CVehicleTaskDataNode"},

					{"CSectorDataNode"},
					{"CSectorPositionDataNode"},
					{"CEntityOrientationDataNode"},
					{"CPhysicalVelocityDataNode"},
					{"CVehicleAngVelocityDataNode"},

					{"CVehicleSteeringDataNode"},
					{"CVehicleControlDataNode"},
					{"CVehicleGadgetDataNode"},

					{"CMigrationDataNode"},
					{"CPhysicalMigrationDataNode"},
					{"CPhysicalScriptMigrationDataNode"},
					{"CVehicleProximityMigrationDataNode"},
				},
				// BIKE
				{
					{"CVehicleCreationDataNode"},

					{"CGlobalFlagsDataNode"},
					{"CDynamicEntityGameStateDataNode"},
					{"CPhysicalGameStateDataNode"},
					{"CVehicleGameStateDataNode"},
					{"CBikeGameStateDataNode"},

					{"CEntityScriptGameStateDataNode"},
					{"CPhysicalScriptGameStateDataNode"},
					{"CVehicleScriptGameStateDataNode"},
					{"CEntityScriptInfoDataNode"},

					{"CPhysicalAttachDataNode"},
					{"CVehicleAppearanceDataNode"},
					{"CVehicleDamageStatusDataNode"},
					{"CVehicleComponentReservationDataNode"},
					{"CVehicleHealthDataNode"},
					{"CVehicleTaskDataNode"},

					{"CSectorDataNode"},
					{"CSectorPositionDataNode"},
					{"CEntityOrientationDataNode"},
					{"CPhysicalVelocityDataNode"},
					{"CVehicleAngVelocityDataNode"},

					{"CVehicleSteeringDataNode"},
					{"CVehicleControlDataNode"},
					{"CVehicleGadgetDataNode"},

					{"CMigrationDataNode"},
					{"CPhysicalMigrationDataNode"},
					{"CPhysicalScriptMigrationDataNode"},
					{"CVehicleProximityMigrationDataNode"},
				},
				// BOAT
				{
					{"CVehicleCreationDataNode"},

					{"CGlobalFlagsDataNode"},
					{"CDynamicEntityGameStateDataNode"},
					{"CPhysicalGameStateDataNode"},
					{"CVehicleGameStateDataNode"},
					{"CBoatGameStateDataNode"},

					{"CEntityScriptGameStateDataNode"},
					{"CPhysicalScriptGameStateDataNode"},
					{"CVehicleScriptGameStateDataNode"},
					{"CEntityScriptInfoDataNode"},

					{"CPhysicalAttachDataNode"},
					{"CVehicleAppearanceDataNode"},
					{"CVehicleDamageStatusDataNode"},
					{"CVehicleComponentReservationDataNode"},
					{"CVehicleHealthDataNode"},
					{"CVehicleTaskDataNode"},

					{"CSectorDataNode"},
					{"CSectorPositionDataNode"},
					{"CEntityOrientationDataNode"},
					{"CPhysicalVelocityDataNode"},
					{"CVehicleAngVelocityDataNode"},

					{"CVehicleSteeringDataNode"},
					{"CVehicleControlDataNode"},
					{"CVehicleGadgetDataNode"},

					{"CMigrationDataNode"},
					{"CPhysicalMigrationDataNode"},
					{"CPhysicalScriptMigrationDataNode"},
					{"CVehicleProximityMigrationDataNode"},
				},
				// DOOR
				{
					{"CDoorCreationDataNode"},

					{"CGlobalFlagsDataNode"},
					{"CDoorScriptInfoDataNode"},
					{"CDoorScriptGameStateDataNode"},

					{"CDoorMovementDataNode"},

					{"CMigrationDataNode"},
					{"CPhysicalScriptMigrationDataNode"},
				},
				// HELI
				{
					{"CVehicleCreationDataNode"},
					{"CAutomobileCreationDataNode"},

					{"CGlobalFlagsDataNode"},
					{"CDynamicEntityGameStateDataNode"},
					{"CPhysicalGameStateDataNode"},
					{"CVehicleGameStateDataNode"},

					{"CEntityScriptGameStateDataNode"},
					{"CPhysicalScriptGameStateDataNode"},
					{"CVehicleScriptGameStateDataNode"},
					{"CEntityScriptInfoDataNode"},

					{"CPhysicalAttachDataNode"},
					{"CVehicleAppearanceDataNode"},
					{"CVehicleDamageStatusDataNode"},
					{"CVehicleComponentReservationDataNode"},
					{"CVehicleHealthDataNode"},
					{"CVehicleTaskDataNode"},
					{"CHeliHealthDataNode"},

					{"CSectorDataNode"},
					{"CSectorPositionDataNode"},
					{"CEntityOrientationDataNode"},
					{"CPhysicalVelocityDataNode"},
					{"CVehicleAngVelocityDataNode"},

					{"CVehicleSteeringDataNode"},
					{"CVehicleControlDataNode"},
					{"CVehicleGadgetDataNode"},
					{"CHeliControlDataNode"},

					{"CMigrationDataNode"},
					{"CPhysicalMigrationDataNode"},
					{"CPhysicalScriptMigrationDataNode"},
					{"CVehicleProximityMigrationDataNode"},
				},
				// OBJECT
				{
					{"CObjectCreationDataNode"},

					{"CGlobalFlagsDataNode"},
					{"CDynamicEntityGameStateDataNode"},
					{"CPhysicalGameStateDataNode"},
					{"CObjectGameStateDataNode"},

					{"CEntityScriptGameStateDataNode"},
					{"CPhysicalScriptGameStateDataNode"},
					{"CObjectScriptGameStateDataNode"},
					{"CEntityScriptInfoDataNode"},

					{"CPhysicalAttachDataNode"},
					{"CPhysicalHealthDataNode"},

					{"CSectorDataNode"},
					{"CObjectSectorPosNode"},
					{"CObjectOrientationNode"},
					{"CPhysicalVelocityDataNode"},
					{"CPhysicalAngVelocityDataNode"},

					{"CMigrationDataNode"},
					{"CPhysicalMigrationDataNode"},
					{"CPhysicalScriptMigrationDataNode"},
				},
				// PED
				{
					{"CPedCreationDataNode"},
					{"CPedScriptCreationDataNode"},

					{"CGlobalFlagsDataNode"},
					{"CDynamicEntityGameStateDataNode"},
					{"CPhysicalGameStateDataNode"},
					{"CPedGameStateDataNode"},
					{"CPedComponentReservationDataNode"},

					{"CEntityScriptGameStateDataNode"},
					{"CPhysicalScriptGameStateDataNode"},
					{"CPedScriptGameStateDataNode"},
					{"CEntityScriptInfoDataNode"},

					{"CPedAttachDataNode"},
					{"CPedHealthDataNode"},
					{"CPedMovementGroupDataNode"},
					{"CPedAIDataNode"},
					{"CPedAppearanceDataNode"},

					{"CPedOrientationDataNode"},
					{"CPedMovementDataNode"},

					{"CPedTaskTreeDataNode"},
					{"CPedTaskSpecificDataNode"},
					{"CPedTaskSpecificDataNode"},
					{"CPedTaskSpecificDataNode"},
					{"CPedTaskSpecificDataNode"},
					{"CPedTaskSpecificDataNode"},
					{"CPedTaskSpecificDataNode"},
					{"CPedTaskSpecificDataNode"},
					{"CPedTaskSpecificDataNode"},

					{"CSectorDataNode"},
					{"CPedSectorPosMapNode"},
					{"CPedSectorPosNavMeshNode"},

					{"CMigrationDataNode"},
					{"CPhysicalMigrationDataNode"},
					{"CPhysicalScriptMigrationDataNode"},
					{"CPedInventoryDataNode"},
					{"CPedTaskSequenceDataNode"},
				},
				// PICKUP
				{
					{"CPickupCreationDataNode"},

					{"CGlobalFlagsDataNode"},
					{"CDynamicEntityGameStateDataNode"},

					{"CPickupScriptGameStateNode"},
					{"CPhysicalGameStateDataNode"},
					{"CEntityScriptGameStateDataNode"},
					{"CPhysicalScriptGameStateDataNode"},
					{"CEntityScriptInfoDataNode"},
					{"CPhysicalHealthDataNode"},

					{"CPhysicalAttachDataNode"},

					{"CSectorDataNode"},
					{"CPickupSectorPosNode"},
					{"CEntityOrientationDataNode"},
					{"CPhysicalVelocityDataNode"},
					{"CPhysicalAngVelocityDataNode"},

					{"CMigrationDataNode"},
					{"CPhysicalMigrationDataNode"},
					{"CPhysicalScriptMigrationDataNode"},
				},
				// PICKUP_PLACEMENT
				{
					{"CPickupPlacementCreationDataNode"},
					{"CMigrationDataNode"},
					{"CGlobalFlagsDataNode"},
					{"CPickupPlacementStateDataNode"},
				},
				// PLANE
				{
					{"CVehicleCreationDataNode"},

					{"CGlobalFlagsDataNode"},
					{"CDynamicEntityGameStateDataNode"},
					{"CPhysicalGameStateDataNode"},
					{"CVehicleGameStateDataNode"},

					{"CEntityScriptGameStateDataNode"},
					{"CPhysicalScriptGameStateDataNode"},
					{"CVehicleScriptGameStateDataNode"},
					{"CEntityScriptInfoDataNode"},

					{"CPhysicalAttachDataNode"},
					{"CVehicleAppearanceDataNode"},
					{"CVehicleDamageStatusDataNode"},
					{"CVehicleComponentReservationDataNode"},
					{"CVehicleHealthDataNode"},
					{"CVehicleTaskDataNode"},
					{"CPlaneGameStateDataNode"},

					{"CSectorDataNode"},
					{"CSectorPositionDataNode"},
					{"CEntityOrientationDataNode"},
					{"CPhysicalVelocityDataNode"},
					{"CVehicleAngVelocityDataNode"},

					{"CVehicleSteeringDataNode"},
					{"CVehicleControlDataNode"},
					{"CVehicleGadgetDataNode"},
					{"CPlaneControlDataNode"},

					{"CMigrationDataNode"},
					{"CPhysicalMigrationDataNode"},
					{"CPhysicalScriptMigrationDataNode"},
					{"CVehicleProximityMigrationDataNode"},
				},
				// SUBMARINE
				{
					{"CVehicleCreationDataNode"},

					{"CGlobalFlagsDataNode"},
					{"CDynamicEntityGameStateDataNode"},
					{"CPhysicalGameStateDataNode"},
					{"CVehicleGameStateDataNode"},
					{"CSubmarineGameStateDataNode"},

					{"CEntityScriptGameStateDataNode"},
					{"CPhysicalScriptGameStateDataNode"},
					{"CVehicleScriptGameStateDataNode"},
					{"CEntityScriptInfoDataNode"},

					{"CPhysicalAttachDataNode"},
					{"CVehicleAppearanceDataNode"},
					{"CVehicleDamageStatusDataNode"},
					{"CVehicleComponentReservationDataNode"},
					{"CVehicleHealthDataNode"},
					{"CVehicleTaskDataNode"},

					{"CSectorDataNode"},
					{"CSectorPositionDataNode"},
					{"CEntityOrientationDataNode"},
					{"CPhysicalVelocityDataNode"},
					{"CVehicleAngVelocityDataNode"},

					{"CVehicleSteeringDataNode"},
					{"CVehicleControlDataNode"},
					{"CVehicleGadgetDataNode"},
					{"CSubmarineControlDataNode"},

					{"CMigrationDataNode"},
					{"CPhysicalMigrationDataNode"},
					{"CPhysicalScriptMigrationDataNode"},
					{"CVehicleProximityMigrationDataNode"},
				},
				// PLAYER
				{
					{"CPlayerCreationDataNode"},

					{"CGlobalFlagsDataNode"},
					{"CDynamicEntityGameStateDataNode"},
					{"CPhysicalGameStateDataNode"},
					{"CPedGameStateDataNode"},
					{"CPedComponentReservationDataNode"},

					{"CEntityScriptGameStateDataNode"},
					{"CPlayerGameStateDataNode"},

					{"CPedAttachDataNode"},
					{"CPedHealthDataNode"},
					{"CPedMovementGroupDataNode"},
					{"CPedAIDataNode"},
					{"CPlayerAppearanceDataNode"},
					{"CPlayerPedGroupDataNode"},
					{"CPlayerAmbientModelStreamingNode"},
					{"CPlayerGamerDataNode"},
					{"CPlayerExtendedGameStateNode"},

					{"CPedOrientationDataNode"},
					{"CPedMovementDataNode"},

					{"CPedTaskTreeDataNode"},
					{"CPedTaskSpecificDataNode"},
					{"CPedTaskSpecificDataNode"},
					{"CPedTaskSpecificDataNode"},
					{"CPedTaskSpecificDataNode"},
					{"CPedTaskSpecificDataNode"},
					{"CPedTaskSpecificDataNode"},
					{"CPedTaskSpecificDataNode"},
					{"CPedTaskSpecificDataNode"},

					{"CSectorDataNode"},
					{"CPlayerSectorPosNode"},
					{"CPlayerCameraDataNode"},
					{"CPlayerWantedAndLOSDataNode"},

					{"CMigrationDataNode"},
					{"CPhysicalMigrationDataNode"},
					{"CPhysicalScriptMigrationDataNode"},
				},
				// TRAILER
				{
					{"CVehicleCreationDataNode"},
					{"CAutomobileCreationDataNode"},

					{"CGlobalFlagsDataNode"},
					{"CDynamicEntityGameStateDataNode"},
					{"CPhysicalGameStateDataNode"},
					{"CVehicleGameStateDataNode"},

					{"CEntityScriptGameStateDataNode"},
					{"CPhysicalScriptGameStateDataNode"},
					{"CVehicleScriptGameStateDataNode"},
					{"CEntityScriptInfoDataNode"},

					{"CPhysicalAttachDataNode"},
					{"CVehicleAppearanceDataNode"},
					{"CVehicleDamageStatusDataNode"},
					{"CVehicleComponentReservationDataNode"},
					{"CVehicleHealthDataNode"},
					{"CVehicleTaskDataNode"},

					{"CSectorDataNode"},
					{"CSectorPositionDataNode"},
					{"CEntityOrientationDataNode"},
					{"CPhysicalVelocityDataNode"},
					{"CVehicleAngVelocityDataNode"},

					{"CVehicleSteeringDataNode"},
					{"CVehicleControlDataNode"},
					{"CVehicleGadgetDataNode"},

					{"CMigrationDataNode"},
					{"CPhysicalMigrationDataNode"},
					{"CPhysicalScriptMigrationDataNode"},
					{"CVehicleProximityMigrationDataNode"},
				},
				// TRAIN
				{
					{"CVehicleCreationDataNode"},

					{"CGlobalFlagsDataNode"},
					{"CDynamicEntityGameStateDataNode"},
					{"CPhysicalGameStateDataNode"},
					{"CVehicleGameStateDataNode"},
					{"CTrainGameStateDataNode"},

					{"CEntityScriptGameStateDataNode"},
					{"CPhysicalScriptGameStateDataNode"},
					{"CVehicleScriptGameStateDataNode"},
					{"CEntityScriptInfoDataNode"},

					{"CPhysicalAttachDataNode"},
					{"CVehicleAppearanceDataNode"},
					{"CVehicleDamageStatusDataNode"},
					{"CVehicleComponentReservationDataNode"},
					{"CVehicleHealthDataNode"},
					{"CVehicleTaskDataNode"},

					{"CSectorDataNode"},
					{"CSectorPositionDataNode"},
					{"CEntityOrientationDataNode"},
					{"CPhysicalVelocityDataNode"},
					{"CVehicleAngVelocityDataNode"},

					{"CVehicleSteeringDataNode"},
					{"CVehicleControlDataNode"},
					{"CVehicleGadgetDataNode"},

					{"CMigrationDataNode"},
					{"CPhysicalMigrationDataNode"},
					{"CPhysicalScriptMigrationDataNode"},
					{"CVehicleProximityMigrationDataNode"},
				},
			},
		};
	};

	class SyncNode
	{
	private:
		static inline sync_node_finder_t finder;
		static inline bool inited = false;

	public:
		static const GetSyncNode& Find(eNetObjType obj_type, uintptr_t addr)
		{
			return finder.sync_trees_sync_node_addr_to_ids[(int)obj_type][addr];
		}

		static sync_node_vft_to_ids& get_object_nodes(eNetObjType obj_type)
		{
			return finder.sync_trees_sync_node_addr_to_ids[(int)obj_type];
		}

		static bool is_initialized() {
			return inited;
		}

		static void Init() {
			for (int i = (int)eNetObjType::NET_OBJ_TYPE_AUTOMOBILE; i <= (int)eNetObjType::NET_OBJ_TYPE_TRAIN; i++) {
				if (i == (int)eNetObjType::NET_OBJ_TYPE_TRAILER)
					continue;


				Rage::netSyncTree* tree = Caller::Call<Rage::netSyncTree*>(Patterns::Vars::g_GetSyncTree, *Patterns::Vars::g_NetworkObjectMgr, i);//g_pointers->m_gta.m_get_sync_tree_for_type(*g_pointers->m_gta.m_network_object_mgr, i);

				if (tree->m_child_node_count != finder.sync_trees_node_array_index_to_node_id[i].size()) {

					throw std::runtime_error("Failed to cache nodes");
				}

				for (int j = 0; j < tree->m_child_node_count; j++) {
					const uintptr_t addr = (uintptr_t)tree->m_child_nodes[j];

					const GetSyncNode node_id = finder.sync_trees_node_array_index_to_node_id[i][j];

					finder.sync_trees_sync_node_addr_to_ids[i][addr] = node_id;
				}
			}

			inited = true;
		}
	};

	inline static std::optional<Rage::joaat_t> VehicleCreationModel = std::nullopt;

	class model_info
	{
	public:
		static bool DoesModelExist(const Rage::joaat_t hash)
		{
			if (const auto model = model_info::GetModel(hash); model)
				return true;
			return false;
		}

		template<typename T = CBaseModelInfo*>
		static T GetModel(const Rage::joaat_t hash)
		{
			const auto model_table = Patterns::Vars::g_ModelTable;
			for (auto i = model_table->m_lookup_table[hash % model_table->m_lookup_key]; i; i = i->m_next)
			{
				if (i->m_hash == hash)
				{
					if (const auto model = model_table->m_data[i->m_idx]; model)
					{
						return reinterpret_cast<T>(model);
					}
				}
			}
			return nullptr;
		}

		static CVehicleModelInfo* GetVehicleModel(const Rage::joaat_t hash)
		{
			if (const auto model = model_info::GetModel<CVehicleModelInfo*>(hash); model && model->m_model_type == eModelType::Vehicle)
				return model;
			return nullptr;
		}

		template<typename T, typename... Args>
		static bool IsModelOfType(const Rage::joaat_t hash, const T arg, const Args... args)
		{
			bool of_type = false;
			if (const auto model = model_info::GetModel(hash))
			{
				of_type = model->m_model_type == arg;
				(
					[&of_type, &model](eModelType type) {
						of_type |= model->m_model_type == type;
					}(args),
						...);
			}
			return of_type;
		}
	};

	inline static uint32_t cages[] = {
				2063962179, 959275690, 1396140175, 2968924808, 2931248133, 1098812088, 379820688, 1692612370,
				2946368461, 1082648418, 3561133533, 3825864590, 3357950520, 4121926986, 109264625, 3955926036, 1181661112,
				joaat("prop_container_01a"), joaat("prop_container_01b"), joaat("prop_container_01c"),
				joaat("prop_container_01d"), joaat("prop_container_01e"), joaat("prop_container_01f"),
				joaat("prop_container_01g"), joaat("prop_container_01h"), joaat("prop_container_01mb"),
				joaat("prop_container_02a"), joaat("prop_container_03a"), joaat("prop_container_03b"),
				joaat("prop_container_03mb"), joaat("prop_container_03_ld"), joaat("prop_container_04a"),
				joaat("prop_container_04mb"), joaat("prop_container_05mb"), joaat("prop_container_door_mb_l"),
				joaat("prop_container_door_mb_r"), joaat("prop_container_hole"), joaat("prop_container_ld"),
				joaat("prop_container_ld2"), joaat("prop_container_old1"), joaat("prop_container_ld"),
				joaat("prop_container_ld2"), joaat("prop_container_old1"), joaat("prop_dog_cage_02"),
				joaat("prop_dog_cage_01"), joaat("prop_dog_cage_02"), joaat("prop_conslift_cage"),
				joaat("prop_rub_cage01a"), joaat("prop_gold_cont_01b"), joaat("prop_gold_trolly"),
				joaat("prop_gold_trolly_full"), joaat("prop_gold_trolly_strap_01"), joaat("prop_cctv_cont_01"),
				joaat("prop_cctv_cont_03"), joaat("prop_cctv_cont_04"), joaat("prop_cctv_cont_05"),
				joaat("prop_cctv_cont_06"), joaat("prop_container_01a"), joaat("prop_container_01b"),
				joaat("prop_container_01c"), joaat("prop_container_01d"), joaat("prop_container_01e"),
				joaat("prop_container_01f"), joaat("prop_container_01g"), joaat("prop_container_01h"),
				joaat("prop_container_01mb"), joaat("prop_container_02a"), joaat("prop_container_03a"),
				joaat("prop_container_03b"), joaat("prop_container_03mb"), joaat("prop_container_03_ld"),
				joaat("prop_container_04a"), joaat("prop_container_04mb"), joaat("prop_container_05mb"),
				joaat("prop_container_door_mb_l"), joaat("prop_container_door_mb_r"), joaat("prop_container_hole"),
				joaat("prop_container_ld"), joaat("prop_container_ld2"), joaat("prop_container_old1"),
				joaat("prop_contnr_pile_01a"), joaat("prop_controller_01"), joaat("prop_control_rm_door_01"),
				joaat("prop_cont_chiller_01"), joaat("prop_container_hole"), joaat("prop_cont_chiller_01"),
				0xC6C3242D, joaat("prop_beach_fire"), 0xA2023E64, 2799454049, 3482883616, 564151899, 710268902,
				3284981583, 3500568834, 1060884015, 4248442390, 4256736313, 2693815128, 1925435073, 682074297,
				3763106032, 621481054, 2908189926, 3763623269, 1369811908, 1098122770
	};

	inline bool IsCageObject(Rage::joaat_t model) {
		for (auto cage : cages) {
			if (cage == model) {
				return true;
			}
		}
		return false;
	}

	inline constexpr auto crash_objects = { RAGE_JOAAT("prop_thindesertfiller_aa"), RAGE_JOAAT("prop_dummy_01"), RAGE_JOAAT("prop_dummy_car"), RAGE_JOAAT("prop_dummy_light"), RAGE_JOAAT("prop_dummy_plane"), RAGE_JOAAT("prop_distantcar_night"), RAGE_JOAAT("prop_distantcar_day"), RAGE_JOAAT("hei_bh1_08_details4_em_night"), RAGE_JOAAT("dt1_18_sq_night_slod"), RAGE_JOAAT("ss1_12_night_slod"), RAGE_JOAAT("hash_b334b5e2_qyquzxq_collision"), RAGE_JOAAT("h4_prop_bush_bgnvla_med_01"), RAGE_JOAAT("h4_prop_bush_bgnvla_lrg_01"), RAGE_JOAAT("h4_prop_bush_buddleia_low_01"), RAGE_JOAAT("h4_prop_bush_ear_aa"), RAGE_JOAAT("h4_prop_bush_ear_ab"), RAGE_JOAAT("h4_prop_bush_fern_low_01"), RAGE_JOAAT("h4_prop_bush_fern_tall_cc"), RAGE_JOAAT("h4_prop_bush_mang_ad"), RAGE_JOAAT("h4_prop_bush_mang_low_aa"), RAGE_JOAAT("h4_prop_bush_mang_low_ab"), RAGE_JOAAT("h4_prop_bush_seagrape_low_01"), RAGE_JOAAT("prop_h4_ground_cover"), RAGE_JOAAT("h4_prop_weed_groundcover_01"), RAGE_JOAAT("h4_prop_grass_med_01"), RAGE_JOAAT("h4_prop_grass_tropical_lush_01"), RAGE_JOAAT("h4_prop_grass_wiregrass_01"), RAGE_JOAAT("h4_prop_weed_01_plant"), RAGE_JOAAT("h4_prop_weed_01_row"), RAGE_JOAAT("urbanweeds02_l1"), RAGE_JOAAT("proc_forest_grass01"), RAGE_JOAAT("prop_small_bushyba"), RAGE_JOAAT("v_res_d_dildo_a"), RAGE_JOAAT("v_res_d_dildo_b"), RAGE_JOAAT("v_res_d_dildo_c"), RAGE_JOAAT("v_res_d_dildo_d"), RAGE_JOAAT("v_res_d_dildo_e"), RAGE_JOAAT("v_res_d_dildo_f"), RAGE_JOAAT("v_res_skateboard"), RAGE_JOAAT("prop_battery_01"), RAGE_JOAAT("prop_barbell_01"), RAGE_JOAAT("prop_barbell_02"), RAGE_JOAAT("prop_bandsaw_01"), RAGE_JOAAT("prop_bbq_3"), RAGE_JOAAT("v_med_curtainsnewcloth2"), RAGE_JOAAT("bh1_07_flagpoles"), RAGE_JOAAT("hash_058a7eb5_deihiws_collision"), RAGE_JOAAT("proc_dry_plants_01"), RAGE_JOAAT("proc_leafyplant_01"), RAGE_JOAAT("proc_grassplantmix_02"), RAGE_JOAAT("proc_dryplantsgrass_01"), RAGE_JOAAT("proc_dryplantsgrass_02"), RAGE_JOAAT("proc_dryplantsgrass_02"), RAGE_JOAAT("proc_grasses01"), RAGE_JOAAT("prop_dryweed_002_a"), RAGE_JOAAT("prop_fernba"), RAGE_JOAAT("prop_weed_001_aa"), RAGE_JOAAT("urbangrnfrnds_01"), RAGE_JOAAT("urbanweeds01"), RAGE_JOAAT("prop_dandy_b"), RAGE_JOAAT("v_proc2_temp"), RAGE_JOAAT("prop_fernbb"), RAGE_JOAAT("proc_drygrassfronds01"), RAGE_JOAAT("prop_log_ae"), RAGE_JOAAT("prop_grass_da"), RAGE_JOAAT("prop_fragtest_cnst_04")};
	inline bool IsCrashObject(Rage::joaat_t model)
	{
		if (!model_info::GetModel(model))
			return false;
		if (!model_info::IsModelOfType(model, eModelType::Object, eModelType::Time, eModelType::Weapon, eModelType::Destructable, eModelType::WorldObject, eModelType::Sprinkler, eModelType::Unk65, eModelType::Plant, eModelType::LOD, eModelType::Unk132, eModelType::Building))
			return true;
		for (auto iterator : crash_objects)
			if (iterator == model)
				return true;
		return false;
	}

	inline constexpr auto crash_peds = { RAGE_JOAAT("slod_human"), RAGE_JOAAT("slod_small_quadped"), RAGE_JOAAT("slod_large_quadped") };
	inline bool IsCrashPed(Rage::joaat_t model)
	{
		for (auto iterator : crash_peds)
			if (iterator == model)
				return true;
		if (!model_info::IsModelOfType(model, eModelType::Ped, eModelType::OnlineOnlyPed))
			return true;
		return false;
	}

	inline constexpr auto crash_vehicles = {  RAGE_JOAAT("avenger3"), RAGE_JOAAT("arbitergt"), RAGE_JOAAT("astron2"), RAGE_JOAAT("cyclone2"), RAGE_JOAAT("ignus2"), RAGE_JOAAT("s95")};
	inline bool IsCrashVehicle(Rage::joaat_t model)
	{
		for (auto iterator : crash_vehicles)
			if (iterator == model)
				return true;
		if (!model_info::IsModelOfType(model, eModelType::Vehicle, eModelType::Unk133))
			return true;
		return false;
	}

	inline constexpr auto valid_player_models = {
		RAGE_JOAAT("mp_m_freemode_01"),
		RAGE_JOAAT("mp_f_freemode_01"),
		RAGE_JOAAT("u_m_m_filmdirector"),
		RAGE_JOAAT("player_zero"),
		RAGE_JOAAT("player_one"),
		RAGE_JOAAT("player_two"),
		// peyote
		RAGE_JOAAT("A_C_Boar"),
		RAGE_JOAAT("A_C_Cat_01"),
		RAGE_JOAAT("A_C_Cow"),
		RAGE_JOAAT("A_C_Coyote"),
		RAGE_JOAAT("A_C_Deer"),
		RAGE_JOAAT("A_C_Husky"),
		RAGE_JOAAT("A_C_MtLion"),
		RAGE_JOAAT("A_C_Pig"),
		RAGE_JOAAT("A_C_Poodle"),
		RAGE_JOAAT("A_C_Pug"),
		RAGE_JOAAT("A_C_Rabbit_01"),
		RAGE_JOAAT("A_C_Retriever"),
		RAGE_JOAAT("A_C_Rottweiler"),
		RAGE_JOAAT("A_C_shepherd"),
		RAGE_JOAAT("A_C_Westy"),
		RAGE_JOAAT("A_C_Chickenhawk"),
		RAGE_JOAAT("A_C_Cormorant"),
		RAGE_JOAAT("A_C_Crow"),
		RAGE_JOAAT("A_C_Hen"),
		RAGE_JOAAT("A_C_Pigeon"),
		RAGE_JOAAT("A_C_Seagull"),
		RAGE_JOAAT("A_C_Dolphin"),
		RAGE_JOAAT("A_C_Fish"),
		RAGE_JOAAT("A_C_KillerWhale"),
		RAGE_JOAAT("A_C_SharkHammer"),
		RAGE_JOAAT("A_C_SharkTiger"),
		RAGE_JOAAT("A_C_Stingray"),
		RAGE_JOAAT("IG_Orleans"),
		RAGE_JOAAT("A_C_Chop"),
		RAGE_JOAAT("A_C_HumpBack"),
	};
	inline bool IsValidPlayerModel(Rage::joaat_t model)
	{
		for (auto iterator : valid_player_models)
			if (iterator == model)
				return true;
		return false;
	}

	inline void CheckPlayerModel(CNetGamePlayer* player, uint32_t model)
	{
		if (!player)
			return;

		if (Native::NetworkIsActivitySession())
			return;

		if (!IsValidPlayerModel(model))
		{
			//LOG_ERROR("%s is spoofing their model", player->get_name());
		}
	}

	// the game function does weird stuff that we don't want
	inline CObject* GetEntityAttachedTo(CObject* entity)
	{
		if (!entity)
			return nullptr;

		if (!entity->gap50)
			return nullptr;

		__int64 component = *(__int64*)((__int64)(entity->gap50) + 0x48);

		if (!component)
			return nullptr;

		int unk_count = *(int*)(component + 0x5C) & 0xF;

		if (unk_count < 2)
			return nullptr;

		return *(CObject**)(component);
	}

	inline bool IsAttachmentInfinite(Rage::netObject* object, uint16_t attached_to_net_id, int from_bone, int to_bone)
	{
		if (object == nullptr)
			return false;

		auto target = Caller::Call<Rage::netObject*>(Patterns::Vars::g_GetNetObject, *Patterns::Vars::g_NetworkObjectMgr, attached_to_net_id, false);

		while (target)
		{
			if (target == object)
				return true;

			auto next = GetEntityAttachedTo(target->GetGameObject());

			if (!next)
				return false;

			if (!next->m_net_object)
				return false;

			target = next->m_net_object;
		}

		return false;
	}

	inline bool IsInVehicle(CPed* ped, CVehicle* vehicle)
	{
		if (!ped || !vehicle)
			return false;

		if (ped == vehicle->m_driver)
			return true;

		for (int i = 0; i < 15; i++)
			if (vehicle->m_passengers[i] == ped)
				return true;

		return false;
	}

	inline bool IsLocalPlayerAnOccupant(CVehicleProximityMigrationDataNode* node)
	{
		for (int i = 0; i < 16; i++)
		{
			if (node->m_has_occupants[i] && node->m_occupants[i] == (*Patterns::Vars::g_PedFactory)->m_local_ped->m_net_object->m_object_id)
				return true;
		}

		return false;
	}

	inline bool IsInvalidOverridePos(float x, float y)
	{
		std::uint32_t x_pos = (((x + 149) + 8192) / 75);
		std::uint32_t y_pos = (((y + 149) + 8192) / 75);
		bool is_x_invalid = x_pos >= UCHAR_MAX;
		bool is_y_invalid = y_pos >= UCHAR_MAX;

		return is_x_invalid || is_y_invalid;
	}

	inline bool IsValidGameInterior(uint32_t interior_index)
	{
		if ((int)(__int64)*(int16_t*)&interior_index >= (int)(*Patterns::Vars::g_InteriorPool)->m_size) // this is the bug, should have been an unsigned compare instead
			return false;

		// some more checks that aren't broken

		auto a1 = (uint16_t*)&interior_index;

		__int16 v2;          // ax
		unsigned __int16 v3; // ax
		bool v4;             // cf
		bool v5;             // zf
		unsigned __int16 v6; // ax

		if (*a1 == 0xFFFF)
			return false;

		v2 = a1[1];
		if ((v2 & 1) != 0)
		{
			v6 = v2 >> 2;
			v4 = v6 < 0xFFu;
			v5 = v6 == 255;
		}
		else
		{
			v3 = v2 >> 2;
			v4 = v3 < 0x1Fu;
			v5 = v3 == 31;
		}
		return v4 || v5;
	}

	inline bool IsValidInteriorFixed(uint32_t interior_index)
	{
		if ((uint32_t)(__int64)*(int16_t*)&interior_index >= (uint32_t)(*Patterns::Vars::g_InteriorPool)->m_size)
			return false;

		return true;
	}

	inline eNetObjType VehicleTypeToObjectType(eVehicleType type)
	{
		switch (type)
		{
		case eVehicleType::VEHICLE_TYPE_CAR:
		case eVehicleType::VEHICLE_TYPE_SUBMARINECAR:
		case eVehicleType::VEHICLE_TYPE_AUTOGYRO:
		case eVehicleType::VEHICLE_TYPE_QUADBIKE:
		case eVehicleType::VEHICLE_TYPE_AMPHIBIOUS_QUADBIKE:
		case eVehicleType::VEHICLE_TYPE_AMPHIBIOUS_AUTOMOBILE: return eNetObjType::NET_OBJ_TYPE_AUTOMOBILE;
		case eVehicleType::VEHICLE_TYPE_HELI:
		case eVehicleType::VEHICLE_TYPE_BLIMP: return eNetObjType::NET_OBJ_TYPE_HELI;
		case eVehicleType::VEHICLE_TYPE_PLANE: return eNetObjType::NET_OBJ_TYPE_PLANE;
		case eVehicleType::VEHICLE_TYPE_BIKE:
		case eVehicleType::VEHICLE_TYPE_BICYCLE: return eNetObjType::NET_OBJ_TYPE_BIKE;
		case eVehicleType::VEHICLE_TYPE_TRAILER: return eNetObjType::NET_OBJ_TYPE_TRAILER;
		case eVehicleType::VEHICLE_TYPE_TRAIN: return eNetObjType::NET_OBJ_TYPE_TRAIN;
		case eVehicleType::VEHICLE_TYPE_SUBMARINE: return eNetObjType::NET_OBJ_TYPE_SUBMARINE;
		case eVehicleType::VEHICLE_TYPE_BOAT: return eNetObjType::NET_OBJ_TYPE_BOAT;
		case eVehicleType::VEHICLE_TYPE_DRAFT: return eNetObjType::NET_OBJ_TYPE_AUTOMOBILE; // this appears to be unused
		}

		return eNetObjType::NET_OBJ_TYPE_AUTOMOBILE;
	}

	inline bool IsCrashVehicleTask(eTaskTypeIndex type)
	{
		switch (type)
		{
		case eTaskTypeIndex::CTaskVehicleGoToPlane:
		case eTaskTypeIndex::CTaskVehicleLandPlane:
		case eTaskTypeIndex::CTaskVehiclePlayerDrivePlane:
		case eTaskTypeIndex::CTaskVehiclePlaneChase: return g_SyncingObjectType != eNetObjType::NET_OBJ_TYPE_PLANE;
		case eTaskTypeIndex::CTaskVehicleGoToHelicopter:
		case eTaskTypeIndex::CTaskVehiclePoliceBehaviourHelicopter:
		case eTaskTypeIndex::CTaskVehiclePlayerDriveHeli:
		case eTaskTypeIndex::CTaskVehicleHeliProtect: return g_SyncingObjectType != eNetObjType::NET_OBJ_TYPE_HELI;
		case eTaskTypeIndex::CTaskVehicleGoToBoat:
		case eTaskTypeIndex::CTaskVehicleCruiseBoat:
		case eTaskTypeIndex::CTaskVehicleFleeBoat:
		case eTaskTypeIndex::CTaskVehiclePoliceBehaviourBoat:
		case eTaskTypeIndex::CTaskVehiclePlayerDriveBoat:
			return g_SyncingObjectType != eNetObjType::NET_OBJ_TYPE_BOAT;
		case eTaskTypeIndex::CTaskVehicleGoToSubmarine:
		case eTaskTypeIndex::CTaskVehiclePlayerDriveSubmarine:
			return g_SyncingObjectType != eNetObjType::NET_OBJ_TYPE_SUBMARINE;
		case eTaskTypeIndex::CTaskVehicleFleeAirborne:
			return g_SyncingObjectType != eNetObjType::NET_OBJ_TYPE_HELI && g_SyncingObjectType != eNetObjType::NET_OBJ_TYPE_PLANE;
		}

		return false;
	}

	inline bool IsCrashPedTask(eTaskTypeIndex type)
	{
		if (type == eTaskTypeIndex::CTaskUnalerted && g_SyncingObjectType == eNetObjType::NET_OBJ_TYPE_PLAYER)
			return true;

		return false;
	}

	inline bool GetPlayerSectorPos(Rage::netSyncNodeBase* node, float& x, float& y, Rage::netObject* object)
	{
		if (node->IsParentNode())
		{
			for (auto child = node->m_first_child; child; child = child->m_next_sibling)
			{
				if (GetPlayerSectorPos(child, x, y, object))
					return true;
			}
		}
		else if (node->IsDataNode())
		{
			const auto& node_id = SyncNode::Find((eNetObjType)object->m_object_type, (uintptr_t)node);

			if ((((CProjectBaseSyncDataNode*)node)->flags & 1) == 0)
				return false;

			if (node_id == GetSyncNode("CPlayerSectorPosNode"))
			{
				CPlayerSectorPosNode* player_sector_pos_node = (CPlayerSectorPosNode*)(node);
				x = player_sector_pos_node->m_sector_pos.x;
				y = player_sector_pos_node->m_sector_pos.y;
				return true;
			}
		}
		return false;
	}

	inline bool CheckNode(Rage::netSyncNodeBase* node, CNetGamePlayer* sender, Rage::netObject* object) {
		if (node->IsParentNode()) {
			for (auto child = node->m_first_child; child; child = child->m_next_sibling) {
				if (CheckNode(child, sender, object))
					return true;
			}
		}
		else if (node->IsDataNode()) {
			const auto nodeAddr = (uintptr_t)node;
			const auto& nodeID = SyncNode::Find((eNetObjType)object->m_object_type, nodeAddr);

			if ((((CProjectBaseSyncDataNode*)node)->flags & 1) == 0)
				return false;

			switch (nodeID) {
			case GetSyncNode("CVehicleCreationDataNode"):
			{
				const auto creation_node = (CVehicleCreationDataNode*)(node);

				//LOG(std::format("CVehicleCreationDataNode: {}", creation_node->m_model).c_str());

				if (IsCrashVehicle(creation_node->m_model)) {
					LOG_ERROR("CVehicleCreationDataNode: Invalid Model");
					Menu::Notify::stacked(std::format("Blocked Invalid Vehicle From {}", sender->get_name()).c_str());
					return true;
				}

				VehicleCreationModel = creation_node->m_model;

				break;
			}
			case GetSyncNode("CDoorCreationDataNode"):
			{
			
				const auto creation_node = (CDoorCreationDataNode*)(node);
				//LOG(std::format("CDoorCreationDataNode: {}", creation_node->m_model).c_str());
				if (IsCrashObject(creation_node->m_model)) {
					Menu::Notify::stacked(std::format("Blocked Invalid Object From {}", sender->get_name()).c_str());
					return true;
				}
				break;
			}
			case GetSyncNode("CPickupCreationDataNode"):
			{
				const auto creation_node = (CPickupCreationDataNode*)(node);
				//LOG(std::format("CPickupCreationDataNode: {}", creation_node->m_custom_model).c_str());
				if (creation_node->m_custom_model && IsCrashObject(creation_node->m_custom_model))
				{
					Menu::Notify::stacked(std::format("Blocked Invalid Pickup From {}", sender->get_name()).c_str());
					return true;
				}

				for (int i = 0; i < creation_node->m_num_weapon_components; i++)
				{
					uint64_t buffer[20]{};
					if (!Native::GetWeaponComponentHudStats(creation_node->m_weapon_component[i], (Any*)buffer)) // trying to save a pointer here
					{
						LOG_ERROR("CPickupCreationDataNode: Invalid Weapon Model");
						return true;
					}
				}
				break;
			}
			case GetSyncNode("CPhysicalAttachDataNode"):
			{
				const auto attach_node = (CPhysicalAttachDataNode*)(node);

				if (attach_node->m_attached
					&& (object->m_object_id == attach_node->m_attached_to
						|| IsAttachmentInfinite(object,
							attach_node->m_attached_to,
							attach_node->m_attach_bone,
							attach_node->m_other_attach_bone)))
				{
					LOG_ERROR("CPhysicalAttachDataNode: Infinite Attachment");
					return true;
				}

				if (attach_node->m_attached && object && object->m_object_type == (int16_t)eNetObjType::NET_OBJ_TYPE_TRAILER)
				{
					if (auto net_obj =
						Caller::Call<Rage::netObject*>(Patterns::Vars::g_GetNetObject, *Patterns::Vars::g_NetworkObjectMgr, attach_node->m_attached_to, false))
					{
						if (auto entity = net_obj->GetGameObject())
						{
							if (entity->m_entity_type != 3)
							{
								LOG_ERROR("CPhysicalAttachDataNode: Invalid Attachment");
								return true;
							}
						}
					}
				}

				break;
			}
			case GetSyncNode("CPedCreationDataNode"):
			{
				const auto creation_node = (CPedCreationDataNode*)(node);
				//LOG(std::format("CPedCreationDataNode: {}", creation_node->m_model).c_str());
				if (IsCrashPed(creation_node->m_model))
				{
					LOG_ERROR("CPedCreationDataNode: Invalid Model");
					return true;
				}
				else if (creation_node->m_has_prop && IsCrashObject(creation_node->m_prop_model))
				{
					LOG_ERROR("CPedCreationDataNode: Invalid Prop Model");
					return true;
				}
				if (Native::GetEntityModel(Native::PlayerPedId()) == creation_node->m_model) {
					LOG_ERROR("CPedCreationDataNode: Clone");
					return true;
				}
				break;
			}
			case GetSyncNode("CPedAttachDataNode"):
			{
				const auto attach_node = (CPedAttachDataNode*)(node);
				if (attach_node->m_attached
					&& (object->m_object_id == attach_node->m_attached_to
						|| IsAttachmentInfinite(object,
							attach_node->m_attached_to,
							attach_node->m_attachment_bone,
							attach_node->m_attachment_bone)))
				{
					if (auto game_object = (CPed*)object->GetGameObject())
						if (!game_object->m_player_info)
							LOG_ERROR("CPedAttachDataNode: Infinite Attachment");

					return true;
				}

				break;
			}
			case GetSyncNode("CObjectCreationDataNode"):
			{
				const auto creation_node = (CObjectCreationDataNode*)(node);
			//	LOG(std::format("CObjectCreationDataNode: {}", creation_node->m_model).c_str());
				if (IsCrashObject(creation_node->m_model))
				{
					LOG_ERROR("CObjectCreationDataNode: Invalid Model");
					return true;
				}
				if (IsCageObject(creation_node->m_model)) {
					LOG_ERROR("CObjectCreationDataNode: Cage");
					return true;
				}
				break;
			}
			case GetSyncNode("CPlayerAppearanceDataNode"):
			{
				const auto player_appearance_node = (CPlayerAppearanceDataNode*)(node);
				if (IsCrashPed(player_appearance_node->m_model_hash))
				{
					LOG_ERROR("CPlayerAppearanceDataNode: Invalid Model");
					return true;
				}

				player_appearance_node->m_mobile_phone_gesture_active = false; // There is a crash with the anim dict index here, but it's difficult to detect. Phone gestures are unused and can be safely disabled

				CheckPlayerModel(sender, player_appearance_node->m_model_hash);
				break;
			}
			case GetSyncNode("CPlayerCreationDataNode"):
			{
				const auto player_creation_node = (CPlayerCreationDataNode*)(node);
				//LOG(std::format("CPlayerCreationDataNode: {}", player_creation_node->m_model).c_str());
				if (IsCrashPed(player_creation_node->m_model))
				{
					LOG_ERROR("CPlayerCreationDataNode: Invalid Model");
					return true;
				}
				CheckPlayerModel(sender, player_creation_node->m_model);
				break;
			}
			case GetSyncNode("CSectorDataNode"):
			{
				if ((eNetObjType)object->m_object_type == eNetObjType::NET_OBJ_TYPE_PLAYER)
				{
					float player_sector_pos_x{}, player_sector_pos_y{};
					GetPlayerSectorPos(node->m_root->m_next_sync_node, player_sector_pos_x, player_sector_pos_y, object);

					const auto sector_node = (CSectorDataNode*)(node);
					int posX = (sector_node->m_pos_x - 512.0f) * 54.0f;
					int posY = (sector_node->m_pos_y - 512.0f) * 54.0f;
					if (IsInvalidOverridePos(posX + player_sector_pos_x, posY + player_sector_pos_y))
					{
						LOG_ERROR("CSectorDataNode: Invalid Sector");
						return true;
					}
				}
				break;
			}
			case GetSyncNode("CPlayerGameStateDataNode"):
			{
				const auto game_state_node = (CPlayerGameStateDataNode*)(node);
				if (game_state_node->m_is_overriding_population_control_sphere
					&& IsInvalidOverridePos(game_state_node->m_population_control_sphere_x, game_state_node->m_population_control_sphere_y))
				{
					LOG_ERROR("CPlayerGameStateDataNode: Invalid Sector");

					return true;
				}

				break;
			}
			case GetSyncNode("CTrainGameStateDataNode"):
			{
				const auto train_node = (CTrainGameStateDataNode*)(node);
				int track_id = train_node->m_track_id;
				if (track_id != -1 && (track_id < 0 || track_id >= 12))
				{
					LOG_ERROR("CTrainGameStateDataNode: Out of Bounds");
					return true;
				}

				int train_config_index = train_node->m_train_config_index;
				int carriage_config_index = train_node->m_carriage_config_index;

				if (train_config_index != -1)
				{
					if ((uint32_t)train_config_index >= Patterns::Vars::g_TrainConfigArray->size())
					{
						LOG_ERROR("CTrainGameStateDataNode: Index Out of Bounds");
						return true;
					}

					if (carriage_config_index != -1)
					{
						if ((uint32_t)carriage_config_index
							>= (*Patterns::Vars::g_TrainConfigArray)[train_config_index].m_carraige_configs.size())
						{
							LOG_ERROR("CTrainGameStateDataNode: Carriage Out of Bounds");
							return true;
						}
					}
				}

				break;
			}
			case GetSyncNode("CPedGameStateDataNode"):
			{
				const auto game_state_node = (CPedGameStateDataNode*)(node);
				if (game_state_node->m_on_mount)
				{
					LOG_ERROR("CPedGameStateDataNode: Invalid Flag");
					return true;
				}
				for (int i = 0; i < game_state_node->m_num_equiped_gadgets; i++)
				{
					if (game_state_node->m_gadget_hash[i] != RAGE_JOAAT("gadget_parachute") && game_state_node->m_gadget_hash[i] != RAGE_JOAAT("gadget_nightvision"))
					{
						LOG_ERROR("CPedGameStateDataNode: Invalid Gadget");
						return true;
					}
				}
				break;
			}
			case GetSyncNode("CVehicleControlDataNode"):
			{
				const auto control_node = (CVehicleControlDataNode*)(node);
				if (control_node->m_is_submarine_car)
				{
					if (auto vehicle = (CVehicle*)object->GetGameObject())
					{
						if (auto model_info = static_cast<CVehicleModelInfo*>(vehicle->m_model_info))
						{
							if (model_info->m_vehicle_type != eVehicleType::VEHICLE_TYPE_SUBMARINECAR)
							{
								LOG_ERROR("CVehicleControlDataNode: Invalid Model");
								return true;
							}
						}
					}
					else if (VehicleCreationModel != std::nullopt)
					{
						// object hasn't been created yet, but we have the model hash from the creation node
						if (auto model_info = model_info::GetVehicleModel(VehicleCreationModel.value()))
						{
							if (model_info->m_vehicle_type != eVehicleType::VEHICLE_TYPE_SUBMARINECAR)
							{
								LOG_ERROR("CVehicleControlDataNode: Invalid Sector");
								return true;
							}
						}
					}
					else // should (probably) never reach here
					{
						control_node->m_is_submarine_car = false; // safe
					}
				}

				break;
			}
			case GetSyncNode("CPlayerCameraDataNode"):
			{
				const auto camera_node = (CPlayerCameraDataNode*)(node);
				if (IsInvalidOverridePos(camera_node->m_free_cam_pos_x, camera_node->m_free_cam_pos_y))
				{
					LOG_ERROR("CPlayerCameraDataNode: Invalid Sector");
					return true;
				}

				break;
			}
			case GetSyncNode("CVehicleGadgetDataNode"):
			{
				const auto gadget_node = (CVehicleGadgetDataNode*)(node);
				for (int i = 0; i < gadget_node->m_gadget_count; i++)
				{
					if (gadget_node->m_gadget_data[i].m_gadget_type > 7)
					{
						LOG_ERROR("CVehicleGadgetDataNode: Out of Bounds");
						return true;
					}
				}
				break;
			}
			case GetSyncNode("CPedTaskTreeDataNode"):
			{
				const auto task_node = (CPedTaskTreeDataNode*)(node);

				for (int i = 0; i < 8; i++)
				{
					if (task_node->m_task_bitset & (1 << i))
					{
						if (IsCrashPedTask((eTaskTypeIndex)task_node->m_tasks[i].m_task_type))
						{
							LOG_ERROR("CPedTaskTreeDataNode: Invalid Task");
							return true;
						}
					}
				}

				break;
			}
			case GetSyncNode("CVehicleTaskDataNode"):
			{
				const auto task_node = (CVehicleTaskDataNode*)(node);
				if (IsCrashVehicleTask((eTaskTypeIndex)task_node->m_task_type))
				{
					LOG_ERROR("CVehicleTaskDataNode: Invalid Task");
					return true;
				}

				break;
			}
			case GetSyncNode("CEntityScriptInfoDataNode"):
			{
				const auto script_node = (CEntityScriptInfoDataNode*)(node);
				if (script_node->m_has_script_info)
				{
					if (script_node->m_script_info.m_local_handle == 0)
					{
						LOG_ERROR("CEntityScriptInfoDataNode: Invalid Script Info");
						return true;
					}
				}
				break;
			}
			case GetSyncNode("CDynamicEntityGameStateDataNode"):
			{
				const auto game_state_node = (CDynamicEntityGameStateDataNode*)(node);

				if (IsValidGameInterior(game_state_node->m_interior_index)
					&& !IsValidInteriorFixed(game_state_node->m_interior_index)) // will crash
				{
					LOG_ERROR("CDynamicEntityGameStateDataNode: Invalid Interior");
					return true;
				}
			}
			}
		}
		return false;
	}
}
