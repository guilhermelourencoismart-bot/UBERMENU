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
 if(field.find("Vars::Aimbot::General::Ignore")!=field.npos) {
  if(entry==Text("Unsimulated")) return "Exclui jogadores cujo movimento ficou sem atualização por mais ticks que a tolerância permitida. Tick é um passo de simulação do jogo.";
  if(entry==Text("Dead ringer")) return "Exclui Spies com o Dead Ringer preparado para fingir a morte. Não significa que o Spy já morreu.";
  if(entry==Text("Sentry busters")) return "Exclui os robôs destruidores de torres do modo Mann vs. Machine.";
  if(entry==Text("Invisible")) return "Exclui jogadores invisíveis conforme o limite de invisibilidade configurado.";
  if(entry==Text("Invulnerable")) return "Exclui jogadores protegidos contra dano, como durante uma Über de invulnerabilidade, respeitando as exceções da arma.";
 }
 return {};
}
inline std::string Help(std::string_view key,const char* caption,bool toggle,bool color,bool choices) {
 static const std::map<std::string,std::string,std::less<>> exact={
 {"Vars::Comp::Palette::Target","Cor da caixa e do esqueleto do jogador selecionado pelo Aimbot. Não confirma disparo ou acerto. A paleta também atende ao ESP principal Comp."},
 {"Vars::Comp::Palette::Relative","Ligado: usa cores de aliado e inimigo. Desligado: usa as cores das equipes RED e BLU. A cor do alvo selecionado tem prioridade."},
 {"Vars::Comp::ESP::Enabled","Escolhe o ESP principal Comp. Desligado, o ESP por grupos do Unibox permanece disponível. Apenas um desenho principal é usado."},
 {"Vars::Aimbot::General::Solver","Escolhe o sistema da mira. Forward Track, Lob Angles, alvos inativos e armas exclusivas mantêm o caminho Unibox, mesmo com V2 selecionado."},
 {"Vars::Aimbot::General::AimFOV","Limita o ângulo em que a mira procura candidatos. Não limita a distância e não garante visibilidade ou acerto."},
 {"Vars::Aimbot::General::AutoShoot","Permite disparar quando a mira e a arma atendem às condições de tiro. Desligado, você controla o disparo."},
 {"Vars::Aimbot::General::AimType","Auxilia automaticamente a mira. Plain direciona o disparo sem suavização; Smooth aproxima a mira gradualmente. Abra a lista e passe o mouse nos modos para comparar seus efeitos."},
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
 {"Vars::SentryESP::Enabled","Acrescenta direção de mira, perigo, caixas e materiais de sentries. Caixas principais já desenhadas podem ter prioridade."},
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
