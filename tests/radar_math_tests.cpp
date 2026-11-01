#include "../Unibox/src/Features/Visuals/Radar/RadarMath.h"
#include "../Unibox/src/Features/Aimbot/AimbotUtils/VelocityScale.h"
#include "../Unibox/src/Features/Aimbot/AimbotUtils/AimSmoothing.h"
#include "../Unibox/src/Features/ImGui/IndicatorCache.h"
#include "../Unibox/src/Features/Aimbot/AimbotUtils/SolverPolicy.h"
#include "../Unibox/src/Features/Aimbot/AimbotUtils/V2Math.h"
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
    cache.Get(0,(uint64_t(5)<<32)|2,compute);Require(calls==5,"entity serial invalidates velocity cache");
    Require(Near(cache.Get(1,3,[]{return std::numeric_limits<float>::quiet_NaN();}),1.f),"nonfinite velocity cache calculation becomes neutral");
    Require(!SolverPolicy::TargetIsUsable(false,false,false,false)&&!SolverPolicy::TargetIsUsable(true,true,true,true)&&!SolverPolicy::TargetIsUsable(true,false,true,false),"missing dormant and dead targets invalidated");
    Require(SolverPolicy::TargetIsUsable(true,false,false,false),"nonplayer targets retain valid semantics");
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
    float normalized;
    Require(V2Math::Normalize(721,normalized)&&Near(normalized,1),"normalize repeated revolutions");
    Require(V2Math::Normalize(-540,normalized)&&Near(normalized,-180),"normalize negative boundary");
    Require(!V2Math::Normalize(std::numeric_limits<float>::infinity(),normalized),"normalize nonfinite rejected");
    Require(Near(V2Math::CalcFov(Angles{0,0,0},Angles{0,90,0}),90),"FOV right angle");
    Require(Near(V2Math::CalcFov(Angles{0,0,0},Angles{0,180,0}),180),"FOV antiparallel");
    Require(Near(V2Math::CalcFov(Angles{0,179,0},Angles{0,-179,0}),2),"FOV angular wrap");
    Require(!std::isfinite(V2Math::CalcFov(Angles{0,0,0},Angles{0,std::numeric_limits<float>::quiet_NaN(),0})),"invalid FOV cannot become perfect aim");
    // Differential oracle: the V2 source uses acos(dot(AngleVectors(a),AngleVectors(b))).
    for(int yaw=-170;yaw<=170;yaw+=17)
    {
        const double rad=.017453292519943295;
        const double dot=std::cos(20*rad)*std::cos(-30*rad)*std::cos((yaw-40)*rad)+std::sin(20*rad)*std::sin(-30*rad);
        const float reference=float(std::acos(std::clamp(dot,-1.0,1.0))/rad);
        Require(Near(V2Math::CalcFov(Angles{20,float(yaw),0},Angles{-30,40,0}),reference),"V2 FOV differential oracle");
    }
    float pitch,flight;
    Require(V2Math::LowerArc(1000,0,1000,0,pitch,flight)&&Near(flight,1)&&Near(pitch,0),"straight projectile time");
    Require(V2Math::LowerArc(1000,800,500,0,pitch,flight),"ballistic lower arc reachable");
    Require(std::abs(1000*std::sin(pitch)*flight-400*flight*flight)<.01f,"ballistic vertical equation");
    Require(std::abs(1000*std::cos(pitch)*flight-500)<.01f,"ballistic horizontal equation");
    Require(!V2Math::LowerArc(100,800,1000,0,pitch,flight),"unreachable projectile rejected");
    Require(!V2Math::LowerArc(0,800,1,0,pitch,flight),"zero speed rejected");
    Require(!V2Math::LowerArc(1000,800,0,100,pitch,flight),"singular vertical ballistic case rejected");
    float entry=0.f;
    Require(V2Math::RayBoxEntry(Angles{1,-1,-1},Angles{2,1,1},Angles{0,0,0},Angles{2,0,0},entry)&&Near(entry,.5f),"axis-parallel hitbox ray entry");
    Require(!V2Math::RayBoxEntry(Angles{1,2,-1},Angles{2,3,1},Angles{0,0,0},Angles{2,0,0},entry),"parallel ray outside slab rejected");
    Require(V2Math::RayBoxEntry(Angles{-1,-1,-1},Angles{1,1,1},Angles{0,0,0},Angles{0,0,0},entry)&&Near(entry,0),"stationary point inside bounds");
    Require(!V2Math::RayBoxEntry(Angles{-1,-1,-1},Angles{1,1,1},Angles{3,0,0},Angles{1,0,0},entry),"ray facing away from box rejected");
    Require(!V2Math::RayBoxEntry(Angles{1,1,1},Angles{-1,-1,-1},Angles{0,0,0},Angles{1,0,0},entry),"inverted hitbox rejected");
    Require(!V2Math::RayBoxEntry(Angles{-1,-1,-1},Angles{1,1,1},Angles{0,0,0},Angles{std::numeric_limits<float>::quiet_NaN(),0,0},entry),"nonfinite hitbox ray rejected");
    Require(V2Math::LowerArc(1000,0,0,100,pitch,flight)&&Near(flight,.1f),"vertical straight projectile");
    Require(!V2Math::LowerArc(std::numeric_limits<float>::infinity(),0,100,0,pitch,flight),"nonfinite projectile speed rejected");
    SolverPolicy::ContextGuard guard;
    SolverPolicy::Context context{1,10,100,50,true};
    Require(guard.Update(context),"initial solver context invalidates");
    context.tick++;Require(!guard.Update(context),"normal tick retains solver state");
    context.weapon++;Require(guard.Update(context),"weapon swap invalidates solver");
    context.owner++;Require(guard.Update(context),"owner identity invalidates solver");
    context.map++;Require(guard.Update(context),"map swap invalidates solver");
    context.tick=0;Require(guard.Update(context),"tick rollback invalidates solver");
    context.valid=false;Require(guard.Update(context),"invalid owner or weapon clears solver");
    Require(!guard.Update(context),"repeated invalid context remains empty");
    Require(SolverPolicy::RecycledTarget(5,10,5,11),"reused target slot invalidates identity");
    Require(!SolverPolicy::RecycledTarget(5,10,6,11),"different target handled by solver selection");
    Require(SolverPolicy::UseV2(true,false)&&!SolverPolicy::UseV2(true,true)&&!SolverPolicy::UseV2(false,false),"exclusive routing preserves compatibility extensions");
    Require(SolverPolicy::NeedsNativeExtension(false,true,true,false,false,false),"native forward tracking preserved");
    Require(SolverPolicy::NeedsNativeExtension(false,false,false,true,true,false)&&SolverPolicy::NeedsNativeExtension(false,false,false,true,false,true),"native lob and dormant targeting preserved");
    Require(!SolverPolicy::NeedsNativeExtension(false,true,false,false,true,true),"unrelated projectile flags do not disable hitscan V2");
    Require(!SolverPolicy::NeedsNativeExtension(false,false,false,true,false,false),"ordinary projectiles dispatch to V2");
    Require(!V2Math::PrioritizedBefore(true,true)&&V2Math::PrioritizedBefore(true,false),"target comparator is irreflexive");
    std::cout << checks << " calculation/cache/policy checks passed\n";
}
