# Créditos e licenças

## Unibox — base principal

Copyright (c) 2025-2026 Unibox Contributors.
https://github.com/GatoPotato658/Unibox

Texto integral preservado em `../LICENSE.md`: licença baseada em MIT com
exigência adicional de atribuição. O README mantém crédito e link visível;
os créditos também permanecem acessíveis na interface original. Não substituir
por MIT padrão. A seção upstream do README foi preservada.

## Amalgam-Comp

https://github.com/coffeegrind123/Amalgam-Comp
Commit de referência: `366fa0dcf5e5caa722ec270e6f87015adf381c50`.
Origem do ESP principal, Radar, HealthBarESP, SentryESP, StickyESP,
OffScreenIndicators, PlayerTrails, AmmoTracker, EnemyCam, UberTracker,
MedicUberBar, PylonESP, FocusFire, HiderESP, CritHeals, SplashRadius e CraterCheck.
Implementações adaptadas para interfaces/renderizador/configurações Unibox.
Texto integral WTFPL v2 em `licenses/Amalgam-Comp-LICENSE.md`.

## Amalgam V2

https://github.com/beyazprens/Amalgam-v2
Commit de referência: `ba07d0a942de2f177a40745f935c2e8c122b0024`.
Origem de AimbotGlobal/Hitscan/Projectile/Melee, cache SmoothVelocity e fórmulas
cubic/remapped. Adaptações usam os tipos/backtrack/simulações nativos e preservam
extensões Unibox. A licença está preservada literalmente em
`licenses/Amalgam-v2-LICENSE.md`, incluindo sua variante textual com “hell”.
Não foi silenciosamente normalizada para outra licença.

## Bibliotecas existentes e distribuição

ImGui 1.92.7 e MinHook são os distribuídos pela base; seus avisos em fontes e
cabeçalhos continuam intactos. Boost 1.87.0 é restaurado do NuGet com os avisos
do pacote. Nenhuma licença substitui a de outra dependência.
Não foram importados CPR, libcurl, libolm, curve25519-donna, doctest, zlib ou
FreeType dos forks. As fontes de referência ZIP não integram o pacote final;
os códigos selecionados já estão adaptados na solução principal.
Commits e hashes dos ZIPs estão em `SOURCES.json`; preserve este documento e os
textos integrais ao redistribuir fontes ou pacotes binários.
