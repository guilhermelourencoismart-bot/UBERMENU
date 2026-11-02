# Estado da integração

As implementações reduzidas da entrega anterior não são tratadas como paridade
completa. Foram comparadas e substituídas no fluxo ativo pelos ports de origem.
O histórico anterior, inclusive `ce9d5442`, foi preservado.

| Etapa | Classificação atual | Evidência/limite |
|---|---|---|
| Proveniência/licenças/estrutura | Concluída e testada | SOURCES.json, avisos, solução e preflight |
| Testes portáveis de cálculo/cache/política | Concluída e testada | 101 verificações GCC C++20 |
| Codec de configurações/binds | Concluída e testada isoladamente | 28 verificações, funções de produção |
| Validação Python | Concluída e testada | 11 testes |
| ESP Comp obrigatório | Implementado, mas não testado em execução | MainESP + 13 auxiliares/câmera, controles, eventos e desenho conectados |
| Visuais adicionais selecionados | Implementados, mas não testados em execução | CritHeals, SplashRadius, CraterCheck |
| V2 Global/Hitscan/Projectile/Melee | Implementados, mas não testados em execução | Ports completos e router exclusivo; adaptadores nativos |
| SmoothVelocity/Aim Smoothing | Implementados, cálculos testados | Backend V2 conectado; cache tick/serial |
| Menu/configuração unificados | Implementados, codec testado | ConfigVar/binds/menu existentes; teste no processo ainda não executado |
| Windows Release/AVX2/Textmode | Concluída e testada | `f180e6b4`, run 41370128705, quatro jobs aprovados |
| TF2 | Não executado; não bloqueia build | Testes locais pelo proprietário após entrega |

Recursos originais permanecem disponíveis. “Use Comp main ESP” seleciona um
único renderer principal e deixa o nativo como padrão compatível com configs
antigas. O solver V2 é selecionável; armas especiais e extensões sem equivalente
continuam no Unibox. Não há dois solvers ativos para a mesma execução.
Veja FEATURES.md para capacidades e exclusões justificadas.

## Checkpoints desta continuação

`c3a364c9`: UberTracker/MedicUberBar/PylonESP/FocusFire/HiderESP e recorder.
`39806b50`: HealthBar/Trails/Ammo/Sentry/Sticky completos.
`2f0bcc77`: Radar completo. `02ad70e3`: Offscreen completo.
`9c602c0a`: quatro solvers V2 e router. `9996e848`: EnemyCam completo.
`dd2aea1a`: matemática/política e codec. `ce04a442`: backtrack nativo.
`cef5de68`: ESP principal e Smooth nos solvers V2.
`68a6ff64`: CritHeals/SplashRadius/CraterCheck e correções de origem.
`9ed251c7`: persistência RGBA/janelas, invalidação por serial/estado.
`git log --oneline d7147181..HEAD` contém todos os checkpoints anteriores.

Checkpoint de estabilidade/CI posteriores: `1f84c37e`, `c9480149`, `ad6dd3e2`,
`19ca1b7c`, `c2bd6d5e`, `c2758128`. Consulte BUILD-FIXES.md para as causas e
correções. Foram preservados os ancestrais `ce9d5442` e `f80eb614`, verificados
com `git merge-base --is-ancestor`.

## Candidata compilada

Implementações selecionadas portadas e compiladas; nenhuma funcionalidade
obrigatória foi entregue somente como placeholder. Compatibilidade de execução,
precisão, visuais e lifecycle dentro do TF2 permanecem não testados.
A entrega está pronta para avaliação local, não é uma certificação de operação
no jogo nem uma afirmação de paridade medida de desempenho.

Build: https://github.com/guilhermelourencoismart-bot/UBERMENU/actions/runs/41370128705

- [Amalgam-Ultimate-Release](https://github.com/guilhermelourencoismart-bot/UBERMENU/actions/runs/41370128705/artifacts/13109575850)
- [Amalgam-Ultimate-ReleaseAVX2](https://github.com/guilhermelourencoismart-bot/UBERMENU/actions/runs/41370128705/artifacts/13108854974)
- [Amalgam-Ultimate-source](https://github.com/guilhermelourencoismart-bot/UBERMENU/actions/runs/41370128705/artifacts/13109019650)
- [Uniboxx64Release](https://github.com/guilhermelourencoismart-bot/UBERMENU/actions/runs/41370128705/artifacts/13109238394)
- [Uniboxx64ReleaseAVX2](https://github.com/guilhermelourencoismart-bot/UBERMENU/actions/runs/41370128705/artifacts/13109297401)
