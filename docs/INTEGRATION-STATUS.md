# Estado da integração

Entrega de desenvolvimento parcial; não é uma versão Windows validada.

| Etapa | Estado | Escopo real |
|---|---|---|
| Inspeção/proveniência | Concluída e testada | Fontes locais, commits dos ZIPs e SHA-256 em SOURCES.json |
| Preflight e testes portáveis | Concluída e testada | Referências, falhas de build, geometria e suavização |
| Build da referência Release x64 | Concluída e testada | Execução 41291735398: Success, quatro jobs, artefato Release; sem execução TF2 |
| Build da integração `cdbfb518` | Concluída e testada | Execução 41293599980: Success, quatro jobs, verificação PE e artefato Release |
| Build da revisão final `ce9d5442` | Concluída e testada | Execução 41294520208: Success, quatro jobs, 12 artefatos, PE AMD64/PE32+ validado |
| Radar | Implementada, mas não testada em execução | Jogadores/construções/pickups, círculo/retângulo, alcance e bordas; sem ícones/avatars do Comp |
| HealthBarESP | Implementada, mas não testada em execução | Barras, overheal, seleção médico e contorno de solo; sem preenchimento do polígono nem todos os ajustes do Comp |
| PlayerTrails | Implementada, mas não testada em execução | Trilhas de inimigos, expiração, serial e limpeza; sem a máquina de estados de visibilidade completa do Comp |
| OffScreenIndicators | Implementada, mas não testada em execução | Setas ImGui usando grupos existentes; sem avatars/textos do Comp |
| AmmoTracker | Implementada, mas não testada em execução | Associação a pickups conhecidos, estimativa de respawn e arco/texto; sem HUD completo, pie chart no solo ou suporte a todos os itens sazonais |
| SentryESP | Implementada, mas não testada em execução | Caixas sem duplicação e linha para alvo networked; sem extração de muzzle por bones nem sistema adicional de Chams |
| StickyESP | Implementada, mas não testada em execução | Filtros de stickybombs, caixas 2D/3D e visibilidade; Chams nativo preservado |
| EnemyCam | Implementada, mas não testada em execução | Inimigo mais próximo, viewpoint em CameraWindow; sem todos os modos/offsets/tracking do Comp, sem textura ImGui independente |
| ESP principal Comp | Pendente por bloqueio identificado | Base Unibox preservada; não há substituição integral do ESP principal |
| SmoothVelocity V2 | Implementada, mas não testada em execução | Cache tick/alvo, null guard; fórmula portável testada |
| Aim Smoothing V2 | Implementada, mas não testada em execução | Fórmulas conectadas aos modos Smooth de Hitscan/Projectile; cálculos portáveis testados |
| Solvers Hitscan/Projectile V2 completos | Pendente por bloqueio identificado | Comparados; Unibox mantém solver, extensões de armas, Passtime e navegação; falta validação Windows para adaptação ampla |
| Outros visuais exclusivos | Pendente por bloqueio identificado | Não importados automaticamente; exigem análise e validação dos módulos acima |
| Regressão TF2 e configurações/binds | Pendente por bloqueio identificado | Sem TF2/Windows nesta máquina |

Cada módulo foi registrado em um commit próprio na branch
`integration/phase1-unibox`. Consulte `git log --oneline d7147181..HEAD`.
Este quadro distingue uma adaptação funcional com escopo reduzido de uma
migração completa: não se afirma paridade integral com os projetos de origem.

## Commits funcionais

| Área | Commits |
|---|---|
| Radar | `7be28dc0` |
| HealthBarESP | `16cf317a` |
| PlayerTrails | `e003b49f` |
| OffScreenIndicators e preservação de setas não jogadoras | `081057a0`, `ce9d5442` |
| AmmoTracker e correção de interface | `ec4fd350`, `849dc2f0` |
| SentryESP | `b5aace44` |
| StickyESP | `c128bc5e` |
| EnemyCam e sincronização | `a4bc7788`, `fa6099bc` |
| Cache SmoothVelocity V2 | `6e8deb17` |
| Fórmulas V2 conectadas a Hitscan/Projectile | `8b6b6147` |
| Testes/CI/estabilidade | `ca7633c0`, `cdbfb518`, `311df519`, `874de499` |

Documentação/proveniência/licenças: `59c37e66` e `11817216`, além do registro
final de resultados. As alterações da Fase 1 permanecem em `56a46a57`.
