# Resultados reais

## Execução local nesta revisão

| Verificação | Resultado |
|---|---|
| Referências do projeto/filtros | 1.335; nenhuma obrigatória ausente/duplicada; ProfileStalker opcional ausente |
| Python unittest | 11 testes aprovados |
| C++20 GCC, -Wall -Wextra -Werror -pthread | 101 verificações de cálculos/cache/política aprovadas |
| Codec C++20 GCC com Boost 1.87.0 | 28 verificações aprovadas |
| git diff --check | Aprovado nas alterações registradas |

O Boost local foi obtido do pacote NuGet 1.87.0; SHA512 foi comparado ao catálogo
NuGet antes da extração. Não foi desativada verificação de assinatura no restore.
Testes de codec usam as mesmas funções chamadas por Configs.cpp: escalares,
RGBA, janela, UTF-8, valores inválidos, defaults ausentes, overrides por bind,
NOBIND, pais inválidos/ciclos e migração de radar/ConVars. Não são teste da GUI.

Cálculos exercitam radar completo via RadarMath, normalização, FOV comparado à
fórmula V2, balística comparada às equações de voo, valores não finitos,
SmoothVelocity tick/serial, contexto de arma/mapa/owner/backend, alvo inexistente,
dormente/morto, curvas cubic/remapped e snapshot concorrente. Não cobrem as
colisões/visibilidade do SDK nem o comportamento completo no jogo.

Os testes Python PE usam headers sintéticos para testar a rejeição de artefatos
inválidos. A verificação PE da DLL real é uma etapa distinta na CI Windows.
Nenhum teste portável é apresentado como compilação da DLL ou teste TF2.

## Windows

A revisão anterior `ce9d5442` passou em 41294520208 nas quatro variantes x64,
com DLL/PDB/logs. Ela precede os ports completos desta continuação.
As primeiras compilações dos ports mostraram diferenças de SDK: pSkip/m_pSkip,
PlayerInfo_t/player_info_t, macro DEG2RAD, records sem m_vHitboxInfos e aggregate
TickRecord incompatível. As correções mantêm os contratos nativos e reconstruem
hitboxes dos bones; não removem recursos para compilar.

Revisão compilada `f180e6b4`: **Success**, quatro jobs x64 concluídos.
Execução: https://github.com/guilhermelourencoismart-bot/UBERMENU/actions/runs/41370128705
Restore Boost, 11 testes Python, 101 verificações nativas de cálculos/cache/política,
28 verificações do codec com MSVC, compilação e linkedição passaram. A etapa
obrigatória verificou as DLLs reais como PE32+/AMD64/DLL e publicou os artefatos.
Foram aprovadas Release, ReleaseAVX2, ReleaseTextmode e ReleaseTextmodeAVX2.
O pacote de fontes foi extraído/validado na CI e os pacotes binários incluem
manifest com commit/SHA256 da DLL. IDs/digests dos artefatos: CI-RESULTS.json.
Os binários não foram baixados neste workspace; a verificação PE ocorreu no
runner Windows. Não confundir o digest do ZIP Actions com o SHA256 da DLL.
Este registro é posterior à build; o commit de documentação mantém o código
compilado idêntico à revisão acima.
Nenhuma sessão TF2 foi executada; isso não é bloqueio para entregar a candidata
que passar na build. Não foi realizado benchmark de precisão/FPS.

## Verificações adicionais de estabilidade e integridade

As 101 verificações de cálculo/cache/política e 28 do codec também passaram
com AddressSanitizer + UBSan (`-O1 -g -fsanitize=address,undefined`).
LeakSanitizer falhou ao iniciar porque o ambiente usa ptrace; o segundo teste
usou `ASAN_OPTIONS=detect_leaks=0`. Não é evidência de ausência de vazamentos.
A revisão estática encontrou e corrigiu a referência do render target auxiliar
de ícones que não era liberada no unload, além de sincronizar o cache de ícones.

Foram inspecionados 1.834 includes locais entre fontes/headers, incluindo vendor.
Doze não resolvidos pertencem a Steam PS3/flat/debug e exemplos/opções desativadas
ImGui (FreeType, stb, user hooks); não são dependências dos ports Windows atuais.
Os oito hooks .cpp fora do projeto já estavam excluídos na base e não foram
removidos. Todos os .cpp novos Comp/V2 possuem referência no vcxproj.
PORT-MANIFEST.json registra hashes e comparação de métodos dos ports.
