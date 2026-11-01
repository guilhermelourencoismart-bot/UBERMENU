#include "../Unibox/src/Features/Visuals/Radar/RadarMath.h"
#include "../Unibox/src/Features/Aimbot/AimbotUtils/VelocityScale.h"
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
    std::cout << checks << " calculation checks passed\n";
}
