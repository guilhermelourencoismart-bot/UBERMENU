#include "CompVisuals.h"
#include "MainESP.h"
#include "CraterCheck.h"
#include "CritHeals.h"
#include "SplashRadius.h"

#include "Radar.h"
#include "OffScreenIndicators.h"
#include "CompEntities.h"
#include "UberTracker.h"
#include "MedicUberBar.h"
#include "PylonESP.h"
#include "FocusFire.h"
#include "HiderESP.h"
#include "HealthBarESP.h"
#include "PlayerTrails.h"
#include "AmmoTracker.h"
#include "SentryESP.h"
#include "StickyESP.h"

void CCompVisuals::Reset()
{
    F::FullUberTracker=CFullUberTracker{}; F::FullMedicUberBar=CFullMedicUberBar{};
    F::FullPylonESP=CFullPylonESP{}; F::FullFocusFire=CFullFocusFire{}; F::FullHiderESP=CFullHiderESP{};
    F::FullHealthBarESP=CFullHealthBarESP{};
    F::FullPlayerTrails=CFullPlayerTrails{};
    F::FullAmmoTracker=CFullAmmoTracker{};
    F::FullSentryESP=CFullSentryESP{};
    F::FullStickyESP=CFullStickyESP{};
    F::FullMainESP=CFullMainESP{}; F::FullOffScreenIndicators=CFullOffScreenIndicators{};
    F::FullCraterCheck=CFullCraterCheck{}; F::FullCritHeals=CFullCritHeals{}; F::FullSplashRadius=CFullSplashRadius{};
    m_serials.clear(); m_time=-1.f;
    F::CompDraw.Begin(); F::CompDraw.Publish();
}
void CCompVisuals::Capture(CTFPlayer* local)
{
    if (!local || !I::EngineClient->IsInGame()) { Reset(); return; }
    const std::string map=I::EngineClient->GetLevelName();
    const float now=I::GlobalVars->curtime;
    if(map!=m_map || now<m_time)Reset();
    const auto& players=H::Entities.GetGroup(EntityEnum::PlayerAll);
    for(auto entity:players)
    {
        if(!entity)continue;
        auto old=m_serials.find(entity->entindex());
        if(old!=m_serials.end() && old->second!=entity->GetRefEHandle().GetSerialNumber()){Reset();break;}
    }
    // Retain identities across temporary absence, bounded by player slots.
    for(auto entity:players)if(entity)m_serials[entity->entindex()]=entity->GetRefEHandle().GetSerialNumber();
    m_map=map;m_time=now;
    F::CompEntities.Refresh();
    F::CompDraw.Begin();
#ifndef TEXTMODE
    if (!SDK::TakingScreenshot())
    {
        if(Vars::Comp::ESP::Enabled.Value) { F::FullMainESP.Store(local); F::FullMainESP.Draw(); }
        F::FullCraterCheck.Draw(); F::FullCritHeals.Draw(); F::FullSplashRadius.Draw();
        F::FullRadar.Run(local);
        F::FullOffScreenIndicators.Draw();
        F::FullUberTracker.Draw(); F::FullMedicUberBar.Draw(); F::FullPylonESP.Draw();
        F::FullFocusFire.Draw(); F::FullHiderESP.Draw();
        F::FullHealthBarESP.Draw();
        F::FullPlayerTrails.Draw();
        F::FullAmmoTracker.Draw();
        F::FullSentryESP.Draw();
        F::FullStickyESP.Draw();

    }
#endif
    F::CompDraw.Publish();
}
void CCompVisuals::Event(IGameEvent* event,uint32_t hash)
{
    if (!event) return;
    F::FullAmmoTracker.Event(event,hash);
    if (hash==FNV1A::Hash32Const("player_hurt"))
        {
        F::FullCritHeals.OnPlayerHurt(I::EngineClient->GetPlayerForUserID(event->GetInt("userid")));
        F::FullFocusFire.OnPlayerHurt(I::EngineClient->GetPlayerForUserID(event->GetInt("userid")),I::EngineClient->GetPlayerForUserID(event->GetInt("attacker")));
    }
    else if (hash==FNV1A::Hash32Const("game_newmap") || hash==FNV1A::Hash32Const("client_disconnect") || hash==FNV1A::Hash32Const("teamplay_round_start")) Reset();
}
