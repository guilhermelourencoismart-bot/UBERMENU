// Full Comp port, source 366fa0dc; see docs/THIRD-PARTY-NOTICES.md.
#include "CompDraw.h"
#include "CritHeals.h"

void CFullCritHeals::Draw()
{
    if (!Vars::Comp::Features::CritHeals.Value)
        return;

    // Early exits
    if (I::EngineVGui->IsGameUIVisible())
        return;

    auto pLocal = H::Entities.GetLocal();
    if (!pLocal || !pLocal->IsAlive())
        return;

    // Only run for medics
    if (pLocal->m_iClass() != TF_CLASS_MEDIC)
        return;

    // Handle uber build warning
    if (Vars::Comp::CritHeals::UberBuildWarning.Value)
        DrawUberBuildWarning();

    Vec3 localPos = pLocal->GetAbsOrigin();
    float currentTime = I::GlobalVars->curtime;

    // Process all players for crit heal indicators
    for (auto pEntity : H::Entities.GetGroup(EntityEnum::PlayerAll))
    {
        if(!pEntity)continue;
        auto pPlayer = pEntity->As<CTFPlayer>();
        if (!pPlayer || !pPlayer->IsAlive() || pPlayer->IsDormant() || pPlayer == pLocal)
            continue;

        bool isEnemy = (pPlayer->m_iTeamNum() != pLocal->m_iTeamNum());

        // Skip enemies if feature is disabled
        if (isEnemy && !Vars::Comp::CritHeals::ShowOnEnemies.Value)
            continue;

        Vec3 playerPos = pPlayer->GetAbsOrigin();

        // Distance check only for teammates
        if (!isEnemy)
        {
            Vec3 delta = playerPos - localPos;
            float dist = delta.Length();
            if (dist > Vars::Comp::CritHeals::Range.Value)
                continue;
        }

        // Visibility check
        if (!IsPlayerVisible(pPlayer, pLocal))
            continue;

        // Get player's health info
        int health = pPlayer->m_iHealth();
        int baseMaxHealth = pPlayer->GetMaxHealth();

        if (health <= 0 || baseMaxHealth <= 0)
            continue;

        int maxBuffedHealth = (int)(baseMaxHealth * Vars::Comp::CritHeals::MaxOverhealMultiplier.Value);
        int bufferAmount = maxBuffedHealth - baseMaxHealth;
        int currentBuffAmount = health - baseMaxHealth;

        // Track when player was last well-buffed
        int playerIndex = pPlayer->entindex();
        if (currentBuffAmount > (bufferAmount * Vars::Comp::CritHeals::BuffThreshold.Value))
            m_LastBuffTimes[playerIndex] = currentTime;

        // Determine if player should be considered still buffed
        bool wasRecentlyBuffed = false;
        if (m_LastBuffTimes.find(playerIndex) != m_LastBuffTimes.end())
        {
            float lastBuffTime = m_LastBuffTimes[playerIndex];
            if (lastBuffTime > 0 && currentBuffAmount > (bufferAmount * Vars::Comp::CritHeals::BuffFadeThreshold.Value))
                wasRecentlyBuffed = true;
        }

        // Check crit heal eligibility
        float lastDamageTime = 0.0f;
        if (m_LastDamageTimes.find(playerIndex) != m_LastDamageTimes.end())
            lastDamageTime = m_LastDamageTimes[playerIndex];

        float timeSinceLastDamage = currentTime - lastDamageTime;

        if (timeSinceLastDamage >= Vars::Comp::CritHeals::CritHealTime.Value &&
            health < maxBuffedHealth &&
            !wasRecentlyBuffed)
        {
            // Convert player position to screen coordinates and add vertical offset
            Vec3 headPos = playerPos;
            headPos.z += Vars::Comp::CritHeals::TriangleVerticalOffset.Value;

            Vec3 screenPos;
            if (SDK::W2S(headPos, screenPos))
            {
                // Allow some off-screen drawing for close-range players
                if (screenPos.x >= -100 && screenPos.x <= H::Draw.m_nScreenW + 100 &&
                    screenPos.y >= -100 && screenPos.y <= H::Draw.m_nScreenH + 100)
                {
                    DrawTriangle((int)screenPos.x, (int)screenPos.y, isEnemy);
                }
            }
        }
    }
}

void CFullCritHeals::DrawTriangle(int x, int y, bool isEnemy)
{
    int centerX = x;
    int centerY = y;

    // Calculate sizes for outer and inner triangles
    int outerSize = Vars::Comp::CritHeals::TriangleSize.Value;
    int innerSize = Vars::Comp::CritHeals::TriangleSize.Value - Vars::Comp::CritHeals::OutlineThickness.Value;

    // Draw the black outline triangle first
    for (int i = 0; i <= outerSize; i++)
    {
        int width = (outerSize - i) * 2;
        int xPos = centerX - width / 2;
        int yPos = centerY - outerSize + i;
        F::CompDraw.FillRect(xPos, yPos, width, 1, Vars::Comp::CritHeals::OutlineColor.Value);
    }

    // Draw the colored inner triangle
    Color_t color = isEnemy ? Vars::Comp::CritHeals::EnemyColor.Value : Vars::Comp::CritHeals::FriendColor.Value;

    // Calculate offset to center the inner triangle
    int offsetY = (outerSize - innerSize) / 2;

    for (int i = 0; i <= innerSize; i++)
    {
        int width = (innerSize - i) * 2;
        int xPos = centerX - width / 2;
        int yPos = centerY - outerSize + offsetY + i;
        F::CompDraw.FillRect(xPos, yPos, width, 1, color);
    }
}

void CFullCritHeals::DrawUberBuildWarning()
{
    auto pLocal = H::Entities.GetLocal();
    if (!pLocal)
        return;

    auto pWeapon = pLocal->m_hActiveWeapon().Get();
    if (!pWeapon)
        return;

    auto pWeaponBase = pWeapon->As<CTFWeaponBase>();
    if (!pWeaponBase || pWeaponBase->GetWeaponID() != TF_WEAPON_MEDIGUN)
        return;

    auto pMedigun = pWeapon->As<CWeaponMedigun>();
    if (!pMedigun)
        return;

    auto pHealTarget = pMedigun->m_hHealingTarget().Get();
    if (!pHealTarget)
        return;

    auto pTargetPlayer = pHealTarget->As<CTFPlayer>();
    if (!pTargetPlayer || !pTargetPlayer->IsAlive())
        return;

    int health = pTargetPlayer->m_iHealth();
    int baseMaxHealth = pTargetPlayer->GetMaxHealth();

    if (health <= 0 || baseMaxHealth <= 0)
        return;

    float healthRatio = (float)health / (float)baseMaxHealth;

    if (healthRatio >= Vars::Comp::CritHeals::UberPenaltyThreshold.Value)
    {
        // Position on same line as KRITZ text but centered (matching UberTracker positioning)
        int screenW, screenH;
        I::MatSystemSurface->GetScreenSize(screenW, screenH);

        int x = screenW / 2;  // Center horizontally
        int y = (screenH / 2) + 250 - 25;  // Same Y as KRITZ text

        const char* message = "Reduced Uber Build Rate!";
        auto font = H::Fonts.GetFont(FONT_ESP);

        F::CompDraw.String(font, x, y, Vars::Comp::CritHeals::WarningColor.Value, ALIGN_CENTER, message);
    }
}

bool CFullCritHeals::IsPlayerVisible(CTFPlayer* pPlayer, CTFPlayer* pLocal)
{
    if (!pPlayer || !pLocal)
        return false;

    Vec3 eyePos = pLocal->GetAbsOrigin() + pLocal->m_vecViewOffset();
    Vec3 targetPos = pPlayer->GetAbsOrigin();

    bool isTeammate = (pPlayer->m_iTeamNum() == pLocal->m_iTeamNum());

    // Check distance first - if very close, skip trace check for teammates
    float distance = (targetPos - eyePos).Length();
    if (isTeammate && distance < 150.0f)
    {
        // At very close range, always show teammates to prevent disappearing
        return true;
    }

    CGameTrace trace = {};
    CTraceFilterHitscan filter = {};
    filter.m_pSkip = pLocal;

    SDK::Trace(eyePos, targetPos, MASK_VISIBLE, &filter, &trace);

    // Use more lenient trace fraction for close range
    float requiredFraction = (distance < 300.0f) ? 0.75f : 0.90f;

    return isTeammate ? (trace.fraction > requiredFraction) : (trace.fraction > requiredFraction || trace.m_pEnt == pPlayer);
}

void CFullCritHeals::OnPlayerHurt(int victimIndex)
{
    m_LastDamageTimes[victimIndex] = I::GlobalVars->curtime;
}
