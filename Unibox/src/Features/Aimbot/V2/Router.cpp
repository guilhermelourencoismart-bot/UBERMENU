#include "Router.h"
#include "AimbotGlobal.h"
#include "AimbotHitscan.h"
#include "AimbotProjectile.h"
#include "AimbotMelee.h"
void CV2Router::Reset()
{
    F::V2Global=CV2AimbotGlobal{}; F::V2Hitscan=CV2AimbotHitscan{};
    F::V2Projectile=CV2AimbotProjectile{}; F::V2Melee=CV2AimbotMelee{};
    G::AimTarget={}; G::AimPoint={}; m_targetIndex=-1; m_targetToken=0;
}
void CV2Router::Update(CTFPlayer* local,CTFWeaponBase* weapon)
{
    if (m_backend!=Vars::Aimbot::General::Solver.Value) { Reset(); m_backend=Vars::Aimbot::General::Solver.Value; }
    SolverPolicy::Context next;
    next.valid=local && weapon && local->IsAlive() && I::EngineClient->IsInGame();
    if(next.valid)
    {
        next.owner=uint64_t(local->GetRefEHandle().ToInt());
        next.weapon=uint64_t(uint32_t(weapon->GetRefEHandle().ToInt())) | (uint64_t(uint32_t(weapon->GetWeaponID()))<<32);
        next.map=FNV1A::Hash32(I::EngineClient->GetLevelName()); next.tick=I::GlobalVars->tickcount;
    }
    if(m_context.Update(next)) Reset();
    if(!next.valid) return;
    const int index=G::AimTarget.m_iEntIndex;
    if(index<=0) return;
    auto entity=I::ClientEntityList->GetClientEntity(index);
    if(!entity) { Reset(); return; }
    const auto token=uint64_t(entity->GetRefEHandle().ToInt());
    if(SolverPolicy::RecycledTarget(m_targetIndex,m_targetToken,index,token)) Reset();
    m_targetIndex=index; m_targetToken=token;
}
bool CV2Router::Use(CTFWeaponBase* weapon) const
{
    if(!weapon) return false;
    const int id=weapon->GetWeaponID();
    const bool extension=id==TF_WEAPON_PASSTIME_GUN || id==TF_WEAPON_GRAPPLINGHOOK || id==TF_WEAPON_LASER_POINTER || id==TF_WEAPON_MECHANICAL_ARM;
    return SolverPolicy::UseV2(Vars::Aimbot::General::Solver.Value==Vars::Aimbot::General::SolverEnum::V2,extension);
}
void CV2Router::Run(int type,CTFPlayer* local,CTFWeaponBase* weapon,CUserCmd* command)
{
    switch(static_cast<EWeaponType>(type))
    {
    case EWeaponType::HITSCAN:F::V2Hitscan.Run(local,weapon,command);break;
    case EWeaponType::PROJECTILE:F::V2Projectile.Run(local,weapon,command);break;
    case EWeaponType::MELEE:F::V2Melee.Run(local,weapon,command);break;
    default:break;
    }
}
