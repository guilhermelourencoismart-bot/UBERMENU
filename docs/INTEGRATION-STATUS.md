# Estado da integração

Entrega de desenvolvimento parcial; não é uma versão Windows validada.

| Etapa | Estado | Escopo real |
|---|---|---|
| Inspeção/proveniência | Concluída e testada | Fontes locais, commits dos ZIPs e SHA-256 em SOURCES.json |
| Preflight e testes portáveis | Concluída e testada | Referências, falhas de build, geometria e suavização |
| Build Release x64 | Pendente por bloqueio identificado | API GitHub bloqueada pelo proxy; sem MSVC local |
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
