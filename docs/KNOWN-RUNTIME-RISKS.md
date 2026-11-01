# Riscos de execução ainda não avaliados

A ausência de TF2 no cloud não impede a entrega compilada. Nenhum teste de
carregamento, partida, GPU ou gameplay foi executado aqui. Estes itens são
limites de evidência e pontos de observação, não falhas inventadas.

- Assinaturas, offsets e contratos do SDK dependem da versão do cliente TF2.
- Os snapshots têm mutex; captura de entidades e eventos usa a premissa de
  thread do Unibox. O teste concorrente do cache não prova ausência de todas
  as races na aplicação completa.
- Texturas de classe/arma e avatars dependem dos recursos/caches nativos.
  Ícones ausentes não recebem placeholders de funcionalidades. Testar assets,
  dispositivo perdido/resetado, troca de resolução e unload.
- EnemyCam usa o render target full-frame nativo e o material da CameraWindow;
  a câmera de projéteis tem prioridade. Medir ciclo de vida, cropping, mapas,
  resolução, recursão e nomes/vida no overlay com as duas opções combinadas.
- Os solvers V2 usam registros e simulações nativos adaptados. As verificações
  matemáticas não cobrem precisão contra entidades reais, latência, colisões,
  armas exclusivas, Navbot/Followbot ou estabilidade de predição no jogo.
- Round-trip do codec e migrações foram testados isoladamente. Carregamento
  com o menu aberto, aplicação de binds e configs completas no processo ainda
  deve ser avaliado pelo proprietário.
- Sem benchmark, não há resultado medido de FPS/latência nem ganho de precisão.
- ProfileStalker continua indisponível sem seus dois fontes originais; sua
  integração opcional não foi removida nem substituída por implementação falsa.

Use LOCAL-TESTING.md para registrar evidências e distinguir defeitos observados
faltantes de cenários não executados. Nenhum desses cenários bloqueia por si só
a publicação dos artefatos que passarem na compilação e nos testes obrigatórios.
