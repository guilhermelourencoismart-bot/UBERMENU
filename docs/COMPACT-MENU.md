# Menu compacto

A abertura começa em Início → Essencial. A superfície apresenta modo do Aimbot,
motor escolhido, FOV, disparo automático, ESP e Radar. O restante fica em
Personalizar, Avançado ou Menu completo. A aba Início permite voltar a qualquer
momento. Ctrl+F busca também opções avançadas, com ou sem acentos.

Os controles usam os mesmos ConfigVars, wrappers e binds das telas completas.
Trocar de apresentação não aplica presets nem redefine filtros. Salve valores
em Configurações → Arquivos como antes.

## Favoritos

Botão direito em um controle abre o popup original de binds, agora com
Adicionar/Remover dos favoritos. Início → Favoritos edita o valor original.
A lista usa snapshots protegidos por mutex entre renderização e persistência.
É persistida automaticamente em Menu.Favorites no arquivo de estado
existente do Unibox, usando identificadores Vars:: estáveis. Limite: 64 opções.
Identificadores inválidos e duplicados são ignorados ao carregar. Opções ausentes
continuam removíveis e não modificam a configuração. Opções NOSAVE/NOBIND que
não possuem popup não podem ser adicionadas por esse atalho.

## Indicadores

ESP mostra o seletor principal Comp/Unibox. Roteamento da arma mostra um snapshot
do caminho elegível no último comando: Unibox, V2 ou Unibox por compatibilidade.
O snapshot expira em um segundo e não lê entidades na thread da interface.
Não é confirmação de alvo adquirido, tiro ou acerto. Não altera algoritmos.

## Linguagem e ajuda

Aimbot mantém seu nome. Plain/Direta explica aplicação no disparo sem Smooth;
os modos de mira e os filtros técnicos possuem ajuda individual na lista.
Dead Ringer é o nome da arma, não 'Morto ringer'. Unsimulated aparece como
Sem atualização de movimento; Sentry/Sentries como Torre(s) automática(s).
Não foram renomeadas chaves salvas, enums ou recursos de skins.

A revisão contextual é mais detalhada nos controles de mira e nos módulos
principais. Ainda existem descrições por tipo/categoria nas opções sem texto
específico; não se deve considerar toda a documentação do menu revisada.

## Validação desta revisão

- 1.339 referências verificadas; ProfileStalker permanece opcional e indisponível.
- 11 testes Python.
- 101 verificações de cálculo/cache/política.
- 34 verificações de codec, incluindo round trip JSON de favoritos e estado antigo.
- 25 verificações de apresentação/ajuda e 64 inserções para limite de favoritos.
- Windows/MSVC v143: quatro variantes x64 aprovadas em `5b42ebd4`.
  [CI aprovada](https://github.com/guilhermelourencoismart-bot/UBERMENU/actions/runs/41388300298): 4 jobs, 17 artefatos, 14m40s.
  DLLs verificadas no runner como PE32+/AMD64 antes da publicação.
- Aparência, legibilidade, hover, navegação e interação no jogo: avaliação local pendente.

## Downloads desta revisão

- [Uniboxx64Release](https://github.com/guilhermelourencoismart-bot/UBERMENU/actions/runs/41388300298/artifacts/13116453522)
- [Uniboxx64ReleaseAVX2](https://github.com/guilhermelourencoismart-bot/UBERMENU/actions/runs/41388300298/artifacts/13117145198)
- [Amalgam-Ultimate-source](https://github.com/guilhermelourencoismart-bot/UBERMENU/actions/runs/41388300298/artifacts/13116108101)

## Revisão em camadas

A revisão posterior separa Essencial, Favoritos e Explorar tudo. Essencial
mostra Aimbot, críticos, ESP e Radar. Personalizar abre opções dentro do recurso;
o motor e os cálculos ficam na camada Avançado. Explorar tudo concentra
atalhos para câmeras, cenário, movimento, bots, binds e perfis.

Nenhum valor é redefinido ao recolher um painel. Todos os controles continuam
ligados às mesmas configurações e binds. O Radar passa a expor explicitamente
a inclusão de jogadores; ESP expõe os filtros de jogadores para evitar uma
aparência de recurso ligado sem entidades permitidas.

Tentar críticos descreve os limites de arma, servidor e reserva. A prioridade
corpo a corpo continua condicionada ao alvo do Aimbot; efeitos visuais não
produzem dano crítico. Nenhum algoritmo de críticos ou mira foi modificado.

Validação local: 11 testes Python, 34 verificações de codec, 30 de apresentação
e 64 inserções para capacidade de favoritos aprovados. A compilação Windows
desta revisão foi iniciada; os links anteriores pertencem à versão anterior.
A revisão integral das descrições avançadas e a avaliação visual permanecem
pendentes. Não considerar todos os textos do menu revisados.

## DLL do menu em camadas

Código `1606efa7`: job Release x64 concluído, com DLL/PDB/log/pacote/fontes
publicados. [Baixar a DLL Release](https://github.com/guilhermelourencoismart-bot/UBERMENU/actions/runs/41391957842/artifacts/13117593447).
[Execução Windows](https://github.com/guilhermelourencoismart-bot/UBERMENU/actions/runs/41391957842).
O workflow exige os testes nativos, a compilação/linkedição e a verificação
PE32+/AMD64 antes de publicar a DLL. As outras três variantes ainda estavam
em execução no momento deste registro; isso não deve ser lido como aprovação
da matriz inteira. O binário não foi executado no TF2.
