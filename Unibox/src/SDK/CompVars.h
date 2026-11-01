#pragma once
// Comp settings registered by the existing Unibox ConfigVar/bind system.
namespace Vars {
NAMESPACE_BEGIN(Comp)
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
NAMESPACE_END(Comp)
}
