# Arquitetura — Amalgam Ultimate

## Solução e dependências

`Unibox.sln` continua como única solução, com um projeto DLL x64, MSVC v143.
SDK, Core, hooks, Misc, Navbot, Followbot, menu ImGui, ConfigVar e binds são os
do Unibox. Boost 1.87.0 é o único pacote NuGet. ImGui/MinHook permanecem os da
base; não foram incorporados CPR, libcurl ou libolm. As origens e hashes estão
em SOURCES.json. Nenhum fonte é necessário fora deste repositório após restore.

## Captura e desenho

`IEngineVGui_Paint` captura estado no contexto nativo e chama `CompVisuals`.
As implementações completas selecionadas do Comp chamam `CompDraw`, que grava
coordenadas, texto, cores, UVs e identificadores de imagens. `CIndicatorCache`
publica a lista sob mutex. `CRender::Render` consome essa lista no ImGui existente.
Nenhum ponteiro de entidade ou CHudTexture é publicado nessa lista. Class icons,
weapon icons e avatars reutilizam os caches do ESP/SteamProfileCache do Unibox.
As listas transitórias de entidades são consumidas na mesma captura. Não foram
adicionados hooks gráficos, threads nem um segundo backend de renderização.

CompVisuals reinicializa estado por troca de mapa, retorno do relógio, reciclagem
de slots, desconexão e round. Eventos alimentam AmmoTracker, FocusFire e
CritHeals no caminho original de Events. Chams integra apenas as escolhas de
entidades/materiais de FocusFire/SentryESP/StickyESP ao pipeline nativo.

O ESP principal completo do Comp possui controles próprios na página Visuals/View.
A opção “Use Comp main ESP” seleciona um único desenho principal: Comp ou grupos
Unibox. O padrão mantém o desenho original para preservar configurações antigas.
Os grupos continuam disponíveis para Chams/Glow e filtros nativos. Barras e caixas
especializadas suprimem primitivas equivalentes para evitar sobreposição.
OffScreenIndicators substitui só as setas de jogadores; outros grupos permanecem.
AmmoTracker assume timers enquanto habilitado. O radar completo assume o Enabled
antigo; o radar reduzido anterior não é chamado.

## Câmera

EnemyCam mantém os cinco modos do Comp, tracking, offsets relativos à visão,
janela e overlay. Entidades persistentes são EHANDLE, não ponteiros. Snapshot
copia posição, ângulos, título, saúde e geometria. Usa CameraWindow, seu render
target/material e hook já existentes. A câmera de projéteis tem prioridade.
A imagem usa o caminho de material nativo e o overlay usa ImGui; não se criou
outro render target EnemyCam nem dependência DirectX adicional.

## Aimbot

O controller Unibox seleciona exclusivamente um solver por comando. O backend
V2 importa Global, Hitscan, Projectile e Melee completos, adaptados aos tipos
Target_t, TickRecord, simulações, configurações e binds nativos. Passtime,
grapple, laser pointer e mechanical arm são encaminhados ao backend Unibox;
habilidades especiais, preview e auxiliares originais continuam no controller.
Prioridade Navbot/Followbot/AutoHeal e seleção de saúde exclusiva são adaptadas.

BacktrackAdapter reconstrói centros de hitboxes dos bones dos registros nativos,
sem mudar TickRecord nem criar uma segunda simulação. Os serviços MoveSim e
ProjSim são os da base. AimSmoothing cubic/remapped está conectado ao caminho
Smooth de Hitscan/Projectile/Melee V2; SmoothVelocity compartilha a fórmula e
cache portado. FOV/solução balística têm funções puras compartilhadas com testes.
Mudança de arma, serial, mapa, relógio, backend e alvo inválido limpa estado.

## Configuração e compatibilidade

Novos controles usam o registro ConfigVar e o serializador original. Não há
segundo menu, pasta de configs ou registro de binds. Aliases mantêm as chaves
Enabled e parâmetros compatíveis das adaptações anteriores. ConfigCodec é usado
na produção e nos testes: valores, RGBA, janelas, chaves de binds, pais inválidos,
raiz antiga ConVars e migração do radar legado. Configurações inválidas preservam
valores anteriores em vez de introduzir floats não finitos ou canais inválidos.
O comportamento de carregar/salvar com o jogo e os callbacks gráficos permanece
pendente de teste local; isso é diferente do round-trip automatizado do codec.
