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

// Text visible beside the controls: no need to discover essential behavior by hovering.
static void CompactExplanation(const char* explanation)
{
    PushStyleColor(ImGuiCol_Text, GetStyleColorVec4(ImGuiCol_TextDisabled));
    TextWrapped("%s", explanation);
    PopStyleColor();
    Spacing();
}

static const char* CurrentAimModeExplanation()
{
    // AimType is an enum in Vars.h (Off, Plain, Smooth, Silent, Locking,
    // Assistive, Legit, SmoothVelocity). These are display-only descriptions.
    constexpr const char* descriptions[] = {
        "Desativado: nenhuma correção automática de mira. O ESP funciona separadamente.",
        "Plain: direciona a mira para o alvo no momento do disparo. Smooth e curvas não têm efeito aqui.",
        "Smooth: aproxima a mira aos poucos. O fator e o algoritmo de suavização podem alterar o movimento.",
        "Silent: muda a direção aplicada ao tiro sem acompanhar o movimento na câmera. Não usa Smooth.",
        "Locking: acompanha o alvo entre disparos. A curva de suavização não se aplica.",
        "Assistive: ajuda a corrigir o movimento do seu próprio mouse; depende de você mover a mira.",
        "Legit: mantém a direção da câmera; a verificação de acerto decide quando pode disparar.",
        "SmoothVelocity: aproxima a mira de forma gradual considerando a velocidade do alvo. Não usa os algoritmos cúbicos do Smooth."
    };
    return descriptions[std::clamp(int(Vars::Aimbot::General::AimType[DEFAULT_BIND]), 0, 7)];
}

static void CompactHome()
{
    if (!PageTable("CompactHome", 2)) return;
    TableNextColumn();
    if (Section("Aimbot")) {
        CompactExplanation("Escolha como a mira deve ajudar. Você não precisa ajustar os cálculos avançados para começar.");
        FDropdown(Vars::Aimbot::General::AimType);
        CompactExplanation(CurrentAimModeExplanation());
        if (Vars::Aimbot::General::AimType[DEFAULT_BIND]) {
            FSlider(Vars::Aimbot::General::AimFOV);
            CompactExplanation("FOV é a área angular em que a mira procura alvos: menor é mais restrito, maior aceita alvos mais afastados do centro. Não é distância.");
            FToggle(Vars::Aimbot::General::AutoShoot);
            CompactExplanation("Disparo automático: atira quando as condições da arma e do alvo são atendidas. Desligado, você decide quando atirar.");
        }
        if (DetailLayer("Personalizar mira")) {
            FDropdown(Vars::Aimbot::General::Target);
            FDropdown(Vars::Aimbot::General::Ignore);
            FToggle(Vars::Aimbot::General::FOVCircle);
            FColorPicker(Vars::Colors::FOVCircle);
            if (DetailLayer("Avançado: suavização e motor")) {
                CompactExplanation("Estas opções são específicas: não alteram Plain ou Silent. O motor V2 e o Unibox preservam caminhos diferentes conforme a arma.");
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
        if (FButton("Abrir todas as opções do Aimbot")) OpenDetail(MenuPageAimbot, 0);
    } EndSection();
    if (Section("Críticos")) {
        PushDisabled(F::AntiCheatCompatibility.Active());
        FToggle(Vars::CritHack::ForceCrits);
        PopDisabled();
        CompactExplanation("Tenta aproveitar disparos críticos aleatórios quando a arma e o servidor permitem. NÃO garante 100% dos críticos e não funciona com todas as armas.");
        if (DetailLayer("Personalizar críticos")) {
            FToggle(Vars::CritHack::AvoidRandomCrits);
            FToggle(Vars::CritHack::AlwaysMeleeCrit);
            FToggle(Vars::CritHack::CritEffects);
        }
    } EndSection();
    TableNextColumn();
    if (Section("ESP")) {
        CompactExplanation("Exibe informações sobre jogadores e construções na tela. Só usa dados disponíveis ao cliente; não altera a mira.");
        FToggle(Vars::Comp::ESP::Enabled);
        CompactExplanation("Ligado: usa o ESP do Comp. Desligado: mantém o ESP por grupos do Unibox. Não são duas camadas principais sobrepostas.");
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
        CompactExplanation("Mini-mapa com jogadores e outras entidades. O alcance controla a área mostrada, não a distância do Aimbot.");
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
