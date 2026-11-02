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

static void CompactHome()
{
    if (!PageTable("CompactHome", 2)) return;
    TableNextColumn();
    if (Section("Aimbot")) {
        FDropdown(Vars::Aimbot::General::AimType);
        FDropdown(Vars::Aimbot::General::Solver);
        FSlider(Vars::Aimbot::General::AimFOV);
        FToggle(Vars::Aimbot::General::AutoShoot);
        const char* routes[] = {"Roteamento: inativo ou sem atualização", "Roteamento da arma: Unibox", "Roteamento da arma: V2", "Roteamento da arma: Unibox (compatibilidade)"};
        FLabel(routes[std::clamp(F::V2Router.DisplayRoute(), 0, 3)]);
        FTooltip("Mostra o motor escolhido para a arma no último comando, não confirma aquisição de alvo ou disparo. Extensões exclusivas usam o Unibox.");
        if (FButton("Personalizar mira")) OpenDetail(MenuPageAimbot, 0);
        if (FButton("Avançado: armas e simulação")) OpenDetail(MenuPageAimbot, 1);
    } EndSection();
    if (Section("ESP")) {
        FToggle(Vars::Comp::ESP::Enabled);
        FLabel(Vars::Comp::ESP::Enabled.Value ? "Sistema selecionado: Comp" : "Sistema selecionado: grupos Unibox");
        if (Vars::Comp::ESP::Enabled[DEFAULT_BIND]) {
            FDropdown(Vars::Comp::ESP::Draw);
            FColorPicker(Vars::Comp::Palette::Target);
        }
        if (FButton("Personalizar ESP")) OpenDetail(MenuPageVisuals, Vars::Comp::ESP::Enabled[DEFAULT_BIND] ? 4 : 0, 0);
        if (FButton("Cores e destaque do alvo")) OpenDetail(MenuPageVisuals, 4, 16);
    } EndSection();
    TableNextColumn();
    if (Section("Radar")) {
        FToggle(Vars::Radar::Enabled);
        FDropdown(Vars::Comp::Radar::Main::Style);
        FSlider(Vars::Radar::Range);
        FDropdown(Vars::Comp::Radar::Player::Draw);
        if (FButton("Personalizar Radar")) OpenDetail(MenuPageVisuals, 4, 13);
    } EndSection();
    if (Section("Seu menu")) {
        FLabel("Essencial aqui. Todos os detalhes a um clique.");
        if (FButton("Menu completo / Avançado")) OpenDetail(MenuPageAimbot, 0);
        if (FButton("Meus favoritos")) OpenDetail(MenuPageHome, 1);
        if (FButton("Configurações: salvar e carregar")) OpenDetail(MenuPageSettings, 0);
        if (FButton("Movimento e utilidades")) OpenDetail(MenuPageMisc, 0);
        if (FButton("Bots e navegação")) OpenDetail(MenuPageMisc, 2);
        FLabel("Ctrl+F: buscar em todas as categorias.");
        FLabel("Botão direito: favoritos e binds.");
        FLabel("Base Unibox — GatoPotato658");
    } EndSection();
    EndTable();
}
