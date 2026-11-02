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
- Windows/MSVC: aguardando [CI desta revisão](https://github.com/guilhermelourencoismart-bot/UBERMENU/actions/runs/41388085381); não usar a CI anterior como prova.
- Aparência, legibilidade, hover, navegação e interação no jogo: avaliação local pendente.
