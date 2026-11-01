#pragma once
#include <algorithm>
#include <cmath>

// Algorithms from Amalgam V2 AimSmoothing.h, adapted without its incompatible
// CUserCmd::m_vViewAngles member. Called by Hitscan/Projectile smooth aim paths.
namespace AimSmoothing
{
    inline float AxisStep(float delta, float smooth, bool cubic)
    {
        if (!std::isfinite(delta) || !std::isfinite(smooth)) return 0.f;
        delta = std::fmod(delta, 360.f);
        if (delta > 180.f) delta -= 360.f;
        if (delta < -180.f) delta += 360.f;
        smooth = std::clamp(smooth, 1.f, 100.f);
        const float normalized = std::abs(delta) / 180.f;
        const float denominator = cubic ? smooth * (1.f + normalized * normalized * normalized)
                                         : 1.5f + (smooth - 1.f) / 99.f * 28.5f;
        return delta / denominator;
    }

    template <class Angle>
    Angle Apply(const Angle& current, const Angle& target, float smooth, bool cubic)
    {
        Angle result = current;
        result.x += AxisStep(target.x - current.x, smooth, cubic);
        result.y += AxisStep(target.y - current.y, smooth, cubic);
        return result;
    }
}
