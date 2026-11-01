#pragma once
#include <algorithm>
#include <cmath>

// Adapted from Amalgam-Comp CRadar::GetDrawPosition (WTFPL v2).
// Input delta is local origin minus entity origin; output is normalized [-1, 1].
namespace RadarMath
{
    struct Point { float x = 0.f, y = 0.f; };

    inline bool Project(float dx, float dy, float yawDegrees, float range,
                        bool circular, bool clampToEdge, Point& output)
    {
        output = {};
        if (!std::isfinite(dx) || !std::isfinite(dy) || !std::isfinite(yawDegrees)
            || !std::isfinite(range) || range <= 0.f)
            return false;
        const float yaw = -yawDegrees * 0.017453292519943295f;
        const float sine = std::sin(yaw), cosine = std::cos(yaw);
        Point position{ (dx * sine + dy * cosine) / range,
                        (dx * cosine - dy * sine) / range };
        const float extent = circular ? std::hypot(position.x, position.y)
                                     : std::max(std::abs(position.x), std::abs(position.y));
        if (!std::isfinite(extent))
            return false;
        if (extent > 1.f)
        {
            if (!clampToEdge)
                return false;
            position.x /= extent;
            position.y /= extent;
        }
        output = position;
        return true;
    }
}
