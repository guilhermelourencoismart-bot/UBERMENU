# Resultados reais

Executados nesta máquina Linux após as alterações:

| Verificação | Resultado |
|---|---|
| `validate_unibox.py` | 1.276 referências verificadas; nenhuma obrigatória ausente/duplicada; aviso esperado de ProfileStalker |
| `unittest discover` | 10 testes executados, 10 aprovados |
| Cálculos/cache C++20 com GCC, `-Wall -Wextra -Werror -pthread` | 31 verificações aprovadas |
| `git diff --check` | Aprovado |
| Includes locais dos novos módulos | Resolvidos na inspeção |

Os testes Python exercitam perda de fonte obrigatória, módulo opcional parcial,
duplicação de referência, pacote ausente e rejeição de PE incorreto/truncado.
Os testes PE usam cabeçalhos sintéticos e não constituem prova de uma DLL real.
O cache real `CIndicatorCache` foi exercitado com cópias independentes e
publicação/leitura concorrente de frames; isso não é teste de races no jogo.
Os cálculos cobrem rotação/clipping/bordas do radar, valores não finitos,
velocidade, invalidação/reutilização do cache e fórmulas V2 com wrap angular.
O teste de repetição confirma que o callback não é reexecutado no mesmo
tick/alvo; não há benchmark de FPS/tempo real nem alegação de ganho medido.

Executados remotamente em `cdbfb518`: restauração NuGet, testes Python,
testes nativos MSVC, compilação e linkedição nas quatro variantes x64 e
verificação PE de DLL real. Evidência: execução 41293599980 com Success e
12 artefatos, incluindo `Uniboxx64Release` (2,54 MB; digest do artefato
`e8c8687f4191a7204cce8285e6546f302fba43fb762ffa4f1d138b64a09e37ad`).
Os 31 testes C++ locais incluem adições posteriores; não devem ser atribuídos
retroativamente ao teste nativo do commit `cdbfb518`.

Não executados nesta máquina: compilação Windows ou NuGet restore via CLI.
Não executados em nenhum ambiente acessível: carregamento no TF2,
estabilidade GPU, screenshots e anti-OBS, serialização
real/round-trip de configurações antigas, binds ou inicialização/unload no jogo.
Esses itens não são considerados testes aprovados nem substituídos pelos
testes portáveis.

CI de referência da Fase 1:
https://github.com/guilhermelourencoismart-bot/UBERMENU/actions/runs/41291735398
O resumo público atualizado confirmou **Success**, quatro jobs e 12 artefatos,
incluindo `Uniboxx64Release` (2,51 MB), PDB e binlog. Ver PHASE1-UNIBOX.md.
Isso comprova a compilação da referência `56a46a57`, anterior às integrações.
As consultas autenticadas à API foram bloqueadas pelo proxy. Não há DLL baixada
no workspace.

Execução da integração `59c37e66`:
https://github.com/guilhermelourencoismart-bot/UBERMENU/actions/runs/41293344283
Terminou com falha na etapa dos testes nativos MSVC, antes da DLL. Os demais
jobs foram cancelados por fail-fast. A causa detalhada não estava exposta no
resumo inicial; a ativação CMD foi substituída por descoberta explícita VS2022
e script temporário, com logs e resumos públicos de falhas, em `cdbfb518`.

Execução dessa correção:
https://github.com/guilhermelourencoismart-bot/UBERMENU/actions/runs/41293599980
Resultado observado: Success, quatro jobs concluídos, 11m04s. A nova ativação
do MSVC e os testes nativos passaram. Sem o log completo original, não se
afirma uma causa técnica mais específica para a falha anterior.

Revisões posteriores: 41294136227 (`11817216`) e 41294520208 (`ce9d5442`).
Ambas terminaram com Success, quatro jobs e 12 artefatos. A última levou
11m24s e contém todos os ajustes de código desta entrega. Os 10 testes Python,
31 verificações nativas MSVC e checagem PE são etapas obrigatórias desse workflow.
`Uniboxx64Release` foi publicado (2,54 MB; digest do artefato
`e502e4868dfd6a72330c76b5b282bf7b55efe590a6bea3b4b2479241f728fccd`).
Arquivo gerado pelo runner: `output/x64/Release/Uniboxx64Release.dll`.
O binário não foi baixado para este workspace nem carregado no jogo.

`CI-RESULTS.json` registra os IDs e commits,
sem confundir build da referência, build da integração e testes locais.
