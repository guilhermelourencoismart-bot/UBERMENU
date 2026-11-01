#include "CompVisuals.h"
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
    m_serials.clear(); m_time=-1.f;
    F::CompDraw.Begin(); F::CompDraw.Publish();
}
void CCompVisuals::Capture(CTFPlayer* local)
{
    if (!local || !I::EngineClient->IsInGame()) { Reset(); return; }
    const std::string map=I::EngineClient->GetLevelName();
    const float now=I::GlobalVars->curtime;
    bool recycled=false; std::unordered_map<int,int> serials;
    for (auto entity:H::Entities.GetGroup(EntityEnum::PlayerAll))
    {
        if (!entity) continue;
        int index=entity->entindex(), serial=entity->GetRefEHandle().GetSerialNumber();
        if (auto it=m_serials.find(index); it!=m_serials.end() && it->second!=serial) recycled=true;
        serials[index]=serial;
    }
    if (map!=m_map || now<m_time || recycled) Reset();
    m_serials=std::move(serials); m_map=map; m_time=now;
    F::CompDraw.Begin();
#ifndef TEXTMODE
    if (!SDK::TakingScreenshot())
    {
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
        F::FullFocusFire.OnPlayerHurt(I::EngineClient->GetPlayerForUserID(event->GetInt("userid")),I::EngineClient->GetPlayerForUserID(event->GetInt("attacker")));
    else if (hash==FNV1A::Hash32Const("game_newmap") || hash==FNV1A::Hash32Const("client_disconnect") || hash==FNV1A::Hash32Const("teamplay_round_start")) Reset();
}
