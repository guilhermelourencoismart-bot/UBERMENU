# Recursos e controles

O menu e as configurações permanecem Unibox. Os controles Comp ficam em
Visuals/View, em um seletor de módulo, e os controles V2 nas categorias General/Weapons do Aimbot. Cada recurso novo
começa desligado; o backend de mira pode ser selecionado entre V2 e Unibox.
“Use Comp main ESP” alterna exclusivamente o desenho principal, preservando
os grupos nativos e suas configurações quando desativado.

| Recurso | Implementação adaptada |
|---|---|
| ESP principal | Players/buildings/projectiles/objectives/world; nomes, tags, distâncias, bones, barras, classes, armas, buffs/debuffs/flags; alpha e dormancy |
| Radar | Círculo/retângulo, alcance/bordas, jogadores/construções/pickups/mundo, filtros, cores, altitude, barras, ícones e avatars |
| HealthBarESP | Barras/overheal, filtro de médico, visibilidade, polígonos preenchidos e contorno, dimensões/alpha/alcance |
| SentryESP | Aim line, muzzle/bones, estado de perigo, caixas/cantos, cores e seleção Chams no pipeline nativo |
| StickyESP | Caixas 2D/3D, equipes/visibilidade/distância, seleção Chams e cores |
| OffScreenIndicators | Setas, nomes, classe, vida, distância, avatars, alpha/posição e cores por jogador |
| PlayerTrails | Histórico, duração, intervalo, limite de pontos, visibilidade e altura |
| AmmoTracker | Respawn por evento, pie chart no solo/tela, texto, segundos/milissegundos, HUD, escala e cores |
| EnemyCam | Closest/Healed/Medic/TopScore/Random, tracking, viewpoint/offsets, janela, nome/classe/vida; câmera nativa compartilhada |
| UberTracker | Medics por time, charge/kritz, vantagem, mortes, previsão e apresentação configurável |
| MedicUberBar | Times, alcance, visibilidade, alpha, porcentagem e cores por nível de charge |
| PylonESP | Marcadores verticais, distância, largura/altura/offset, segmentos e gradiente alpha |
| FocusFire | Eventos de dano, janela temporal/atacantes, queda de vida, caixas e Chams |
| HiderESP | Tempo parado, limite de movimento/distância, caixas/tracers/cores |
| CritHeals | Elegibilidade por tempo desde dano/buff, triângulos e aviso de penalidade de construção de uber |
| SplashRadius | Projéteis selecionados, círculos orientados à superfície, impacto de foguetes, união de círculos, LOD, times/cores/contorno |
| CraterCheck | Estimativa de queda na superfície apontada, marcador letal/seguro, preenchimento e contorno |
| AimbotGlobal V2 | Seleção/ordenação, FOV, filtros, hitboxes, prioridade, integração Navbot/Followbot/AutoHeal |
| Hitscan V2 | Run/CanHit/Aim/ShouldFire, scan de hitboxes/multipontos/backtrack e delay |
| Projectile V2 | Seleção, simulação, trajetória, tempo de impacto, splash/direct, processamento e disparo |
| Melee V2 | Seleção, simulação de swing/doubletap, alcance, hit detection e aplicação de mira |
| SmoothVelocity/Aim Smoothing | Cache tick/serial, curva/velocidade, cubic/remapped conectados aos solvers |

As tabelas descrevem código conectado ao fluxo, não resultados de execução no
TF2. Consulte TEST-RESULTS.md e INTEGRATION-STATUS.md para o estado da build.
Não há alegação de melhoria de precisão ou FPS; não foi realizado benchmark.

Não foram selecionados ChatBubbles, MarkSpot e StickyCam: incluem comunicação,
ferramentas interativas ou outro sistema de câmera fora dos módulos obrigatórios.
PlayerConditions, Chams, Glow, materiais e câmera nativos foram preservados.
PlayerArrows sobrepõe o indicador de jogadores já integrado. O header isolado
SwingPrediction.h do V2 não é chamado no upstream; a predição usada vem de
AimbotMelee.cpp e das simulações nativas. Nenhum header morto é anunciado como
um algoritmo em execução. Quatro variáveis CritHeals sem uso na origem
(FontSize/CritColor/TextColor/ShowPercentage) não geraram controles inertes.

## Seleção e extensões exclusivas

O solver V2 é o padrão selecionado. Forward Track, Lob Angles, Target Dormant e
armas exclusivas encaminham exclusivamente ao Unibox quando solicitados. Como
Lob Angles é uma opção habilitada por padrão na base, desative-a (e Target
Dormant) para usar Projectile V2; desative Forward Track para Hitscan V2.
As opções nativas mantêm seus valores e binds. V2 usa Single/Multi para sampling
de splash; Unibox continua com Trace/Face. O menu mostra a opção aplicável.
