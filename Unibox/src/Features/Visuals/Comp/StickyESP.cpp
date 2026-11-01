// Complete Comp port, source 366fa0dc, SHA256 748b9fafbb26bdef78e02f4de8fd1842b789a13284fc462aaa456c729d6443d9.
#include "CompDraw.h"
#include "MainESP.h"
#include "../Groups/Groups.h"
#include "StickyESP.h"

void CFullStickyESP::Draw()
{
    if (!Vars::Comp::Features::StickyESP.Value)
        return;

    // Early exits (like HealthBarESP)
    if (I::EngineVGui->IsGameUIVisible())
        return;

    auto pLocal = H::Entities.GetLocal();
    if (!pLocal)
        return;

    Vec3 localPos = pLocal->GetAbsOrigin();
    float maxDistSqr = Vars::Comp::StickyESP::MaxDistance.Value * Vars::Comp::StickyESP::MaxDistance.Value;

    // Process all projectiles directly (no caching, like HealthBarESP)
    for (auto pEntity : H::Entities.GetGroup(EntityEnum::WorldProjectile))
    {
        // Check if it's a stickybomb
        if (!pEntity || pEntity->GetClassID() != ETFClassID::CTFGrenadePipebombProjectile)
            continue;

        if (pEntity->IsDormant())
            continue;

        auto pSticky = pEntity->As<CTFGrenadePipebombProjectile>();
        if (!pSticky || pSticky->m_iType() != 1) // Only actual stickybombs (not pipes)
            continue;

        Vec3 stickyPos = pSticky->GetAbsOrigin();

        // Distance check (exactly like Lua script)
        Vec3 delta = stickyPos - localPos;
        float distanceSqr = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
        if (distanceSqr > maxDistSqr)
            continue;

        // Enemy filter (exactly like Lua script)
        if (Vars::Comp::StickyESP::EnemyOnly.Value && pSticky->m_iTeamNum() == pLocal->m_iTeamNum())
            continue;

        // Convert to screen coordinates (exactly like Lua script)
        Vec3 screenPos;
        if (!SDK::W2S(stickyPos, screenPos))
            continue;

        // Check visibility (exactly like Lua script)
        bool isVisible = IsVisible(pSticky, pLocal);

        // Skip if only showing when visible and not visible (exactly like Lua script)
        if (Vars::Comp::StickyESP::BoxOnlyWhenVisible.Value && !isVisible)
            continue;

        // Chams are now handled in UpdateChamsEntities()

        // Choose color based on team and visibility
        Color_t color;
        bool isEnemy = pSticky->m_iTeamNum() != pLocal->m_iTeamNum();

        if (isEnemy)
        {
            color = isVisible ? Vars::Comp::StickyESP::EnemyVisibleColor.Value : Vars::Comp::StickyESP::EnemyInvisibleColor.Value;
        }
        else
        {
            color = isVisible ? Vars::Comp::StickyESP::TeamVisibleColor.Value : Vars::Comp::StickyESP::TeamInvisibleColor.Value;
        }

        if(Vars::Comp::ESP::Enabled.Value && F::FullMainESP.HasBox(pEntity))continue;
        Group_t* nativeGroup=nullptr;
        if(!Vars::Comp::ESP::Enabled.Value && F::Groups.GetGroup(pEntity,nativeGroup,false) && nativeGroup && (nativeGroup->m_tESP.Draw & ESPEnum::Box)) continue;
        // Draw 2D box (exactly like Lua script)
        if (Vars::Comp::StickyESP::Box2D.Value)
        {
            Draw2DBox(static_cast<int>(screenPos.x), static_cast<int>(screenPos.y), Vars::Comp::StickyESP::BoxSize.Value, color);
        }

        // Draw 3D box (exactly like Lua script)
        if (Vars::Comp::StickyESP::Box3D.Value)
        {
            auto hitbox = GetHitbox(pSticky);
            if (hitbox.first != Vec3() && hitbox.second != Vec3())
            {
                Vec3 min = hitbox.first;
                Vec3 max = hitbox.second;

                // Create 8 vertices of the bounding box (exactly like Lua script)
                std::vector<Vec3> vertices = {
                    Vec3(min.x, min.y, min.z), Vec3(min.x, max.y, min.z),
                    Vec3(max.x, max.y, min.z), Vec3(max.x, min.y, min.z),
                    Vec3(min.x, min.y, max.z), Vec3(min.x, max.y, max.z),
                    Vec3(max.x, max.y, max.z), Vec3(max.x, min.y, max.z)
                };

                Draw3DBox(vertices, color);
            }
        }
    }
}

bool CFullStickyESP::IsVisible(CBaseEntity* pEntity, CBaseEntity* pLocal)
{
    if (!pEntity || !pLocal)
        return false;

    // Perform visibility check (exactly like Lua script: engine.TraceLine)
    Vec3 source = pLocal->GetAbsOrigin() + pLocal->As<CTFPlayer>()->m_vecViewOffset();
    Vec3 target = pEntity->GetAbsOrigin();

    CGameTrace trace = {};
    CTraceFilterHitscan filter = {};
    filter.m_pSkip = pLocal;

    SDK::Trace(source, target, MASK_SHOT, &filter, &trace);

    // Exactly like Lua script: trace.fraction > 0.99 or trace.entity == entity
    return trace.fraction > 0.99f || trace.m_pEnt == pEntity;
}

std::pair<Vec3, Vec3> CFullStickyESP::GetHitbox(CBaseEntity* pEntity)
{
    if (!pEntity)
        return {Vec3(), Vec3()};

    // Simple hitbox calculation (like Lua script: entity:HitboxSurroundingBox())
    Vec3 origin = pEntity->GetAbsOrigin();
    Vec3 mins = pEntity->m_vecMins();
    Vec3 maxs = pEntity->m_vecMaxs();

    return {origin + mins, origin + maxs};
}

void CFullStickyESP::Draw3DBox(const std::vector<Vec3>& vertices, const Color_t& color)
{
    if (vertices.size() < 8)
        return;

    // Convert all vertices to screen coordinates (exactly like Lua script)
    std::vector<Vec3> screenVertices;
    for (const auto& vertex : vertices)
    {
        Vec3 screenPos;
        if (SDK::W2S(vertex, screenPos))
        {
            screenVertices.push_back(screenPos);
        }
        else
        {
            return; // If any vertex is off-screen, don't draw (like Lua script)
        }
    }

    // Define the edges of the box (exactly like Lua script)
    std::vector<std::pair<int, int>> edges = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0}, // Bottom face
        {4, 5}, {5, 6}, {6, 7}, {7, 4}, // Top face
        {0, 4}, {1, 5}, {2, 6}, {3, 7}  // Vertical edges
    };

    // Draw all edges (exactly like Lua script)
    for (const auto& edge : edges)
    {
        Vec3 v1 = screenVertices[edge.first];
        Vec3 v2 = screenVertices[edge.second];
        F::CompDraw.Line(static_cast<int>(v1.x), static_cast<int>(v1.y),
                    static_cast<int>(v2.x), static_cast<int>(v2.y), color);
    }
}

void CFullStickyESP::Draw2DBox(int x, int y, int size, const Color_t& color)
{
    // Exactly like Lua script: draw_2d_box function
    int halfSize = size / 2;
    int x1 = x - halfSize;
    int y1 = y - halfSize;
    int x2 = x + halfSize;
    int y2 = y + halfSize;

    // Draw box outline (exactly like Lua script)
    F::CompDraw.Line(x1, y1, x2, y1, color); // Top
    F::CompDraw.Line(x1, y2, x2, y2, color); // Bottom
    F::CompDraw.Line(x1, y1, x1, y2, color); // Left
    F::CompDraw.Line(x2, y1, x2, y2, color); // Right
}

void CFullStickyESP::UpdateChamsEntities()
{
    // Clear previous chams entries
    m_mEntities.clear();

    if (!Vars::Comp::Features::StickyESP.Value || !Vars::Comp::StickyESP::EnableChams.Value)
        return;

    auto pLocal = H::Entities.GetLocal();
    if (!pLocal)
        return;

    Vec3 localPos = pLocal->GetAbsOrigin();
    float maxDistSqr = Vars::Comp::StickyESP::MaxDistance.Value * Vars::Comp::StickyESP::MaxDistance.Value;

    // Process all projectiles to find chams-eligible stickies
    for (auto pEntity : H::Entities.GetGroup(EntityEnum::WorldProjectile))
    {
        // Check if it's a stickybomb
        if (!pEntity || pEntity->GetClassID() != ETFClassID::CTFGrenadePipebombProjectile)
            continue;

        if (pEntity->IsDormant())
            continue;

        auto pSticky = pEntity->As<CTFGrenadePipebombProjectile>();
        if (!pSticky || pSticky->m_iType() != 1) // Only actual stickybombs (not pipes)
            continue;

        Vec3 stickyPos = pSticky->GetAbsOrigin();

        // Distance check
        Vec3 delta = stickyPos - localPos;
        float distanceSqr = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
        if (distanceSqr > maxDistSqr)
            continue;

        // Enemy filter
        if (Vars::Comp::StickyESP::EnemyOnly.Value && pSticky->m_iTeamNum() == pLocal->m_iTeamNum())
            continue;

        // Check visibility if required
        if (Vars::Comp::StickyESP::BoxOnlyWhenVisible.Value && !IsVisible(pSticky, pLocal))
            continue;

        // Add to chams tracking
        m_mEntities[pSticky->entindex()] = true;
    }
}
