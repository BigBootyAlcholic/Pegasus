#pragma once
#include "stdafx.hpp"

enum class eTransitionState
{
	TRANSITION_STATE_EMPTY,
	TRANSITION_STATE_SP_SWOOP_UP,
	TRANSITION_STATE_MP_SWOOP_UP,
	TRANSITION_STATE_CREATOR_SWOOP_UP,
	TRANSITION_STATE_PRE_HUD_CHECKS,
	TRANSITION_STATE_WAIT_HUD_EXIT,
	TRANSITION_STATE_WAIT_FOR_SUMMON,
	TRANSITION_STATE_SP_SWOOP_DOWN,
	TRANSITION_STATE_MP_SWOOP_DOWN,
	TRANSITION_STATE_CANCEL_JOINING,
	TRANSITION_STATE_RETRY_LOADING,
	TRANSITION_STATE_RETRY_LOADING_SLOT_1,
	TRANSITION_STATE_RETRY_LOADING_SLOT_2,
	TRANSITION_STATE_RETRY_LOADING_SLOT_3,
	TRANSITION_STATE_RETRY_LOADING_SLOT_4,
	TRANSITION_STATE_WAIT_ON_INVITE,
	TRANSITION_STATE_PREJOINING_FM_SESSION_CHECKS,
	TRANSITION_STATE_LOOK_FOR_FRESH_JOIN_FM,
	TRANSITION_STATE_LOOK_TO_JOIN_ANOTHER_SESSION_FM,
	TRANSITION_STATE_CONFIRM_FM_SESSION_JOINING,
	TRANSITION_STATE_WAIT_JOIN_FM_SESSION,
	TRANSITION_STATE_CREATION_ENTER_SESSION,
	TRANSITION_STATE_PRE_FM_LAUNCH_SCRIPT,
	TRANSITION_STATE_FM_TEAMFULL_CHECK,
	TRANSITION_STATE_START_FM_LAUNCH_SCRIPT,
	TRANSITION_STATE_FM_TRANSITION_CREATE_PLAYER,
	TRANSITION_STATE_IS_FM_AND_TRANSITION_READY,
	TRANSITION_STATE_FM_SWOOP_DOWN,
	TRANSITION_STATE_POST_BINK_VIDEO_WARP,
	TRANSITION_STATE_FM_FINAL_SETUP_PLAYER,
	TRANSITION_STATE_MOVE_FM_TO_RUNNING_STATE,
	TRANSITION_STATE_FM_HOW_TO_TERMINATE,
	TRANSITION_STATE_START_CREATOR_PRE_LAUNCH_SCRIPT_CHECK,
	TRANSITION_STATE_START_CREATOR_LAUNCH_SCRIPT,
	TRANSITION_STATE_CREATOR_TRANSITION_CREATE_PLAYER,
	TRANSITION_STATE_IS_CREATOR_AND_TRANSITION_READY,
	TRANSITION_STATE_CREATOR_SWOOP_DOWN,
	TRANSITION_STATE_CREATOR_FINAL_SETUP_PLAYER,
	TRANSITION_STATE_MOVE_CREATOR_TO_RUNNING_STATE,
	TRANSITION_STATE_PREJOINING_TESTBED_SESSION_CHECKS,
	TRANSITION_STATE_LOOK_FOR_FRESH_JOIN_TESTBED,
	TRANSITION_STATE_LOOK_FOR_FRESH_HOST_TESTBED,
	TRANSITION_STATE_LOOK_TO_JOIN_ANOTHER_SESSION_TESTBED,
	TRANSITION_STATE_LOOK_TO_HOST_SESSION_TESTBED,
	TRANSITION_STATE_CONFIRM_TESTBED_SESSION_JOINING,
	TRANSITION_STATE_WAIT_JOIN_TESTBED_SESSION,
	TRANSITION_STATE_START_TESTBED_LAUNCH_SCRIPT,
	TRANSITION_STATE_TESTBED_TRANSITION_CREATE_PLAYER,
	TRANSITION_STATE_IS_TESTBED_AND_TRANSITION_READY,
	TRANSITION_STATE_TESTBED_SWOOP_DOWN,
	TRANSITION_STATE_TESTBED_FINAL_SETUP_PLAYER,
	TRANSITION_STATE_MOVE_TESTBED_TO_RUNNING_STATE,
	TRANSITION_STATE_TESTBED_HOW_TO_TERMINATE,
	TRANSITION_STATE_QUIT_CURRENT_SESSION_PROMPT,
	TRANSITION_STATE_WAIT_FOR_TRANSITION_SESSION_TO_SETUP,
	TRANSITION_STATE_TERMINATE_SP,
	TRANSITION_STATE_WAIT_TERMINATE_SP,
	TRANSITION_STATE_KICK_TERMINATE_SESSION,
	TRANSITION_STATE_TERMINATE_SESSION,
	TRANSITION_STATE_WAIT_TERMINATE_SESSION,
	TRANSITION_STATE_TERMINATE_SESSION_AND_HOLD,
	TRANSITION_STATE_TERMINATE_SESSION_AND_MOVE_INTO_HOLDING_STATE,
	TRANSITION_STATE_TEAM_SWAPPING_CHECKS,
	TRANSITION_STATE_RETURN_TO_SINGLEPLAYER,
	TRANSITION_STATE_WAIT_FOR_SINGLEPLAYER_TO_START,
	TRANSITION_STATE_WAITING_FOR_EXTERNAL_TERMINATION_CALL,
	TRANSITION_STATE_TERMINATE_MAINTRANSITION,
	TRANSITION_STATE_WAIT_FOR_DIRTY_LOAD_CONFIRM,
	TRANSITION_STATE_DLC_INTRO_BINK,
	TRANSITION_STATE_SPAWN_INTO_PERSONAL_VEHICLE
};

enum class ScriptEntityChangeType
{
	BlockingOfNonTemporaryEvents,
	SettingOfPedRelationshipGroupHash,
	SettingOfDriveTaskCruiseSpeed,
	SettingOfLookAtEntity,
	SettingOfPlaneMinHeightAboveTerrain,
	SetPedRagdollBlockFlag,
	SettingOfTaskVehicleTempAction,
	SetPedFacialIdleAnimOverride,
	SetVehicleLockState,
	SetVehicleExclusiveDriver
};

enum class WorldStateDataType
{
	CarGen,
	EntityArea,
	PopGroupOverride,
	PopMultiplierArea,
	PTFX,
	RoadNode,
	Rope,
	ScenarioBlockingArea,
	VehiclePlayerLocking
};

enum class eNetworkEvents : uint16_t
{
	OBJECT_ID_FREED_EVENT,
	OBJECT_ID_REQUEST_EVENT,
	ARRAY_DATA_VERIFY_EVENT,
	SCRIPT_ARRAY_DATA_VERIFY_EVENT,
	REQUEST_CONTROL_EVENT,
	GIVE_CONTROL_EVENT,
	WEAPON_DAMAGE_EVENT,
	REQUEST_PICKUP_EVENT,
	REQUEST_MAP_PICKUP_EVENT,
	GAME_CLOCK_EVENT,
	GAME_WEATHER_EVENT,
	RESPAWN_PLAYER_PED_EVENT,
	GIVE_WEAPON_EVENT,
	REMOVE_WEAPON_EVENT,
	REMOVE_ALL_WEAPONS_EVENT,
	VEHICLE_COMPONENT_CONTROL_EVENT,
	FIRE_EVENT,
	EXPLOSION_EVENT,
	START_PROJECTILE_EVENT,
	UPDATE_PROJECTILE_TARGET_EVENT,
	REMOVE_PROJECTILE_ENTITY_EVENT,
	BREAK_PROJECTILE_TARGET_LOCK_EVENT,
	ALTER_WANTED_LEVEL_EVENT,
	CHANGE_RADIO_STATION_EVENT,
	RAGDOLL_REQUEST_EVENT,
	PLAYER_TAUNT_EVENT,
	PLAYER_CARD_STAT_EVENT,
	DOOR_BREAK_EVENT,
	SCRIPTED_GAME_EVENT,
	REMOTE_SCRIPT_INFO_EVENT,
	REMOTE_SCRIPT_LEAVE_EVENT,
	MARK_AS_NO_LONGER_NEEDED_EVENT,
	CONVERT_TO_SCRIPT_ENTITY_EVENT,
	SCRIPT_WORLD_STATE_EVENT,
	CLEAR_AREA_EVENT,
	CLEAR_RECTANGLE_AREA_EVENT,
	NETWORK_REQUEST_SYNCED_SCENE_EVENT,
	NETWORK_START_SYNCED_SCENE_EVENT,
	NETWORK_STOP_SYNCED_SCENE_EVENT,
	NETWORK_UPDATE_SYNCED_SCENE_EVENT,
	INCIDENT_ENTITY_EVENT,
	GIVE_PED_SCRIPTED_TASK_EVENT,
	GIVE_PED_SEQUENCE_TASK_EVENT,
	NETWORK_CLEAR_PED_TASKS_EVENT,
	NETWORK_START_PED_ARREST_EVENT,
	NETWORK_START_PED_UNCUFF_EVENT,
	NETWORK_SOUND_CAR_HORN_EVENT,
	NETWORK_ENTITY_AREA_STATUS_EVENT,
	NETWORK_GARAGE_OCCUPIED_STATUS_EVENT,
	PED_CONVERSATION_LINE_EVENT,
	SCRIPT_ENTITY_STATE_CHANGE_EVENT,
	NETWORK_PLAY_SOUND_EVENT,
	NETWORK_STOP_SOUND_EVENT,
	NETWORK_PLAY_AIRDEFENSE_FIRE_EVENT,
	NETWORK_BANK_REQUEST_EVENT,
	NETWORK_AUDIO_BARK_EVENT,
	REQUEST_DOOR_EVENT,
	NETWORK_TRAIN_REPORT_EVENT,
	NETWORK_TRAIN_REQUEST_EVENT,
	NETWORK_INCREMENT_STAT_EVENT,
	MODIFY_VEHICLE_LOCK_WORD_STATE_DATA,
	MODIFY_PTFX_WORD_STATE_DATA_SCRIPTED_EVOLVE_EVENT,
	REQUEST_PHONE_EXPLOSION_EVENT,
	REQUEST_DETACHMENT_EVENT,
	KICK_VOTES_EVENT,
	GIVE_PICKUP_REWARDS_EVENT,
	NETWORK_CRC_HASH_CHECK_EVENT,
	BLOW_UP_VEHICLE_EVENT,
	NETWORK_SPECIAL_FIRE_EQUIPPED_WEAPON,
	NETWORK_RESPONDED_TO_THREAT_EVENT,
	NETWORK_SHOUT_TARGET_POSITION,
	VOICE_DRIVEN_MOUTH_MOVEMENT_FINISHED_EVENT,
	PICKUP_DESTROYED_EVENT,
	UPDATE_PLAYER_SCARS_EVENT,
	NETWORK_CHECK_EXE_SIZE_EVENT,
	NETWORK_PTFX_EVENT,
	NETWORK_PED_SEEN_DEAD_PED_EVENT,
	REMOVE_STICKY_BOMB_EVENT,
	NETWORK_CHECK_CODE_CRCS_EVENT,
	INFORM_SILENCED_GUNSHOT_EVENT,
	PED_PLAY_PAIN_EVENT,
	CACHE_PLAYER_HEAD_BLEND_DATA_EVENT,
	REMOVE_PED_FROM_PEDGROUP_EVENT,
	REPORT_MYSELF_EVENT,
	REPORT_CASH_SPAWN_EVENT,
	ACTIVATE_VEHICLE_SPECIAL_ABILITY_EVENT,
	BLOCK_WEAPON_SELECTION,
	NETWORK_CHECK_CATALOG_CRC
};
enum class eAckCode : uint32_t
{
	ACKCODE_SUCCESS,
	ACKCODE_FAIL,
	ACKCODE_WRONG_OWNER,
	ACKCODE_OUT_OF_SEQUENCE,
	ACKCODE_NO_OBJECT,
	ACKCODE_TOO_MANY_OBJECTS,
	ACKCODE_CANT_APPLY_DATA,
	ACKCODE_MATCH_NOT_STARTED,
	ACKCODE_NONE
};
enum class eNetObjType
{
	NET_OBJ_TYPE_AUTOMOBILE,
	NET_OBJ_TYPE_BIKE,
	NET_OBJ_TYPE_BOAT,
	NET_OBJ_TYPE_DOOR,
	NET_OBJ_TYPE_HELI,
	NET_OBJ_TYPE_OBJECT,
	NET_OBJ_TYPE_PED,
	NET_OBJ_TYPE_PICKUP,
	NET_OBJ_TYPE_PICKUP_PLACEMENT,
	NET_OBJ_TYPE_PLANE,
	NET_OBJ_TYPE_SUBMARINE,
	NET_OBJ_TYPE_PLAYER,
	NET_OBJ_TYPE_TRAILER,
	NET_OBJ_TYPE_TRAIN
};

constexpr const auto net_object_type_strs = std::to_array({
	"AUTOMOBILE",
	"BIKE",
	"BOAT",
	"DOOR",
	"HELI",
	"OBJECT",
	"PED",
	"PICKUP",
	"PICKUP_PLACEMENT",
	"PLANE",
	"SUBMARINE",
	"PLAYER",
	"TRAILER",
	"TRAIN",
	});

enum class eNetObjectFlags : uint16_t
{
	NET_OBJ_FLAGS_FROM_SCRIPT = 1 << 2,
	NET_OBJ_FLAGS_SCRIPTED = 1 << 6,
};

enum class eTaskTypeIndex
{
	CTaskHandsUp = 0,
	CTaskClimbLadder = 1,
	CTaskExitVehicle = 2,
	CTaskCombatRoll = 3,
	CTaskAimGunOnFoot = 4,
	CTaskMovePlayer = 5,
	CTaskPlayerOnFoot = 6,
	CTaskWeapon = 8,
	CTaskPlayerWeapon = 9,
	CTaskPlayerIdles = 10,
	CTaskAimGun = 12,
	CTaskComplex = 12,
	CTaskFSMClone = 12,
	CTaskMotionBase = 12,
	CTaskMove = 12,
	CTaskMoveBase = 12,
	CTaskNMBehaviour = 12,
	CTaskNavBase = 12,
	CTaskScenario = 12,
	CTaskSearchBase = 12,
	CTaskSearchInVehicleBase = 12,
	CTaskShockingEvent = 12,
	CTaskTrainBase = 12,
	CTaskVehicleFSM = 12,
	CTaskVehicleGoTo = 12,
	CTaskVehicleMissionBase = 12,
	CTaskVehicleTempAction = 12,
	CTaskPause = 14,
	CTaskDoNothing = 15,
	CTaskGetUp = 16,
	CTaskGetUpAndStandStill = 17,
	CTaskFallOver = 18,
	CTaskFallAndGetUp = 19,
	CTaskCrawl = 20,
	CTaskComplexOnFire = 25,
	CTaskDamageElectric = 26,
	CTaskTriggerLookAt = 28,
	CTaskClearLookAt = 29,
	CTaskSetCharDecisionMaker = 30,
	CTaskSetPedDefensiveArea = 31,
	CTaskUseSequence = 32,
	CTaskMoveStandStill = 34,
	CTaskComplexControlMovement = 35,
	CTaskMoveSequence = 36,
	CTaskAmbientClips = 38,
	CTaskMoveInAir = 39,
	CTaskNetworkClone = 40,
	CTaskUseClimbOnRoute = 41,
	CTaskUseDropDownOnRoute = 42,
	CTaskUseLadderOnRoute = 43,
	CTaskSetBlockingOfNonTemporaryEvents = 44,
	CTaskForceMotionState = 45,
	CTaskSlopeScramble = 46,
	CTaskGoToAndClimbLadder = 47,
	CTaskClimbLadderFully = 48,
	CTaskRappel = 49,
	CTaskVault = 50,
	CTaskDropDown = 51,
	CTaskAffectSecondaryBehaviour = 52,
	CTaskAmbientLookAtEvent = 53,
	CTaskOpenDoor = 54,
	CTaskShovePed = 55,
	CTaskSwapWeapon = 56,
	CTaskGeneralSweep = 57,
	CTaskPolice = 58,
	CTaskPoliceOrderResponse = 59,
	CTaskPursueCriminal = 60,
	CTaskArrestPed = 62,
	CTaskArrestPed2 = 63,
	CTaskBusted = 64,
	CTaskFirePatrol = 65,
	CTaskHeliOrderResponse = 66,
	CTaskHeliPassengerRappel = 67,
	CTaskAmbulancePatrol = 68,
	CTaskPoliceWantedResponse = 69,
	CTaskSwat = 70,
	CTaskSwatWantedResponse = 72,
	CTaskSwatOrderResponse = 73,
	CTaskSwatGoToStagingArea = 74,
	CTaskSwatFollowInLine = 75,
	CTaskWitness = 76,
	CTaskGangPatrol = 77,
	CTaskArmy = 78,
	CTaskShockingEventWatch = 80,
	CTaskShockingEventGoto = 82,
	CTaskShockingEventHurryAway = 83,
	CTaskShockingEventReactToAircraft = 84,
	CTaskShockingEventReact = 85,
	CTaskShockingEventBackAway = 86,
	CTaskShockingPoliceInvestigate = 87,
	CTaskShockingEventStopAndStare = 88,
	CTaskShockingNiceCarPicture = 89,
	CTaskShockingEventThreatResponse = 90,
	CTaskTakeOffHelmet = 92,
	CTaskCarReactToVehicleCollision = 93,
	CTaskCarReactToVehicleCollisionGetOut = 95,
	CTaskDyingDead = 97,
	CTaskWanderingScenario = 100,
	CTaskWanderingInRadiusScenario = 101,
	CTaskMoveBetweenPointsScenario = 103,
	CTaskChatScenario = 104,
	CTaskCowerScenario = 106,
	CTaskDeadBodyScenario = 107,
	CTaskSayAudio = 114,
	CTaskWaitForSteppingOut = 116,
	CTaskCoupleScenario = 117,
	CTaskUseScenario = 118,
	CTaskUseVehicleScenario = 119,
	CTaskUnalerted = 120,
	CTaskStealVehicle = 121,
	CTaskReactToPursuit = 122,
	CTaskHitWall = 125,
	CTaskCower = 126,
	CTaskCrouch = 127,
	CTaskMelee = 128,
	CTaskMoveMeleeMovement = 129,
	CTaskMeleeActionResult = 130,
	CTaskMeleeUpperbodyAnims = 131,
	CTaskMoVEScripted = 133,
	CTaskScriptedAnimation = 134,
	CTaskSynchronizedScene = 135,
	CTaskComplexEvasiveStep = 137,
	CTaskWalkRoundCarWhileWandering = 138,
	CTaskComplexStuckInAir = 140,
	CTaskWalkRoundEntity = 141,
	CTaskMoveWalkRoundVehicle = 142,
	CTaskReactToGunAimedAt = 144,
	CTaskDuckAndCover = 146,
	CTaskAggressiveRubberneck = 147,
	CTaskInVehicleBasic = 150,
	CTaskCarDriveWander = 151,
	CTaskLeaveAnyCar = 152,
	CTaskComplexGetOffBoat = 153,
	CTaskCarSetTempAction = 155,
	CTaskBringVehicleToHalt = 156,
	CTaskCarDrive = 157,
	CTaskPlayerDrive = 159,
	CTaskEnterVehicle = 160,
	CTaskEnterVehicleAlign = 161,
	CTaskOpenVehicleDoorFromOutside = 162,
	CTaskEnterVehicleSeat = 163,
	CTaskCloseVehicleDoorFromInside = 164,
	CTaskInVehicleSeatShuffle = 165,
	CTaskExitVehicleSeat = 167,
	CTaskCloseVehicleDoorFromOutside = 168,
	CTaskControlVehicle = 169,
	CTaskMotionInAutomobile = 170,
	CTaskMotionOnBicycle = 171,
	CTaskMotionOnBicycleController = 172,
	CTaskMotionInVehicle = 173,
	CTaskMotionInTurret = 174,
	CTaskReactToBeingJacked = 175,
	CTaskReactToBeingAskedToLeaveVehicle = 176,
	CTaskTryToGrabVehicleDoor = 177,
	CTaskGetOnTrain = 178,
	CTaskGetOffTrain = 179,
	CTaskRideTrain = 180,
	CTaskMountThrowProjectile = 190,
	CTaskGoToCarDoorAndStandStill = 195,
	CTaskMoveGoToVehicleDoor = 196,
	CTaskSetPedInVehicle = 197,
	CTaskSetPedOutOfVehicle = 198,
	CTaskVehicleMountedWeapon = 199,
	CTaskVehicleGun = 200,
	CTaskVehicleProjectile = 201,
	CTaskSmashCarWindow = 204,
	CTaskMoveGoToPoint = 205,
	CTaskMoveAchieveHeading = 206,
	CTaskMoveFaceTarget = 207,
	CTaskComplexGoToPointAndStandStillTimed = 208,
	CTaskMoveGoToPointAndStandStill = 208,
	CTaskMoveFollowPointRoute = 209,
	CTaskMoveSeekEntity_CEntitySeekPosCalculatorStandard = 210,
	CTaskMoveSeekEntity_CEntitySeekPosCalculatorLastNavMeshIntersection = 211,
	CTaskMoveSeekEntity_CEntitySeekPosCalculatorLastNavMeshIntersection2 = 212,
	CTaskMoveSeekEntity_CEntitySeekPosCalculatorXYOffsetFixed = 213,
	CTaskMoveSeekEntity_CEntitySeekPosCalculatorXYOffsetFixed2 = 214,
	CTaskExhaustedFlee = 215,
	CTaskGrowlAndFlee = 216,
	CTaskScenarioFlee = 217,
	CTaskSmartFlee = 218,
	CTaskFlyAway = 219,
	CTaskWalkAway = 220,
	CTaskWander = 221,
	CTaskWanderInArea = 222,
	CTaskFollowLeaderInFormation = 223,
	CTaskGoToPointAnyMeans = 224,
	CTaskTurnToFaceEntityOrCoord = 225,
	CTaskFollowLeaderAnyMeans = 226,
	CTaskFlyToPoint = 228,
	CTaskFlyingWander = 229,
	CTaskGoToPointAiming = 230,
	CTaskGoToScenario = 231,
	CTaskSeekEntityAiming = 233,
	CTaskSlideToCoord = 234,
	CTaskSwimmingWander = 235,
	CTaskMoveTrackingEntity = 237,
	CTaskMoveFollowNavMesh = 238,
	CTaskMoveGoToPointOnRoute = 239,
	CTaskEscapeBlast = 240,
	CTaskMoveWander = 241,
	CTaskMoveBeInFormation = 242,
	CTaskMoveCrowdAroundLocation = 243,
	CTaskMoveCrossRoadAtTrafficLights = 244,
	CTaskMoveWaitForTraffic = 245,
	CTaskMoveGoToPointStandStillAchieveHeading = 246,
	CTaskMoveGetOntoMainNavMesh = 251,
	CTaskMoveSlideToCoord = 252,
	CTaskMoveGoToPointRelativeToEntityAndStandStill = 253,
	CTaskHelicopterStrafe = 254,
	CTaskGetOutOfWater = 256,
	CTaskMoveFollowEntityOffset = 259,
	CTaskFollowWaypointRecording = 261,
	CTaskMotionPed = 264,
	CTaskMotionPedLowLod = 265,
	CTaskHumanLocomotion = 268,
	CTaskMotionBasicLocomotionLowLod = 269,
	CTaskMotionStrafing = 270,
	CTaskMotionTennis = 271,
	CTaskMotionAiming = 272,
	CTaskBirdLocomotion = 273,
	CTaskFlightlessBirdLocomotion = 274,
	CTaskFishLocomotion = 278,
	CTaskQuadLocomotion = 279,
	CTaskMotionDiving = 280,
	CTaskMotionSwimming = 281,
	CTaskMotionParachuting = 282,
	CTaskMotionDrunk = 283,
	CTaskRepositionMove = 284,
	CTaskMotionAimingTransition = 285,
	CTaskThrowProjectile = 286,
	CTaskCover = 287,
	CTaskMotionInCover = 288,
	CTaskAimAndThrowProjectile = 289,
	CTaskGun = 290,
	CTaskAimFromGround = 291,
	CTaskAimGunVehicleDriveBy = 295,
	CTaskAimGunScripted = 296,
	CTaskReloadGun = 298,
	CTaskWeaponBlocked = 299,
	CTaskEnterCover = 300,
	CTaskExitCover = 301,
	CTaskAimGunFromCoverIntro = 302,
	CTaskAimGunFromCoverOutro = 303,
	CTaskAimGunBlindFire = 304,
	CTaskCombatClosestTargetInArea = 307,
	CTaskCombatAdditionalTask = 308,
	CTaskInCover = 309,
	CTaskAimSweep = 313,
	CTaskSharkCircle = 319,
	CTaskSharkAttack = 320,
	CTaskAgitated = 321,
	CTaskAgitatedAction = 322,
	CTaskConfront = 323,
	CTaskIntimidate = 324,
	CTaskShove = 325,
	CTaskShoved = 326,
	CTaskCrouchToggle = 328,
	CTaskRevive = 329,
	CTaskParachute = 335,
	CTaskParachuteObject = 336,
	CTaskTakeOffPedVariation = 337,
	CTaskCombatSeekCover = 340,
	CTaskCombatFlank = 342,
	CTaskCombat = 343,
	CTaskCombatMounted = 344,
	CTaskMoveCircle = 345,
	CTaskMoveCombatMounted = 346,
	CTaskSearch = 347,
	CTaskSearchOnFoot = 348,
	CTaskSearchInAutomobile = 349,
	CTaskSearchInBoat = 350,
	CTaskSearchInHeli = 351,
	CTaskThreatResponse = 352,
	CTaskInvestigate = 353,
	CTaskStandGuardFSM = 354,
	CTaskPatrol = 355,
	CTaskShootAtTarget = 356,
	CTaskSetAndGuardArea = 357,
	CTaskStandGuard = 358,
	CTaskSeparate = 359,
	CTaskStayInCover = 360,
	CTaskVehicleCombat = 361,
	CTaskVehiclePersuit = 362,
	CTaskVehicleChase = 363,
	CTaskDraggingToSafety = 364,
	CTaskDraggedToSafety = 365,
	CTaskVariedAimPose = 366,
	CTaskMoveWithinAttackWindow = 367,
	CTaskMoveWithinDefensiveArea = 368,
	CTaskShootOutTire = 369,
	CTaskShellShocked = 370,
	CTaskBoatChase = 371,
	CTaskBoatCombat = 372,
	CTaskBoatStrafe = 373,
	CTaskHeliChase = 374,
	CTaskHeliCombat = 375,
	CTaskSubmarineCombat = 376,
	CTaskSubmarineChase = 377,
	CTaskPlaneChase = 378,
	CTaskTargetUnreachable = 379,
	CTaskTargetUnreachableInInterior = 380,
	CTaskTargetUnreachableInExterior = 381,
	CTaskStealthKill = 382,
	CTaskWrithe = 383,
	CTaskAdvance = 384,
	CTaskCharge = 385,
	CTaskMoveToTacticalPoint = 386,
	CTaskToHurtTransit = 387,
	CTaskAnimatedHitByExplosion = 388,
	CTaskNMRelax = 389,
	CTaskNMPose = 391,
	CTaskNMBrace = 392,
	CTaskNMBuoyancy = 393,
	CTaskNMInjuredOnGround = 394,
	CTaskNMShot = 395,
	CTaskNMHighFall = 396,
	CTaskNMBalance = 397,
	CTaskNMElectrocute = 398,
	CTaskNMPrototype = 399,
	CTaskNMExplosion = 400,
	CTaskNMOnFire = 401,
	CTaskNMScriptControl = 402,
	CTaskNMJumpRollFromRoadVehicle = 403,
	CTaskNMFlinch = 404,
	CTaskNMSit = 405,
	CTaskNMFallDown = 406,
	CTaskBlendFromNM = 407,
	CTaskNMControl = 408,
	CTaskNMDangle = 409,
	CTaskNMGenericAttach = 412,
	CTaskNMDraggingToSafety = 414,
	CTaskNMThroughWindscreen = 415,
	CTaskNMRiverRapids = 416,
	CTaskNMSimple = 417,
	CTaskRageRagdoll = 418,
	CTaskJumpVault = 421,
	CTaskJump = 422,
	CTaskFall = 423,
	CTaskReactAimWeapon = 425,
	CTaskChat = 426,
	CTaskMobilePhone = 427,
	CTaskReactToDeadPed = 428,
	CTaskSearchForUnknownThreat = 430,
	CTaskBomb = 432,
	CTaskDetonator = 433,
	CTaskAnimatedAttach = 435,
	CTaskCutScene = 441,
	CTaskReactToExplosion = 442,
	CTaskReactToImminentExplosion = 443,
	CTaskDiveToGround = 444,
	CTaskReactAndFlee = 445,
	CTaskSidestep = 446,
	CTaskCallPolice = 447,
	CTaskReactInDirection = 448,
	CTaskReactToBuddyShot = 449,
	CTaskVehicleGoToAutomobileNew = 454,
	CTaskVehicleGoToPlane = 455,
	CTaskVehicleGoToHelicopter = 456,
	CTaskVehicleGoToSubmarine = 457,
	CTaskVehicleGoToBoat = 458,
	CTaskVehicleGoToPointAutomobile = 459,
	CTaskVehicleGoToPointWithAvoidanceAutomobile = 460,
	CTaskVehiclePursue = 461,
	CTaskVehicleRam = 462,
	CTaskVehicleSpinOut = 463,
	CTaskVehicleApproach = 464,
	CTaskVehicleThreePointTurn = 465,
	CTaskVehicleDeadDriver = 466,
	CTaskVehicleCruiseNew = 467,
	CTaskVehicleCruiseBoat = 468,
	CTaskVehicleStop = 469,
	CTaskVehiclePullOver = 470,
	CTaskVehiclePassengerExit = 471,
	CTaskVehicleFlee = 472,
	CTaskVehicleFleeAirborne = 473,
	CTaskVehicleFleeBoat = 474,
	CTaskVehicleFollowRecording = 475,
	CTaskVehicleFollow = 476,
	CTaskVehicleBlock = 477,
	CTaskVehicleBlockCruiseInFront = 478,
	CTaskVehicleBlockBrakeInFront = 479,
	CTaskVehicleBlockBackAndForth = 478,
	CTaskVehicleCrash = 481,
	CTaskVehicleLand = 482,
	CTaskVehicleLandPlane = 483,
	CTaskVehicleHover = 484,
	CTaskVehicleAttack = 485,
	CTaskVehicleAttackTank = 486,
	CTaskVehicleCircle = 487,
	CTaskVehiclePoliceBehaviour = 488,
	CTaskVehiclePoliceBehaviourHelicopter = 489,
	CTaskVehiclePoliceBehaviourBoat = 490,
	CTaskVehicleEscort = 491,
	CTaskVehicleHeliProtect = 492,
	CTaskVehiclePlayerDriveAutomobile = 494,
	CTaskVehiclePlayerDriveBike = 495,
	CTaskVehiclePlayerDriveBoat = 496,
	CTaskVehiclePlayerDriveSubmarine = 497,
	CTaskVehiclePlayerDriveSubmarineCar = 498,
	CTaskVehiclePlayerDriveAmphibiousAutomobile = 499,
	CTaskVehiclePlayerDrivePlane = 500,
	CTaskVehiclePlayerDriveHeli = 501,
	CTaskVehiclePlayerDriveAutogyro = 502,
	CTaskVehiclePlayerDriveDiggerArm = 503,
	CTaskVehiclePlayerDriveTrain = 504,
	CTaskVehiclePlaneChase = 505,
	CTaskVehicleNoDriver = 506,
	CTaskVehicleAnimation = 507,
	CTaskVehicleConvertibleRoof = 508,
	CTaskVehicleParkNew = 509,
	CTaskVehicleFollowWaypointRecording = 510,
	CTaskVehicleGoToNavmesh = 511,
	CTaskVehicleReactToCopSiren = 512,
	CTaskVehicleGotoLongRange = 513,
	CTaskVehicleWait = 514,
	CTaskVehicleReverse = 515,
	CTaskVehicleBrake = 516,
	CTaskVehicleHandBrake = 517,
	CTaskVehicleTurn = 518,
	CTaskVehicleGoForward = 519,
	CTaskVehicleSwerve = 520,
	CTaskVehicleFlyDirection = 521,
	CTaskVehicleHeadonCollision = 522,
	CTaskVehicleBoostUseSteeringAngle = 523,
	CTaskVehicleShotTire = 524,
	CTaskVehicleBurnout = 525,
	CTaskVehicleRevEngine = 526,
	CTaskVehicleSurfaceInSubmarine = 527,
	CTaskVehiclePullAlongside = 528,
	CTaskVehicleTransformToSubmarine = 529,
	CTaskAnimatedFallback = 530
};


enum class eDoorId
{
	VEH_EXT_DOOR_INVALID_ID = -1,
	VEH_EXT_DOOR_DSIDE_F,
	VEH_EXT_DOOR_DSIDE_R,
	VEH_EXT_DOOR_PSIDE_F,
	VEH_EXT_DOOR_PSIDE_R,
	VEH_EXT_BONNET,
	VEH_EXT_BOOT
};

enum class eWindowId
{
	WINDOW_INVALID_ID = -1,
	FRONT_LEFT_WINDOW = 0,
	FRONT_RIGHT_WINDOW = 1,
	REAR_LEFT_WINDOW = 2,
	REAR_RIGHT_WINDOW = 3,
};

enum class eVehicleSeats
{
	DRIVER = -1,
	PASSENGER,
	LEFT_BACK,
	RIGHT_BACK,
	OUTSIDE_LEFT,
	OUTSIDE_RIGHT,
};

enum class eRemoteEvent
{
	Bounty = 1517551547, // (137, "FM_TXT_BNTY0", iVar1, PLAYER::GET_PLAYER_NAME(Var2.f_1), "", 5000, Var2.f_6);
	CeoBan = 1531565154, // mpply_vipgameplaydisabledtimer
	CeoKick = -11681548,
	CeoMoney = -337848027, // Goon_Paid_Large
	ClearWantedLevel = -1704545346,
	ForceMission = 259469385,  // ), Var0.f_2, 1))
	GiveCollectible = 968269233,  // DLC_SUM20_HIDDEN_COLLECTIBLES xref
	GtaBanner = -330501227, // NETWORK::NETWORK_IS_SCRIPT_ACTIVE("BUSINESS_BATTLES", -1, true, 0) second one
	NetworkBail = -901348601, // NETWORK::NETWORK_BAIL(16, 0, 0); xref func
	PersonalVehicleDestroyed = 1655503526,  // PLYVEH_INS_DES1
	RemoteOffradar = 57493695,    // NETWORK::GET_TIME_DIFFERENCE(NETWORK::GET_NETWORK_TIME(), Var0.f_2)
	SendToCutscene = -1951335381, // (bVar3, bVar4, 125f, 1)
	SendToCayoPerico = 373376135,   // CELL_HI_INV
	SendToLocation = 1669592503,  // &Var222, 11);
	SoundSpam = 996099702,   // CELL_APTINVYACHT
	Spectate = 1840946429,  // SPEC_TCK1
	Teleport = -1321657966, // Mission_Pass_Notify
	TeleportToWarehouse = -1253241415, // .f_4 == 50
	TransactionError = -830063381,  // NETWORK_RECEIVE_PLAYER_JOBSHARE_CASH
	VehicleKick = -503325966,  // PIM_RFMOC
	MCTeleport = 1103127469, // NETWORK::NETWORK_HASH_FROM_PLAYER_HANDLE(PLAYER::PLAYER_ID()) == (first one)
	StartActivity = 1450115979, // (Var0.f_2, -1); first match
	MarkPlayerAsBeast = 1649541577, // GB_BOSSBST xref == PLAYER... global (sadly doesn't actually turn people into the beast)
	KickFromInterior = -1496371358, // ENTITY::SET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID(), PED::GET_PED_MAX_HEALTH(PLAYER::PLAYER_PED_ID()), false)
	InteriorControl = -1638522928, // eventData.f_3 != _INVALID_PLAYER_INDEX_0()
	SendTextLabelSMS = -1773335296,

	Crash = -1604421397, // SET_NO_LOADING_SCREEN, xref it
	Crash2 = -375628860,
	Crash3 = 323285304,

	TSECommand = 800157557, // CnCTG_IN_BF
	TSECommandRotateCam = 225624744, // != 29) && f

	Notification = -642704387,
	NotificationMoneyBanked = 94410750,   // TICK_TC_BANK
	NotificationMoneyRemoved = -242911964, // TICK_TC_REMO
	NotificationMoneyStolen = -295926414, // TICK_TC_STOL
	NotificationCrash1 = -994541138, // AMCH_ATTEMPT2
	NotificationCrash2 = 782258655,  // GBSAL_COLL

	DestroyPersonalVehicle = 109434679, // CnC_PV_THEFT
	TriggerCEORaid = -1906536929,

	StartScriptBegin = -366707054,
	StartScriptProceed = 1757622014
};

enum class eActivityType
{
	HeistPrep = 233,
	Gunrunning = 180,
	Sightseer = 142,
	HeadHunter = 166,
	BuySpecialCargo = 167,
	SellSpecialCargo = 168,
	DefendSpecialCargo = 169,
	StealVehicle = 178,
	ExportVehicle = 188,
	Gunrunning2 = 225,
	GunrunningSell = 226,
	GunrunningDefend = 227,
	BikerSell = 190,
	BikerDefend = 191,
	BusinessResupply = 192,
	Survival = 3,
	Darts = 14,
	ArmWresling = 15,
	GangAttack = 6,
	PilotSchool = 122,
	Golf = 11,
	ShootingRange = 13,
	Tennis = 12,
	BaseJump = 8,
	Deathmatch = 1,
	ImpromptuDeathmatch = 5,
	Mission = 0,
	Race = 2,
	ExecutiveDeathmatch = 148,
	MarkedForDeath = 151,
	PiracyPrevention = 152,
	MostWanted = 153,
	AssetRecovery = 157,
	HostileTakeover = 159,
	Point2Point = 162,
	AmphibiousAssault = 216,
	Velocity = 219,
	GunsForHire = 185,
	ByThePound = 182,
	RippingItUp = 194,
	RaceToPoint = 189,
	HitAndRide = 193,
	CriminalMischief = 205,
	WeaponOfChoice = 186,
	FragileGoods = 207,
	Torched = 208,
	Outrider = 209,
	WheelieRider = 210,
	POW = 183,
	ExecutiveSearch = 199,
	StandYourGround = 201,
	AutoBuyout = 163,
	DueDiligence = 160,
	MarketManipulation = 154,
	CourierService = 155,
	Skydive = 267
};

enum class eBossGoonFlags
{
	kOneOnOneDM = 4,
	kJoinSuccess = 7,
	kJoinFail = 8,
	kSpectating = 24
};

enum class eGoonInviteType
{
	DEBUG,
	NEARBY,
	FRIENDS,
	CREW,
	INDIVIDUAL,
	LOOKING_FOR_WORK
};

enum class eBossVehicleState
{
	NONE,
	SPAWNED,
	DESTROYED = 3
};

enum class eMCRole
{
	PROSPECT = -1,
	VICE_PRESIDENT,
	ROAD_CAPTAIN,
	SERGEANT_IN_ARMS,
	ENFORCER
};

enum class eClubhouseActivity
{
	NONE = -1,
	DARTS,
	ARM_WRESTLING
};