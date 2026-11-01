// Complete V2 port, source ba07d0a9 SHA256 81aaca14ca786055c26966ac5de5ea97c3848f6a36290f4aed2f780e81eb55b4
#pragma once
#include "../../../SDK/SDK.h"

#include "AimbotGlobal.h"

class CV2AimbotHitscan
{
private:
	int GetHitboxPriority(int nHitbox, CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CBaseEntity* pTarget);
	int CanHit(Target_t& tTarget, CTFPlayer* pLocal, CTFWeaponBase* pWeapon);

	bool Aim(Vec3 vCurAngle, Vec3 vToAngle, Vec3& vOut, int iMethod = Vars::Aimbot::General::AimType.Value);
	void Aim(CUserCmd* pCmd, Vec3& vAngle, int iMethod = Vars::Aimbot::General::AimType.Value);
	bool ShouldFire(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd, const Target_t& tTarget);

	Vec3 m_vEyePos = {};

	float m_flShootDelayEnd = 0.f;
	int m_iDelayTargetIdx = -1;

public:
	void Run(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd);
};

ADD_FEATURE(CV2AimbotHitscan, V2Hitscan);
