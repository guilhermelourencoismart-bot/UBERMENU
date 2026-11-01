# Resultados reais

## Execução local nesta revisão

| Verificação | Resultado |
|---|---|
| Referências do projeto/filtros | 1.334; nenhuma obrigatória ausente/duplicada; ProfileStalker opcional ausente |
| Python unittest | 10 testes aprovados |
| C++20 GCC, -Wall -Wextra -Werror -pthread | 82 verificações de cálculos/cache/política aprovadas |
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

O resultado atual será registrado após o término da CI; os runs e evidências
históricas estão em CI-RESULTS.json. Não se declara sucesso de um run em andamento.
Nenhuma sessão TF2 foi executada; isso não é bloqueio para entregar a candidata
que passar na build. Não foi realizado benchmark de precisão/FPS.
