// Ported from Amalgam-Comp 366fa0dc; original SHA256 399567ba3fbe9be2bda9056c237e83d2c3823a7c1e9d499eb003902a7133f840.
#include "CompDraw.h"
#include "PylonESP.h"
#include "../../../SDK/SDK.h"

CFullPylonESP::CFullPylonESP()
{
    // Defer settings access until capture, after static initialization.

}

void CFullPylonESP::InitializeAlphaSteps()
{
    int segments = std::clamp(Vars::Comp::PylonESP::Segments.Value, 1, 128);
    Vars::Comp::PylonESP::Segments.Value = segments;
    m_AlphaSteps.clear();
    m_AlphaSteps.reserve(segments + 1);
    for (int i = 0; i <= segments; i++)
    {
        float progress = static_cast<float>(i) / segments;
        int alpha = static_cast<int>(Vars::Comp::PylonESP::StartAlpha.Value - (progress * (Vars::Comp::PylonESP::StartAlpha.Value - Vars::Comp::PylonESP::EndAlpha.Value)));
        m_AlphaSteps.push_back(alpha);
    }
}

void CFullPylonESP::Draw()
{
    if (!Vars::Comp::Features::PylonESP.Value)
        return;
    
    // Early exits
    if (I::EngineVGui->IsGameUIVisible())
        return;
    
    auto pLocal = H::Entities.GetLocal();
    if (!pLocal)
        return;
    
    InitializeAlphaSteps();
    // Cache frequently used values
    Vec3 localPos = pLocal->GetAbsOrigin();
    Vec3 eyePos = pLocal->GetEyePosition();
    int localTeam = pLocal->m_iTeamNum();
    
    // Process all players in real-time
    for (auto pEntity : H::Entities.GetGroup(EntityEnum::PlayerAll))
    {
        auto pPlayer = pEntity->As<CTFPlayer>();
        if (!pPlayer || !pPlayer->IsAlive() || pPlayer->IsDormant())
            continue;
        
        // Only track enemy medics
        if (pPlayer->m_iTeamNum() == localTeam || pPlayer->m_iClass() != TF_CLASS_MEDIC)
            continue;
        
        Vec3 playerPos = pPlayer->GetAbsOrigin();
        
        // Skip if we can see the medic directly
        if (IsPlayerDirectlyVisible(pPlayer, eyePos))
            continue;
        
        // Skip if we're too close to the medic
        float distanceSqr = GetDistanceSqr(localPos, playerPos);
        if (distanceSqr < (Vars::Comp::PylonESP::MinDistance.Value * Vars::Comp::PylonESP::MinDistance.Value))
            continue;
        
        // Calculate pylon base position (real-time)
        Vec3 playerMaxs = pPlayer->m_vecMaxs();
        Vec3 pylonBase = Vec3(playerPos.x, playerPos.y, playerPos.z + playerMaxs.z + Vars::Comp::PylonESP::PylonOffset.Value);
        
        // Draw pylon immediately
        DrawPylon(pylonBase, eyePos);
    }
}

bool CFullPylonESP::IsVisible(const Vec3& fromPos, const Vec3& targetPos)
{
    CGameTrace trace;
    CTraceFilterHitscan filter;
    filter.m_pSkip = H::Entities.GetLocal();
    SDK::Trace(fromPos, targetPos, MASK_VISIBLE, &filter, &trace);
    
    return trace.fraction >= 0.99f;
}

float CFullPylonESP::GetDistanceSqr(const Vec3& pos1, const Vec3& pos2)
{
    Vec3 delta = pos2 - pos1;
    return delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
}

bool CFullPylonESP::IsPlayerDirectlyVisible(CTFPlayer* pPlayer, const Vec3& eyePos)
{
    Vec3 playerPos = pPlayer->GetAbsOrigin();
    return IsVisible(eyePos, playerPos);
}

void CFullPylonESP::DrawPylon(const Vec3& basePosition, const Vec3& eyePos)
{
    // First pass: check if any segment is visible
    bool anySegmentVisible = false;
    for (int i = 0; i <= Vars::Comp::PylonESP::Segments.Value; i++)
    {
        Vec3 segmentPos = Vec3(basePosition.x, basePosition.y, basePosition.z + (i * (Vars::Comp::PylonESP::PylonHeight.Value / Vars::Comp::PylonESP::Segments.Value)));
        
        if (IsVisible(eyePos, segmentPos))
        {
            anySegmentVisible = true;
            break;
        }
    }
    
    // Skip drawing if no segments are visible
    if (!anySegmentVisible)
        return;
    
    // Second pass: draw visible segments
    Vec3 lastScreenPos;
    bool hasLastScreenPos = false;
    
    for (int i = 0; i <= Vars::Comp::PylonESP::Segments.Value; i++)
    {
        Vec3 segmentPos = Vec3(basePosition.x, basePosition.y, basePosition.z + (i * (Vars::Comp::PylonESP::PylonHeight.Value / Vars::Comp::PylonESP::Segments.Value)));
        
        bool visible = IsVisible(eyePos, segmentPos);
        
        if (!visible)
        {
            hasLastScreenPos = false;
            continue;
        }
        
        Vec3 screenPos;
        if (!SDK::W2S(segmentPos, screenPos))
        {
            hasLastScreenPos = false;
            continue;
        }
        
        if (hasLastScreenPos)
        {
            // Draw lines with fading alpha
            Color_t segmentColor = Vars::Comp::PylonESP::PylonColor.Value;
            segmentColor.a = static_cast<byte>(m_AlphaSteps[i]);
            
            for (int w = 0; w < Vars::Comp::PylonESP::PylonWidth.Value; w++)
            {
                F::CompDraw.Line(static_cast<int>(lastScreenPos.x + w), static_cast<int>(lastScreenPos.y),
                           static_cast<int>(screenPos.x + w), static_cast<int>(screenPos.y), segmentColor);
            }
        }
        
        lastScreenPos = screenPos;
        hasLastScreenPos = true;
    }
}