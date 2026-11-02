#pragma once
#include "Localization.h"
namespace MenuPTBR {
inline std::string ChoiceHelp(std::string_view field, std::string_view entry) {
 // Match only feature fields, so names of skins/assets never acquire gameplay help.
 if(field.find("Vars::Aimbot::General::AimType")!=field.npos) {
  if(entry==Text("Plain")) return "Puxa a mira diretamente para o alvo ao aplicar o disparo. Smooth e curva de suavização não influenciam este modo.";
  if(entry==Text("Smooth")) return "Move a mira gradualmente até o alvo. O algoritmo, o fator e a curva de suavização determinam como ela se aproxima.";
  if(entry==Text("SmoothVelocity")) return "Suaviza a aproximação levando em conta a velocidade do alvo. Usa a curva compartilhada por velocidade, não os algoritmos cúbico e remapeado do Smooth.";
  if(entry==Text("Silent")) return "Aplica a direção do tiro no comando enviado ao jogo sem acompanhar essa direção com a câmera. Não usa suavização.";
  if(entry==Text("Locking")) return "Mantém o direcionamento para o alvo nos comandos, mesmo entre disparos. Não usa curva de suavização.";
  if(entry==Text("Assistive")) return "Corrige a direção do movimento que você faz com o mouse em direção ao alvo. A assistência depende do seu movimento e da força configurada.";
  if(entry==Text("Legit")) return "Mantém a direção atual da mira; o teste de acerto decide quando o disparo pode ocorrer. Não puxa a câmera até o alvo.";
  if(entry==Text("Off")) return "Suspende a aplicação da mira automática. Os indicadores visuais possuem controles independentes.";
 }
 if(field.find("Vars::Aimbot::General::SmoothingAlgorithm")!=field.npos) {
  if(entry==Text("Unibox")) return "Usa a suavização original do Unibox. Apenas os modos de mira suave usam este ajuste.";
  if(entry==Text("V2 cubic")) return "Usa uma progressão cúbica do V2 para aproximar a mira. Só afeta Smooth; o fator V2 ajusta a velocidade da aproximação.";
  if(entry==Text("V2 remapped")) return "Reorganiza a intensidade da aproximação ao longo do movimento. Só afeta Smooth e depende do fator V2; não modifica Plain.";
 }
 if(field.find("TargetSelectionHitscan")!=field.npos || field.find("TargetSelectionProjectile")!=field.npos || field.find("TargetSelectionMelee")!=field.npos) {
  if(entry==Text("FOV")) return "Prioriza quem está mais perto do centro da sua mira, e não necessariamente o inimigo mais próximo no mapa.";
  if(entry==Text("Distance")) return "Prioriza o candidato fisicamente mais próximo. Os limites de visibilidade e alcance da arma continuam valendo.";
  if(entry==Text("Hybrid")) return "Combina proximidade e ângulo para ordenar alvos. Não garante que o primeiro candidato possa ser atingido.";
  if(entry==Text("Most health")) return "Prioriza alvos com mais vida. Isso muda a ordem de avaliação, não o dano causado.";
  if(entry==Text("Least health")) return "Prioriza alvos com menos vida. Não ignora bloqueios ou proteções.";
 }
 if(field.find("Vars::Aimbot::General::Ignore")!=field.npos) {
  if(entry==Text("Friends")) return "Não considerar jogadores marcados como amigos na Steam. Este filtro não impede que o ESP os exiba.";
  if(entry==Text("Party")) return "Não considerar integrantes do seu grupo (party) como alvos da mira.";
  if(entry==Text("Unprioritized")) return "Não mirar em jogadores sem uma prioridade marcada; pode deixar vários inimigos fora da seleção.";
  if(entry==Text("Team")) return "Ignorar seus companheiros de equipe na seleção de alvos.";
  if(entry==Text("Disguised")) return "Ignorar Spies disfarçados. Não altera a exibição dos disfarces no ESP.";
  if(entry==Text("Taunting")) return "Ignorar jogadores que estão executando uma provocação (taunt).";
  if(entry==Text("Vaccinator")) return "Filtrar jogadores protegidos pelo Vaccinator conforme o tipo de dano relevante; não remove a proteção do jogador.";
  if(entry==Text("Unsimulated")) return "Exclui jogadores cujo movimento ficou sem atualização por mais ticks que a tolerância permitida. Aplica-se a hitscan e corpo a corpo; o teste está desativado no caminho de projéteis. Tick é um passo de simulação do jogo.";
  if(entry==Text("Dead ringer")) return "Exclui Spies com o Dead Ringer preparado para fingir a morte. Não significa que o Spy já morreu.";
  if(entry==Text("Sentry busters")) return "Exclui os robôs destruidores de torres do modo Mann vs. Machine.";
  if(entry==Text("Invisible")) return "Exclui jogadores invisíveis conforme o limite de invisibilidade configurado.";
  if(entry==Text("Invulnerable")) return "Exclui jogadores protegidos contra dano, como durante uma Über de invulnerabilidade, respeitando as exceções da arma.";
 }
 return {};
}
inline std::string Help(std::string_view key,const char* caption,bool toggle,bool color,bool choices) {
 static const std::map<std::string,std::string,std::less<>> exact={
 {"Vars::CritHack::ForceCrits","Tenta reservar disparos críticos aleatórios quando a arma e o servidor permitem. Não funciona em toda arma nem garante 100% dos tiros críticos: depende da reserva disponível, dos limites do jogo e de encontrar um disparo válido."},
 {"Vars::CritHack::AvoidRandomCrits","Tenta escolher disparos sem crítico aleatório. Não remove críticos garantidos por condições do jogo, como uma Über de críticos, e não impede todo tipo de dano crítico."},
 {"Vars::CritHack::AlwaysMeleeCrit","Pede crítico nos ataques corpo a corpo associados ao alvo do Aimbot. Ainda exige arma compatível e reserva disponível; o nome da opção não significa garantia de crítico em cada golpe."},
 {"Vars::CritHack::CritEffects","Mostra efeitos de crítico quando o recurso considera o crítico disponível ou já ativo. É uma indicação visual: não cria dano crítico e não comprova o resultado aceito pelo servidor."},
 {"Vars::Radar::Enabled","Mostra um mapa simples ao seu redor com as entidades escolhidas. Não revela posições que o jogo não forneceu ao cliente; jogadores, construções e itens têm filtros separados."},
 {"Vars::Radar::Players","Inclui jogadores no Radar. Marcar apenas os filtros de inimigo ou aliado não basta se esta opção estiver desligada."},
 {"Vars::Radar::Range","Distância do mundo representada no Radar. Aumentar mostra uma área maior no mesmo espaço da tela. Entidades além desse limite podem ficar na borda se essa opção estiver habilitada."},
 {"Vars::Comp::ESP::Draw","Escolhe os tipos de entidade desenhados: jogadores, construções, projéteis e itens. Marcar um tipo ainda exige selecionar os filtros e informações correspondentes; não muda os alvos do Aimbot."},
 {"Vars::Comp::ESP::Player","Escolhe quais jogadores aparecem e o que mostrar neles: nome, caixa, vida, arma e outras informações. Se nenhum grupo de jogadores estiver marcado, adicionar caixas ou nomes não faz ninguém aparecer."},
 {"Vars::Comp::ESP::Building","Escolhe as construções e informações desenhadas sobre elas. Esses filtros são visuais: não fazem o Aimbot atacar construções automaticamente."},
 {"Vars::Comp::Palette::Enemy","Cor dos inimigos no ESP Comp e nos elementos do Radar que usam essa paleta. O alvo selecionado, amigos e outras prioridades podem usar cores próprias."},
 {"Vars::Comp::Palette::Team","Cor dos aliados nos elementos que usam a paleta por relação. Não muda a cor de efeitos que possuem configuração própria."},
 {"Vars::Comp::Palette::Target","Cor da caixa e do esqueleto do jogador selecionado pelo Aimbot. Não confirma disparo ou acerto. A paleta também atende ao ESP principal Comp."},
 {"Vars::Comp::Palette::Relative","Ligado: usa cores de aliado e inimigo. Desligado: usa as cores das equipes RED e BLU. A cor do alvo selecionado tem prioridade."},
 {"Vars::Comp::ESP::Enabled","Escolhe o ESP principal Comp. Desligado, o ESP por grupos do Unibox permanece disponível. Apenas um desenho principal é usado."},
 {"Vars::Aimbot::General::Solver","Escolhe o sistema da mira. Forward Track, Lob Angles, alvos inativos e armas exclusivas mantêm o caminho Unibox, mesmo com V2 selecionado."},
 {"Vars::Aimbot::General::AimFOV","Limita o ângulo em que a mira procura candidatos. Não limita a distância e não garante visibilidade ou acerto."},
 {"Vars::Aimbot::General::AutoShoot","Permite disparar quando a mira e a arma atendem às condições de tiro. Desligado, você controla o disparo."},
 {"Vars::Aimbot::General::AimType","Auxilia automaticamente a mira nos alvos permitidos pelos filtros. Não faz a arma atravessar paredes nem garante acertos. Plain direciona no disparo; Smooth aproxima gradualmente. Passe o mouse nos modos para comparar."},
 {"Vars::Aimbot::General::Target","Define que objetos a mira pode considerar: jogadores, torres, dispensadores, teleportadores, stickies e outros. Selecionar um tipo não garante que a arma consiga atingi-lo."},
 {"Vars::Aimbot::General::TargetSelectionHitscan","Ordena os alvos de armas hitscan (como rifles) por ângulo, distância ou vida. A ordem não dispensa a verificação de acerto."},
 {"Vars::Aimbot::General::TargetSelectionProjectile","Ordena os candidatos de foguetes, granadas e outros projéteis antes de calcular a trajetória; não altera a velocidade real do projétil."},
 {"Vars::Aimbot::General::TargetSelectionMelee","Define a prioridade de inimigos para ataques corpo a corpo. O alcance da arma e a simulação do golpe continuam obrigatórios."},
 {"Vars::Aimbot::General::MaxTargets","Limita quantos alvos o sistema avalia em cada ciclo. Mais candidatos podem ajudar em situações cheias, mas aumentam o trabalho do processador."},
 {"Vars::Aimbot::General::NoSpread","Tenta compensar a dispersão dos tiros onde o comportamento da arma e do servidor permite. Não elimina toda aleatoriedade nem faz todo disparo acertar."},
 {"Vars::Aimbot::General::IgnoreInvisible","Define o limiar de invisibilidade que faz um Spy ser ignorado. Só tem efeito se o filtro de invisíveis também estiver marcado."},
 {"Vars::Aimbot::General::FOVCircle","Exibe na tela o limite angular de procura. Esconder o círculo não desliga o limite do Aimbot."},
 {"Vars::Aimbot::General::SmoothingAlgorithm","Seleciona o algoritmo de aproximação quando o modo Smooth está em uso. Plain e Silent não são afetados; SmoothVelocity tem cálculo próprio."},
 {"Vars::Aimbot::General::LeadAndRestrict","Exibe limites e previsões relacionados à mira. É uma apresentação visual; não força a trajetória da arma nem garante acerto."},
 {"Vars::Aimbot::Hitscan::Hitboxes","Define quais regiões do corpo a mira testa em armas de acerto instantâneo. Selecionar mais regiões aumenta alternativas, mas pode aumentar o custo de cálculo."},
 {"Vars::Aimbot::Hitscan::MultipointHitboxes","Permite testar mais de um ponto em regiões do corpo escolhidas. Ajuda na checagem de visibilidade, mas pode exigir mais processamento."},
 {"Vars::Aimbot::Hitscan::MultipointScale","Ajusta a distribuição dos pontos adicionais dentro da região de acerto. Não muda o tamanho real da hitbox do jogo."},
 {"Vars::Aimbot::Melee::AutoBackstab","Tenta executar uma facada pelas costas com Spy quando houver oportunidade válida. Não garante backstab se posição, alcance ou proteção não permitirem."},

 {"Vars::Aimbot::General::Ignore","Retira da busca os jogadores nas condições marcadas, mesmo que estejam dentro do FOV. Passe o mouse nas condições para entender os filtros técnicos."},
 {"Vars::Aimbot::General::TickTolerance","Quantidade de passos de simulação sem atualização de movimento tolerada pelo filtro 'Sem atualização de movimento'. Aumentar aceita posições mais atrasadas; não corrige o atraso."},
 {"Vars::Aimbot::General::AssistStrength","Força de aproximação da mira nos modos que usam a suavização compartilhada. Não altera Plain ou Silent; algoritmos próprios do Smooth V2 usam o fator V2."},
 {"Vars::Aimbot::General::SmoothCurve","Distribui a velocidade de aproximação: início rápido, final rápido ou avanço linear. Afeta a suavização compartilhada; não altera Plain ou Silent."},
 {"Vars::Aimbot::General::SmoothCurveAmount","Intensidade da curva de aproximação. Zero reduz a influência da curva. Não altera Plain, Silent ou os algoritmos próprios do Smooth V2."},
 {"Vars::Aimbot::General::V2SmoothFactor","Controla o passo dos algoritmos cúbico e remapeado no modo Smooth. Valores maiores tornam a aproximação mais lenta. Não altera Plain ou SmoothVelocity."},
 {"Vars::Aimbot::General::SmoothingAlgorithm","Escolhe a curva do modo de mira suave. Suave por velocidade mantém sua própria curva ligada à velocidade do alvo."},
 {"Vars::Menu::SecondaryKey","Segundo botão para abrir ou fechar o menu. O padrão é Mouse4, o primeiro botão lateral do mouse. A tecla principal continua disponível."},
 {"Vars::Menu::PrimaryKey","Tecla principal para abrir ou fechar o menu. O atalho secundário também funciona."},
 {"Vars::Aimbot::Hitscan::ForwardTrack","Prevê a posição futura do alvo para armas de acerto instantâneo. Ao ativar, essa arma usa o sistema Unibox para preservar a função."},
 {"Vars::EnemyCam::Enabled","Mostra outra visão usando o inimigo escolhido. A câmera de projéteis tem prioridade quando as duas estão ativas."},
 {"Vars::Comp::EnemyCam::Mode","Escolhe como procurar o inimigo da câmera: mais próximo, curado, médico, maior pontuação ou aleatório."},
 {"Vars::Comp::EnemyCam::TrackTime","Tempo de acompanhamento antes de procurar outro alvo. Alvos inválidos também provocam nova busca."},
 {"Vars::Comp::HealthBarESP::ShowHealthBars","Mostra barras auxiliares de vida. Uma barra já desenhada pelo ESP principal pode suprimir esta para evitar duplicação."},
 {"Vars::Comp::MedicUberBar::ShowPercentage","Mostra o número da carga de Über junto à barra auxiliar do médico."},
 {"Vars::Comp::Radar::Main::DrawOutOfRange","Mantém entidades além do alcance na borda do Radar. Desligado, essas entidades são ocultadas."},
 {"Vars::Comp::Radar::Main::Style","Escolhe Radar circular ou retangular. Os filtros de entidades continuam independentes dos filtros do Aimbot."},
 {"Vars::Comp::Features::FocusFire","Destaca inimigos recebendo ataques de várias pessoas na janela de tempo configurada. Não força o Aimbot a escolher esses inimigos."},
 {"Vars::Comp::Features::PylonESP","Marca médicos inimigos ocultos e distantes com indicadores verticais. Respeita as opções de distância e aparência."},
 {"Vars::Comp::Features::HiderESP","Marca inimigos que ficam parados pelo tempo configurado. A tolerância de movimento determina quando a contagem é reiniciada."},
 {"Vars::Comp::Features::CritHeals","Indica oportunidades de cura crítica conforme dano recente e sobrecura; pode avisar sobre redução na construção de Über."},
 {"Vars::Comp::Features::CraterCheck","Estima dano de queda na superfície apontada e diferencia pouso seguro e letal. É uma estimativa, não uma garantia."},
 {"Vars::Comp::Features::SplashRadius","Desenha áreas de explosão dos projéteis selecionados. Pode estimar o impacto de foguetes e unir círculos sobrepostos."},
 {"Vars::Comp::Features::UberTracker","Mostra resumo dos médicos, cargas e vantagem entre equipes. Informações estimadas não são confirmação do estado do servidor."},
 {"Vars::Comp::Features::MedicUberBar","Mostra carga de Über nos médicos filtrados. Uma barra principal já desenhada pode ter prioridade."},
 {"Vars::HealthOverlay::Enabled","Mostra barras de vida e polígonos auxiliares. A barra do ESP principal pode ter prioridade; o polígono é controlado separadamente."},
 {"Vars::PlayerTrails::Enabled","Mostra o caminho recente de jogadores. Os pontos expiram conforme duração, visibilidade e limites configurados."},
 {"Vars::AmmoTracker::Enabled","Mostra estimativas de retorno de suprimentos após a coleta. Substitui o desenho dos temporizadores nativos enquanto ativo."},
 {"Vars::OffScreenIndicators::Enabled","Mostra jogadores fora da tela com setas ou avatares e informações opcionais. Substitui as setas nativas de jogadores."},
 {"Vars::SentryESP::Enabled","Acrescenta direção de mira, perigo, caixas e materiais de torres automáticas. Caixas principais já desenhadas podem ter prioridade."},
 {"Vars::StickyESP::Enabled","Mostra stickies conforme equipe, distância e visibilidade. Caixas principais já desenhadas podem ter prioridade."}
 };
 if(auto it=exact.find(key);it!=exact.end())return it->second;
 std::string context;
 if(key.find("Radar::")!=key.npos)context="no Radar; não altera a seleção da mira";
 else if(key.find("EnemyCam::")!=key.npos)context="na câmera de inimigos; a câmera de projéteis mantém prioridade";
 else if(key.find("HealthBarESP::")!=key.npos || key.find("HealthOverlay::")!=key.npos)context="nos indicadores auxiliares de vida; a barra principal pode ter prioridade";
 else if(key.find("MedicUberBar::")!=key.npos)context="nas barras auxiliares de Über dos médicos";
 else if(key.find("Palette::")!=key.npos)context="na paleta compartilhada do ESP principal Comp e Radar";
 else if(key.find("PlayerTrails::")!=key.npos)context="nos rastros de posições recentes dos jogadores";
 else if(key.find("AmmoTracker::")!=key.npos)context="nos indicadores de retorno de suprimentos";
 else if(key.find("OffScreenIndicators")!=key.npos)context="nos indicadores de jogadores fora da tela";
 else if(key.find("FocusFire::")!=key.npos)context="no destaque de inimigos sob fogo de vários atacantes";
 else if(key.find("HiderESP::")!=key.npos)context="na detecção de inimigos parados";
 else if(key.find("PylonESP::")!=key.npos)context="nos marcadores de médicos ocultos";
 else if(key.find("Aimbot::")!=key.npos)context="no sistema de mira selecionado; filtros e condições da arma continuam valendo";
 else if(key.find("ESP::")!=key.npos)context="no ESP principal; não altera os filtros do Aimbot";
 else if(key.find("Binds::")!=key.npos)context="nos atalhos e suas condições de ativação";
 else context="nesta categoria do menu";
 static const std::map<std::string_view,std::string_view,std::less<>> parameters={
 {"Width","Largura horizontal do desenho. Aumentar deixa o elemento mais largo."},
 {"Height","Altura vertical do desenho. Aumentar deixa o elemento mais alto."},
 {"Thickness","Espessura do traço. Aumentar reforça as linhas sem ampliar a área detectada."},
 {"LineWidth","Espessura das linhas do indicador. Não altera o alcance de detecção."},
 {"Segments","Número de lados usados para desenhar círculos. Mais lados suavizam a borda e exigem mais desenho."},
 {"Radius","Raio da área representada pelo indicador. Não muda o alcance real da arma ou da explosão."},
 {"ShowName","Escreve o nome junto ao marcador para identificar o jogador ou a entidade."},
 {"ShowNames","Escreve nomes junto aos marcadores para identificar jogadores ou entidades."},
 {"ShowDistance","Escreve a distância até a entidade ao lado do indicador. Não filtra entidades distantes por si só."},
 {"ShowHealth","Acrescenta informação de vida ao marcador para comparar o estado dos jogadores."},
 {"ShowHealthBars","Representa a vida restante como uma barra que diminui conforme o jogador recebe dano."},
 {"ShowPercentage","Escreve a porcentagem da carga de Über, além da representação visual da barra."},
 {"ShowSeconds","Escreve os segundos restantes estimados até o suprimento reaparecer."},
 {"ShowMilliseconds","Acrescenta frações de segundo ao tempo estimado de retorno dos suprimentos."},
 {"ShowGroundPieChart","Desenha no chão do local de coleta um círculo de progresso para o retorno do suprimento."},
 {"ShowScreenPieChart","Desenha um círculo de progresso sobre a posição do suprimento na tela."},
 {"ShowHudOverlay","Acrescenta uma representação na interface para acompanhar o retorno dos suprimentos."},
 {"ScaleWithDistance","Diminui o indicador com a distância para reduzir a ocupação da tela por objetos distantes."},
 {"ScaleTextWithDistance","Varia o tamanho do texto com a distância, usando os limites mínimo e máximo configurados."},
 {"TextSizeMin","Menor tamanho permitido para o texto quando ele varia com a distância."},
 {"TextSizeMax","Maior tamanho permitido para o texto quando ele varia com a distância."},
 {"TextSizeDefault","Tamanho do texto quando o escalonamento por distância não está em uso."},
 {"PieChartSegments","Número de lados do círculo de progresso. Mais lados deixam a borda mais suave."},
 {"PieChartSize","Tamanho do círculo que representa o progresso até o retorno do suprimento."},
 {"MinScreenChartSize","Menor tamanho permitido para o círculo na tela quando ele varia com a distância."},
 {"MaxScreenChartSize","Maior tamanho permitido para o círculo na tela quando ele varia com a distância."},
 {"MaxScaleDistance","Distância usada como limite para reduzir o tamanho de textos ou indicadores."},
 {"ShowFill","Preenche o interior da área indicada. A cor e sua transparência determinam quanto do cenário fica encoberto."},
 {"ShowEdge","Desenha o contorno da área indicada, permitindo enxergar seu limite sem depender do preenchimento."},
 {"EdgeWidth","Espessura do contorno da área desenhada. Não muda a área detectada."},
 {"OnlyShowLethal","Oculta estimativas de pouso seguro e mostra somente os locais estimados como letais."},
 {"DrawOutOfRange","Mantém entidades além do alcance na borda do Radar em vez de escondê-las."},
 {"BackgroundAlpha","Opacidade do fundo: zero deixa o fundo invisível; valores maiores facilitam a leitura e encobrem mais o cenário."},
 {"LineAlpha","Opacidade das linhas: zero oculta os traços; valores maiores deixam as linhas mais visíveis."},
 {"Window","Posição e dimensões da janela. Não muda o campo de visão nem o alcance da detecção."},
 {"DormantDuration","Tempo durante o qual informações antigas de uma entidade sem atualização continuam sendo desenhadas."},
 {"DormantPriority","Mantém informações antigas somente para jogadores marcados como prioritários, em vez de todos os jogadores."},
 {"MaxTargets","Limita quantos candidatos são avaliados. Aumentar permite comparar mais alvos e aumenta o trabalho por comando."},
 {"IgnoreInvisible","Porcentagem de invisibilidade usada pelo filtro de jogadores invisíveis. Só tem efeito quando esse filtro está marcado."},
 {"FOVCircle","Desenha o limite angular de busca na tela. O círculo é visual; o FOV continua valendo com o desenho oculto."},
 {"PrioritizeFollowbot","Dá preferência ao alvo usado pelo Followbot na seleção da mira, respeitando as condições da arma."},
 {"PrioritizeNavbot","Dá preferência ao alvo usado pelo Navbot na seleção da mira, respeitando as condições da arma."},
 {"Target","Define quais tipos de entidade podem ser candidatos da mira: jogadores, construções ou outros objetos selecionados."},
 {"TargetSelectionHitscan","Ordena os candidatos de armas de acerto instantâneo por ângulo, distância, vida ou combinação de critérios."},
 {"TargetSelectionProjectile","Ordena os candidatos de armas de projéteis antes da avaliação de trajetória e tempo de impacto."},
 {"TargetSelectionMelee","Ordena os candidatos corpo a corpo antes do teste de alcance e acerto."}
 };
 const auto separator=key.rfind("::");
 const auto leaf=separator==key.npos?key:key.substr(separator+2);
 if(auto it=parameters.find(leaf);it!=parameters.end()) return std::string(it->second)+" Aplicação: "+context+".";
 const std::string label=Text(caption);
 if(key.ends_with("MaxDistance"))return "Distância máxima para mostrar ou considerar entidades neste recurso. Aumentar inclui entidades mais distantes; não altera outros módulos.";
 if(key.ends_with("ShowThroughWalls") || key.ends_with("ThroughWalls"))return "Permite mostrar este indicador mesmo quando uma parede bloqueia a visão. Não dá ao Aimbot permissão para acertar através da parede.";
 if(key.ends_with("Alpha") || key.ends_with("ActiveAlpha"))return "Controla a transparência deste desenho: zero oculta; valores maiores deixam mais visível. As cores de outros módulos continuam independentes.";
 if(key.ends_with("TrailLifetime"))return "Tempo que cada ponto do rastro permanece visível antes de desaparecer. Aumentar conserva mais histórico, sem prever o futuro do jogador.";
 if(key.ends_with("TrackerTimeWindow"))return "Período recente em que ataques são contados para identificar fogo concentrado. Ataques mais antigos deixam de contar.";
 if(key.ends_with("MinAttackers"))return "Número mínimo de atacantes diferentes necessário para destacar um inimigo sob fogo concentrado.";
 if(key.ends_with("TimeToMark"))return "Tempo que o inimigo precisa permanecer parado para receber a marca. Movimento acima da tolerância reinicia a contagem.";
 if(key.ends_with("ShowFriendly"))return "Inclui entidades aliadas neste recurso. Não modifica a seleção de alvos dos outros sistemas.";
 if(key.ends_with("EnemyOnly"))return "Restringe este recurso aos inimigos. Desligado, aliados também podem ser incluídos conforme os demais filtros.";
 if(key.ends_with("ShowChams") || key.ends_with("EnableChams"))return "Aplica o destaque por materiais deste módulo usando o renderizador nativo. As cores e a visibilidade são definidas nas opções deste recurso.";
 if(color)return "Altera a cor e a transparência de '"+label+"' "+context+". Efeitos com paleta própria não são alterados.";
 if(key.find("Show")!=key.npos || key.find("Draw")!=key.npos)return "Informação exibida: "+label+", "+context+". As marcações definem quais elementos aparecem juntos.";
 if(key.find("Size")!=key.npos || key.find("Scale")!=key.npos)return "Dimensão de "+label+", "+context+". Valores maiores ocupam mais espaço na tela.";
 if(key.find("Duration")!=key.npos || key.find("Lifetime")!=key.npos)return "Tempo de permanência de "+label+", "+context+". Valores maiores conservam o efeito por mais tempo.";
 if(key.find("Distance")!=key.npos || key.find("Range")!=key.npos)return "Limite espacial de "+label+", "+context+". Um limite maior inclui posições mais distantes.";
 if(choices)return label+": define quais alternativas serão usadas "+context+". Marcações múltiplas são combinadas; uma escolha única substitui a anterior.";
 if(toggle)return label+": participa "+context+" quando marcado. Os parâmetros relacionados determinam o resultado.";
 return label+": parâmetro de intensidade ou limite "+context+". A unidade apresentada indica a escala usada.";
}
}
