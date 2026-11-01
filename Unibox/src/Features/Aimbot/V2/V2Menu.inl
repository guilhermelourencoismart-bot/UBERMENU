// V2-specific settings alongside the corresponding native weapon categories.
static void V2SolverSettings() {
FDropdown(Vars::Aimbot::General::Solver);
FTooltip("Forward track, lob angles, dormant targets and exclusive weapons retain Unibox compatibility. Disable those extensions to select V2 for normal weapons.");
FDropdown(Vars::Aimbot::General::BypassIgnore);
FToggle(Vars::Aimbot::General::PrioritizeFollowbot);
FToggle(Vars::Aimbot::General::PrioritizeNavbot);
FSlider(Vars::Aimbot::General::ShootDelayMax);
FSlider(Vars::Aimbot::General::ShootDelayMin);
}
static void V2MeleeSettings() {
FToggle(Vars::Aimbot::Melee::BackstabAccountPing);
FToggle(Vars::Aimbot::Melee::BackstabDoubleTest);
FSlider(Vars::Aimbot::Melee::SwingOffset);
}
static void V2ProjectileSettings() {
FSlider(Vars::Aimbot::Projectile::Out2NormalMax);
FSlider(Vars::Aimbot::Projectile::Out2NormalMin);
FDropdown(Vars::Aimbot::Projectile::RocketSplashMode);
FSlider(Vars::Aimbot::Projectile::SplashCountArc);
FSlider(Vars::Aimbot::Projectile::SplashCountDirect);
FToggle(Vars::Aimbot::Projectile::SplashGrates);
FSlider(Vars::Aimbot::Projectile::SplashNormalSkip);
}
