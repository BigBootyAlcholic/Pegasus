#include "stdafx.hpp"
#include "../hooks.hpp"
#include "cheat/menu/submenus/player.hpp"

namespace Hooks {

	enum class eTaskFlags : uint32_t {

		// Control flags
		JF_ForceVault = (1 << 0),		// If there is a climb available, climbs up
		JF_DisableVault = (1 << 1),		// Disables transitions to vaulting
		JF_DisableJumpingIfNoClimb = (1 << 2),		// Prevent task from jumping if a climb cannot be found
		JF_AutoJump = (1 << 3),		// Indicates we are automatically jumping over an obstacle.
		JF_JumpingUpHill = (1 << 4),		// Jump is going uphill (to pick a different anim).
		JF_SteepHill = (1 << 5),		// Jump is very steep. 
		JF_JumpLaunchFootSync = (1 << 6),		// Do foot syncing (with motion anims) when launching
		JF_ScaleQuadrupedJump = (1 << 7),		// Need to scale jump velocity to get over obstacles.
		JF_UseCachedHandhold = (1 << 8),		// Use cached handhold, else we need to get a new one.
		JF_FromCover = (1 << 9),		// Triggered from cover.
		JF_ForceParachuteJump = (1 << 10),	// Force jump to be parachute version.
		JF_ForceDiveJump = (1 << 11),	// Force jump to be dive version.
		JF_ForceNMFall = (1 << 12),	// Force NM Fall behaviour.
		JF_ForceRunningLand = (1 << 13),	// Force running land for small running drops.
		JF_ForceRightFootLand = (1 << 14),	// Force a right foot land.
		JF_SuperJump = (1 << 15),	// Do a super jump.
		JF_ShortJump = (1 << 16),	// Used for shorter less significant jumps to get through small obstacles
		JF_BeastJump = (1 << 17),	// Used for shorter less significant jumps to get through small obstacles
		JF_AIUseFullForceBeastJump = (1 << 18),	// Make AI do a beast/super jump as if he was holding the jump button
	};

	u64* FallTaskCreateHook(u64* _this, u32 flags) {

		if (PlayerMenuVars::m_Vars.m_NoFallAnim) {
			flags |= (u32)eTaskFlags::JF_ForceParachuteJump;
		}

		if (PlayerMenuVars::m_Vars.m_GraceFullLanding) {
			flags |= (u32)eTaskFlags::JF_ForceParachuteJump;
		}

		return OgFallTaskCreateHook(_this, flags);
	}

	u64 JumpTaskCreateHook(u64 _this, u32 flags) {

		if (PlayerMenuVars::m_Vars.m_SuperJump) {
			flags |= 1 << 15;
		}

		if (PlayerMenuVars::m_Vars.m_BeastJump) {
			flags |= (1 << 15) | (1 << 17);
		}

		if (PlayerMenuVars::m_Vars.m_GraceFullLanding) {
			flags |= (u32)eTaskFlags::JF_ForceParachuteJump;
		}

		return OgJumpTaskCreateHook(_this, flags);
	}
}