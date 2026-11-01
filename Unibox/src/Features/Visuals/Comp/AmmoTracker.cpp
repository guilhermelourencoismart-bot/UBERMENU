// Complete Comp port, source 366fa0dc, SHA256 67bf7a4dc8260ab0e75736782d8628c8209de645471f958fa9648cd2f6b22329.
#include "CompDraw.h"
#include "../Groups/Groups.h"
#include "AmmoTracker.h"
#include "../../../SDK/SDK.h"
#include <algorithm>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


std::string CFullAmmoTracker::GetPickupType(CBaseEntity* pEntity)
{
    if (!pEntity)
        return "";
    
    const char* pModelName = I::ModelInfoClient->GetModelName(pEntity->GetModel());
    if (!pModelName)
        return "";
    
    std::string sModelName = pModelName;
    if (sModelName.empty())
        return "";
    
    // Convert to lowercase for comparison
    std::transform(sModelName.begin(), sModelName.end(), sModelName.begin(), ::tolower);
    
    // Check cache first
    if (m_ModelCache.find(sModelName) != m_ModelCache.end())
        return m_ModelCache[sModelName];
    
    std::string sType = "";
    if (sModelName.find("ammopack") != std::string::npos)
        sType = "ammo";
    else if (sModelName.find("medkit") != std::string::npos || sModelName.find("healthkit") != std::string::npos)
        sType = "health";
    
    // Cache result
    m_ModelCache[sModelName] = sType;
    return sType;
}

std::string CFullAmmoTracker::GetPositionKey(const Vec3& vPos)
{
    return std::format("{:.0f}_{:.0f}_{:.0f}", vPos.x, vPos.y, vPos.z);
}

bool CFullAmmoTracker::IsStationaryPickup(CBaseEntity* pEntity)
{
    if (!pEntity)
        return false;
    
    // Method 1: Check entity class - exclude dropped weapons entirely
    auto classID = pEntity->GetClassID();
    
    // Exclude dropped weapon classes
    if (classID == ETFClassID::CTFDroppedWeapon)
        return false;
    
    // Include Halloween pickup classes that we know exist
    if (classID == ETFClassID::CHalloweenPickup ||
        classID == ETFClassID::CHalloweenSoulPack ||
        classID == ETFClassID::CHalloweenGiftPickup)
    {
        return true; // These are always valid stationary pickups
    }
    
    // Method 2: Check the model name for filtering
    const char* pModelName = I::ModelInfoClient->GetModelName(pEntity->GetModel());
    if (!pModelName)
        return false;
    
    std::string modelName = pModelName;
    std::transform(modelName.begin(), modelName.end(), modelName.begin(), ::tolower);
    
    // Exclude dropped weapons/packs - these are temporary
    if (modelName.find("dropped") != std::string::npos ||
        modelName.find("w_") != std::string::npos ||  // Weapon models typically start with w_
        modelName.find("thrown") != std::string::npos ||
        modelName.find("weapon") != std::string::npos ||
        modelName.find("backpack") != std::string::npos) // Dropped backpacks
    {
        return false;
    }
    
    // Method 3: Check movement type (dropped items usually have physics)
    auto movetype = pEntity->m_MoveType();
    if (movetype == MOVETYPE_VPHYSICS || movetype == MOVETYPE_PUSH)
        return false; // These are usually dropped/physics items
    
    // Method 4: Only include items that are in the items/ folder (stationary map pickups)
    if (modelName.find("items/") == std::string::npos)
        return false;
    
    // Method 5: Check if it's a proper stationary pickup model
    bool isStationaryModel = (modelName.find("medkit") != std::string::npos ||
                             modelName.find("healthkit") != std::string::npos ||
                             modelName.find("ammopack") != std::string::npos ||
                             modelName.find("health") != std::string::npos ||
                             modelName.find("ammo") != std::string::npos);
    
    return isStationaryModel;
}

bool CFullAmmoTracker::IsVisible(const Vec3& vPos)
{
    auto pLocal = H::Entities.GetLocal();
    if (!pLocal)
        return false;
    
    std::string sKey = GetPositionKey(vPos);
    int iCurrentTick = I::GlobalVars->tickcount;
    
    // Check cache first
    if (m_NextVisCheck.find(sKey) != m_NextVisCheck.end() && iCurrentTick < m_NextVisCheck[sKey])
        return m_VisibilityCache[sKey];
    
    // Get eye position
    Vec3 vEyePos = pLocal->GetAbsOrigin() + pLocal->As<CTFPlayer>()->GetViewOffset();
    
    // Perform trace
    CGameTrace trace = {};
    CTraceFilterHitscan filter = {};
    filter.m_pSkip = pLocal;
    
    SDK::Trace(vEyePos, vPos, MASK_VISIBLE, &filter, &trace);
    
    bool bVisible = trace.fraction > 0.99f;
    
    // Cache result
    m_VisibilityCache[sKey] = bVisible;
    m_NextVisCheck[sKey] = iCurrentTick + 3;
    
    return bVisible;
}

void CFullAmmoTracker::UpdateSupplyPositions()
{
    int iCurrentTick = I::GlobalVars->tickcount;
    if (iCurrentTick - m_LastUpdateTick < UPDATE_INTERVAL)
        return;
    
    m_LastUpdateTick = iCurrentTick;
    
    std::unordered_map<std::string, bool> vCurrentSupplies;
    float fCurrentTime = I::GlobalVars->curtime;
    
    // Find all current pickups
    std::vector<int> vPickupGroups = {EntityEnum::PickupHealth, EntityEnum::PickupAmmo};
    
    for (auto eGroupType : vPickupGroups)
    {
        for (auto pEntity : H::Entities.GetGroup(eGroupType))
        {
            if (!pEntity)
                continue;
            
            // Only skip dormant entities if "show through walls" is disabled
            if (pEntity->IsDormant() && !Vars::Comp::AmmoTracker::ShowThroughWalls.Value)
                continue;
                
            // Skip non-stationary (moving/dropped) pickups
            if (!IsStationaryPickup(pEntity))
                continue;
                
            std::string sType = GetPickupType(pEntity);
            if (sType.empty())
                continue;
                
            Vec3 vPos = pEntity->GetAbsOrigin();
            std::string sKey = GetPositionKey(vPos);
            vCurrentSupplies[sKey] = true;
            
            // Add new supply if not tracked (only add if currently visible - not respawning)
            if (m_SupplyPositions.find(sKey) == m_SupplyPositions.end())
            {
                SupplyInfo info;
                info.Position = vPos;
                info.Type = sType;
                info.Respawning = false;  // New supplies start as available
                info.DisappearTime = 0.0f;
                m_SupplyPositions[sKey] = info;
            }
            else
            {
                // Update existing supply - if we can see it and it was respawning, it's now available
                SupplyInfo& info = m_SupplyPositions[sKey];
                if (info.Respawning)
                {
                    info.Respawning = false;
                    info.DisappearTime = 0.0f;
                }
            }
        }
    }
    
    // Clean up supplies that have finished respawning and expired
    for (auto it = m_SupplyPositions.begin(); it != m_SupplyPositions.end();)
    {
        SupplyInfo& info = it->second;
        
        // Remove supplies that have been respawning for longer than respawn time + buffer
        if (info.Respawning && (fCurrentTime - info.DisappearTime) > (RESPAWN_TIME + 5.0f))
        {
            it = m_SupplyPositions.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

int CFullAmmoTracker::GetScreenChartSize(float flDistance)
{
    if (!Vars::Comp::AmmoTracker::ScaleWithDistance.Value)
        return Vars::Comp::AmmoTracker::PieChartSize.Value;
    
    float flScale = 1.0f - std::min(flDistance / Vars::Comp::AmmoTracker::MaxScaleDistance.Value, 1.0f);
    int iSize = Vars::Comp::AmmoTracker::MinScreenChartSize.Value + 
                static_cast<int>((Vars::Comp::AmmoTracker::MaxScreenChartSize.Value - Vars::Comp::AmmoTracker::MinScreenChartSize.Value) * flScale);
    return std::max(Vars::Comp::AmmoTracker::MinScreenChartSize.Value, 
                    std::min(Vars::Comp::AmmoTracker::MaxScreenChartSize.Value, iSize));
}

int CFullAmmoTracker::GetTextSize(float flDistance)
{
    if (!Vars::Comp::AmmoTracker::ScaleTextWithDistance.Value)
        return Vars::Comp::AmmoTracker::TextSizeDefault.Value;
    
    float flScale = 1.0f - std::min(flDistance / Vars::Comp::AmmoTracker::MaxScaleDistance.Value, 1.0f);
    int iResult = Vars::Comp::AmmoTracker::TextSizeMin.Value + 
                  static_cast<int>((Vars::Comp::AmmoTracker::TextSizeMax.Value - Vars::Comp::AmmoTracker::TextSizeMin.Value) * flScale);
    return std::max(Vars::Comp::AmmoTracker::TextSizeMin.Value,
                    std::min(Vars::Comp::AmmoTracker::TextSizeMax.Value, iResult));
}

bool CFullAmmoTracker::IsPlayerNearSupply(const Vec3& vPlayerPos, const Vec3& vSupplyPos, float flMaxDistance)
{
    float fDistance = vPlayerPos.DistTo(vSupplyPos);
    return fDistance <= flMaxDistance;
}

bool CFullAmmoTracker::IsAnyPlayerNearSupply(const Vec3& vSupplyPos, float flMaxDistance)
{
    // Check if any player (alive, on any team) is near the supply position
    for (auto pEntity : H::Entities.GetGroup(EntityEnum::PlayerAll))
    {
        auto pPlayer = pEntity->As<CTFPlayer>();
        if (!pPlayer || !pPlayer->IsAlive() || pPlayer->IsDormant())
            continue;
            
        Vec3 vPlayerPos = pPlayer->GetAbsOrigin();
        if (IsPlayerNearSupply(vPlayerPos, vSupplyPos, flMaxDistance))
            return true;
    }
    return false;
}

void CFullAmmoTracker::DrawHudOverlay(const Vec3& vPlayerPos)
{
    if (!Vars::Comp::AmmoTracker::ShowHudOverlay.Value)
        return;
    
    // Get screen dimensions
    int iScreenW, iScreenH;
    I::MatSystemSurface->GetScreenSize(iScreenW, iScreenH);
    
    // HUD overlay settings
    const int iHudCircleSize = 30;
    const int iHudSpacing = 10;
    const int iMarginFromEdge = 20;
    
    std::vector<std::pair<SupplyInfo, float>> vNearbySupplies;
    float fCurrentTime = I::GlobalVars->curtime;
    
    // Find supplies the player is standing on
    for (const auto& [sKey, info] : m_SupplyPositions)
    {
        if (!info.Respawning)
            continue;
            
        if (IsPlayerNearSupply(vPlayerPos, info.Position))
        {
            float fTimeLeft = std::max(0.0f, RESPAWN_TIME - (fCurrentTime - info.DisappearTime));
            vNearbySupplies.push_back({info, fTimeLeft});
        }
    }
    
    if (vNearbySupplies.empty())
        return;
    
    // Calculate starting position (bottom right, with spacing between multiple items)
    int iStartX = iScreenW - iMarginFromEdge - iHudCircleSize;
    int iStartY = iScreenH - iMarginFromEdge - iHudCircleSize;
    
    // Draw each nearby supply
    for (size_t i = 0; i < vNearbySupplies.size(); i++)
    {
        const auto& [info, fTimeLeft] = vNearbySupplies[i];
        
        // Calculate position for this circle (offset left for multiple items)
        int iCircleX = iStartX - (static_cast<int>(i) * (iHudCircleSize * 2 + iHudSpacing)) + iHudCircleSize;
        int iCircleY = iStartY + iHudCircleSize;
        
        // Get colors
        Color_t color = (info.Type == "health") ? 
            Vars::Comp::AmmoTracker::HealthColor.Value : 
            Vars::Comp::AmmoTracker::AmmoColor.Value;
            
        // Calculate percentage
        float fTimePercent = fTimeLeft / RESPAWN_TIME;
        
        // Draw filled circle background
        Color_t bgColor = {0, 0, 0, 150};
        F::CompDraw.FillCircle(iCircleX, iCircleY, static_cast<float>(iHudCircleSize), 32, bgColor);
        
        // Draw progress pie chart
        DrawClockFill(iCircleX, iCircleY, iHudCircleSize - 2, fTimePercent, 32, color);
        
        // Draw timer text in center
        std::string sTimerText;
        if (Vars::Comp::AmmoTracker::ShowMilliseconds.Value)
            sTimerText = std::format("{:.1f}", fTimeLeft);
        else
            sTimerText = std::format("{}", static_cast<int>(std::ceil(fTimeLeft)));
        
        auto pFont = H::Fonts.GetFont(FONT_ESP);
        Vec2 vTextSize = F::CompDraw.GetTextSize(sTimerText.c_str(), pFont);
        
        F::CompDraw.String(pFont, 
                      iCircleX - static_cast<int>(vTextSize.x / 2), iCircleY - static_cast<int>(vTextSize.y / 2), 
                      {255, 255, 255, 255}, ALIGN_CENTER, sTimerText.c_str());
    }
}

void CFullAmmoTracker::DrawClockFill(int iCenterX, int iCenterY, int iRadius, float flPercentage, int iVertices, const Color_t& color)
{
    if (flPercentage <= 0.0f || flPercentage > 1.0f)
        return;
    
    // First draw a background circle (optional - creates border effect)
    Color_t bgColor = {0, 0, 0, 100};
    F::CompDraw.FillCircle(iCenterX, iCenterY, static_cast<float>(iRadius), iVertices, bgColor);
    
    // Create vertices for the pie slice
    std::vector<Vertex_t> vertices;
    
    // Add center point  
    vertices.emplace_back(Vertex_t({ { static_cast<float>(iCenterX), static_cast<float>(iCenterY) } }));
    
    // Calculate pie slice
    float flStartAngle = -90.0f * static_cast<float>(M_PI) / 180.0f; // Start at top
    float flEndAngle = flStartAngle + (2.0f * static_cast<float>(M_PI) * flPercentage);
    float flStep = (flEndAngle - flStartAngle) / iVertices;
    
    // Add edge vertices
    for (int i = 0; i <= iVertices; i++)
    {
        float flAngle = flStartAngle + (i * flStep);
        float fX = iCenterX + cos(flAngle) * iRadius;
        float fY = iCenterY + sin(flAngle) * iRadius;
        vertices.emplace_back(Vertex_t({ { fX, fY } }));
    }
    
    // Draw filled polygon
    if (vertices.size() >= 3)
        F::CompDraw.FillPolygon(vertices, color);
}

void CFullAmmoTracker::DrawFilledProgress(const Vec3& vPos, int iRadius, float flPercentage, int iSegments, const Color_t& color)
{
    Vec3 vCenterScreen;
    Vec3 vCenterWorld = {vPos.x, vPos.y, vPos.z + 1.0f};
    if (!SDK::W2S(vCenterWorld, vCenterScreen))
        return;

    int iVisibleVerts = static_cast<int>(std::floor((1.0f - flPercentage) * iSegments));
    if (iVisibleVerts <= 0)
        return;

    // Create vertices for the filled ground pie chart
    std::vector<Vertex_t> vertices;
    
    // Add center point
    vertices.emplace_back(Vertex_t({ { vCenterScreen.x, vCenterScreen.y } }));
    
    // Add edge vertices for the visible portion
    for (int i = 0; i <= iVisibleVerts; i++)
    {
        float fAngle = i * (2.0f * M_PI / iSegments);
        
        Vec3 vWorldPos = {
            vPos.x + iRadius * cos(fAngle),
            vPos.y + iRadius * sin(fAngle),
            vPos.z + 1.0f
        };
        
        Vec3 vScreenPos;
        if (SDK::W2S(vWorldPos, vScreenPos))
        {
            vertices.emplace_back(Vertex_t({ { vScreenPos.x, vScreenPos.y } }));
        }
        else
        {
            return; // Can't draw if any point is off-screen
        }
    }
    
    // Draw filled polygon
    if (vertices.size() >= 3)
        F::CompDraw.FillPolygon(vertices, color);
}

void CFullAmmoTracker::Draw()
{
    if (!Vars::Comp::Features::AmmoTracker.Value)
        return;
        
    if (I::EngineVGui->IsGameUIVisible())
        return;
    
    auto pLocal = H::Entities.GetLocal();
    if (!pLocal || !pLocal->IsAlive())
        return;
    
    UpdateSupplyPositions();
    
    float fCurrentTime = I::GlobalVars->curtime;
    Vec3 vPlayerPos = pLocal->GetAbsOrigin();
    
    // Draw HUD overlay for supplies the player is standing on
    DrawHudOverlay(vPlayerPos);
    
    for (const auto& [sKey, info] : m_SupplyPositions)
    {
        if (!info.Respawning)
            continue;
            
        // Check visibility if "Show through walls" is disabled
        if (!Vars::Comp::AmmoTracker::ShowThroughWalls.Value && !IsVisible(info.Position))
            continue;
            
        Vec3 vScreenPos;
        if (!SDK::W2S(info.Position, vScreenPos))
            continue;
            
        float fTimeLeft = std::max(0.0f, RESPAWN_TIME - (fCurrentTime - info.DisappearTime));
        float fTimePercent = fTimeLeft / RESPAWN_TIME;
        
        Color_t colorConfig = (info.Type == "health") ? 
            Vars::Comp::AmmoTracker::HealthColor.Value : 
            Vars::Comp::AmmoTracker::AmmoColor.Value;
        
        float fDistance = vPlayerPos.DistTo(info.Position);
        
        // Draw ground pie chart
        if (Vars::Comp::AmmoTracker::ShowGroundPieChart.Value && 
            fDistance <= Vars::Comp::AmmoTracker::MaxDistanceGroundPieChart.Value)
        {
            DrawFilledProgress(info.Position, Vars::Comp::AmmoTracker::PieChartSize.Value, 
                             fTimePercent, Vars::Comp::AmmoTracker::PieChartSegments.Value, colorConfig);
        }
        
        // Draw screen pie chart
        if (Vars::Comp::AmmoTracker::ShowScreenPieChart.Value && 
            fDistance <= Vars::Comp::AmmoTracker::MaxDistanceScreenPieChart.Value)
        {
            int iScreenChartSize = GetScreenChartSize(fDistance);
            DrawClockFill(static_cast<int>(vScreenPos.x), static_cast<int>(vScreenPos.y), 
                         iScreenChartSize, fTimePercent, Vars::Comp::AmmoTracker::PieChartSegments.Value, colorConfig);
        }
        
        // Draw text timer
        if (Vars::Comp::AmmoTracker::ShowSeconds.Value && 
            fDistance <= Vars::Comp::AmmoTracker::MaxDistanceText.Value)
        {
            std::string sTimerText;
            if (Vars::Comp::AmmoTracker::ShowMilliseconds.Value)
                sTimerText = std::format("{:.1f}s", fTimeLeft);
            else
                sTimerText = std::format("{}s", static_cast<int>(std::ceil(fTimeLeft)));
            
            int iTextSize = GetTextSize(fDistance);
            auto pFont = (iTextSize <= 12) ? H::Fonts.GetFont(FONT_INDICATORS) : H::Fonts.GetFont(FONT_ESP);
            
            F::CompDraw.String(pFont, 
                          static_cast<int>(vScreenPos.x), static_cast<int>(vScreenPos.y - 15), 
                          Vars::Comp::AmmoTracker::SecondsColor.Value, 
                          ALIGN_CENTER, sTimerText.c_str());
        }
    }
}

void CFullAmmoTracker::Event(IGameEvent* pEvent, uint32_t uHash)
{
    if (uHash == FNV1A::Hash32Const("item_pickup"))
    {
        if (!Vars::Comp::Features::AmmoTracker.Value)
            return;
            
        auto client = I::ClientEntityList->GetClientEntity(I::EngineClient->GetPlayerForUserID(pEvent->GetInt("userid")));
        if (!client) return;
        auto pEntity = client->As<CTFPlayer>();
        if (!pEntity || !pEntity->IsPlayer())
            return;
            
        const char* sItemName = pEvent->GetString("item");
        if (!sItemName)
            return;
            
        // Determine pickup type - check for any health/ammo related items including Halloween
        std::string itemName = sItemName;
        std::transform(itemName.begin(), itemName.end(), itemName.begin(), ::tolower);
        
        std::string sType = "";
        if (itemName.find("medkit") != std::string::npos || 
            itemName.find("healthkit") != std::string::npos ||
            itemName.find("health") != std::string::npos ||
            itemName.find("candy") != std::string::npos ||      // Halloween candy items
            itemName.find("soul") != std::string::npos)         // Halloween soul packs
        {
            sType = "health";
        }
        else if (itemName.find("ammopack") != std::string::npos ||
                 itemName.find("ammo") != std::string::npos)
        {
            sType = "ammo";
        }
        
        if (sType.empty())
            return;
            
        // Find the nearest pickup entity to the player who picked it up
        Vec3 playerPos = pEntity->m_vecOrigin();
        Vec3 pickupPos = playerPos; // Default fallback
        float closestDist = 200.0f; // Max pickup distance
        bool foundPickup = false;
        
        // Search for nearby pickup entities of the correct type
        std::vector<int> vPickupGroups = {EntityEnum::PickupHealth, EntityEnum::PickupAmmo};
        
        for (auto eGroupType : vPickupGroups)
        {
            for (auto pPickupEntity : H::Entities.GetGroup(eGroupType))
            {
                if (!pPickupEntity)
                    continue;
                
                // Only consider stationary map pickups, not dropped weapons
                if (!IsStationaryPickup(pPickupEntity))
                    continue;
                    
                std::string entityType = GetPickupType(pPickupEntity);
                if (entityType == sType)
                {
                    Vec3 entityPos = pPickupEntity->GetAbsOrigin();
                    float dist = playerPos.DistTo(entityPos);
                    
                    if (dist < closestDist)
                    {
                        closestDist = dist;
                        pickupPos = entityPos;
                        foundPickup = true;
                    }
                }
            }
        }
        
        // If no pickup entity found nearby, try to find from our existing tracked positions
        if (!foundPickup)
        {
            for (const auto& [sExistingKey, existingInfo] : m_SupplyPositions)
            {
                if (existingInfo.Type == sType && !existingInfo.Respawning)
                {
                    float dist = playerPos.DistTo(existingInfo.Position);
                    if (dist < closestDist)
                    {
                        closestDist = dist;
                        pickupPos = existingInfo.Position;
                        foundPickup = true;
                    }
                }
            }
        }
        
        std::string sKey = GetPositionKey(pickupPos);
        
        // Check if we're already tracking this position
        if (m_SupplyPositions.find(sKey) != m_SupplyPositions.end())
        {
            // Mark as picked up (start respawn timer)
            SupplyInfo& info = m_SupplyPositions[sKey];
            info.Respawning = true;
            info.DisappearTime = I::GlobalVars->curtime;
        }
        else if (foundPickup)
        {
            // Add new supply position and immediately mark as respawning
            SupplyInfo info;
            info.Position = pickupPos;
            info.Type = sType;
            info.Respawning = true;
            info.DisappearTime = I::GlobalVars->curtime;
            m_SupplyPositions[sKey] = info;
        }
    }
}