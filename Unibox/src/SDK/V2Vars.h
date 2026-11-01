// V2 settings use the existing ConfigVar serialization and bind registry.
namespace Vars { namespace Aimbot { namespace General {
CVarEnum(BypassIgnore, "Bypass ignore", 0, DROPDOWN_MULTI, nullptr,
				VA_LIST("Friends", "Ignored", "Local bots"),
				Friends = 1 << 0, Ignored = 1 << 1, LocalBots = 1 << 2);
}}}
namespace Vars { namespace Aimbot { namespace General {
CVar(PrioritizeFollowbot, "Prioritize followbot target", false);
}}}
namespace Vars { namespace Aimbot { namespace General {
CVar(PrioritizeNavbot, "Prioritize navbot target", false);
}}}
namespace Vars { namespace Aimbot { namespace General {
CVar(ShootDelayMax, "Shoot delay max", 0, SLIDER_CLAMP, 0, 300, 10, "%ims");
}}}
namespace Vars { namespace Aimbot { namespace General {
CVar(ShootDelayMin, "Shoot delay min", 0, SLIDER_CLAMP, 0, 300, 10, "%ims");
}}}
namespace Vars { namespace Aimbot { namespace Melee {
CVar(BackstabAccountPing, "Backstab account ping", true, NOSAVE | DEBUGVAR);
}}}
namespace Vars { namespace Aimbot { namespace Melee {
CVar(BackstabDoubleTest, "Backstab double test", true, NOSAVE | DEBUGVAR);
}}}
namespace Vars { namespace Aimbot { namespace Melee {
CVar(SwingOffset, "Swing offset", -1, NOSAVE | DEBUGVAR, -1, 1);
}}}
namespace Vars { namespace Aimbot { namespace Projectile {
CVar(Out2NormalMax, "Out2 normal max", 0.7f, NOSAVE | DEBUGVAR | SLIDER_CLAMP | SLIDER_PRECISION, -1.f, 1.f, 0.1f);
}}}
namespace Vars { namespace Aimbot { namespace Projectile {
CVar(Out2NormalMin, "Out2 normal min", -0.7f, NOSAVE | DEBUGVAR | SLIDER_CLAMP | SLIDER_PRECISION, -1.f, 1.f, 0.1f);
}}}
namespace Vars { namespace Aimbot { namespace Projectile {
CVarEnum(RocketSplashMode, "Rocket splash mode", 0, NOSAVE | DEBUGVAR, nullptr,
				VA_LIST("Regular", "Special light", "Special heavy"),
				Regular, SpecialLight, SpecialHeavy);
}}}
namespace Vars { namespace Aimbot { namespace Projectile {
CVar(SplashCountArc, "Splash count arc", 5, NOSAVE | DEBUGVAR | SLIDER_MIN | SLIDER_PRECISION, 1, 400, 5);
}}}
namespace Vars { namespace Aimbot { namespace Projectile {
CVar(SplashCountDirect, "Splash count direct", 100, NOSAVE | DEBUGVAR | SLIDER_MIN | SLIDER_PRECISION, 1, 400, 5);
}}}
namespace Vars { namespace Aimbot { namespace Projectile {
CVar(SplashGrates, "Splash grates", true, NOSAVE | DEBUGVAR);
}}}
namespace Vars { namespace Aimbot { namespace Projectile {
CVar(SplashNormalSkip, "Splash normal skip", 1, NOSAVE | DEBUGVAR | SLIDER_MIN, 1, 10);
}}}
namespace Vars { namespace Visuals { namespace Simulation {
CVar(DrawDuration, VA_LIST("Draw duration", "Simulation draw duration"), 5.f, VISUAL | SLIDER_MIN | SLIDER_PRECISION, 0.f, 10.f);
}}}
namespace Vars { namespace Visuals { namespace Simulation {
CVarValues(PlayerPath, "Player path", 0, VISUAL, nullptr,
				"Off", "Line", "Separators", "Spaced", "Arrows", "Boxes");
}}}
namespace Vars { namespace Visuals { namespace Simulation {
CVarValues(ProjectilePath, "Projectile path", 0, VISUAL, nullptr,
				"Off", "Line", "Separators", "Spaced", "Arrows", "Boxes");
}}}
namespace Vars { namespace Visuals { namespace Simulation {
CVarValues(RealPath, "Real path", 0, NOSAVE | DEBUGVAR, nullptr,
				"Off", "Line", "Separators", "Spaced", "Arrows", "Boxes");
}}}
namespace Vars { namespace Visuals { namespace Simulation {
CVar(SwingLines, "Swing lines", false, VISUAL);
}}}
namespace Vars::Aimbot::General {
CVarEnum(Solver, "Aimbot solver", 1, NONE, nullptr, VA_LIST("Unibox", "Amalgam V2"), Unibox, V2);
}
