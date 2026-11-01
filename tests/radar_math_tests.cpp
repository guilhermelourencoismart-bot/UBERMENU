#include "../Unibox/src/Features/Visuals/Radar/RadarMath.h"
#include "../Unibox/src/Features/Aimbot/AimbotUtils/VelocityScale.h"
#include "../Unibox/src/Features/Aimbot/AimbotUtils/AimSmoothing.h"
#include "../Unibox/src/Features/ImGui/IndicatorCache.h"
#include <atomic>
#include <thread>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <limits>

static int checks = 0;
static void Require(bool condition, const char* message)
{
    ++checks;
    if (!condition) { std::cerr << "FAIL: " << message << '\n'; std::exit(1); }
}
static bool Near(float a, float b) { return std::abs(a - b) < 0.0001f; }

int main()
{
    RadarMath::Point p;
    Require(RadarMath::Project(100, 0, 0, 1000, true, false, p) && Near(p.x, 0) && Near(p.y, .1f), "forward orientation");
    Require(RadarMath::Project(100, 0, 90, 1000, true, false, p) && Near(p.x, -.1f) && Near(p.y, 0), "yaw rotation");
    Require(RadarMath::Project(0, 0, 0, 1000, true, true, p) && Near(p.x, 0) && Near(p.y, 0), "coincident point");
    Require(!RadarMath::Project(2000, 0, 0, 1000, true, false, p), "out of range hidden");
    Require(RadarMath::Project(2000, 0, 0, 1000, true, true, p) && Near(p.y, 1), "circular edge clamping");
    Require(RadarMath::Project(2000, 2000, 0, 1000, false, true, p) && Near(p.x, 1) && Near(p.y, 1), "rectangular diagonal clamping");
    Require(RadarMath::Project(2000, 2000, 0, 1000, true, true, p) && Near(std::hypot(p.x, p.y), 1), "circular diagonal clamping");
    Require(!RadarMath::Project(0, 0, 0, 0, true, true, p), "zero range rejected");
    Require(!RadarMath::Project(0, 0, 0, -1, true, true, p), "negative range rejected");
    Require(!RadarMath::Project(std::numeric_limits<float>::quiet_NaN(), 0, 0, 1000, true, true, p), "NaN rejected");
    Require(!RadarMath::Project(0, 0, std::numeric_limits<float>::infinity(), 1000, true, true, p), "infinite yaw rejected");
    Require(!RadarMath::Project(std::numeric_limits<float>::max(), 0, 0, .001f, true, true, p), "overflow rejected");
    Require(Near(AimSmoothing::VelocityScale(0), .65f), "stationary velocity factor");
    Require(Near(AimSmoothing::VelocityScale(320), 1.05f), "normal velocity factor");
    Require(Near(AimSmoothing::VelocityScale(2000), 1.35f), "velocity factor saturation");
    Require(Near(AimSmoothing::VelocityScale(std::numeric_limits<float>::quiet_NaN()), 1.f), "invalid velocity neutral factor");
    AimSmoothing::VelocityCache cache;
    int calls = 0;
    const auto compute = [&] { ++calls; return .8f; };
    Require(Near(cache.Get(5, 1, compute), .8f) && calls == 1, "first cache calculation");
    cache.Get(5, 1, compute);
    Require(calls == 1, "same tick and target reuse calculation");
    cache.Get(5, 2, compute);
    Require(calls == 2, "target change invalidates cache");
    cache.Get(6, 2, compute);
    Require(calls == 3, "tick change invalidates cache");
    cache.Get(0, 2, compute);
    Require(calls == 4, "tick rollback invalidates cache");
    for (int i = 0; i < 1000; ++i) cache.Get(0, 2, compute);
    Require(calls == 4, "repeated calls do not repeat velocity computation");
    Require(Near(AimSmoothing::AxisStep(90, 10, true), 8.f), "V2 cubic denominator");
    Require(Near(AimSmoothing::AxisStep(90, 1, false), 60.f), "V2 remapped minimum");
    Require(Near(AimSmoothing::AxisStep(90, 100, false), 3.f), "V2 remapped maximum");
    Require(Near(AimSmoothing::AxisStep(-358, 1, false), 2.f / 1.5f), "yaw wraps across boundary");
    Require(Near(AimSmoothing::AxisStep(0, 100, true), 0.f), "aligned target unchanged");
    Require(Near(AimSmoothing::AxisStep(std::numeric_limits<float>::infinity(), 10, true), 0.f), "invalid delta rejected");
    struct Angles { float x, y, z; };
    const auto angle = AimSmoothing::Apply(Angles{0, 179, 0}, Angles{90, -179, 0}, 1.f, false);
    Require(Near(angle.x, 60.f) && Near(angle.y, 179.f + 2.f / 1.5f), "independent pitch/yaw application");
    CIndicatorCache<std::vector<int>> snapshot;
    snapshot.Set({1, 1, 1});
    auto detached = snapshot.Get(); detached[0] = 9;
    Require(snapshot.Get()[0] == 1, "snapshot readers receive independent copies");
    std::atomic<bool> torn = false;
    std::thread producer([&] { for (int i = 0; i < 1000; ++i) snapshot.Set({i, i, i}); });
    std::thread consumer([&]
    {
        for (int i = 0; i < 1000; ++i)
        {
            auto value = snapshot.Get();
            if (value.size() != 3 || value[0] != value[1] || value[1] != value[2]) torn = true;
        }
    });
    producer.join(); consumer.join();
    Require(!torn && snapshot.Get()[0] == 999, "snapshot publication does not expose partial frames");
    std::cout << checks << " calculation/cache checks passed\n";
}
