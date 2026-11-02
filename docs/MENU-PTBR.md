# Menu PT-BR, categorias, ajuda e Mouse4

Escopo: itens 3, 5, 6 e 7. Os algoritmos e os dois backends de ESP/Aimbot foram
preservados. Não é a unificação definitiva dos itens 1/2 nem um redesenho visual
completo do item 4.

## Navegação

- Aimbot: Geral, Armas, Apoio e Indicadores.
- Visuais: ESP Unibox, Câmera, Cenário, Interface e ESP Comp.
- Combate avançado: Ritmo de comandos e Ângulos.
- Utilidades: Movimento, Automação, Bots e navegação e Filas.
- Monitoramento, Jogadores, Skins e Configurações mantêm suas funções.

Em Visuais → ESP Comp, escolha a categoria à esquerda e ajuste o módulo à
direita: ESP e cores; Vida e médicos; Ameaças e combate; Radar;
Acompanhamento e itens. A paleta compartilhada do ESP/Radar fica em ESP e cores.
Os índices das páginas anteriores foram mantidos; a página Comp foi acrescentada.

## Tradução e descrições

Localization.h e PT-BR-LABELS.json registram a tradução dos rótulos. As legendas
e opções numéricas são traduzidas; nomes de arquivos, materiais, usuários,
identificadores de recursos e chaves de configuração não são renomeados.
Aimbot, ESP, Radar, classes/armas e nomes de produtos conhecidos são mantidos
quando apropriado. A busca aceita português com ou sem acentos.

Ao passar o mouse sobre controles, a interface apresenta ajuda curta em PT-BR.
Há descrições específicas para alvo, FOV, sistema da mira, cores, câmeras e
módulos principais, além de descrições por tipo/categoria para os parâmetros.
O painel é único por controle para evitar textos sobrepostos. Os filtros
diretos do ESP nativo também recebem ajuda.

Exemplo: a cor do alvo sinaliza seleção do Aimbot, não confirmação de acerto.
Barras e caixas podem ser suprimidas por equivalentes do ESP principal;
a câmera de projéteis tem prioridade sobre EnemyCam. Essas relações são
explicadas na ajuda dos controles principais.

## Atalho secundário

O padrão é Mouse4 (VK_XBUTTON1, primeiro botão lateral). A tecla principal
permanece Insert por padrão. Ambos podem ser ajustados em Visuais → Interface.
Configurações antigas com o padrão F3 são migradas para Mouse4. Outros valores
personalizados são preservados. A migração é idempotente e também aceita a
raiz antiga ConVars. Nenhum bind de funcionalidades foi removido ou renomeado.

## Validação

Preflight: 1.337 referências; 11 testes Python aprovados.
Cálculos/cache/política: 101 verificações já existentes.
Codec: 31 verificações, incluindo F3→Mouse4 e preservação de tecla personalizada.
Apresentação: 13 verificações para tradução, IDs preservados, ponteiros estáveis,
ajuda, identificação de recursos preservada e busca sem acentos.
MSVC v143/Windows-2022: quatro configurações x64 aprovadas em `9e8df21b`.
Execução: https://github.com/guilhermelourencoismart-bot/UBERMENU/actions/runs/41384099020
Os testes acima passaram; as DLLs reais foram verificadas como PE32+/AMD64 e
17 artefatos foram publicados. O commit posterior registra documentação e um
gerador de catálogo verificado localmente; não altera o C++ compilado.

Catálogo: 1.287 entradas de apresentação. Para atualizar traduções, edite
PT-BR-LABELS.json e execute `python scripts/generate_menu_catalog.py`.
`--check` verifica que o header e o JSON estão sincronizados.

A inspeção visual, o tamanho dos textos, a posição dos painéis de ajuda e o
botão físico Mouse4 precisam de avaliação local; testes portáveis não comprovam
interação gráfica dentro do TF2.

## Downloads

- [Uniboxx64Release](https://github.com/guilhermelourencoismart-bot/UBERMENU/actions/runs/41384099020/artifacts/13114233796)
- [Uniboxx64ReleaseAVX2](https://github.com/guilhermelourencoismart-bot/UBERMENU/actions/runs/41384099020/artifacts/13115100957)
- [Amalgam-Ultimate-source](https://github.com/guilhermelourencoismart-bot/UBERMENU/actions/runs/41384099020/artifacts/13114797778)
