# Diferenças, cores e interação dos recursos

Este guia descreve o código da candidata compilada f180e6b4. Não substitui
os testes locais de comportamento. Nenhuma mudança de DLL é necessária para
usar os controles de cores descritos abaixo.

## Por que uma caixa azul vira vermelha

O ESP principal Comp consulta o alvo compartilhado do Aimbot (G::AimTarget).
Para jogadores, a prioridade é: Local; Target quando o índice coincide com o
alvo; depois a cor de time ou relação. Target é vermelho por padrão. O ESP
mostra esse estado; não escolhe o alvo e não aumenta o alcance da mira.

O Hitscan V2 pode registrar o primeiro candidato antes de CanHit/ShouldFire.
Portanto, vermelho não garante tiro, acerto, visibilidade ou solução válida.
Estar dentro do FOV pode coincidir com a seleção, mas o FOV sozinho não determina
a cor. FOV é uma restrição angular; filtros, prioridade, arma e hit detection
participam da seleção.

Controle já existente:
Visuals → View → Comp visuals → Module → Radar colors → Target color.
Apesar do nome Radar colors, essa paleta também é usada pelo ESP principal Comp.
A cor Target afeta o destaque que usa essa paleta, sobretudo caixas/bones;
nomes, barras, Chams, Glow e módulos com cores próprias não mudam necessariamente.

Relative colors desligado: RED color / BLU color seguem o time absoluto.
Relative colors ligado: Enemy color / Team color seguem a relação com o local.
Local color e Target color têm precedência no ESP principal para jogadores.
Para manter a mesma cor em inimigos selecionados ou não: ligue Relative colors
e iguale Enemy color e Target color. Isso não afeta a lógica de mira.
Não existe nessa DLL um toggle independente para desativar só o destaque Target.
Igualar cores é uma alternativa visual, não a inclusão desse toggle.

## Sistemas e diferenças

| Sistema | Papel e combinação |
|---|---|
| ESP Unibox | Grupos nativos com seus próprios filtros/estilos. Mantido quando Use Comp main ESP está desligado |
| Main ESP Comp | Outro desenho principal, com máscaras de categorias, flags e paleta Comp; não desenha simultaneamente com o principal Unibox |
| Aimbot | Seleciona alvo e calcula/aplica a mira conforme modo/condições; o ESP pode apresentar o alvo compartilhado |
| FOV circle | Visualiza uma restrição angular; não é um raio de distância nem confirmação de disparo |
| Radar | Posições e filtros independentes do FOV de mira; compartilha parte da paleta, não todas as regras do ESP principal |
| Offscreen players | Indicadores de jogadores fora da tela; cores geradas por Steam ID, não a cor Target do principal |
| Chams / Glow | Materiais/efeitos de modelos nativos, com controles próprios; não são a cor da caixa do ESP |

## Combinações e supressão de duplicação

| Combinação | Comportamento implementado |
|---|---|
| Main ESP + HealthBarESP | A barra principal Comp realmente desenhada tem prioridade; a barra auxiliar é suprimida nessa entidade. Polígonos são recurso separado. No principal Unibox, o auxiliar consulta a opção HealthBar do grupo |
| Main ESP + MedicUberBar | A barra uber principal Comp desenhada suprime a auxiliar nessa entidade; no nativo, é consultado UberBar do grupo |
| Main ESP + SentryESP | Caixa principal tem prioridade; SentryESP ainda pode fornecer aim line e seleção Chams conforme opções |
| Main ESP + StickyESP | Caixa principal pode suprimir as caixas do auxiliar; seleção Chams segue seu caminho próprio |
| Offscreen players + setas nativas | O módulo assume as setas de jogadores; outros grupos nativos são preservados |
| AmmoTracker + pickup timers | AmmoTracker assume o desenho de timers enquanto habilitado |
| EnemyCam + Projectile camera | Compartilham CameraWindow; a câmera de projéteis tem prioridade |
| FocusFire + Aimbot | FocusFire sinaliza dano de vários atacantes; isso não é garantia de seleção ou ordem de prioridade do Aimbot |
| UberTracker + MedicUberBar | Resumo de medics/vantagem e indicador individual de charge se complementam; estimativas não são informação garantida do servidor |
| PlayerTrails / HiderESP / PylonESP | Acrescentam histórico, tempo parado e indicação de medics ocultos/distantes; não equivalem a alvo automaticamente selecionado |

Para experimentar a barra ou caixa auxiliar, desligue primeiro a primitiva
equivalente no principal. Se houver Chams/Glow/FocusFire, desligue-os para
identificar qual sistema está produzindo a mudança visual.

## Qual Aimbot está sendo usado

O seletor V2 não força a substituição de extensões exclusivas. Forward Track
seleciona o caminho nativo para Hitscan; Lob Angles ou Target Dormant selecionam
o nativo para Projectile. Passtime, grappling hook, laser pointer e mechanical
arm também mantêm compatibilidade nativa. A regra escolhe um solver por comando.
Lob Angles está habilitado por padrão na base; desative-o e Target Dormant para
avaliar Projectile V2. Desative Forward Track para avaliar Hitscan V2.

Smooth interpola a mira; cubic/remapped são opções desse modo. SmoothVelocity
usa a curva e o fator de velocidade compartilhados. Assistive depende também
do movimento de mira do usuário. Nenhum desses modos promete disparo automático
ou melhora medida de precisão/FPS.

## Perfil simples para entender a interação

1. Faça backup e crie uma config nova. Desligue Chams, Glow e auxiliares.
2. Ative apenas Main ESP Comp: Players, Enemy, Box e Bones.
3. Em Radar colors, ligue Relative colors; escolha Enemy azul e Target amarelo.
4. Ative o Aimbot por um bind e observe a mudança com um bot de cada vez.
5. Igualar Target e Enemy deve eliminar a diferença visual, mantendo a mira.
6. Acrescente um módulo por vez; teste a supressão de barras/caixas e a câmera.
7. Salve/recarregue para conferir persistência. Registre opções e capturas se
   algo divergir, conforme LOCAL-TESTING.md.

Config antiga pode produzir mensagens de chave ausente e restaurar defaults
nas opções novas; isso não prova conflito entre módulos. Overrides de binds
também podem mudar valores enquanto ativos. Compare com uma config nova e
confira o contexto do bind antes de concluir que uma cor não é configurável.

## Navegação na atualização PT-BR

Na atualização de menu posterior à candidata f180e6b4, use **Visuais → ESP Comp**.
A paleta fica na categoria **ESP e cores**, módulo **Paleta do ESP e Radar**.
As regras de cores e os algoritmos descritos acima permanecem iguais.
Para novos nomes de categorias, ajuda ao passar o mouse e Mouse4, veja MENU-PTBR.md.
