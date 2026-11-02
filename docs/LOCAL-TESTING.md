# Testes locais e relato de falhas

A entrega deve ser identificada pelo commit e BUILD-MANIFEST.json do pacote.
Não é necessário aguardar uma sessão TF2 para gerar a candidata compilada.
Este procedimento é para o proprietário avaliar o comportamento posteriormente.

1. Registre Windows, CPU (inclusive suporte AVX2), GPU/driver, resolução, versão
   do TF2, commit, variante DLL e SHA256. Guarde o PDB da mesma build.
2. Faça backup das configurações. Registre o método de inicialização já usado
   no seu ambiente e quaisquer mensagens de erro; esta entrega não inclui um
   novo carregador. Anote mapa, modo, arma/classe e condições do teste.
3. Compare primeiro com os módulos desligados. Habilite um módulo por vez;
   registre as opções e os passos exatos. Teste combinações depois, incluindo
   Comp/native ESP, HealthBar, caixas, timers/setas e câmera de projéteis.
4. Salve, recarregue e compare config/binds. Teste cópia de uma config antiga.
   Registre o nome/caminho e forneça uma versão sem informações privadas.
5. Exercite morte/respawn, troca de classe/arma, desconexão, troca de mapa,
   resolução, screenshots, unload/reload. Para EnemyCam, teste todos os modos,
   desaparecimento do alvo e prioridade da câmera de projéteis.
6. Para Aimbot, registre backend, modo, arma, filtros, binds e cenário. Para
   Projectile V2, desative Lob Angles/Target Dormant; para Hitscan V2, Forward
   Track deve estar desligado. Essas extensões usam o solver nativo. Compare
   Passtime, armas especiais, Navbot e Followbot com a configuração original.
   Separe erro determinístico, resultado intermitente e comportamento esperado.

Para cada falha, envie um relato contendo:

- Commit/variante e BUILD-MANIFEST.json; resultado esperado e observado.
- Passos mínimos, frequência e se reaparece após reinício/config limpa.
- Logs de inicialização, console e MSBuild relevantes, com horário da falha.
- Capturas/vídeo mostrando menu, valores e resultado; sem dados privados.
- Em crash: dump e stack trace quando disponíveis, módulo/endereço e PDB exato.
  Dumps podem conter dados da memória; revise antes de compartilhar.

Marque cada cenário como aprovado, falhou ou não executado. Guarde capturas e
logs com o mesmo identificador do cenário. Não atribua uma falha a um módulo
somente porque ele ainda não foi testado. Relatórios/dumps reais orientarão as
correções; este documento não afirma qualquer teste de jogo já realizado.

## Menu compacto e revisão de linguagem

1. Abra o menu em Início → Essencial. Compare os valores de Aimbot e Radar com
   Personalizar e com as respectivas páginas completas. Alterar um valor em
   qualquer apresentação deve aparecer na outra, sem redefinir os demais.
2. Use botão direito em FOV, escolha Adicionar aos favoritos e confira
   Início → Favoritos. Salve o perfil, recarregue o módulo e confira a lista.
   Remova o favorito e confirme que o valor do recurso foi preservado.
3. Busque por 'distancia', 'ângulos', 'torres' e 'Sem atualização'. Confira também
   que os identificadores Vars:: continuam localizáveis pela busca.
4. Passe o mouse no modo de mira selecionado e nas alternativas da lista aberta.
   Plain deve informar ausência de suavização; Dead Ringer deve explicar
   preparação de morte falsa, e não um Spy morto.
5. Compare o indicador de roteamento com o motor escolhido e as extensões da
   arma. 'Unibox (compatibilidade)' pode aparecer com V2 selecionado. O texto
   não comprova alvo adquirido ou disparo.
6. Registre capturas de textos cortados ou traduções ruins com caminho da página,
   nome da opção, modo selecionado, escala do menu, resolução e commit da DLL.
   Anexe a configuração usada, removendo nomes pessoais quando necessário.

Esta lista é um roteiro para avaliação local, não um registro de testes realizados.
