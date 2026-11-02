// Controls use the native menu and ConfigVar registry; only the selected panel is drawn.
static int s_CompRequestedModule = -1;
static void FullCompSettings()
{
 static int selected=0,category=0;
 const std::vector<std::vector<int>> groups={{0,16},{6,8,11,20,3},{5,7,17,19,18,2},{12,13,14,15},{4,9,10,1}};
 if (s_CompRequestedModule >= 0 && s_CompRequestedModule <= 20) {
  selected=s_CompRequestedModule;
  for(size_t i=0;i<groups.size();++i)
   if(std::find(groups[i].begin(),groups[i].end(),selected)!=groups[i].end())category=int(i);
  s_CompRequestedModule=-1;
 }
 const std::vector<const char*> names={"ESP principal","Retorno de suprimentos","Risco de queda","Cura crítica","Câmera de inimigos","Fogo concentrado","Barras de vida","Jogadores parados","Barras de Über","Jogadores fora da tela","Rastros de jogadores","Médicos ocultos","Radar: construções","Radar: aparência","Radar: jogadores","Radar: itens","Paleta do ESP e Radar","Torres automáticas","Área de explosão","Stickies","Resumo de Über"};
 if(Section("Módulos Comp")) {
  FDropdown("Categoria",&category,{"ESP e cores","Vida e médicos","Ameaças e combate","Radar","Acompanhamento e itens"});
  const auto& values=groups.at(category);
  if(std::find(values.begin(),values.end(),selected)==values.end())selected=values.front();
  std::vector<const char*> entries;for(int index:values)entries.push_back(names[index]);
  FDropdown("Módulo",&selected,entries,values);
  FLabel("Base: Unibox — GatoPotato658");
  FLabel("github.com/GatoPotato658/Unibox");
 } EndSection();
 TableNextColumn();
 switch(selected) {
 case 0:
if(Section("Principal ESP - Comp")) {
 FToggle(Vars::Comp::ESP::Enabled);
 FDropdown(Vars::Comp::ESP::Draw);
 FDropdown(Vars::Comp::ESP::Player);
 FDropdown(Vars::Comp::ESP::Building);
 FDropdown(Vars::Comp::ESP::Projectile);
 FDropdown(Vars::Comp::ESP::Objective);
 FSlider(Vars::Comp::ESP::ActiveAlpha);
 FSlider(Vars::Comp::ESP::DormantAlpha);
 FSlider(Vars::Comp::ESP::DormantDuration);
 FToggle(Vars::Comp::ESP::DormantPriority);
 FColorPicker(Vars::Comp::Palette::NPC);
} EndSection();
 break;
 case 1:
if(Section("AmmoTracker - Comp")) {
FToggle(Vars::Comp::Features::AmmoTracker);
FToggle(Vars::Comp::AmmoTracker::ShowGroundPieChart);
FToggle(Vars::Comp::AmmoTracker::ShowScreenPieChart);
FToggle(Vars::Comp::AmmoTracker::ShowSeconds);
FToggle(Vars::Comp::AmmoTracker::ShowMilliseconds);
FToggle(Vars::Comp::AmmoTracker::ShowThroughWalls);
FToggle(Vars::Comp::AmmoTracker::ShowHudOverlay);
FToggle(Vars::Comp::AmmoTracker::ScaleWithDistance);
FToggle(Vars::Comp::AmmoTracker::ScaleTextWithDistance);
FSlider(Vars::Comp::AmmoTracker::MaxDistanceGroundPieChart);
FSlider(Vars::Comp::AmmoTracker::MaxDistanceScreenPieChart);
FSlider(Vars::Comp::AmmoTracker::MaxDistanceText);
FSlider(Vars::Comp::AmmoTracker::PieChartSize);
FSlider(Vars::Comp::AmmoTracker::PieChartSegments);
FSlider(Vars::Comp::AmmoTracker::MinScreenChartSize);
FSlider(Vars::Comp::AmmoTracker::MaxScreenChartSize);
FSlider(Vars::Comp::AmmoTracker::MaxScaleDistance);
FSlider(Vars::Comp::AmmoTracker::TextSizeMin);
FSlider(Vars::Comp::AmmoTracker::TextSizeMax);
FSlider(Vars::Comp::AmmoTracker::TextSizeDefault);
FColorPicker(Vars::Comp::AmmoTracker::HealthColor);
FColorPicker(Vars::Comp::AmmoTracker::AmmoColor);
FColorPicker(Vars::Comp::AmmoTracker::SecondsColor);
} EndSection();
 break;
 case 2:
if(Section("CraterCheck - Comp")) {
FToggle(Vars::Comp::Features::CraterCheck);
FSlider(Vars::Comp::CraterCheck::Radius);
FSlider(Vars::Comp::CraterCheck::Segments);
FToggle(Vars::Comp::CraterCheck::ShowFill);
FToggle(Vars::Comp::CraterCheck::ShowEdge);
FSlider(Vars::Comp::CraterCheck::EdgeWidth);
FToggle(Vars::Comp::CraterCheck::OnlyShowLethal);
FColorPicker(Vars::Comp::CraterCheck::LethalFillColor);
FColorPicker(Vars::Comp::CraterCheck::LethalEdgeColor);
FColorPicker(Vars::Comp::CraterCheck::SafeFillColor);
FColorPicker(Vars::Comp::CraterCheck::SafeEdgeColor);
} EndSection();
 break;
 case 3:
if(Section("CritHeals - Comp")) {
FToggle(Vars::Comp::Features::CritHeals);
FSlider(Vars::Comp::CritHeals::Range);
FSlider(Vars::Comp::CritHeals::CritHealTime);
FSlider(Vars::Comp::CritHeals::MaxOverhealMultiplier);
FSlider(Vars::Comp::CritHeals::UberPenaltyThreshold);
FSlider(Vars::Comp::CritHeals::BuffThreshold);
FSlider(Vars::Comp::CritHeals::BuffFadeThreshold);
FSlider(Vars::Comp::CritHeals::TriangleSize);
FSlider(Vars::Comp::CritHeals::TriangleVerticalOffset);
FSlider(Vars::Comp::CritHeals::OutlineThickness);
FColorPicker(Vars::Comp::CritHeals::FriendColor);
FColorPicker(Vars::Comp::CritHeals::EnemyColor);
FColorPicker(Vars::Comp::CritHeals::OutlineColor);
FColorPicker(Vars::Comp::CritHeals::WarningColor);
FToggle(Vars::Comp::CritHeals::UberBuildWarning);
FToggle(Vars::Comp::CritHeals::ShowOnEnemies);
} EndSection();
 break;
 case 4:
if(Section("Inimigo câmera - Comp")) {
FToggle(Vars::EnemyCam::Enabled);
FToggle(Vars::EnemyCam::VisibleOnly);
FSlider(Vars::EnemyCam::MaxDistance);
FDropdown(Vars::Comp::EnemyCam::Mode);
FDropdown(Vars::Comp::EnemyCam::ViewMode);
FSlider(Vars::Comp::EnemyCam::TrackTime);
FSlider(Vars::Comp::EnemyCam::OffsetX);
FSlider(Vars::Comp::EnemyCam::OffsetY);
FSlider(Vars::Comp::EnemyCam::WindowX);
FSlider(Vars::Comp::EnemyCam::WindowY);
FSlider(Vars::Comp::EnemyCam::WindowWidth);
FSlider(Vars::Comp::EnemyCam::WindowHeight);
} EndSection();
 break;
 case 5:
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
 break;
 case 6:
if(Section("HealthBarESP - Comp")) {
FToggle(Vars::Comp::Features::HealthBarESP);
FSlider(Vars::Comp::HealthBarESP::Alpha);
FSlider(Vars::Comp::HealthBarESP::BarHeight);
FSlider(Vars::Comp::HealthBarESP::BarWidth);
FSlider(Vars::Comp::HealthBarESP::MaxDistance);
FToggle(Vars::Comp::HealthBarESP::MedicMode);
FColorPicker(Vars::Comp::HealthBarESP::OverhealColor);
FToggle(Vars::Comp::HealthBarESP::ShowHealthBars);
FToggle(Vars::Comp::HealthBarESP::ShowThroughWalls);
FToggle(Vars::Comp::HealthBarESP::ShowPolygons);
FToggle(Vars::Comp::HealthBarESP::PolygonThroughWalls);
FSlider(Vars::Comp::HealthBarESP::PolygonRadius);
FSlider(Vars::Comp::HealthBarESP::PolygonSegments);
FToggle(Vars::Comp::HealthBarESP::ShowPolygonFill);
FToggle(Vars::Comp::HealthBarESP::ShowPolygonEdge);
} EndSection();
 break;
 case 7:
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
 break;
 case 8:
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
 break;
 case 9:
if(Section("Offscreen jogadores - Comp")) {
FToggle(Vars::Comp::Features::OffScreenIndicators);
FToggle(Vars::Comp::Features::OffScreenIndicatorsName);
FToggle(Vars::Comp::Features::OffScreenIndicatorsClass);
FToggle(Vars::Comp::Features::OffScreenIndicatorsHealth);
FToggle(Vars::Comp::Features::OffScreenIndicatorsDistance);
FSlider(Vars::Comp::Features::OffScreenIndicatorsRange);
FSlider(Vars::Comp::Features::OffScreenIndicatorsSize);
FSlider(Vars::Comp::Features::OffScreenIndicatorsAlpha);
FToggle(Vars::Comp::Features::OffScreenIndicatorsAvatars);
} EndSection();
 break;
 case 10:
if(Section("PlayerTrails - Comp")) {
FToggle(Vars::Comp::Features::PlayerTrails);
FSlider(Vars::Comp::PlayerTrails::MaxTrailLength);
FSlider(Vars::Comp::PlayerTrails::TrailLifetime);
FSlider(Vars::Comp::PlayerTrails::MaxDistance);
FSlider(Vars::Comp::PlayerTrails::UpdateInterval);
FSlider(Vars::Comp::PlayerTrails::VisibilityTimeout);
FSlider(Vars::Comp::PlayerTrails::MaxVisibleDuration);
FSlider(Vars::Comp::PlayerTrails::TrailHeight);
FSlider(Vars::Comp::PlayerTrails::MinMovementDistance);
} EndSection();
 break;
 case 11:
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
 break;
 case 12:
if(Section("Radar construção - Comp")) {
FToggle(Vars::Comp::Radar::Building::Enabled);
FToggle(Vars::Comp::Radar::Building::Background);
FDropdown(Vars::Comp::Radar::Building::Draw);
FSlider(Vars::Comp::Radar::Building::Size);
FToggle(Vars::Comp::Radar::Building::Health);
} EndSection();
 break;
 case 13:
if(Section("Radar principal - Comp")) {
FToggle(Vars::Comp::Radar::Main::Enabled);
FToggle(Vars::Comp::Radar::Main::DrawOutOfRange);
FDropdown(Vars::Comp::Radar::Main::Style);
FSlider(Vars::Comp::Radar::Main::Range);
FSlider(Vars::Comp::Radar::Main::BackgroundAlpha);
FSlider(Vars::Comp::Radar::Main::LineAlpha);
} EndSection();
 break;
 case 14:
if(Section("Radar jogador - Comp")) {
FToggle(Vars::Comp::Radar::Player::Enabled);
FToggle(Vars::Comp::Radar::Player::Background);
FDropdown(Vars::Comp::Radar::Player::Draw);
FDropdown(Vars::Comp::Radar::Player::Icon);
FSlider(Vars::Comp::Radar::Player::Size);
FToggle(Vars::Comp::Radar::Player::Health);
FToggle(Vars::Comp::Radar::Player::Height);
} EndSection();
 break;
 case 15:
if(Section("Radar mundo - Comp")) {
FToggle(Vars::Comp::Radar::World::Enabled);
FToggle(Vars::Comp::Radar::World::Background);
FDropdown(Vars::Comp::Radar::World::Draw);
FSlider(Vars::Comp::Radar::World::Size);
} EndSection();
 break;
 case 16:
if(Section("Radar cores - Comp")) {
FToggle(Vars::Comp::Palette::Relative);
FColorPicker(Vars::Comp::Palette::TeamRed);
FColorPicker(Vars::Comp::Palette::TeamBlu);
FColorPicker(Vars::Comp::Palette::Enemy);
FColorPicker(Vars::Comp::Palette::Team);
FColorPicker(Vars::Comp::Palette::Local);
FColorPicker(Vars::Comp::Palette::Target);
FColorPicker(Vars::Comp::Palette::Health);
FColorPicker(Vars::Comp::Palette::Ammo);
FColorPicker(Vars::Comp::Palette::Money);
FColorPicker(Vars::Comp::Palette::Powerup);
FColorPicker(Vars::Comp::Palette::Halloween);
FColorPicker(Vars::Comp::Palette::IndicatorGood);
FColorPicker(Vars::Comp::Palette::IndicatorBad);
FColorPicker(Vars::Comp::Palette::IndicatorMisc);
} EndSection();
 break;
 case 17:
if(Section("SentryESP - Comp")) {
FToggle(Vars::Comp::Features::SentryESP);
FToggle(Vars::Comp::SentryESP::ShowLine);
FToggle(Vars::Comp::SentryESP::ShowBoxes);
FToggle(Vars::Comp::SentryESP::ShowChams);
FToggle(Vars::Comp::SentryESP::ShowThroughWalls);
FToggle(Vars::Comp::SentryESP::ShowFriendly);
FToggle(Vars::Comp::SentryESP::UseCorners);
FSlider(Vars::Comp::SentryESP::CornerLength);
FSlider(Vars::Comp::SentryESP::BoxThickness);
FSlider(Vars::Comp::SentryESP::LineLength);
FColorPicker(Vars::Comp::SentryESP::SafeColor);
FColorPicker(Vars::Comp::SentryESP::DangerColor);
FColorPicker(Vars::Comp::SentryESP::HiddenColor);
FColorPicker(Vars::Comp::SentryESP::LineColor);
FColorPicker(Vars::Comp::SentryESP::TextColor);
} EndSection();
 break;
 case 18:
if(Section("SplashRadius - Comp")) {
FToggle(Vars::Comp::Features::SplashRadius);
FColorPicker(Vars::Comp::SplashRadius::FillColor);
FColorPicker(Vars::Comp::SplashRadius::EdgeColor);
FSlider(Vars::Comp::SplashRadius::Segments);
FToggle(Vars::Comp::SplashRadius::ShowFill);
FToggle(Vars::Comp::SplashRadius::ShowEdge);
FSlider(Vars::Comp::SplashRadius::EdgeWidth);
FToggle(Vars::Comp::SplashRadius::EnemyOnly);
FToggle(Vars::Comp::SplashRadius::ShowRockets);
FToggle(Vars::Comp::SplashRadius::ShowPipebombs);
FToggle(Vars::Comp::SplashRadius::ShowStickybombs);
FToggle(Vars::Comp::SplashRadius::MergeOverlapping);
FToggle(Vars::Comp::SplashRadius::UseLOD);
FToggle(Vars::Comp::SplashRadius::TeamColors);
FToggle(Vars::Comp::SplashRadius::ShowOnlyVisible);
} EndSection();
 break;
 case 19:
if(Section("StickyESP - Comp")) {
FToggle(Vars::Comp::Features::StickyESP);
FSlider(Vars::Comp::StickyESP::MaxDistance);
FSlider(Vars::Comp::StickyESP::BoxSize);
FToggle(Vars::Comp::StickyESP::EnemyOnly);
FToggle(Vars::Comp::StickyESP::Box3D);
FToggle(Vars::Comp::StickyESP::Box2D);
FToggle(Vars::Comp::StickyESP::BoxOnlyWhenVisible);
FColorPicker(Vars::Comp::StickyESP::EnemyVisibleColor);
FColorPicker(Vars::Comp::StickyESP::EnemyInvisibleColor);
FColorPicker(Vars::Comp::StickyESP::TeamVisibleColor);
FColorPicker(Vars::Comp::StickyESP::TeamInvisibleColor);
FToggle(Vars::Comp::StickyESP::EnableChams);
} EndSection();
 break;
 case 20:
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
 break;
 }
}
