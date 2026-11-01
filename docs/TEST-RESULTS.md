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

Não executados: compilação do código dependente do SDK/Windows, NuGet restore
via CLI nesta máquina, linkedição da DLL, verificação PE de artefato real,
carregamento no TF2, estabilidade GPU, screenshots e anti-OBS, serialização
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

Nova execução dessa correção:
https://github.com/guilhermelourencoismart-bot/UBERMENU/actions/runs/41293599980
O resultado dessa execução deve ser avaliado antes de afirmar que a correção
resolveu a etapa. `CI-RESULTS.json` registra os IDs e commits, sem confundir
build da referência, build da integração e testes locais.
