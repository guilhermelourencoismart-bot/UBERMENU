// Full Comp module controls, using the original menu components.
static void FullCompSettings()
{
if (Section("UberTracker - Comp")) {
FToggle(Vars::Comp::Features::UberTracker);
FToggle(Vars::Comp::UberTracker::ShowTopBox);
FToggle(Vars::Comp::UberTracker::ShowAdvantage);
FToggle(Vars::Comp::UberTracker::ShowKritz);
FToggle(Vars::Comp::UberTracker::IgnoreKritz);
FSlider(Vars::Comp::UberTracker::MedDeathDuration);
FSlider(Vars::Comp::UberTracker::EvenThresholdRange);
FSlider(Vars::Comp::UberTracker::MidUberThreshold);
FSlider(Vars::Comp::UberTracker::HighUberThreshold);
FSlider(Vars::Comp::UberTracker::UberRate);
FSlider(Vars::Comp::UberTracker::KritzRate);
FSlider(Vars::Comp::UberTracker::KritzDropThreshold);
FSlider(Vars::Comp::UberTracker::BoxX);
FSlider(Vars::Comp::UberTracker::BoxY);
FSlider(Vars::Comp::UberTracker::BoxWidth);
FSlider(Vars::Comp::UberTracker::BoxFontSize);
FSlider(Vars::Comp::UberTracker::AdvantageX);
FSlider(Vars::Comp::UberTracker::AdvantageY);
FSlider(Vars::Comp::UberTracker::AdvantageFontSize);
} EndSection();
if (Section("MedicUberBar - Comp")) {
FToggle(Vars::Comp::Features::MedicUberBar);
FToggle(Vars::Comp::MedicUberBar::ShowFriendly);
FToggle(Vars::Comp::MedicUberBar::ShowEnemy);
FSlider(Vars::Comp::MedicUberBar::BarWidth);
FSlider(Vars::Comp::MedicUberBar::BarHeight);
FSlider(Vars::Comp::MedicUberBar::MaxDistance);
FSlider(Vars::Comp::MedicUberBar::Alpha);
FToggle(Vars::Comp::MedicUberBar::ShowThroughWalls);
FToggle(Vars::Comp::MedicUberBar::ShowPercentage);
FColorPicker(Vars::Comp::MedicUberBar::LowUberColor);
FColorPicker(Vars::Comp::MedicUberBar::MidUberColor);
FColorPicker(Vars::Comp::MedicUberBar::HighUberColor);
FColorPicker(Vars::Comp::MedicUberBar::FullUberColor);
FColorPicker(Vars::Comp::MedicUberBar::DeadColor);
} EndSection();
if (Section("PylonESP - Comp")) {
FToggle(Vars::Comp::Features::PylonESP);
FSlider(Vars::Comp::PylonESP::PylonWidth);
FSlider(Vars::Comp::PylonESP::PylonHeight);
FSlider(Vars::Comp::PylonESP::MinDistance);
FColorPicker(Vars::Comp::PylonESP::PylonColor);
FSlider(Vars::Comp::PylonESP::PylonOffset);
FSlider(Vars::Comp::PylonESP::StartAlpha);
FSlider(Vars::Comp::PylonESP::EndAlpha);
FSlider(Vars::Comp::PylonESP::Segments);
} EndSection();
if (Section("FocusFire - Comp")) {
FToggle(Vars::Comp::Features::FocusFire);
FSlider(Vars::Comp::FocusFire::MaxTargets);
FToggle(Vars::Comp::FocusFire::ShowHealthDrop);
FSlider(Vars::Comp::FocusFire::MinAttackers);
FSlider(Vars::Comp::FocusFire::TrackerTimeWindow);
FToggle(Vars::Comp::FocusFire::EnableChams);
FToggle(Vars::Comp::FocusFire::EnableBox);
FToggle(Vars::Comp::FocusFire::VisibleOnly);
FColorPicker(Vars::Comp::FocusFire::Color);
FSlider(Vars::Comp::FocusFire::BoxThickness);
FSlider(Vars::Comp::FocusFire::BoxPadding);
FToggle(Vars::Comp::FocusFire::UseCorners);
FSlider(Vars::Comp::FocusFire::CornerLength);
} EndSection();
if (Section("HiderESP - Comp")) {
FToggle(Vars::Comp::Features::HiderESP);
FToggle(Vars::Comp::HiderESP::ShowBoxes);
FToggle(Vars::Comp::HiderESP::ShowTracers);
FColorPicker(Vars::Comp::HiderESP::BoxColor);
FColorPicker(Vars::Comp::HiderESP::TracerColor);
FSlider(Vars::Comp::HiderESP::TimeToMark);
FSlider(Vars::Comp::HiderESP::MovementThreshold);
FSlider(Vars::Comp::HiderESP::MaxDistance);
} EndSection();
}
