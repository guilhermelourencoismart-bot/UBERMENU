// Complete V2 port, source ba07d0a9 SHA256 82cb18577502e7c443e16bd53a7aab576d2e37fa4f0dabf82e24cb684ca8b5f4
#pragma once
#include "../../../SDK/SDK.h"
#include "../../Backtrack/Backtrack.h"

#include "../AimbotGlobal/AimbotGlobal.h"

class CV2AimbotGlobal
{
public:
	std::vector<Target_t> ManageTargets(std::vector<Target_t>(*GetTargets)(CTFPlayer* pLocal, CTFWeaponBase* pWeapon), CTFPlayer* pLocal, CTFWeaponBase* pWeapon,
		int iMethod = Vars::Aimbot::General::TargetSelectionHitscan.Value, int iMaxTargets = Vars::Aimbot::General::MaxTargets.Value);
	void SortTargetsPre(std::vector<Target_t>& vTargets, int iMethod);
	void SortTargetsPost(std::vector<Target_t>& vTargets, int iMethod);

	bool PlayerBoneInFOV(CTFPlayer* pTarget, Vec3 vLocalPos, Vec3 vLocalAngles, float& flFOVTo, Vec3& vPos, Vec3& vAngleTo, int iHitboxes = Vars::Aimbot::Hitscan::HitboxesEnum::Head | Vars::Aimbot::Hitscan::HitboxesEnum::Body | Vars::Aimbot::Hitscan::HitboxesEnum::Pelvis | Vars::Aimbot::Hitscan::HitboxesEnum::Arms | Vars::Aimbot::Hitscan::HitboxesEnum::Legs);
	bool IsHitboxValid(CBaseEntity* pEntity, int nHitbox, int iHitboxes = Vars::Aimbot::Hitscan::HitboxesEnum::Head | Vars::Aimbot::Hitscan::HitboxesEnum::Body | Vars::Aimbot::Hitscan::HitboxesEnum::Pelvis | Vars::Aimbot::Hitscan::HitboxesEnum::Arms | Vars::Aimbot::Hitscan::HitboxesEnum::Legs);
	bool IsHitboxValid(int nHitbox, int iHitboxes = Vars::Aimbot::Projectile::HitboxesEnum::Head | Vars::Aimbot::Projectile::HitboxesEnum::Body | Vars::Aimbot::Projectile::HitboxesEnum::Feet);
	bool ShouldMultipoint(CBaseEntity* pEntity = nullptr, int nHitbox = -1, int iHitboxes = Vars::Aimbot::Hitscan::HitboxesEnum::Head | Vars::Aimbot::Hitscan::HitboxesEnum::Body | Vars::Aimbot::Hitscan::HitboxesEnum::Pelvis | Vars::Aimbot::Hitscan::HitboxesEnum::Arms | Vars::Aimbot::Hitscan::HitboxesEnum::Legs);

	bool ShouldIgnore(CBaseEntity* pTarget, CTFPlayer* pLocal, CTFWeaponBase* pWeapon, bool bIgnoreDormant = true);
	int GetPriority(int iIndex);
	bool FriendlyFire();

	bool ShouldAim();
	bool ShouldHoldAttack(CTFWeaponBase* pWeapon);
	bool ValidBomb(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CBaseEntity* pBomb);
};

ADD_FEATURE(CV2AimbotGlobal, V2Global);
