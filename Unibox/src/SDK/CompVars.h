#pragma once
// Comp settings registered by the existing Unibox ConfigVar/bind system.
namespace Vars {
NAMESPACE_BEGIN(Comp)
	NAMESPACE_BEGIN(ESP)
 CVar(Enabled, "Use Comp main ESP", false, VISUAL);
		CVarEnum(Draw, VA_LIST("Draw", "Draw ESP"), 0b0, VISUAL | DROPDOWN_MULTI, nullptr,
			VA_LIST("Players", "Buildings", "Projectiles", "Objective", "NPCs", "Health", "Ammo", "Money", "Powerups", "Bombs", "Spellbook", "Gargoyle"),
			Players = 1 << 0, Buildings = 1 << 1, Projectiles = 1 << 2, Objective = 1 << 3, NPCs = 1 << 4, Health = 1 << 5, Ammo = 1 << 6, Money = 1 << 7, Powerups = 1 << 8, Bombs = 1 << 9, Spellbook = 1 << 10, Gargoyle = 1 << 11);
		CVarEnum(Player, VA_LIST("Player", "Player ESP"), 0b0, VISUAL | DROPDOWN_MULTI, nullptr,
			VA_LIST("Enemy", "Team", "Local", "Prioritized", "Friends", "Party", "##Divider", "Name", "Box", "Distance", "Bones", "Health bar", "Health text", "Uber bar", "Uber text", "Class icon", "Class text", "Weapon icon", "Weapon text", "Priority", "Labels", "Buffs", "Debuffs", "Misc", "Lag compensation", "Ping", "KDR"),
			Enemy = 1 << 0, Team = 1 << 1, Local = 1 << 2, Prioritized = 1 << 3, Friends = 1 << 4, Party = 1 << 5, Name = 1 << 6, Box = 1 << 7, Distance = 1 << 8, Bones = 1 << 9, HealthBar = 1 << 10, HealthText = 1 << 11, UberBar = 1 << 12, UberText = 1 << 13, ClassIcon = 1 << 14, ClassText = 1 << 15, WeaponIcon = 1 << 16, WeaponText = 1 << 17, Priority = 1 << 18, Labels = 1 << 19, Buffs = 1 << 20, Debuffs = 1 << 21, Misc = 1 << 22, LagCompensation = 1 << 23, Ping = 1 << 24, KDR = 1 << 25);
		CVarEnum(Building, VA_LIST("Building", "Building ESP"), 0b0, VISUAL | DROPDOWN_MULTI, nullptr,
			VA_LIST("Enemy", "Team", "Local", "Prioritized", "Friends", "Party", "##Divider", "Name", "Box", "Distance", "Health bar", "Health text", "Owner", "Level", "Flags"),
			Enemy = 1 << 0, Team = 1 << 1, Local = 1 << 2, Prioritized = 1 << 3, Friends = 1 << 4, Party = 1 << 5, Name = 1 << 6, Box = 1 << 7, Distance = 1 << 8, HealthBar = 1 << 9, HealthText = 1 << 10, Owner = 1 << 11, Level = 1 << 12, Flags = 1 << 13);
		CVarEnum(Projectile, VA_LIST("Projectile", "Projectile ESP"), 0b0, VISUAL | DROPDOWN_MULTI, nullptr,
			VA_LIST("Enemy", "Team", "Local", "Prioritized", "Friends", "Party", "##Divider", "Name", "Box", "Distance", "Owner", "Flags"),
			Enemy = 1 << 0, Team = 1 << 1, Local = 1 << 2, Prioritized = 1 << 3, Friends = 1 << 4, Party = 1 << 5, Name = 1 << 6, Box = 1 << 7, Distance = 1 << 8, Owner = 1 << 9, Flags = 1 << 10);
		CVarEnum(Objective, VA_LIST("Objective", "Objective ESP"), 0b0, VISUAL | DROPDOWN_MULTI, nullptr,
			VA_LIST("Enemy", "Team", "##Divider", "Name", "Box", "Distance", "Flags", "Intel return time"),
			Enemy = 1 << 0, Team = 1 << 1, Name = 1 << 2, Box = 1 << 3, Distance = 1 << 4, Flags = 1 << 5, IntelReturnTime = 1 << 6);

		CVar(ActiveAlpha, "Active alpha", 255, VISUAL | SLIDER_CLAMP, 0, 255, 5);
		CVar(DormantAlpha, "Dormant alpha", 50, VISUAL | SLIDER_CLAMP, 0, 255, 5);
		CVar(DormantDuration, "Dormant duration", 1.f, VISUAL | SLIDER_CLAMP | SLIDER_PRECISION, 0.015f, 5.0f, 0.1f, "%gs");
		CVar(DormantPriority, "Dormant priority only", false, VISUAL);


NAMESPACE_END(ESP)

NAMESPACE_BEGIN(Features)
CVar(UberTracker, "UberTracker", false, VISUAL);
CVar(MedicUberBar, "MedicUberBar", false, VISUAL);
CVar(PylonESP, "PylonESP", false, VISUAL);
CVar(FocusFire, "FocusFire", false, VISUAL);
CVar(HiderESP, "HiderESP", false, VISUAL);
NAMESPACE_END(Features)
NAMESPACE_BEGIN(UberTracker)
			CVar(ShowTopBox, "Show info box", true, VISUAL);
			CVar(ShowAdvantage, "Show advantage text", true, VISUAL);
			CVar(ShowKritz, "Show kritzkrieg", true, VISUAL);
			CVar(IgnoreKritz, "Treat kritz as uber", false, VISUAL);
			CVar(MedDeathDuration, "Medic death timer duration", 3.0f, VISUAL | SLIDER_MIN | SLIDER_PRECISION, 1.0f, 10.0f, 0.5f);
			CVar(EvenThresholdRange, "Even uber threshold range", 5, VISUAL | SLIDER_CLAMP, 1, 20, 1);
			CVar(MidUberThreshold, "Mid uber threshold", 40, VISUAL | SLIDER_CLAMP, 20, 80, 5);
			CVar(HighUberThreshold, "High uber threshold", 70, VISUAL | SLIDER_CLAMP, 50, 99, 5);
			CVar(UberRate, "Uber build rate", 2.5f, VISUAL | SLIDER_MIN | SLIDER_PRECISION, 1.0f, 5.0f, 0.1f);
			CVar(KritzRate, "Kritz build rate", 3.125f, VISUAL | SLIDER_MIN | SLIDER_PRECISION, 1.0f, 5.0f, 0.1f);
			CVar(KritzDropThreshold, "Kritz drop threshold", 10.0f, VISUAL | SLIDER_MIN | SLIDER_PRECISION, 5.0f, 50.0f, 1.0f);
			CVar(BoxX, "Info box X position", 5, VISUAL | SLIDER_CLAMP, 0, 1920, 5);
			CVar(BoxY, "Info box Y position", 115, VISUAL | SLIDER_CLAMP, 0, 1080, 5);
			CVar(BoxWidth, "Info box width", 360, VISUAL | SLIDER_CLAMP, 200, 800, 10);
			CVar(BoxFontSize, "Info box font size", 14, VISUAL | SLIDER_CLAMP, 10, 24, 1);
			CVar(AdvantageX, "Advantage text X offset", 0, VISUAL | SLIDER_CLAMP, -500, 500, 5);
			CVar(AdvantageY, "Advantage text Y offset", 0, VISUAL | SLIDER_CLAMP, -500, 500, 5);
			CVar(AdvantageFontSize, "Advantage font size", 16, VISUAL | SLIDER_CLAMP, 10, 24, 1);
		NAMESPACE_END(UberTracker)
NAMESPACE_BEGIN(MedicUberBar)
			CVar(ShowFriendly, "Show friendly medics", true, VISUAL);
			CVar(ShowEnemy, "Show enemy medics", true, VISUAL);
			CVar(BarWidth, "Bar width", 90, VISUAL | SLIDER_CLAMP, 50, 150, 5);
			CVar(BarHeight, "Bar height", 10, VISUAL | SLIDER_CLAMP, 5, 25, 1);
			CVar(MaxDistance, "Max distance", 2000.0f, VISUAL | SLIDER_MIN | SLIDER_PRECISION, 500.0f, 5000.0f, 100.0f);
			CVar(Alpha, "Transparency", 200, VISUAL | SLIDER_CLAMP, 50, 255, 5);
			CVar(ShowThroughWalls, "Show through walls", false, VISUAL);
			CVar(ShowPercentage, "Show percentage text", true, VISUAL);
			CVar(LowUberColor, "Low uber color (0-33%)", Color_t(255, 0, 0, 255), VISUAL);
			CVar(MidUberColor, "Mid uber color (34-66%)", Color_t(255, 255, 0, 255), VISUAL);
			CVar(HighUberColor, "High uber color (67-99%)", Color_t(0, 255, 0, 255), VISUAL);
			CVar(FullUberColor, "Full uber color (100%)", Color_t(0, 255, 255, 255), VISUAL);
			CVar(DeadColor, "Dead medic color", Color_t(128, 128, 128, 255), VISUAL);
		NAMESPACE_END(MedicUberBar)
NAMESPACE_BEGIN(PylonESP)
			CVar(PylonWidth, "Pylon width", 6, VISUAL | SLIDER_CLAMP, 2, 15, 1);
			CVar(PylonHeight, "Pylon height", 350.0f, VISUAL | SLIDER_MIN | SLIDER_PRECISION, 100.0f, 800.0f, 25.0f);
			CVar(MinDistance, "Min distance", 800.0f, VISUAL | SLIDER_MIN | SLIDER_PRECISION, 200.0f, 2000.0f, 50.0f);
			CVar(PylonColor, "Pylon color", Color_t(255, 0, 0, 255), VISUAL);
			CVar(PylonOffset, "Pylon offset", 35.0f, VISUAL | SLIDER_MIN | SLIDER_PRECISION, 0.0f, 100.0f, 5.0f);
			CVar(StartAlpha, "Start alpha", 200, VISUAL | SLIDER_CLAMP, 50, 255, 5);
			CVar(EndAlpha, "End alpha", 25, VISUAL | SLIDER_CLAMP, 0, 100, 5);
			CVar(Segments, "Segments", 10, VISUAL | SLIDER_CLAMP, 5, 20, 1);
		NAMESPACE_END(PylonESP)
NAMESPACE_BEGIN(FocusFire)
			CVar(MaxTargets, "Max targets shown", 5, VISUAL | SLIDER_CLAMP, 1, 15, 1);
			CVar(ShowHealthDrop, "Show health drop", true, VISUAL);
			CVar(MinAttackers, "Min attackers required", 2, VISUAL | SLIDER_CLAMP, 1, 10, 1);
			CVar(TrackerTimeWindow, "Tracker time window", 4.5f, VISUAL | SLIDER_MIN | SLIDER_PRECISION, 1.0f, 10.0f, 0.5f);
			CVar(EnableChams, "Enable chams", true, VISUAL);
			CVar(EnableBox, "Enable box", true, VISUAL);
			CVar(VisibleOnly, "Visible only", false, VISUAL);
			CVar(Color, "Focus color", Color_t(255, 0, 0, 190), VISUAL);
			CVar(BoxThickness, "Box thickness", 2, VISUAL | SLIDER_CLAMP, 1, 5, 1);
			CVar(BoxPadding, "Box padding", 3, VISUAL | SLIDER_CLAMP, 0, 10, 1);
			CVar(UseCorners, "Use corners", true, VISUAL);
			CVar(CornerLength, "Corner length", 10, VISUAL | SLIDER_CLAMP, 5, 20, 1);
		NAMESPACE_END(FocusFire)
NAMESPACE_BEGIN(HiderESP)
			CVar(ShowBoxes, "Show Boxes", true, VISUAL);
			CVar(ShowTracers, "Show Tracers", false, VISUAL);
			CVar(BoxColor, "Box Color", Color_t(255, 165, 0, 255), VISUAL);
			CVar(TracerColor, "Tracer Color", Color_t(0, 0, 255, 255), VISUAL);
			CVar(TimeToMark, "Time to Mark (seconds)", 5.5f, VISUAL | SLIDER_MIN | SLIDER_PRECISION, 1.0f, 20.0f, 0.5f);
			CVar(MovementThreshold, "Movement Threshold", 1.0f, VISUAL | SLIDER_MIN | SLIDER_PRECISION, 0.1f, 10.0f, 0.1f);
			CVar(MaxDistance, "Max Distance", 1650.0f, VISUAL | SLIDER_MIN | SLIDER_PRECISION, 100.0f, 5000.0f, 50.0f);
		NAMESPACE_END(HiderESP)
namespace Features { inline auto& HealthBarESP = Vars::HealthOverlay::Enabled; }
NAMESPACE_BEGIN(HealthBarESP)
			CVar(Alpha, "Transparency", 70, VISUAL | SLIDER_CLAMP, 0, 255, 5);
			CVar(BarHeight, "Bar height", 10, VISUAL | SLIDER_CLAMP, 5, 25, 1);
			CVar(BarWidth, "Bar width", 90, VISUAL | SLIDER_CLAMP, 50, 150, 5);
inline auto& MaxDistance = Vars::HealthOverlay::MaxDistance;
inline auto& MedicMode = Vars::HealthOverlay::MedicMode;
			CVar(OverhealColor, "Overheal color", Color_t(71, 166, 255, 255), VISUAL);
inline auto& ShowHealthBars = Vars::HealthOverlay::Bars;
inline auto& ShowThroughWalls = Vars::HealthOverlay::ThroughWalls;
inline auto& ShowPolygons = Vars::HealthOverlay::Polygons;
			CVar(PolygonThroughWalls, "Show polygons through walls", false, VISUAL);
			CVar(PolygonRadius, "Polygon radius", 50.0f, VISUAL | SLIDER_MIN | SLIDER_PRECISION, 20.0f, 200.0f, 5.0f);
			CVar(PolygonSegments, "Polygon segments", 16, VISUAL | SLIDER_CLAMP, 8, 64, 1);
			CVar(ShowPolygonFill, "Show polygon fill", true, VISUAL);
			CVar(ShowPolygonEdge, "Show polygon edge", true, VISUAL);
NAMESPACE_END(HealthBarESP)
namespace Features { inline auto& PlayerTrails = Vars::PlayerTrails::Enabled; }
NAMESPACE_BEGIN(PlayerTrails)
			CVar(MaxTrailLength, "Max trail length", 19, VISUAL | SLIDER_CLAMP, 10, 50, 1);
inline auto& TrailLifetime = Vars::PlayerTrails::Duration;
inline auto& MaxDistance = Vars::PlayerTrails::MaxDistance;
			CVar(UpdateInterval, "Update interval", 6, VISUAL | SLIDER_CLAMP, 1, 20, 1);
			CVar(VisibilityTimeout, "Visibility timeout", 4.0f, VISUAL | SLIDER_MIN | SLIDER_PRECISION, 1.0f, 10.0f, 0.5f);
			CVar(MaxVisibleDuration, "Max visible duration", 2.0f, VISUAL | SLIDER_MIN | SLIDER_PRECISION, 0.5f, 10.0f, 0.5f);
			CVar(TrailHeight, "Trail height", 1.5f, VISUAL | SLIDER_MIN | SLIDER_PRECISION, 0.0f, 10.0f, 0.5f);
			CVar(MinMovementDistance, "Min movement distance", 10.0f, VISUAL | SLIDER_MIN | SLIDER_PRECISION, 5.0f, 50.0f, 1.0f);
NAMESPACE_END(PlayerTrails)
namespace Features { inline auto& AmmoTracker = Vars::AmmoTracker::Enabled; }
NAMESPACE_BEGIN(AmmoTracker)
			CVar(ShowGroundPieChart, "Show ground pie charts", false, VISUAL);
			CVar(ShowScreenPieChart, "Show screen pie charts", true, VISUAL);
			CVar(ShowSeconds, "Show seconds timer", false, VISUAL);
			CVar(ShowMilliseconds, "Show milliseconds", false, VISUAL);
			CVar(ShowThroughWalls, "Show through walls", false, VISUAL);
			CVar(ShowHudOverlay, "Show HUD overlay when standing on supplies", true, VISUAL);
			CVar(ScaleWithDistance, "Scale with distance", true, VISUAL);
			CVar(ScaleTextWithDistance, "Scale text with distance", true, VISUAL);
			CVar(MaxDistanceGroundPieChart, "Max distance for ground charts", 1000.0f, VISUAL | SLIDER_MIN | SLIDER_PRECISION, 100.0f, 3000.0f, 50.0f);
			CVar(MaxDistanceScreenPieChart, "Max distance for screen charts", 1000.0f, VISUAL | SLIDER_MIN | SLIDER_PRECISION, 100.0f, 3000.0f, 50.0f);
inline auto& MaxDistanceText = Vars::AmmoTracker::MaxDistance;
			CVar(PieChartSize, "Pie chart size", 20, VISUAL | SLIDER_CLAMP, 10, 50, 2);
			CVar(PieChartSegments, "Pie chart segments", 32, VISUAL | SLIDER_CLAMP, 8, 64, 4);
			CVar(MinScreenChartSize, "Min screen chart size", 5, VISUAL | SLIDER_CLAMP, 3, 15, 1);
			CVar(MaxScreenChartSize, "Max screen chart size", 20, VISUAL | SLIDER_CLAMP, 10, 50, 2);
			CVar(MaxScaleDistance, "Max scale distance", 1000.0f, VISUAL | SLIDER_MIN | SLIDER_PRECISION, 500.0f, 2000.0f, 50.0f);
			CVar(TextSizeMin, "Min text size", 12, VISUAL | SLIDER_CLAMP, 8, 20, 1);
			CVar(TextSizeMax, "Max text size", 20, VISUAL | SLIDER_CLAMP, 14, 30, 1);
			CVar(TextSizeDefault, "Default text size", 16, VISUAL | SLIDER_CLAMP, 10, 24, 1);
			CVar(HealthColor, "Health pack color", Color_t(155, 200, 155, 200), VISUAL);
			CVar(AmmoColor, "Ammo pack color", Color_t(255, 255, 155, 200), VISUAL);
			CVar(SecondsColor, "Timer text color", Color_t(255, 255, 255, 255), VISUAL);
NAMESPACE_END(AmmoTracker)
namespace Features { inline auto& SentryESP = Vars::SentryESP::Enabled; }
NAMESPACE_BEGIN(SentryESP)
			CVar(ShowLine, "Show aim line", true, VISUAL);
			CVar(ShowBoxes, "Show ESP boxes", true, VISUAL);
			CVar(ShowChams, "Show chams on targeting sentries", true, VISUAL);
inline auto& ShowThroughWalls = Vars::SentryESP::ThroughWalls;
			CVar(ShowFriendly, "Show friendly sentries", false, VISUAL);
			CVar(UseCorners, "Use corner boxes", true, VISUAL);
			CVar(CornerLength, "Corner length", 10, VISUAL | SLIDER_CLAMP, 5, 25, 1);
			CVar(BoxThickness, "Box thickness", 2, VISUAL | SLIDER_CLAMP, 1, 5, 1);
			CVar(LineLength, "Aim line length", 1100.0f, VISUAL | SLIDER_MIN | SLIDER_PRECISION, 200.0f, 2000.0f, 50.0f);
			CVar(SafeColor, "Safe color (visible)", Color_t(0, 255, 0, 125), VISUAL);
			CVar(DangerColor, "Danger color (targeting)", Color_t(255, 0, 0, 255), VISUAL);
			CVar(HiddenColor, "Hidden color (not visible)", Color_t(150, 150, 150, 125), VISUAL);
			CVar(LineColor, "Aim line color", Color_t(255, 0, 0, 255), VISUAL);
			CVar(TextColor, "Level text color", Color_t(255, 255, 255, 255), VISUAL);
NAMESPACE_END(SentryESP)
namespace Features { inline auto& StickyESP = Vars::StickyESP::Enabled; }
NAMESPACE_BEGIN(StickyESP)
inline auto& MaxDistance = Vars::StickyESP::MaxDistance;
			CVar(BoxSize, "Box size", 20, VISUAL | SLIDER_CLAMP, 10, 50, 2);
inline auto& EnemyOnly = Vars::StickyESP::EnemyOnly;
inline auto& Box3D = Vars::StickyESP::Box3D;
			CVar(Box2D, "2D boxes", true, VISUAL);
			CVar(BoxOnlyWhenVisible, "Only show when visible", true, VISUAL);
			CVar(EnemyVisibleColor, "Enemy visible color", Color_t(255, 100, 100, 255), VISUAL);
			CVar(EnemyInvisibleColor, "Enemy invisible color", Color_t(255, 0, 0, 255), VISUAL);
			CVar(TeamVisibleColor, "Team visible color", Color_t(100, 100, 255, 255), VISUAL);
			CVar(TeamInvisibleColor, "Team invisible color", Color_t(0, 0, 255, 255), VISUAL);
			CVar(EnableChams, "Enable chams", false, VISUAL);
NAMESPACE_END(StickyESP)
NAMESPACE_BEGIN(Radar)

		NAMESPACE_BEGIN(Main)
			inline auto& Enabled=Vars::Radar::Enabled;
			CVar(DrawOutOfRange, "Draw out of range", true, VISUAL);
			CVarEnum(Style, VA_LIST("Style", "Radar style"), 0, VISUAL, nullptr,
				VA_LIST("Circle", "Rectangle"),
				Circle, Rectangle);
			CVar(Window, "Radar window", WindowBox_t(), VISUAL | NOBIND);
			inline auto& Range=Vars::Radar::Range;
			CVar(BackgroundAlpha, VA_LIST("Background alpha", "Radar background alpha"), 128, VISUAL | SLIDER_CLAMP, 0, 255, 5);
			CVar(LineAlpha, VA_LIST("Line alpha", "Radar line alpha"), 255, VISUAL | SLIDER_CLAMP, 0, 255, 5);
		}

		NAMESPACE_BEGIN(Player)
			inline auto& Enabled=Vars::Radar::Players;
			CVar(Background, VA_LIST("Background", "Radar player background"), true, VISUAL);
			CVarEnum(Draw, VA_LIST("Draw", "Radar player draw"), 0b1001010, VISUAL | DROPDOWN_MULTI, nullptr,
				VA_LIST("Local", "Enemy", "Team", "Friends", "Party", "Prioritized", "Cloaked"),
				Local = 1 << 0, Enemy = 1 << 1, Team = 1 << 2, Prioritized = 1 << 3, Friends = 1 << 4, Party = 1 << 5, Cloaked = 1 << 6);
			CVarEnum(Icon, VA_LIST("Icon", "Radar player icon"), 1, VISUAL, nullptr,
				VA_LIST("Icons", "Portraits", "Avatar"),
				Icons, Portraits, Avatars);
			CVar(Size, VA_LIST("Size", "Radar player size"), 24, VISUAL, 12, 30, 2);
			CVar(Health, VA_LIST("Health bar", "Radar player health bar"), false, VISUAL);
			CVar(Height, VA_LIST("Height indicator", "Radar player height indicator"), false, VISUAL);
		}

		NAMESPACE_BEGIN(Building)
			inline auto& Enabled=Vars::Radar::Buildings;
			CVar(Background, VA_LIST("Background", "Radar building background"), true, VISUAL);
			CVarEnum(Draw, VA_LIST("Draw", "Radar building draw"), 0b001011, VISUAL | DROPDOWN_MULTI, nullptr,
				VA_LIST("Local", "Enemy", "Team", "Friends", "Party", "Prioritized"),
				Local = 1 << 0, Enemy = 1 << 1, Team = 1 << 2, Prioritized = 1 << 3, Friends = 1 << 4, Party = 1 << 5);
			CVar(Size, VA_LIST("Size", "Radar building size"), 18, VISUAL, 12, 30, 2);
			CVar(Health, VA_LIST("Health bar", "Radar building health bar"), false, VISUAL);
		}

		NAMESPACE_BEGIN(World)
			inline auto& Enabled=Vars::Radar::Pickups;
			CVar(Background, VA_LIST("Background", "Radar world background"), true, VISUAL);
			CVarEnum(Draw, VA_LIST("Draw", "Radar world draw"), 0b0000011, VISUAL | DROPDOWN_MULTI, nullptr,
				VA_LIST("Health", "Ammo", "Money", "Bombs", "Powerup", "Spellbook", "Gargoyle"),
				Health = 1 << 0, Ammo = 1 << 1, Money = 1 << 2, Bombs = 1 << 3, Powerup = 1 << 4, Spellbook = 1 << 5, Gargoyle = 1 << 6);
			CVar(Size, VA_LIST("Size", "Radar world size"), 14, VISUAL, 12, 30, 2);
		}

NAMESPACE_END(Radar)
NAMESPACE_BEGIN(Palette)
CVar(NPC, "NPC color", Color_t(255, 255, 0, 255), VISUAL);
		CVar(Relative, "Relative colors", false, VISUAL);
		CVar(TeamRed, "RED color", Color_t(225, 60, 60, 255), VISUAL);
		CVar(TeamBlu, "BLU color", Color_t(75, 175, 225, 255), VISUAL);
		CVar(Enemy, "Enemy color", Color_t(225, 60, 60, 255), VISUAL);
		CVar(Team, "Team color", Color_t(75, 175, 225, 255), VISUAL);
		CVar(Local, "Local color", Color_t(255, 255, 255, 255), VISUAL);
		CVar(Target, "Target color", Color_t(255, 0, 0, 255), VISUAL);
		CVar(Health, "Health color", Color_t(0, 225, 75, 255), VISUAL);
		CVar(Ammo, "Ammo color", Color_t(127, 127, 127, 255), VISUAL);
		CVar(Money, "Money color", Color_t(0, 150, 75, 255), VISUAL);
		CVar(Powerup, "Powerup color", Color_t(255, 175, 0, 255), VISUAL);
		CVar(Halloween, "Halloween color", Color_t(100, 0, 255, 255), VISUAL);
		CVar(IndicatorGood, "Indicator good", Color_t(0, 255, 100, 255), NOSAVE | DEBUGVAR);
		CVar(IndicatorBad, "Indicator bad", Color_t(255, 0, 0, 255), NOSAVE | DEBUGVAR);
		CVar(IndicatorMisc, "Indicator misc", Color_t(75, 175, 255, 255), NOSAVE | DEBUGVAR);
NAMESPACE_END(Palette)
namespace Features {
inline auto& OffScreenIndicators=Vars::OffScreenIndicators::Enabled;
			CVar(OffScreenIndicatorsName, "Show player names", true, VISUAL);
			CVar(OffScreenIndicatorsClass, "Show player classes", true, VISUAL);
			CVar(OffScreenIndicatorsHealth, "Show player health", true, VISUAL);
			CVar(OffScreenIndicatorsDistance, "Show distance", false, VISUAL);
inline auto& OffScreenIndicatorsRange=Vars::OffScreenIndicators::Radius;
			CVar(OffScreenIndicatorsSize, "Arrow size", 15, VISUAL | SLIDER_CLAMP, 5, 30, 1);
			CVar(OffScreenIndicatorsAlpha, "Text transparency", 255, VISUAL | SLIDER_CLAMP, 100, 255, 5);
			CVar(OffScreenIndicatorsAvatars, "Use Steam avatars", true, VISUAL);
}
NAMESPACE_BEGIN(EnemyCam)
			CVarEnum(Mode, "Camera mode", 0, VISUAL, nullptr,
				VA_LIST("Closest", "Healed", "Medic", "Top Score", "Random"),
				Closest, Healed, Medic, TopScore, Random);
			CVarEnum(ViewMode, "View mode", 1, VISUAL, nullptr,
				VA_LIST("Raw", "Offset"),
				Raw, Offset);
			CVar(TrackTime, "Track time", 3.0f, VISUAL | SLIDER_MIN | SLIDER_PRECISION, 0.0f, 10.0f, 0.5f);
			CVar(OffsetX, "Offset X (forward/back)", 50.0f, VISUAL | SLIDER_MIN | SLIDER_PRECISION, -200.0f, 200.0f, 5.0f);
			CVar(OffsetY, "Offset Y (up/down)", 37.5f, VISUAL | SLIDER_MIN | SLIDER_PRECISION, -200.0f, 200.0f, 5.0f);
			CVar(WindowX, "Window X position", -1, VISUAL | SLIDER_CLAMP, -1, 1920, 5);
			CVar(WindowY, "Window Y position", -1, VISUAL | SLIDER_CLAMP, -1, 1080, 5);
			CVar(WindowWidth, "Window width", 320, VISUAL | SLIDER_CLAMP, 160, 640, 10);
			CVar(WindowHeight, "Window height", 240, VISUAL | SLIDER_CLAMP, 120, 480, 10);

NAMESPACE_END(EnemyCam)
NAMESPACE_END(Comp)
}
