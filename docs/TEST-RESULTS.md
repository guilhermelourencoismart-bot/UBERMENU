# Resultados reais

Executados nesta máquina Linux após as alterações:

| Verificação | Resultado |
|---|---|
| `validate_unibox.py` | 1.276 referências verificadas; nenhuma obrigatória ausente/duplicada; aviso esperado de ProfileStalker |
| `unittest discover` | 10 testes executados, 10 aprovados |
| Cálculos C++20 com GCC, `-Wall -Wextra -Werror` | 29 verificações aprovadas |
| `git diff --check` | Aprovado |
| Includes locais dos novos módulos | Resolvidos na inspeção |

Os testes Python exercitam perda de fonte obrigatória, módulo opcional parcial,
duplicação de referência, pacote ausente e rejeição de PE incorreto/truncado.
Os testes PE usam cabeçalhos sintéticos e não constituem prova de uma DLL real.
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
O resumo público consultado não forneceu comprovação de conclusão. As consultas
autenticadas à API foram bloqueadas pelo proxy. Não há DLL confirmada no workspace.
As execuções posteriores à publicação desta integração devem ser consultadas
pelo SHA da branch, não confundidas com a execução da Fase 1 acima.
