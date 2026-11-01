#pragma once
#include <algorithm>
#include <cmath>

namespace AimSmoothing
{
    // Shared by Unibox and V2; V2 adds tick/target memoization around this factor.
    inline float VelocityScale(float speed)
    {
        if (!std::isfinite(speed))
            return 1.f;
        return std::clamp(0.65f + std::clamp(speed / 320.f, 0.f, 1.75f) * 0.4f, 0.35f, 1.35f);
    }

    class VelocityCache
    {
        bool m_valid = false;
        int m_tick = 0, m_target = 0;
        float m_scale = 1.f;
    public:
        template <typename Calculate>
        float Get(int tick, int target, Calculate calculate)
        {
            if (!m_valid || tick != m_tick || target != m_target)
            {
                m_scale = calculate();
                m_tick = tick;
                m_target = target;
                m_valid = true;
            }
            return m_scale;
        }
    };
}
