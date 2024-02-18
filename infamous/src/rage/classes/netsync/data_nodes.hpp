#pragma once
#include "stdafx.hpp"
#include "CProjectBaseSyncDataNode.hpp"
#include "../rage/vector.hpp"
#include "../script/CGameScriptObjInfo.hpp"
#include "../network/ClanData.hpp"

#pragma pack(push, 4)
class CDoorCreationDataNode : CProjectBaseSyncDataNode
{
public:
    uint32_t m_model; //0x00C0
    char pad_00C4[12]; //0x00C4
    Rage::fvector3 m_pos; //0x00D0
    char pad_00DC[12]; //0x00DC
    bool m_is_script_door; //0x00E8
    bool m_player_wants_control; //0x00E9
}; //Size: 0x00EC
static_assert(sizeof(CDoorCreationDataNode) == 0xEC);
#pragma pack(pop)

#pragma pack(push, 4)
class CDoorMovementDataNode : CSyncDataNodeFrequent
{
public:
    bool m_is_manual_door; // 0xC0
    float m_open_ratio;    // 0xC4
    bool m_opening;        // 0xC8
    bool m_fully_open;     // 0xC9
    bool m_closed;         // 0xCA
};
static_assert(sizeof(CDoorMovementDataNode) == 0xCC);
#pragma pack(pop)

#pragma pack(push, 4)
struct CDoorScriptGameStateDataNode : CSyncDataNodeInfrequent
{
public:
    uint32_t m_door_system_state; // 0xC0
    float m_automatic_distance;   // 0xC4
    float m_slide_rate;           // 0xC8
    bool m_has_broken_flags;      // 0xCC
    uint32_t m_broken_flags;      // 0xD0
    bool m_has_damaged_flags;     // 0xD4
    uint32_t m_damaged_flags;     // 0xD8
    bool m_hold_open;             // 0xDC
};
static_assert(sizeof(CDoorScriptGameStateDataNode) == 0xE0);
#pragma pack(pop)

#pragma pack(push, 4)
struct CDoorScriptInfoDataNode : CSyncDataNodeInfrequent
{
public:
    bool m_has_script_info;
    int m_pad;
    CGameScriptObjInfo m_script_info;
    uint32_t m_door_system_hash;
    bool m_existing_door_system_entry;
};
static_assert(sizeof(CDoorScriptInfoDataNode) == 0x120);
#pragma pack(pop)

#pragma pack(push, 1)
struct CDecorator
{
    uint32_t m_type;
    uint32_t m_name_hash;
    uint32_t m_value;
};
#pragma pack(pop)

#pragma pack(push, 4)
class CDynamicEntityGameStateDataNode : CSyncDataNodeInfrequent
{
public:
    uint32_t m_interior_index; // 0x00C0
    bool unk_00C4; // 0x00C4
    bool unk_00C5; // 0x00C5
    uint32_t m_decor_count; // 0x00C8
    CDecorator m_decors[10]; // 0x00CC
    char pad[8]; // TODO!
}; //Size: 0x15C
static_assert(sizeof(CDynamicEntityGameStateDataNode) == 0x14C);
#pragma pack(pop)

#pragma pack(push, 4)
class CEntityOrientationDataNode : CSyncDataNodeFrequent
{
public:
    Rage::fmatrix44 m_eulers;
}; //Size: 0x00EC
static_assert(sizeof(CEntityOrientationDataNode) == 0x100);
#pragma pack(pop)

#pragma pack(push, 4)
struct CEntityScriptGameStateDataNode : CSyncDataNodeInfrequent
{
    bool m_fixed; //0x00C0
    bool m_uses_collision; //0x00C1
    bool m_completely_disabled_collision; //0x00C2
}; //Size: 0x00C3
static_assert(sizeof(CEntityScriptGameStateDataNode) == 0xC4);
#pragma pack(pop)

#pragma pack(push, 4)
struct CEntityScriptInfoDataNode : CSyncDataNodeInfrequent
{
public:
    bool m_has_script_info;
    int m_pad;
    CGameScriptObjInfo m_script_info;
};
static_assert(sizeof(CEntityScriptInfoDataNode) == 0x118);
#pragma pack(pop)

#pragma pack(push,4)
class CVehicleCreationDataNode : CProjectBaseSyncDataNode
{
public:
    uint32_t m_pop_type; //0x00C0
    uint32_t m_random_seed; //0x00C4
    uint32_t m_model; //0x00C8
    uint32_t m_vehicle_status; //0x00CC
    uint32_t m_max_health; //0x00D0
    uint32_t m_creation_token; //0x00D4
    bool m_car_budget; //0x00D8
    bool m_needs_to_be_hotwired; //0x00D9
    bool m_tires_dont_burst; //0x00DA
    char pad_00DB[165]; //0x00DB
}; //Size: 0x0180
static_assert(sizeof(CVehicleCreationDataNode) == 0x180);
#pragma pack(pop)

#pragma pack(push, 4)
class CVehicleDamageStatusDataNode : CSyncDataNodeInfrequent
{
public:
    uint8_t m_bullet_counts[6];         // 0xC0
    uint8_t m_front_damage_level;       // 0xC6
    uint8_t m_rear_damage_level;        // 0xC7
    uint8_t m_left_damage_level;        // 0xC8
    uint8_t m_right_damage_level;       // 0xC9
    uint8_t m_rear_left_damage_level;   // 0xCA
    uint8_t m_rear_right_damage_level;  // 0xCB
    uint8_t m_windows_state[8];         // 0xCC
    float m_unk2[8];                    // 0xD4
    bool m_sirens_broken[20];           // 0xF4
    bool m_lights_broken[20];           // 0x108
    uint8_t m_front_bumper_state;       // 0x11E
    uint8_t m_rear_bumper_state;        // 0x11F
    uint8_t m_unk3[8];                  // 0x120
    bool m_has_damage_levels;           // 0x128
    bool m_has_broken_lights;           // 0x129
    bool m_has_broken_sirens;           // 0x12A
    bool m_has_broken_windows;          // 0x12B
    bool m_has_broken_bumpers;          // 0x12C
    bool m_has_bullets;                 // 0x12D
    bool m_has_unk;                     // 0x12E
}; //Size: 0x00C8
#pragma pack(pop)
static_assert(sizeof(CVehicleDamageStatusDataNode) == 0x130);

enum eVehicleGadgetType : uint32_t
{
    FORKS,
    SEARCH_LIGHT,
    PICK_UP_ROPE_WITH_HOOK,
    PICK_UP_ROPE_WITH_MAGNET,
    DIGGER_ARM,
    HANDLER_FRAME,
    BOMB_BAY,
};

#pragma pack(push,4)
class CVehicleGadgetData
{
public:
    eVehicleGadgetType m_gadget_type; //0x0000
    uint8_t m_data[94]; //0x0004
}; //Size: 0x64
static_assert(sizeof(CVehicleGadgetData) == 0x64);

class CVehicleGadgetDataNode : CSyncDataNodeFrequent
{
public:
    bool m_has_parent_offset; //0x00C0
    char pad_00C1[15]; //0x00C1
    uint32_t m_parent_offset_x; //0x00D0
    uint32_t m_parent_offset_y; //0x00D4
    uint32_t m_parent_offset_z; //0x00D8
    uint32_t m_parent_offset_w; //0x00DC
    uint32_t m_gadget_count; //0x00E0
    CVehicleGadgetData m_gadget_data[2]; //0x00E4
}; //Size: 0x01AC
static_assert(sizeof(CVehicleGadgetDataNode) == 0x1AC);
#pragma pack(pop)

#pragma pack(push, 2)
class CVehicleGameStateDataNode : CSyncDataNodeInfrequent
{
public:
    float m_unk66;
    float m_unk32;
    float m_unk200;
    float m_unk204;
    int m_radio_station;
    uint32_t m_lock_status;
    uint32_t m_unk17;
    uint32_t m_unbreakable_doors;
    uint32_t m_open_windows;
    uint32_t m_unk34;
    uint8_t m_unk49[4];
    unsigned int m_unk62_1;
    uint32_t m_unk240;
    uint16_t m_unk35;
    uint16_t m_unk37[2];
    uint16_t m_unk39;
    uint8_t m_unk252;
    char m_light_state;
    uint8_t m_unk21;
    char m_unk27[8];
    uint8_t m_doors_open;
    char m_door_positions[8];
    uint32_t m_locked_players;
    uint8_t m_is_engine_on;
    bool m_is_engine_starting;
    bool unk4;
    bool m_handbrake;
    bool m_unk6;
    uint8_t m_siren_on;
    bool m_unk1;
    uint8_t m_unk13;
    uint8_t m_unk12;
    uint8_t m_unk14;
    uint8_t m_unk25;
    uint8_t m_unk26;
    bool m_no_longer_needed;
    uint8_t m_unk28;
    uint8_t m_unk33;
    uint8_t m_unk30;
    bool m_lights_on;
    bool m_highbeams_on;
    char m_unk43;
    char m_unk44;
    bool m_unk7;
    char m_unk29;
    char m_unk45;
    char m_unk46;
    char m_unk47;
    char m_unk48;
    uint8_t m_unk38;
    char m_unk51;
    bool m_has_been_owned_by_player;
    char m_unk53;
    char m_unk54;
    char m_unk55;
    char m_unk56;
    char m_unk57;
    char m_unk58;
    char m_unk61;
    uint8_t m_unk62;
    char m_unk63;
    char m_unk64;
    char m_unk67;
    char m_unk68;
    char m_unk138;
    char m_unk139;
    char m_unk59;
    char m_unk60;
    char pad[6];
};
#pragma pack(pop)
static_assert(sizeof(CVehicleGameStateDataNode) == 0x148);

#pragma pack(push, 4)
class CVehicleHealthDataNode : CSyncDataNodeInfrequent
{
public:
    float m_tires_unk14[10]; //0x00C0
    float m_tires_open_wheel_heat[10]; //0x00E8
    bool m_tires_bursted[10]; //0x0110
    bool m_tires_bursted_on_rim[10]; //0x011A
    bool m_tires_unk11[10]; //0x0124
    bool m_tires_unk12[10]; //0x012E
    uint64_t m_unk24; //0x0138
    int32_t m_engine_health; //0x0140
    uint32_t m_petrol_tank_health; //0x0144
    uint32_t m_num_tires; //0x0148
    bool m_tires_fine; //0x014C
    bool m_unk7; //0x014D
    char pad_014E[1]; //0x014E
    bool m_health_changed; //0x014F
    uint32_t m_health; //0x0150
    uint32_t m_body_health; //0x0154
    uint32_t m_damage_weapon; //0x0158
    int16_t m_damager_net_id; //0x015C
    uint8_t m_total_repairs; //0x015E
    uint8_t m_unk21; //0x015F
    bool m_unk1; //0x0160
    bool m_unk2; //0x0161
    bool m_body_health_changed; //0x0162
    uint32_t m_pad2; // 0x0164
}; //Size: 0x0380
#pragma pack(pop)
static_assert(sizeof(CVehicleHealthDataNode) == 0x168);

#pragma pack(push,4)
class CVehicleProximityMigrationDataNode : CProjectBaseSyncDataNode
{
public:
    uint32_t m_max_occupants;
    bool m_has_occupants[16];
    int16_t m_occupants[16];
    char pad[16];
    bool m_override_position;
    char pad2[8];
    Rage::fvector3 m_position;
    Rage::vector3<int32_t> m_velocity;
    char pad3[352];
}; //Size: 0x0180
static_assert(sizeof(CVehicleProximityMigrationDataNode) == 0x288);
#pragma pack(pop)

#pragma pack(push, 4)
class CVehicleSteeringNodeData : CSyncDataNodeFrequent
{
public:
    float m_steering_handle; // 0xC0
    char pad[4];             // 0xC4
}; //Size: 0x00C8
#pragma pack(pop)
static_assert(sizeof(CVehicleSteeringNodeData) == 0xC8);


#pragma pack(push,4)
class CVehicleTaskDataNode : CSyncDataNodeInfrequent
{
public:
    uint32_t m_task_type;       // 0xC0
    uint32_t m_task_data_size;  // 0xC4
    char m_task_data[255];      // 0xC8
}; //Size: 0x0180
static_assert(sizeof(CVehicleTaskDataNode) == 0x1C8);
#pragma pack(pop)

#pragma pack(push, 4)
class CVehicleControlDataNode : CSyncDataNodeFrequent
{
public:
    uint32_t m_num_wheels;
    uint32_t dwordC4;
    uint32_t m_brake_control;
    uint32_t dwordCC;
    uint32_t m_road_node_address;
    bool m_kers_active;
    bool m_bringing_vehicle_to_halt;
    float m_halt_distance;
    bool m_control_vertical_velocity;
    bool m_has_suspension_data;
    bool byteDE;
    float m_suspension_heights[10];
    bool byte108;
    bool byte109;
    bool byte10A;
    bool byte10B;
    bool byte10C;
    bool byte10D;
    bool byte10E;
    float float110;
    uint32_t dword114;
    char byte118;
    bool m_is_submarine_car;
    char gap11A[2];
    float m_rudder_rotation_x;
    float m_rudder_rotation_y;
    float m_rudder_rotation_z;
    char byte128;
    char byte129;
    char pad[5];
};
static_assert(sizeof(CVehicleControlDataNode) == 0x130);
#pragma pack(pop)

#pragma pack(push, 8)
class CHeliControlDataNode : CVehicleControlDataNode
{
public:
    char m_pad[0x10];                   // 0x130
    float m_yaw_control;                // 0x140
    float m_pitch_control;              // 0x144
    float m_roll_control;               // 0x148
    float m_throttle_control;           // 0x14C
    bool m_engine_off;                  // 0x150
    int m_landing_gear_state;           // 0x154
    bool m_has_landing_gear;            // 0x158
    bool m_has_vehicle_task;            // 0x159
    bool m_is_thruster_model;           // 0x15A
    float m_thruster_side_rcs_throttle; // 0x15C
    float m_thruster_throttle;          // 0x160
    bool m_unk8;                        // 0x164
};
static_assert(sizeof(CHeliControlDataNode) == 0x168);
#pragma pack(pop)


#pragma pack(push, 4)
class CPhysicalAngVelocityDataNode : CSyncDataNodeFrequent
{
public:
    int32_t m_ang_velocity_x; //0x00C0 Divide by 16.
    int32_t m_ang_velocity_y; //0x00C4 Divide by 16.
    int32_t m_ang_velocity_z; //0x00C8 Divide by 16.
}; // 0x00CC
static_assert(sizeof(CPhysicalAngVelocityDataNode) == 0xCC);
#pragma pack(pop)

#pragma pack(push,4)
class CPhysicalAttachDataNode : CSyncDataNodeInfrequent
{
public:
    bool m_attached; //0x00C0
    bool unk_00C1; //0x00C1
    uint16_t m_attached_to; //0x00C2
    char pad_00C4[12]; //0x00C4
    Rage::fvector3 m_offset; //0x00D0
    char pad_00DC[4]; //0x00DC
    Rage::fvector4 m_orientation; //0x00E0
    Rage::fvector3 m_parent_offset; //0x00F0
    char pad_00FC[4]; //0x00FC
    uint16_t m_other_attach_bone; //0x0100
    uint16_t m_attach_bone; //0x0102
    uint32_t m_attach_flags; //0x0104
    bool m_allow_initial_separation; //0x0108
    char pad_00109[3]; //0x0109
    float unk_010C; //0x010C
    float unk_0110; //0x0110
    bool unk_0114; //0x0114
    bool unk_0115; //0x0115
    bool m_is_cargo_vehicle; //0x0116
}; //Size: 0x0118
static_assert(sizeof(CPhysicalAttachDataNode) == 0x118);
#pragma pack(pop)

#pragma pack(push, 4)
class CPhysicalGameStateDataNode : CSyncDataNodeInfrequent
{
public:
    bool m_is_visible; // 0xC0
    bool m_flag2;      // 0xC1
    bool m_flag3;      // 0xC2
    bool m_flag4;      // 0xC3
    char m_pad;        // 0xC4
    uint32_t m_val1;   // 0xC8
    int16_t m_unk204;  // 0xCC
    bool m_unk5;       // 0xCE
}; // 0x00CC
static_assert(sizeof(CPhysicalGameStateDataNode) == 0xD0);
#pragma pack(pop)

#pragma pack(push, 8)
struct CPhysicalHealthDataNode : CSyncDataNodeInfrequent
{
    bool unk_00C0; //0x00C0
    bool m_has_max_health_changed; //0x00C1
    uint32_t m_max_health; //0x00C4
    uint32_t m_current_health; //0x00C8
    uint16_t m_weapon_damage_entity; //0x00CC
    uint32_t m_weapon_damage_hash; //0x00D0
    uint64_t unk_00D8; //0x00D8
};
static_assert(sizeof(CPhysicalHealthDataNode) == 0xE0);
#pragma pack(pop)

#pragma pack(push, 4)
class CPhysicalMigrationDataNode : CProjectBaseSyncDataNode
{
public:
    bool m_unk;
};
static_assert(sizeof(CPhysicalMigrationDataNode) == 0xC4);
#pragma pack(pop)

#pragma pack(push, 4)
class CPhysicalScriptGameStateDataNode : CSyncDataNodeInfrequent
{
public:
    bool m_godmode;
    bool m_dont_load_collision;
    bool m_freeze_on_collision_load;
    bool m_only_damaged_by_player;
    bool m_bullet_proof;
    bool m_fire_proof;
    bool m_explosion_proof;
    bool m_collision_proof;
    bool m_melee_proof;
    bool m_cannot_be_damaged_by_relationship_group;
    bool m_can_only_be_damaged_by_relationship_group;
    bool m_smoke_proof;
    bool m_steam_proof;
    bool m_can_only_be_damaged_by_participants;
    bool m_dont_reset_proofs_on_cleanup_mission;
    bool m_no_reassign;
    bool m_pass_control_in_tutorial;
    bool m_visible_in_cutscene;
    bool m_visible_in_cutscene_remain_hack;
    bool m_pickup_by_cargobob_disabled;
    uint32_t m_relationship_group;
    uint32_t m_always_cloned_for_players;
    bool m_modified_max_speed;
    bool m_trigger_damage_event_for_zero_damage;
    float m_max_speed;
};
static_assert(sizeof(CPhysicalScriptGameStateDataNode) == 0xE4); // don't know the actual size of this one
#pragma pack(pop)

#pragma pack(push, 4)
class CPhysicalScriptMigrationDataNode : CProjectBaseSyncDataNode
{
public:
    bool m_has_data;           // 0xC0
    int m_script_participants; // 0xC4
    uint16_t m_host_token;     // 0xC8
};
static_assert(sizeof(CPhysicalScriptMigrationDataNode) == 0xCC);
#pragma pack(pop)

#pragma pack(push, 4)
class CPhysicalVelocityDataNode : CSyncDataNodeFrequent
{
public:
    int32_t m_velocity_x; //0x00C0 Divide by 16.
    int32_t m_velocity_y; //0x00C4 Divide by 16.
    int32_t m_velocity_z; //0x00C8 Divide by 16.
}; // 0x00CC
static_assert(sizeof(CPhysicalVelocityDataNode) == 0xCC);
#pragma pack(pop)

#pragma pack(push, 8)
class CHeliHealthDataNode : CPhysicalHealthDataNode // intentionally private since the physical health node fields aren't serialized
{
public:
    char m_pad[0x10]; // 0xF0
    uint32_t m_main_rotor_health;
    uint32_t m_rear_rotor_health;
    bool m_can_tail_boom_break_off;
    bool m_is_tail_boom_broken;
    bool m_unk;
    bool m_disable_explode_from_body_damage;
    uint32_t m_body_health;
    uint32_t m_petrol_tank_health;
    uint32_t m_engine_health;
    float m_unk_deformation_level;
    float m_unk2_deformation_level;
    float m_unk3_deformation_level;
};
static_assert(sizeof(CHeliHealthDataNode) == 0x118);
#pragma pack(pop)

#pragma pack(push, 4)
class CPedComponents
{
public:
    uint32_t m_component_bitset; //0x0
    char pad_0x4[4]; //0x4
    uint32_t unk_0x8[12]; //0x8
    uint32_t m_drawables[12]; //0x38
    uint32_t m_textures[12]; //0x68
    uint32_t m_palettes[12]; //0x98

    inline uint32_t get_drawable(int index)
    {
        if (m_component_bitset & (1 << index))
        {
            return m_drawables[index];
        }

        return 0;
    }

    inline uint32_t get_texture(int index)
    {
        if (m_component_bitset & (1 << index))
        {
            return m_textures[index];
        }

        return 0;
    }

    inline uint32_t get_palette(int index)
    {
        if (m_component_bitset & (1 << index))
        {
            return m_palettes[index];
        }

        return 0;
    }
};
static_assert(sizeof(CPedComponents) == 0xC8);
#pragma pack(pop)

#pragma pack(push,4)
class CTrainGameStateDataNode : CSyncDataNodeInfrequent
{
public:
    bool m_is_engine; //0x00C0
    bool m_is_caboose; //0x00C1
    bool m_is_mission_train; //0x00C2
    bool m_direction; //0x00C3
    bool m_has_passenger_carriages; //0x00C4
    bool m_render_derailed; //0x00C5
    bool unk_00C6; //0x00C6
    bool unk_00C7; //0x00C7
    uint16_t m_engine_id; //0x00C8
    int8_t m_train_config_index; //0x00CA
    int8_t m_carriage_config_index; //0x00CB
    int8_t m_track_id; //0x00CC
    char pad_00CD[3]; //0x00CD
    float m_distance_from_engine; //0x00D0
    float m_cruise_speed; //0x00D4
    uint16_t m_linked_to_backward_id; //0x00D8
    uint16_t m_linked_to_forward_id; //0x00DA
    uint32_t m_train_state; //0x0DC
    bool unk_00E0; //0x00E0
    bool m_force_doors_open; //0x0E1
}; //Size: 0x00E4
static_assert(sizeof(CTrainGameStateDataNode) == 0xE4);
#pragma pack(pop)

#pragma pack(push, 1)
class CClonedGeneralSweepInfo
{
public:
    char pad_0000[48]; //0x0000
    float m_track_point[4]; //0x0030
    uint16_t m_tracked_entity; //0x0040
    char pad_0042[14]; //0x0042
    float m_delta_per_second; //0x0050
    char pad_0054[4]; //0x0054
    uint32_t m_low_clip; //0x0058
    uint32_t m_med_clip; //0x005C
    uint32_t m_high_clip; //0x0060
    uint32_t m_clip_set_id; //0x0064
    uint32_t m_sweep_flags; //0x0068
    float m_turn_rate; //0x006C
};
static_assert(sizeof(CClonedGeneralSweepInfo) == 0x70);
#pragma pack(pop)

#pragma pack(push, 1)
class ClonedTakeOffPedVariationInfo
{
public:
    char pad_0000[48]; //0x0000
    Rage::fvector3 m_attach_offset; //0x0030
    char pad_003C[4]; //0x003C
    Rage::fvector3 m_velocity; //0x0040
    char pad_004C[4]; //0x004C
    Rage::fvector3 m_attach_offset_rotation; //0x0050
    char pad_005C[4]; //0x005C
    uint32_t m_clip_set_id; //0x0060
    uint32_t m_clip_id_ped; //0x0064
    uint32_t m_clip_id_prop; //0x0068
    int32_t m_variation_component; //0x006C
    uint32_t m_prop_hash; //0x0070
    int32_t m_attach_bone_tag; //0x0074
    float m_blend_in_delta_ped; //0x0078
    float m_blend_in_delta_prop; //0x007C
    float m_phase_to_blend_out; //0x0080
    float m_blend_out_delta; //0x0084
    float m_force_to_apply; //0x0088
    uint8_t m_variation_drawable_id; //0x008C
    uint8_t m_variation_drawable_alt_id; //0x008D
    char pad_008E[1]; //0x008E Drawable texture maybe?
    uint8_t m_running_flags; //0x008F
}; //Size: 0x0090
static_assert(sizeof(ClonedTakeOffPedVariationInfo) == 0x90);
#pragma pack(pop)

#pragma pack(push, 4)
class CGlobalFlagsDataNode : CSyncDataNodeInfrequent
{
public:
    uint32_t m_global_flags; //0x00C0
    uint32_t m_ownership_token; //0x00C4
}; //Size: 0x00C8
static_assert(sizeof(CGlobalFlagsDataNode) == 0xC8);
#pragma pack(pop)

#pragma pack(push, 4)
class CMigrationDataNode : CProjectBaseSyncDataNode
{
public:
    uint32_t m_cloned_state; //0x00C0
    uint32_t m_cloned_players_that_left; //0x00C4
    uint32_t m_unsynced_nodes; //0x00C8
}; //Size: 0x00CC
static_assert(sizeof(CMigrationDataNode) == 0xCC);
#pragma pack(pop)

#pragma pack(push, 2)
class CSectorDataNode : CSyncDataNodeFrequent
{
public:
    uint16_t m_pos_x; //0xC0
    uint16_t m_pos_y; //0xC2
    uint16_t m_pos_z; //0xC4
};
static_assert(sizeof(CSectorDataNode) == 0xC6);
#pragma pack(pop)

#pragma pack(push, 4)
class CSectorPositionDataNode : CSyncDataNodeFrequent
{
public:
    float m_sector_pos_x; //0x00C0
    float m_sector_pos_y; //0x00C4
    float m_sector_pos_z; //0x00C8
}; //Size: 0x00CC
static_assert(sizeof(CSectorPositionDataNode) == 0xCC);
#pragma pack(pop)

#pragma pack(push,4)
class CPlayerAmbientModelStreamingNode : CSyncDataNodeInfrequent
{
public:
    int m_allowed_ped_model_start_offset;           // 0xC0
    int m_allowed_vehicle_model_start_offset;       // 0xC4
    int m_vehicle_anim_streaming_target_entrypoint; // 0xC8
    int16_t m_vehicle_anim_streaming_target;        // 0xCC
};
static_assert(sizeof(CPlayerAmbientModelStreamingNode) == 0xD0);
#pragma pack(pop)

#pragma pack(push, 4)
class CPlayerAppearanceDataNode : CSyncDataNodeInfrequent
{
public:
    uint32_t unk_0xC0[60]; //0xC0
    class CPedComponents components; //0x1A0
    char pad_0x268[8]; //0x268
    uint32_t unk_0x270[6]; //0x270
    uint32_t unk_0x288[6]; //0x288
    char pad_0x2A0[8]; //0x2A0
    float unk_0x2A8; //0x2A8
    uint8_t unk_0x2AC; //0x2AC
    uint8_t unk_0x2AD; //0x2AD
    char pad_0x2AE[26]; //0x2AE
    float m_shape_mix; //0x2C8
    float m_skin_mix; //0x2CC
    float m_third_mix; //0x2D0
    float unk_0x2D4; //0x2D4
    float unk_0x2D8[13]; //0x2D8
    float unk_0x30C[13]; //0x30C
    float unk_0x340[20]; //0x340
    uint8_t unk_0x390[13]; //0x390
    uint8_t unk_0x39D[13]; //0x39D
    uint8_t unk_0x3AA[13]; //0x3AA
    uint8_t m_shape_first; //0x3B7
    uint8_t m_shape_second; //0x3B8
    uint8_t m_shape_third; //0x3B9
    uint8_t m_skin_first; //0x3BA
    uint8_t m_skin_second; //0x3BB
    uint8_t m_skin_third; //0x3BC
    uint8_t unk_0x3BD[13]; //0x3BD
    uint8_t unk_0x3CA[11]; //0x3CA
    int16_t unk_0x3D6; //0x3D6
    uint8_t unk_0x3D8; //0x3D8
    uint8_t unk_0x3D9; //0x3D9
    char pad_0x3DA[1]; //0x3DA
    bool unk_0x3DB; //0x3DB
    bool unk_0x3DC; //0x3DC
    char pad_0x3DD[3]; //0x3DD
    uint32_t unk_0x3E0; //0x3E0
    uint32_t unk_0x3E4; //0x3E4
    uint32_t unk_0x3E8; //0x3E8
    uint32_t unk_0x3EC; //0x3EC
    uint32_t unk_0x3F0; //0x3F0
    float unk_0x3F4; //0x3F4
    float m_blend_in_duration; //0x3F8
    float m_blend_out_duration; //0x3FC
    uint32_t m_anim_name_hash; //0x400
    uint32_t m_anim_dict_index; //0x404
    uint32_t m_anim_flags; //0x408
    uint32_t unk_0x40C; //0x40C
    uint32_t unk_0x410; //0x410
    bool m_anim_task_active; //0x414
    bool unk_0x415; //0x415
    bool m_task_move_active; //0x416
    bool m_mobile_phone_task_active; //0x417
    bool m_mobile_phone_gesture_active; //0x418
    bool unk_0x419; //0x419
    uint32_t unk_0x41C; //0x41C
    uint32_t m_model_hash; //0x420
    uint32_t m_voice_hash; //0x424
    uint32_t m_phone_mode; //0x428
    uint32_t unk_0x42C; //0x42C
    uint8_t m_parachute_tint_index; //0x430
    uint8_t m_parachute_pack_tint_index; //0x431
    uint16_t m_respawn_object; //0x432
    bool m_has_head_blend_data; //0x434
    bool unk_0x435; //0x435
    bool m_has_respawn_object; //0x436
    char pad_0x437; //0x437
    uint32_t unk_0x438_clip_maybe; //0x438
    uint32_t unk_0x43C; //0x43C
    uint32_t unk_0x440; //0x440
    bool unk_0x444; //0x444
    bool unk_0x445; //0x445
    bool unk_0x446; //0x446
    uint8_t unk_0x447; //0x447
    uint16_t unk_0x448; //0x448
    uint16_t unk_0x44A; //0x44A
    uint16_t unk_0x44C; //0x44C
    bool unk_0x44E; //0x44E
    bool unk_0x44F; //0x44F
    bool unk_0x450; //0x450
    uint8_t unk_0x451; //0x451
    uint32_t unk_0x452; //0x452
    uint32_t unk_0x456; //0x456
};
static_assert(sizeof(CPlayerAppearanceDataNode) == 0x46C);
#pragma pack(pop)

#pragma pack(push, 4)
class CPlayerCameraDataNode : CSyncDataNodeFrequent
{
public:
    float m_free_cam_pos_x; //0x00C0
    float m_free_cam_pos_y; //0x00C4
    float m_free_cam_pos_z; //0x00C8
    char pad_00CC[4]; //0x00CC
    float m_lock_on_target_offset_x; //0x00D0
    float m_lock_on_target_offset_y; //0x00D4
    char pad_00D8[40]; //0x00D8
    float m_camera_x; //0x0100
    float m_camera_z; //0x0104
    int16_t m_free_aim_locked_on_target; //0x0108
    bool m_free_cam; //0x010A
    char pad_010B[2]; //0x010B
    bool m_has_position_offset; //0x010D
    char pad_010E[1]; //0x010E
    bool m_is_long_range_target; //0x010F
    char pad_0110[48]; //0x0110
}; //Size: 0x0140
static_assert(sizeof(CPlayerCameraDataNode) == 0x140);
#pragma pack(pop)

#pragma pack(push,4)
class CPlayerCreationDataNode : CProjectBaseSyncDataNode
{
public:
    uint32_t m_model; //0x00C0
    uint32_t m_num_scars; //0x00C4
    char unk_struct_0xC8[192]; //0x00C8
    uint32_t unk_0188; //0x0188
    char pad_018C[4]; //0x018C
    char m_scar_struct[176]; //0x0190
    bool unk_0240; //0x0240
    char pad_0241[19]; //0x0241
}; //Size: 0x0254
static_assert(sizeof(CPlayerCreationDataNode) == 0x254);
#pragma pack(pop)

#pragma pack(push, 1)
class CPlayerExtendedGameStateNode : CSyncDataNodeInfrequent
{
public:
    float waypoint_x;       //0x00C0
    float waypoint_y;       //0x00C4
    bool unk1;              //0x00C5
    bool unk2;              //0x00C6
    bool unk3;              //0x00C7
    bool unk4;              //0x00C8
    bool unk5;              //0x00C9
    bool has_waypoint_data; //0x00CA
    bool is_waypoint_set;   //0x00CB
};
#pragma pack(pop)

#pragma pack(push,4)
class CPlayerGameStateDataNode : CSyncDataNodeInfrequent
{
public:
    uint32_t m_player_state; //0x00C0
    bool m_controls_disabled_by_script; //0x00C4
    bool m_is_max_armor_and_health_default; //0x00C5
    bool unk_000C6; //0x00C6
    bool m_is_spectating; //0x00C7
    bool m_is_antagonistic_to_another_player; //0x00C8
    bool m_never_target; //0x00C9
    bool m_use_kinematic_physics; //0x00CA
    bool m_has_tutorial_data; //0x00CB
    bool m_pending_tutorial_change; //0x00CC
    bool unk_00CD; //0x00CD
    bool m_respawning; //0x00CE
    bool m_will_jack_any_player; //0x00CF
    bool m_will_jack_wanted_players; //0x00D0
    bool m_dont_drag_from_car; //0x00D1
    bool m_random_peds_flee; //0x00D2
    bool m_every_ped_back_away; //0x00D3
    bool m_has_microphone; //0x00D4
    bool m_is_invincible; //0x00D5
    bool unk_00D6; //0x00D6
    bool unk_00D7; //0x00D7
    bool m_seatbelt; //0x00D8
    bool unk_00D9; //0x00D9
    bool m_bullet_proof; //0x00DA
    bool m_fire_proof; //0x00DB
    bool m_explosion_proof; //0x00DC
    bool m_collision_proof; //0x00DD
    bool m_melee_proof; //0x00DE
    bool m_water_proof; //0x00DF
    bool m_steam_proof; //0x00E0
    bool unk_00E1; //0x00E1
    bool unk_00E2; //0x00E2
    bool unk_00E3; //0x00E3
    bool unk_00E4; //0x00E4
    bool unk_00E5; //0x00E5
    bool unk_00E6; //0x00E6
    bool unk_00E7; //0x00E7
    bool unk_00E8; //0x00E8
    bool unk_00E9; //0x00E9
    bool unk_00EA; //0x00EA
    bool unk_00EB; //0x00EB
    bool unk_00EC; //0x00EC
    bool unk_00ED; //0x00ED
    bool unk_00EE; //0x00EE
    bool unk_00EF; //0x00EF
    bool unk_00F0; //0x00F0
    bool unk_00F1; //0x00F1
    bool unk_00F2; //0x00F2
    bool unk_00F3; //0x00F3
    bool unk_00F4; //0x00F4
    bool unk_00F5; //0x00F5
    bool unk_00F6; //0x00F6
    bool unk_00F7; //0x00F7
    bool unk_00F8; //0x00F8
    bool unk_00F9; //0x00F9
    bool unk_00FA; //0x00FA
    bool unk_00FB; //0x00FB
    uint32_t unk_00FC; //0x00FC
    uint32_t m_mobile_ring_state; //0x0100
    int32_t m_player_team; //0x0104
    float m_air_drag_multiplier; //0x0108
    uint32_t m_max_health; //0x010C
    uint32_t m_max_armor; //0x0110
    uint32_t m_jack_speed; //0x0114
    uint16_t m_player_is_targetable_by_team; //0x0118
    uint32_t m_override_receive_chat; //0x011C
    uint32_t m_override_send_chat; //0x0120
    bool unk_0124; //0x0124
    bool unk_0125; //0x0125
    bool unk_0126; //0x0126
    bool unk_0127; //0x0127
    uint16_t m_spectating_net_id; //0x0128
    uint8_t m_antagonistic_to_player_id; //0x012C
    uint8_t m_tutorial_index; //0x012B
    uint8_t m_tutorial_instance_id; //0x012C
    char pad_012D[2]; //0x012D
    float m_microphone_volume; //0x0130
    uint32_t m_voice_channel; //0x0134
    bool m_is_overriding_voice_proximity; //0x0138
    char pad_0139[7]; //0x0139
    float m_voice_proximity_x; //0x0140
    float m_voice_proximity_y; //0x0144
    float m_voice_proximity_z; //0x0148
    float m_voice_proximity_radius_maybe; //0x014C
    uint32_t unk_0150; //0x0150
    uint32_t m_vehicle_weapon_index; //0x0154
    bool m_has_vehicle_weapon_index; //0x0158
    uint32_t m_decor_count; //0x015C
    uint32_t m_decor_type[3]; // 0x0160
    uint32_t m_decor_value[3]; // 0x016C
    uint32_t m_decor_name_hash[3]; // 0x0178
    bool m_friendly_fire_allowed; //0x0184
    bool unk_0185; //0x0185
    uint8_t m_current_garage_instance_index; //0x0186
    uint8_t m_current_property_id; //0x0187
    uint8_t unk_0188; //0x0188
    uint8_t unk_0189; //0x0189
    bool m_battle_aware; //0x018A
    bool m_vehicle_jump_down; //0x018B
    float m_weapon_defence_modifier; //0x018C
    float m_weapon_defence_modifier_2; //0x0190
    bool m_is_overriding_population_control_sphere; //0x0194
    char pad_0195[11]; //0x0195
    float m_population_control_sphere_x; //0x01A0
    float m_population_control_sphere_y; //0x01A4
    float m_population_control_sphere_z; //0x01A8
    uint16_t unk_01AC; //0x01AC
    uint16_t unk_01AE; //0x01AE
    uint16_t unk_01B0; //0x01B0
    bool new_01B2;
    bool new_01B3;
    bool pad_01B2; //0x01B2
    bool unk_01B3; //0x01B3
    bool m_no_collision; //0x01B4
    bool unk_01B5; //0x01B5
    bool unk_01B6; //0x01B6
    bool m_super_jump; //0x01B7
    bool unk_01B8; //0x01B8
    bool unk_01B9; //0x01B9
    uint16_t unk_01BA; //0x01BA
    uint32_t unk_01BC; //0x01BC
    float unk_01C0; //0x01C0
    float m_weapon_damage_modifier;  //0x01C4 Divisor: 0x3F800000
    float m_melee_weapon_damage_modifier;  //0x01C8 Divisor: 0x3F800000
    float unk_01CC; //0x01CC
    bool unk_01D0; //0x01D0
    char pad_01D1[11]; //0x01D1
    float unk_01E0; //0x01E0
    float unk_01E4; //0x01E4
    float unk_01E8; //0x01E8
    uint32_t unk_01EC; //0x01EC
    uint8_t unk_01F0; //0x01F0
    uint8_t unk_01F1; //0x01F1
    bool unk_01F2; //0x01F2
    uint8_t unk_01F3; //0x01F3
    bool unk_01F4; //0x01F4
    bool unk_01F5; //0x01F5
}; //Size: 0x01F8
static_assert(sizeof(CPlayerGameStateDataNode) == 0x1F8);
#pragma pack(pop)

/*#pragma pack(push, 8)
class CPlayerGamerDataNode : CSyncDataNodeInfrequent
{
public:
    ClanData m_clan_data;               //0x00C0
    bool m_need_crew_rank_sysflags;     //0x0178
    bool m_need_crew_rank_title;        //0x0179
    char m_crew_rank_title[25];         //0x017A
    bool m_has_started_transition;      //0x0193
    bool m_has_transition_info;         //0x0194
    char m_transition_info_buffer[169]; //0x0195
    int m_player_privilege_flags;       //0x0240
    uint32_t m_matchmaking_group;       //0x0244
    bool m_need_mute_data;              //0x0248
    int32_t m_mute_count;               //0x024C
    int32_t m_mute_talkers_count;       //0x0250
    uint32_t m_unk;                     //0x0254
    int32_t m_account_id;               //0x0258
};
static_assert(sizeof(CPlayerGamerDataNode) == 0x260);
#pragma pack(pop)*/

#pragma pack(push, 8)
class CGroup;
class CGroupMember
{
public:
    int16_t m_net_id;
    CPed* m_ped; // this isn't serialized
};
static_assert(sizeof(CGroupMember) == 0x10);

class CPlayerPedGroupDataNode : CSyncDataNodeInfrequent
{
public:
    char m_unused[0x10];		   // 0xC0
    CGroup* m_group;			   // 0xD0 (not serialized)
    CGroupMember m_members[7];	   // 0xD8
    CGroupMember m_leader;         // 0x148
    char m_unused2[8];             // 0x158
    float m_max_separation;        // 0x160
    char m_unused3[0xC];           // 0x164
    int m_pop_type;                // 0x170
    bool m_needs_group_event_scan; // 0x175
    char m_unused4[6];             // 0x176
    int m_formation_type;          // 0x17C
    float m_formation_distance;    // 0x180
    char m_unused5[0xC];           // 0x184
};
static_assert(sizeof(CPlayerPedGroupDataNode) == 0x190);
#pragma pack(pop)

#pragma pack(push,4)
class CPlayerSectorPosNode : CSyncDataNodeFrequent
{
public:
    Rage::fvector3 m_sector_pos; //0x00C0
    bool m_is_standing_on_entity; //0x00CC
    bool unk_00CD; //0x00CD
    bool unk_00CE; //0x00CE
    char pad_00CF[1]; //0x00CF
    uint16_t m_entity_standing_on; //0x00D0
    char pad_00D2[12]; //0x00D2
    Rage::fvector3 m_standing_on_entity_offset; //0x00E0
    char pad_00EC[8]; //0x00EC
    uint32_t m_stealth_noise; //0x00F4
}; //Size: 0x00F8
static_assert(sizeof(CPlayerSectorPosNode) == 0xF8);
#pragma pack(pop)

#pragma pack(push,4)
class CPlayerWantedAndLOSDataNode : CSyncDataNodeInfrequent
{
public:
    Rage::fvector3 m_wanted_position; // 0xC0
    int m_time_in_prev_pursuit;       // 0xCC
    Rage::fvector3 m_unk_position;    // 0xD0
    int m_time_in_pursuit;            // 0xDC
    int m_wanted_level;               // 0xE0
    int m_unk_wanted_level;           // 0xE4
    int m_current_time;               // 0xE8
    int m_unk_player_bitset;          // 0xEC
    int m_pursuit_start_time;         // 0xF0
    uint8_t m_fake_wanted_level;      // 0xF4
    bool m_cops_cant_see_player;      // 0xF5
    bool m_is_evading;                // 0xF6
    bool m_pending_wanted_level;      // 0xF7
    bool m_unk3;                      // 0xF8
    uint8_t m_unk_player_index;       // 0xF9
};
static_assert(sizeof(CPlayerWantedAndLOSDataNode) == 0xFC);
#pragma pack(pop)

#pragma pack(push, 8)
class CPickupPlacementCreationDataNode : CProjectBaseSyncDataNode
{
public:
    bool m_has_pickup_data; //0x00C0
    char pad_00C1[15]; //0x00C1
    Rage::fvector3 m_pickup_pos; //0x00D0
    char pad_00DC[4]; //0x00DC
    Rage::fvector4 m_pickup_orientation; //0x00E0
    uint32_t m_pickup_type; //0x00F0
    uint32_t m_pickup_flags; //0x00F4
    uint32_t m_amount; //0x00F8
    uint32_t m_custom_model; //0x00FC
    uint32_t m_custom_regeneration_time; //0x0100
    uint32_t m_team_permits; //0x0104
    uint64_t* unk_struct_0108; //0x0108
}; //Size: 0x0110
static_assert(sizeof(CPickupPlacementCreationDataNode) == 0x110);
#pragma pack(pop)

#pragma pack(push, 8)
class CPickupCreationDataNode : CProjectBaseSyncDataNode
{
public:
    bool m_has_placement; //0x00C0
    char pad_00C1[7]; //0x00C1
    CGameScriptObjInfo m_script_object_info; //0x00C8
    uint32_t m_pickup_hash; //0x0118
    uint32_t m_amount; //0x011C
    uint32_t m_custom_model; //0x0120
    uint32_t m_life_time; //0x0124
    uint32_t m_weapon_component[12]; //0x0128
    uint32_t m_num_weapon_components; //0x0154
    uint32_t m_tint_index; //0x0158
    bool m_player_gift; //0x015C
    bool unk_015D; //0x015D
    char pad_015E[6]; //0x015E
    uint32_t unk_0164; //0x0164
    bool unk_0168; //0x0168
}; //Size: 0x0170
static_assert(sizeof(CPickupCreationDataNode) == 0x170);
#pragma pack(pop)

class CPedAIDataNode : CSyncDataNodeInfrequent
{
public:
    uint32_t m_relationship_group; //0x00C0
    uint32_t m_decision_maker_type; //0x00C4
}; //Size: 0x00C8
static_assert(sizeof(CPedAIDataNode) == 0xC8);

#pragma pack(push, 4)
class CPedAppearanceDataNode : CSyncDataNodeInfrequent
{
public:
    uint32_t unk_0xC0[6]; //0xC0
    uint32_t unk_0xD8[6]; //0xD8
    class CPedComponents components; //0xF0
    char pad_0x1B8[8]; //0x1B8
    uint32_t unk_0x1C0; //0x1C0
    uint8_t unk_0x1C4; //0x1C4
    uint8_t unk_0x1C5; //0x1C5
    char pad_0x1C6[2]; //0x1C6
    uint32_t unk_0x1C8; //0x1C8
    uint32_t unk_0x1CC; //0x1CC
    uint32_t unk_0x1D0; //0x1D0
    bool unk_0x1D4; //0x1D4
    bool unk_0x1D5; //0x1D5
    bool unk_0x1D6; //0x1D6
    uint8_t unk_0x1D7; //0x1D7
    uint16_t unk_0x1D8; //0x1D8
    uint16_t unk_0x1DA; //0x1DA
    uint16_t unk_0x1DC; //0x1DC
    bool unk_0x1DE; //0x1DE
    bool unk_0x1DF; //0x1DF
    bool unk_0x1E0; //0x1E0
    uint8_t unk_0x1E1; //0x1E1
};
static_assert(sizeof(CPedAppearanceDataNode) == 0x1E4);
#pragma pack(pop)

#pragma pack(push,4)
class CPedAttachDataNode : CSyncDataNodeInfrequent
{
public:
    Rage::fvector3 m_offset; //0x00C0
    char pad_00CC[4]; //0x00CC
    Rage::fvector4 m_orientation; //0x00D0
    uint16_t m_attached_to; //0x00E0
    uint16_t m_attachment_bone; //0x00E2
    uint32_t m_attachment_flags; //0x00E4
    float m_heading_1; //0x00E8
    float m_heading_2; //0x00EC
    bool m_attached; //0x00F0
    bool unk_00F1; //0x00F1
};
static_assert(sizeof(CPedAttachDataNode) == 0xF4);
#pragma pack(pop)

#pragma pack(push, 4)
class CPedComponentReservationDataNode : CSyncDataNodeInfrequent
{
public:
    uint32_t m_num_peds_using_component; //0x00C0
    uint16_t m_peds_using_component[32]; //0x00C4
}; //Size: 0x00C8
#pragma pack(pop)

static_assert(sizeof(CPedComponentReservationDataNode) == 0x104);

#pragma pack(push,4)
class CPedCreationDataNode : CProjectBaseSyncDataNode
{
public:
    uint32_t m_pop_type;  //0x00C0
    uint32_t m_model; //0x00C4
    uint32_t m_random_seed; //0x00C8
    uint32_t m_max_health; //0x00CC
    bool m_in_vehicle; //0x00D0
    char pad_0xD1[1]; //0x00D1
    uint16_t m_vehicle_id; //0x00D2
    uint32_t m_vehicle_seat; //0x00D4
    bool m_has_prop; //0x00D8
    char pad_0xD9[3];  //0x00D9
    uint32_t m_prop_model; //0x00DC
    bool m_is_standing; //0x00E0
    bool m_is_respawn_object_id; //0x00E1
    bool m_is_respawn_flagged_for_removal; //0x00E2
    bool m_has_attr_damage_to_player; //0x00E3
    uint8_t m_attribute_damage_to_player; //0x00E4
    uint32_t m_voice_hash; //0x00E8
}; //Size: 0x00EC
static_assert(sizeof(CPedCreationDataNode) == 0xEC);
#pragma pack(pop)

#pragma pack(push,2)
class CPedGameStateDataNode : CSyncDataNodeInfrequent
{
public:
    bool m_weapon_component_something[12]; //0x0C0
    uint32_t m_weapon_component_hash[12]; //0x0CC
    uint32_t m_gadget_hash[3]; //0x0F8
    uint32_t unk_0104; // 0x0104
    uint32_t unk_0108; // 0x0108
    uint32_t unk_010C; // 0x010C
    float unk_0110; // 0x0110
    float unk_0114; // 0x0114
    float unk_0118; // 0x0118
    bool unk_011C; // 0x011C
    char pad_011D[3]; //0x011D
    uint32_t m_arrest_state; //0x0120
    uint32_t m_death_state; //0x0124
    uint32_t m_weapon_hash; //0x0128
    uint32_t m_num_weapon_components; //0x012C
    uint32_t m_num_equiped_gadgets; //0x0130
    uint32_t m_seat; //0x0134
    uint32_t m_action_mode_override; //0x0138
    uint32_t unk_013C; // 0x013C
    uint16_t m_vehicle; //0x0140
    uint16_t m_mount_id; //0x0142
    uint16_t m_custodian_id; //0x0144
    uint16_t unk_0146; // 0x0146
    bool m_tint_index; //0x0148
    char pad_0149; //0x0149
    uint8_t unk_014A; // 0x014A
    bool m_is_handcuffed; //0x014B
    bool m_can_preform_arrest; //0x014C
    bool m_can_preform_uncuff; //0x014D
    bool m_can_be_arrested; //0x014E
    bool m_is_in_custody; //0x014F
    char pad_0150; //0x0150
    bool m_weapon_exists; //0x0151
    bool m_weapon_visible; //0x0152
    bool m_weapon_has_ammo; //0x0153
    bool m_weapon_attach_left; //0x0154
    char pad_0155; //0x0155
    bool m_in_seat; //0x0156
    bool m_in_vehicle; //0x0157
    bool m_on_mount; //0x0158
    bool m_has_custodian_or_arrest_flags; //0x0159
    char pad_015A; //0x015A
    bool m_action_mode_enabled; //0x015B
    bool m_stealth_mode_enabled; //0x015C
    bool unk_015D; // 0x015D
    bool unk_015E; // 0x015E
    bool unk_015F; // 0x015F
    bool unk_0160; // 0x0160
    bool unk_0161; // 0x0161
    bool unk_0162; // 0x0162
    bool unk_0163; // 0x0163
    bool unk_0164; // 0x0164
    bool unk_0165; // 0x0165
    bool unk_0166; // 0x0166
    bool unk_0167; // 0x0167
    bool unk_0168; // 0x0168
    bool unk_0169; // 0x0169
    bool unk_016A; // 0x016A
    bool unk_016B; // 0x016B
    bool unk_016C; // 0x016C
    bool unk_016D; // 0x016D
    bool unk_016E; // 0x016E
    bool unk_016F; // 0x016F
    bool unk_0170; // 0x0170
    bool unk_0171; // 0x0171
    bool unk_0172; // 0x0172
}; //Size: 0x0174
static_assert(sizeof(CPedGameStateDataNode) == 0x178);
#pragma pack(pop)

#pragma pack(push,2)
class CPedHealthDataNode : CSyncDataNodeInfrequent
{
public:
    uint32_t unk_00C0; //0x00C0
    uint32_t m_health; //0x00C4
    uint32_t m_armor; //0x00C8
    uint32_t unk_00CC; //0x00CC
    uint32_t unk_00D0; //0x00D0
    uint32_t m_weapon_damage_hash; //0x00D4
    uint32_t m_hurt_end_time; //0x00D8
    uint32_t m_weapon_damage_component; //0x00DC
    uint16_t m_weapon_damage_entity; //0x00E0
    bool m_has_max_health; //0x00E2
    bool m_has_default_armor; //0x00E3
    bool unk_00E4; //0x00E4
    bool m_killed_with_headshot; //0x00E5
    bool m_killed_with_melee; //0x00E6
    char m_hurt_started; //0x00E7
    bool unk_00E8; //0x00E8
    bool unk_00E9; //0x00E9
}; //Size: 0x0EA
static_assert(sizeof(CPedHealthDataNode) == 0xEA);
#pragma pack(pop)

#pragma pack(push, 4)
class CPedInventoryDataNode
{
public:
    char pad_0000[5232];
    uint32_t m_items[105];
    uint32_t m_num_items;
    uint32_t m_ammos[65];
    uint32_t m_ammo_quantities[65];
    uint32_t m_num_ammos;
    uint8_t unk_1680[105];
    uint8_t unk_16E9[105];
    char pad_1680[1260];
    bool m_infinite_ammos[65];
    bool m_ammo_all_infinite;
};
static_assert(sizeof(CPedInventoryDataNode) == 0x1E24);
#pragma pack(pop)

#pragma pack(push,4)
class CPedMovementDataNode : CSyncDataNodeFrequent
{
public:
    bool m_has_desired_move_blend_ratio_x; //0x00C0
    bool m_has_desired_move_blend_ratio_y; //0x00C1
    bool unk_00C2; //0x00C2
    float m_desired_move_blend_ratio_x; //0x00C4
    float m_desired_move_blend_ratio_y; //0x00C8
    float unk_00CC; //0x00CC
    float m_desired_pitch; //0x00D0
}; //Size: 0x00D4
static_assert(sizeof(CPedMovementDataNode) == 0xD4);
#pragma pack(pop)

#pragma pack(push, 4)
class CPedMovementGroupDataNode : CSyncDataNodeFrequent
{
public:
    float m_unk;			                  // 0xC0 (CTaskMotionInAutomobile+0x1EC)
    uint32_t m_movement_task_index;           // 0xC4
    uint32_t m_movement_task_stage;           // 0xC8
    uint32_t m_movement_group;                // 0xCC
    uint32_t m_overridden_weapon_group;       // 0xD0
    uint32_t m_overridden_unk_group;          // 0xD4 (SET_PED_ALTERNATE_MOVEMENT_ANIM?)
    bool m_is_crouching;                      // 0xD8
    bool m_is_stealthy;                       // 0xD9
    bool m_is_strafing;                       // 0xDA
    bool m_is_ragdolling;                     // 0xDB
    bool m_is_ragdoll_constraint_ankle_active;// 0xDC
    bool m_is_ragdoll_constraint_wrist_active;// 0xDD
    char m_pad1[2];                           // 0xDE
    char m_tennis_data[0x20];                 // 0xE0 TODO
};
static_assert(sizeof(CPedMovementGroupDataNode) == 0x100);
#pragma pack(pop)

#pragma pack(push, 4)
class CPedOrientationDataNode : CSyncDataNodeFrequent
{
public:
    bool m_has_desired_heading_x; //000C1
    bool m_has_desired_heading_y; //000C2
    float m_desired_heading_x; //0x00C4
    float m_desired_heading_y; //0x00C8
};
static_assert(sizeof(CPedOrientationDataNode) == 0xCC);
#pragma pack(pop)

#pragma pack(push, 4)
class CPedScriptCreationDataNode : CProjectBaseSyncDataNode
{
public:
    bool m_stay_in_car_when_jacked; //0x00C0
}; //Size: 0x00C1
static_assert(sizeof(CPedScriptCreationDataNode) == 0xC4);
#pragma pack(pop)

#pragma pack(push,4)
class CPedTaskSequenceData
{
public:
    int m_task_type;       // 0x00
    int m_task_data_size;  // 0x04
    char m_task_data[602]; // 0x08
};
static_assert(sizeof(CPedTaskSequenceData) == 0x264);

class CPedTaskSequenceDataNode : CSyncDataNodeFrequent
{
public:
    bool m_has_sequence;                  // 0xC0
    int m_sequence_resource_id;           // 0xC4
    int m_num_tasks_in_sequence;          // 0xC8
    CPedTaskSequenceData m_task_data[10]; // 0xCC
    int m_unk;                            // 0x18B4
};
static_assert(sizeof(CPedTaskSequenceDataNode) == 0x18B8);
#pragma pack(pop)

#pragma pack(push,4)
class CPedTaskSpecificDataNode : CSyncDataNodeFrequent
{
public:
    uint32_t m_task_index; //0x00C0
    uint32_t m_task_type; //0x00C4
    uint32_t m_buffer_size; //0x00C8
    uint8_t m_task_data_buffer[603]; //0x00CC
}; //Size: 0x0328
static_assert(sizeof(CPedTaskSpecificDataNode) == 0x328);
#pragma pack(pop)

#pragma pack(push,4)
class CPedTaskData
{
public:
    int m_task_type;   // 0x00
    int m_priority;    // 0x04
    int m_tree_depth;  // 0x08
    int m_sequence_id; // 0x0C
    bool m_active;     // 0x10
};
static_assert(sizeof(CPedTaskData) == 0x14);

class CPedTaskTreeDataNode : CSyncDataNodeFrequent
{
public:
    CPedTaskData m_tasks[8];    // 0xC0
    int m_task_bitset;          // 0x160
    int m_script_command;       // 0x164
    int m_script_command_stage; // 0x168
};
static_assert(sizeof(CPedTaskTreeDataNode) == 0x16C); // tree offset != size for this one
#pragma pack(pop)

#pragma pack(push, 4)
class CObjectCreationDataNode : CProjectBaseSyncDataNode
{
public:
    uint32_t unk_00C0; //0x00C0
    uint32_t unk_00C4; //0x00C4
    uint32_t unk_00C8; //0x00C8
    bool unk_00CC;
    bool unk_00CD;
    uint16_t unk_00D0; //0x00D0
    char pad_0xC2[14]; //0x00D2
    Rage::fvector4 m_object_orientation; //0x00E0
    char pad_00E0[30]; //0x00F0
    Rage::fvector3 m_object_position; //0x0110
    char pad_010C[4]; //0x011C
    Rage::fvector3 m_dummy_position; //0x011E
    char pad_011A[20]; //0x012C
    Rage::fvector3 m_script_grab_position; //0x0140
    char pad_013C[12]; //0x013C
    float m_script_grab_radius; //0x0148
    uint32_t m_created_by; //0x014C
    uint32_t m_model; //0x0150
    uint32_t m_frag_group_index; //0x0154
    uint32_t m_ownership_token; //0x0158
    uint32_t unk_015C; //0x015C
    bool m_no_reassign; //0x0160
    bool unk_0161; //0x0161
    bool m_player_wants_control; //0x0162
    bool m_has_init_physics; //0x0163
    bool m_script_grabbed_from_world; //0x0164
    bool m_has_frag_group; //0x0165
    bool m_is_broken; //0x0166
    bool m_has_exploded; //0x0167
    bool m_keep_registered; //0x0168
    bool unk_0169; //0x0169
    bool unk_016A; //0x016A
    bool unk_016B; //0x016B
}; //Size: 0x016C
static_assert(sizeof(CObjectCreationDataNode) == 0x17C);
#pragma pack(pop)