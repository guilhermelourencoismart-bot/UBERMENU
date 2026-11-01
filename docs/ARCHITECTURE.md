# Arquitetura — Amalgam Ultimate

## Base e proveniência

Uma solução (`Unibox.sln`), um projeto DLL x64, MSVC v143. SDK, Core,
registro de hooks, renderizador, configurações e binds continuam sendo Unibox.
Os ZIPs fornecidos foram usados primeiro e extraídos apenas como referências
fora do checkout, em `/workspace/references`. Não se compilam projetos Comp/V2.
Commits registrados nos comentários dos ZIPs e hashes dos arquivos estão em
`SOURCES.json`; não são referências inferidas dos nomes das pastas.

## Fluxo dos visuais

`IEngineVGui_Paint` → `CacheDrawInfo` → `CIndicatorCache<T>` →
`CRender::Render` → `ImDrawList`.

O cache existente usa mutex e copia os dados. Nenhum módulo novo retém ponteiros
de entidades para o desenho: só coordenadas, cores e valores. PlayerTrails
mantém posições por índice/serial, com limites de tamanho, expiração e limpeza
em troca de mapa/rollback de tempo/respawn. Não há novas threads ou hooks.
Os módulos dependem da premissa da base de que captura de entidades e eventos
ocorre no contexto do jogo; concorrência real exige validação no Windows/TF2.

EnemyCam captura origem/ângulos do inimigo mais próximo. `CameraWindow` lê o
snapshot e reutiliza seu material, render target, janela e hook existentes.
A câmera de projéteis tem prioridade; não há duas câmeras renderizando em
paralelo. Esta apresentação continua usando o caminho de material da câmera
original, não um segundo backend ImGui. A conversão para textura ImGui não foi
implementada nem declarada concluída.

## Evitar duplicação

- O ESP principal e seus grupos continuam intactos.
- HealthBarESP não adiciona barra se o grupo já define HealthBar.
- SentryESP/StickyESP não adicionam caixas se o grupo já define Box.
- OffScreenIndicators substitui as setas de jogadores enquanto ativado;
  indicadores nativos de construções/projéteis/outros grupos são preservados.
- AmmoTracker substitui o desenho dos timers originais enquanto ativado.
- Chams e Glow continuam pertencendo exclusivamente aos grupos da base.
- Configurações novas começam desligadas; o modo de suavização padrão é Unibox.

## Configurações e menu

As variáveis usam `CVar`/`CVarEnum`, os tipos existentes bool/int/float e os
serializadores/binds do Unibox. Não foi importado Configs.cpp, outro registro de
variáveis ou outro menu. Chaves antigas não foram renomeadas. A preservação do
formato foi inspecionada; round-trip e carregamento real de arquivos antigos
ainda não foram executados.

## Aimbot

O cache de SmoothVelocity do V2 foi adaptado em `GetSmoothStrength`; invalida
por tick/alvo e não mantém entidades. Hitscan e Projectile continuam usando
seus seletores, simulações e extensões Unibox. O modo Smooth desses dois
caminhos agora pode usar as fórmulas cúbica/remapeada do V2, testadas isoladamente.
O cabeçalho V2 original usa `CUserCmd::m_vViewAngles`, incompatível com esta base:
foram extraídas funções sem SDK e conectadas aos ângulos calculados antes da
aplicação ao comando. SmoothVelocity e Assistive não são sobrescritos por esse
modo opcional.

A substituição dos solvers completos Hitscan/Projectile não foi realizada.
Diferenças incluem interfaces de seleção, records/hitboxes, flags de cálculo,
splash/drag/lob, Passtime, habilidades especiais e integração Navbot. Uma troca
direta removeria extensões da base. A continuação exige build Windows acessível
e regressões representativas dos solvers, não somente os testes matemáticos.
