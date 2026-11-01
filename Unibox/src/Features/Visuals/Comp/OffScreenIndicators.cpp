// Full Comp port, source 366fa0dc SHA256 0c4d8cb7764f6f8228010586cc9dc2a182b397604608f8e60535b30efa34f9f7
#include "CompDraw.h"
#include "OffScreenIndicators.h"
#include "../../../SDK/SDK.h"
#include "../../Players/PlayerUtils.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Static member initialization



bool CFullOffScreenIndicators::IsOnScreen(CBaseEntity* entity)
{
    Vec3 w2s;
    if (SDK::W2S(entity->GetAbsOrigin(), w2s))
    {
        int screenW, screenH;
        I::MatSystemSurface->GetScreenSize(screenW, screenH);

        // Check if the entity is within screen bounds
        return (w2s.x >= 0 && w2s.x <= screenW && w2s.y >= 0 && w2s.y <= screenH);
    }
    return false;
}

std::string CFullOffScreenIndicators::GetPlayerSteamID(CTFPlayer* pPlayer)
{
    if (!pPlayer)
        return "";

    player_info_t playerInfo;
    if (I::EngineClient->GetPlayerInfo(pPlayer->entindex(), &playerInfo))
    {
        if (std::string(playerInfo.guid) == "BOT")
            return "";
        return std::string(playerInfo.guid);
    }
    return "";
}

std::string CFullOffScreenIndicators::GetPlayerClassName(int classId)
{
    switch (classId)
    {
        case 1: return "Scout";
        case 2: return "Sniper";
        case 3: return "Soldier";
        case 4: return "Demoman";
        case 5: return "Medic";
        case 6: return "Heavy";
        case 7: return "Pyro";
        case 8: return "Spy";
        case 9: return "Engineer";
        default: return "Unknown";
    }
}

Color_t CFullOffScreenIndicators::GenerateColor(const std::string& steamID)
{
    if (m_ColorCache.find(steamID) != m_ColorCache.end())
        return m_ColorCache[steamID];

    // Simple hash function for color generation (same as MarkSpot/PlayerTrails)
    uint32_t hash = 0;
    for (char c : steamID)
    {
        hash += static_cast<uint32_t>(c);
    }

    int r = static_cast<int>((hash * 11) % 200 + 55);
    int g = static_cast<int>((hash * 23) % 200 + 55);
    int b = static_cast<int>((hash * 37) % 200 + 55);

    Color_t color = {static_cast<byte>(r), static_cast<byte>(g), static_cast<byte>(b), 255};
    m_ColorCache[steamID] = color;
    return color;
}

void CFullOffScreenIndicators::DrawArrow(int centerX, int centerY, float angle, const Color_t& color, int size)
{
    // Calculate arrow points based on angle
    float halfSize = size * 0.5f;

    // Arrow points (pointing right initially)
    Vec3 tip = {static_cast<float>(centerX) + halfSize, static_cast<float>(centerY), 0.0f};
    Vec3 base1 = {static_cast<float>(centerX) - halfSize, static_cast<float>(centerY) - halfSize * 0.6f, 0.0f};
    Vec3 base2 = {static_cast<float>(centerX) - halfSize, static_cast<float>(centerY) + halfSize * 0.6f, 0.0f};

    // Rotate points around center
    float cosA = std::cos(angle);
    float sinA = std::sin(angle);

    auto rotatePoint = [&](Vec3& point) {
        float x = point.x - static_cast<float>(centerX);
        float y = point.y - static_cast<float>(centerY);
        point.x = static_cast<float>(centerX) + (x * cosA - y * sinA);
        point.y = static_cast<float>(centerY) + (x * sinA + y * cosA);
    };

    rotatePoint(tip);
    rotatePoint(base1);
    rotatePoint(base2);

    // Draw arrow using lines
    F::CompDraw.Line(static_cast<int>(tip.x), static_cast<int>(tip.y),
                static_cast<int>(base1.x), static_cast<int>(base1.y), color);
    F::CompDraw.Line(static_cast<int>(tip.x), static_cast<int>(tip.y),
                static_cast<int>(base2.x), static_cast<int>(base2.y), color);
    F::CompDraw.Line(static_cast<int>(base1.x), static_cast<int>(base1.y),
                static_cast<int>(base2.x), static_cast<int>(base2.y), color);
}

void CFullOffScreenIndicators::Draw()
{
    // Early exits
    if (!Vars::Comp::Features::OffScreenIndicators.Value)
        return;

    if (I::EngineVGui->IsGameUIVisible())
        return;

    auto pLocal = H::Entities.GetLocal();
    if (!pLocal)
        return;

    // Get screen dimensions
    int screenW, screenH;
    I::MatSystemSurface->GetScreenSize(screenW, screenH);

    // Get local player's view angle for directional calculations
    Vec3 realAngle = I::EngineClient->GetViewAngles();
    float yaw = realAngle.y * 3.14159265358979323846f / 180.0f;

    // Set font for text rendering
    auto font = H::Fonts.GetFont(FONT_ESP);

    // Process all players
    for (auto pEntity : H::Entities.GetGroup(EntityEnum::PlayerAll))
    {
        auto pPlayer = pEntity->As<CTFPlayer>();
        if (!pPlayer || pPlayer->IsDormant() || !pPlayer->IsAlive())
            continue;

        // Skip local player and teammates
        if (pPlayer == pLocal || pPlayer->m_iTeamNum() == pLocal->m_iTeamNum())
            continue;

        // Check if player is off-screen
        if (IsOnScreen(pPlayer))
            continue;

        // Calculate directional position relative to local player
        Vec3 localPos = pLocal->GetAbsOrigin();
        Vec3 playerPos = pPlayer->GetAbsOrigin();
        Vec3 positionDiff = localPos - playerPos;

        float x = std::cos(yaw) * positionDiff.y - std::sin(yaw) * positionDiff.x;
        float y = std::cos(yaw) * positionDiff.x + std::sin(yaw) * positionDiff.y;
        float len = std::sqrt(x * x + y * y);

        if (len == 0)
            continue;

        // Normalize direction
        x = x / len;
        y = y / len;

        // Position indicator at configurable distance from screen center
        float indicatorRange = Vars::Comp::Features::OffScreenIndicatorsRange.Value;
        float pos1 = screenW / 2.0f + x * indicatorRange;
        float pos2 = screenH / 2.0f + y * indicatorRange;

        if (pos1 != pos1 || pos2 != pos2)  // NaN check
            continue;

        int finalX = static_cast<int>(std::floor(pos1));
        int finalY = static_cast<int>(std::floor(pos2));

        // Get player name
        std::string playerName = "Player " + std::to_string(pPlayer->entindex());
        player_info_t pi{};
        if (I::EngineClient->GetPlayerInfo(pPlayer->entindex(), &pi))
        {
            playerName = F::PlayerUtils.GetPlayerName(pPlayer->entindex(), pi.name);
        }

        // Get health info
        int health = pPlayer->m_iHealth();
        std::string healthText = std::to_string(health);

        // Calculate distance for alpha
        float distance = len;
        int baseAlpha = Vars::Comp::Features::OffScreenIndicatorsAlpha.Value;
        int alpha = static_cast<int>(std::max(0.0f, static_cast<float>(baseAlpha) - (distance * 0.05f)));

        // Get color based on player Steam ID
        std::string steamID = GetPlayerSteamID(pPlayer);
        Color_t playerColor = {255, 255, 255, static_cast<byte>(alpha)};
        if (!steamID.empty())
        {
            playerColor = GenerateColor(steamID);
            playerColor.a = static_cast<byte>(alpha);
        }

        // Calculate angle pointing toward the target (opposite of the direction to indicator)
        float directionAngle = std::atan2(y, x); // Direction from indicator toward target

        // Draw indicator - either Steam avatar or arrow
        int arrowSize = Vars::Comp::Features::OffScreenIndicatorsSize.Value;

        if (Vars::Comp::Features::OffScreenIndicatorsAvatars.Value)
        {
            int avatarSize = arrowSize * 2; // Make avatar bigger than arrow for visibility
            bool avatarDrawn = false;

            // Calculate avatar position
            int avatarX = finalX - avatarSize/2;
            int avatarY = finalY - avatarSize/2;

            if (pi.friendsID != 0)
            {
                // Draw 1-pixel Steam ID color border around Steam avatar
                F::CompDraw.LineRect(avatarX - 1, avatarY - 1, avatarSize + 2, avatarSize + 2, playerColor);

                // Try to draw Steam avatar for real players
                // The Avatar method now has built-in safety checks and will fail gracefully
                F::CompDraw.Avatar(avatarX, avatarY, avatarSize, avatarSize, pi.friendsID, ALIGN_TOPLEFT);

                // Note: We can't easily detect if Avatar() succeeded, but it now fails safely
                // If Steam avatar fails, we'll still show the fallback on next frame
                avatarDrawn = true;
            }

            if (!avatarDrawn)
            {
                // Fallback to embedded avatar for bots/players without Steam ID or failed Steam avatars
                F::CompDraw.LineRect(avatarX-1,avatarY-1,avatarSize+2,avatarSize+2,playerColor);
                F::CompDraw.FillRect(avatarX,avatarY,avatarSize,avatarSize,Color_t(128,128,128,255));
                avatarDrawn=true;
            }

            if (!avatarDrawn)
            {
                // Final fallback to arrow if entire avatar system failed
                DrawArrow(finalX, finalY, directionAngle, playerColor, arrowSize);
            }
        }
        else
        {
            // Use arrows when avatar mode is disabled
            DrawArrow(finalX, finalY, directionAngle, playerColor, arrowSize);
        }

        // Draw information with toggleable options
        int currentY = finalY + (Vars::Comp::Features::OffScreenIndicatorsAvatars.Value ? arrowSize * 2 : arrowSize) + 5;

        // Draw player name if enabled
        if (Vars::Comp::Features::OffScreenIndicatorsName.Value)
        {
            Vec2 nameSize = F::CompDraw.GetTextSize(playerName.c_str(), font);
            F::CompDraw.String(font, finalX - static_cast<int>(nameSize.x) / 2, currentY, {255, 255, static_cast<byte>(alpha)}, ALIGN_TOPLEFT, playerName.c_str());
            currentY += 15;
        }

        // Draw player class if enabled
        if (Vars::Comp::Features::OffScreenIndicatorsClass.Value)
        {
            std::string className = GetPlayerClassName(pPlayer->m_iClass());
            Vec2 classSize = F::CompDraw.GetTextSize(className.c_str(), font);
            F::CompDraw.String(font, finalX - static_cast<int>(classSize.x) / 2, currentY, {255, 0, static_cast<byte>(alpha)}, ALIGN_TOPLEFT, className.c_str());
            currentY += 15;
        }

        // Draw health if enabled
        if (Vars::Comp::Features::OffScreenIndicatorsHealth.Value)
        {
            Vec2 healthSize = F::CompDraw.GetTextSize(healthText.c_str(), font);
            F::CompDraw.String(font, finalX - static_cast<int>(healthSize.x) / 2, currentY, {0, 0, static_cast<byte>(alpha)}, ALIGN_TOPLEFT, healthText.c_str());
            currentY += 15;
        }

        // Draw distance if enabled
        if (Vars::Comp::Features::OffScreenIndicatorsDistance.Value)
        {
            std::string distanceText = std::to_string(static_cast<int>(distance)) + "u";
            Vec2 distSize = F::CompDraw.GetTextSize(distanceText.c_str(), font);
            F::CompDraw.String(font, finalX - static_cast<int>(distSize.x) / 2, currentY, {200, 200, static_cast<byte>(alpha)}, ALIGN_TOPLEFT, distanceText.c_str());
        }
    }
}

