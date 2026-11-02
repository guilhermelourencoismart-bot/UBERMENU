// A second presentation of the same registered values, never a second configuration.
static void OpenDetail(int page, int sub, int compModule = -1)
{
    Shell::Subs.resize(vMenuPages.size());
    Shell::Subs[page] = sub;
    Shell::Page = page;
    Shell::SearchText.clear();
    mLastHeights.clear();
    if (compModule >= 0) s_CompRequestedModule = compModule;
}

static void FavoritesPage()
{
    if (!PageTable("Favorites", 2)) return;
    TableNextColumn();
    if (Section("Meus favoritos")) {
        FLabel("Use o botão direito em uma opção para favoritá-la.");
        FLabel("A busca encontra também os ajustes avançados.");
        if (FButton("Adicionar pela busca")) Shell::SearchText = "aim";
    } EndSection();
    const auto keys = MenuPresentation::Preferences.Snapshot().favorites;
    int column = 0;
    for (const auto& key : keys) {
        auto it = std::find_if(G::Vars.begin(), G::Vars.end(), [&](auto var){ return key == var->Name(); });
        auto var = it == G::Vars.end() ? nullptr : *it;
        const std::string title = (var ? std::string(var->Section()) : "Opção indisponível") + "##favorite-" + key;
        TableSetColumnIndex(column++ % TableGetColumnCount());
        if (Section(title.c_str())) {
            if (var && (!(var->m_iFlags & DEBUGVAR) || Vars::Debug::Options[DEFAULT_BIND])) DrawRegisteredVar(var);
            else FLabel("Opção ausente ou de depuração. Seus valores não foram alterados.");
            PushID(key.c_str());
            if (FButton("Remover dos favoritos")) {
                MenuPresentation::Preferences.Remove(key);
                F::Configs.SaveMenuState();
            }
            PopID();
        } EndSection();
    }
    EndTable();
}

static bool DetailLayer(const char* title)
{
    Spacing();
    return CollapsingHeader(title, ImGuiTreeNodeFlags_None);
}

static void CompactHome()
{
    if (!PageTable("CompactHome", 2)) return;
    TableNextColumn();
    if (Section("Aimbot")) {
        FDropdown(Vars::Aimbot::General::AimType);
        if (Vars::Aimbot::General::AimType[DEFAULT_BIND]) {
            FSlider(Vars::Aimbot::General::AimFOV);
            FToggle(Vars::Aimbot::General::AutoShoot);
        }
        if (DetailLayer("Personalizar mira")) {
            FDropdown(Vars::Aimbot::General::Target);
            FDropdown(Vars::Aimbot::General::Ignore);
            FToggle(Vars::Aimbot::General::FOVCircle);
            FColorPicker(Vars::Colors::FOVCircle);
            if (DetailLayer("Avançado: suavização e motor")) {
                FDropdown(Vars::Aimbot::General::Solver);
                FDropdown(Vars::Aimbot::General::SmoothingAlgorithm);
                FSlider(Vars::Aimbot::General::AssistStrength);
                FDropdown(Vars::Aimbot::General::SmoothCurve);
                FSlider(Vars::Aimbot::General::SmoothCurveAmount);
                FSlider(Vars::Aimbot::General::V2SmoothFactor);
                const char* routes[] = {"Motor: sem atualização", "Motor da arma: Unibox", "Motor da arma: V2", "Motor da arma: Unibox por compatibilidade"};
                FLabel(routes[std::clamp(F::V2Router.DisplayRoute(),0,3)]);
                if(FButton("Todas as opções de armas")) OpenDetail(MenuPageAimbot,1);
            }
        }
    } EndSection();
    if (Section("Críticos")) {
        FToggle(Vars::CritHack::ForceCrits);
        if (DetailLayer("Personalizar críticos")) {
            FToggle(Vars::CritHack::AvoidRandomCrits);
            FToggle(Vars::CritHack::AlwaysMeleeCrit);
            FToggle(Vars::CritHack::CritEffects);
        }
    } EndSection();
    TableNextColumn();
    if (Section("ESP")) {
        FToggle(Vars::Comp::ESP::Enabled);
        if(Vars::Comp::ESP::Enabled[DEFAULT_BIND]) {
            FDropdown(Vars::Comp::ESP::Draw);
            FDropdown(Vars::Comp::ESP::Player);
        }
        if(DetailLayer("Personalizar ESP")) {
            FColorPicker(Vars::Comp::Palette::Target);
            FDropdown(Vars::Comp::ESP::Building);
            if(FButton("Todos os detalhes do ESP")) OpenDetail(MenuPageVisuals,Vars::Comp::ESP::Enabled[DEFAULT_BIND]?4:0,0);
            if(FButton("Paleta completa de cores")) OpenDetail(MenuPageVisuals,4,16);
        }
    } EndSection();
    if (Section("Radar")) {
        FToggle(Vars::Radar::Enabled);
        if(Vars::Radar::Enabled[DEFAULT_BIND]) {
            FToggle(Vars::Radar::Players);
            FSlider(Vars::Radar::Range);
        }
        if(DetailLayer("Personalizar Radar")) {
            FDropdown(Vars::Comp::Radar::Main::Style);
            FDropdown(Vars::Comp::Radar::Player::Draw);
            if(FButton("Aparência e demais entidades")) OpenDetail(MenuPageVisuals,4,13);
        }
    } EndSection();
    EndTable();
}

static void CompactLibrary()
{
    if(!PageTable("CompactLibrary",2)) return;
    TableNextColumn();
    if(Section("Personalização visual")) {
        if(FButton("ESP e indicadores Comp"))OpenDetail(MenuPageVisuals,4);
        if(FButton("Câmeras"))OpenDetail(MenuPageVisuals,1);
        if(FButton("Cenário"))OpenDetail(MenuPageVisuals,2);
        if(FButton("Aparência do menu e atalhos"))OpenDetail(MenuPageVisuals,3);
    } EndSection();
    if(Section("Movimento e automação")) {
        if(FButton("Movimento"))OpenDetail(MenuPageMisc,0);
        if(FButton("Automação"))OpenDetail(MenuPageMisc,1);
        if(FButton("Bots e navegação"))OpenDetail(MenuPageMisc,2);
    } EndSection();
    TableNextColumn();
    if(Section("Perfil e opções completas")) {
        if(FButton("Salvar e carregar configurações"))OpenDetail(MenuPageSettings,0);
        if(FButton("Binds: condições e atalhos"))OpenDetail(MenuPageSettings,1);
        if(FButton("Abrir menu completo"))OpenDetail(MenuPageAimbot,0);
        FLabel("Ctrl+F busca também opções avançadas.");
        FLabel("Botão direito em uma opção: favorito e bind.");
        FLabel("Base Unibox — GatoPotato658");
    } EndSection();
    EndTable();
}
