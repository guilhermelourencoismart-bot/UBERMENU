# Estado da integração

As implementações reduzidas da entrega anterior não são tratadas como paridade
completa. Foram comparadas e substituídas no fluxo ativo pelos ports de origem.
O histórico anterior, inclusive `ce9d5442`, foi preservado.

| Etapa | Classificação atual | Evidência/limite |
|---|---|---|
| Proveniência/licenças/estrutura | Concluída e testada | SOURCES.json, avisos, solução e preflight |
| Testes portáveis de cálculo/cache/política | Concluída e testada | 82 verificações GCC C++20 |
| Codec de configurações/binds | Concluída e testada isoladamente | 28 verificações, funções de produção |
| Validação Python | Concluída e testada | 10 testes |
| ESP Comp obrigatório | Implementado, mas não testado em execução | MainESP + 13 auxiliares/câmera, controles, eventos e desenho conectados |
| Visuais adicionais selecionados | Implementados, mas não testados em execução | CritHeals, SplashRadius, CraterCheck |
| V2 Global/Hitscan/Projectile/Melee | Implementados, mas não testados em execução | Ports completos e router exclusivo; adaptadores nativos |
| SmoothVelocity/Aim Smoothing | Implementados, cálculos testados | Backend V2 conectado; cache tick/serial |
| Menu/configuração unificados | Implementados, codec testado | ConfigVar/binds/menu existentes; teste no processo ainda não executado |
| Windows Release/AVX2 atual | Pendente de resultado CI | Não atribuir à revisão atual o sucesso antigo `ce9d5442` |
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
