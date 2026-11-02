#pragma once
#include "Localization.h"
namespace MenuPTBR {
inline std::string Help(std::string_view key,const char* caption,bool toggle,bool color,bool choices) {
 static const std::map<std::string,std::string,std::less<>> exact={
 {"Vars::Comp::Palette::Target","Cor da caixa e do esqueleto do jogador selecionado pelo Aimbot. Não confirma disparo ou acerto. A paleta também atende ao ESP principal Comp."},
 {"Vars::Comp::Palette::Relative","Ligado: usa cores de aliado e inimigo. Desligado: usa as cores das equipes RED e BLU. A cor do alvo selecionado tem prioridade."},
 {"Vars::Comp::ESP::Enabled","Escolhe o ESP principal Comp. Desligado, o ESP por grupos do Unibox permanece disponível. Apenas um desenho principal é usado."},
 {"Vars::Aimbot::General::Solver","Escolhe o sistema da mira. Forward Track, Lob Angles, alvos inativos e armas exclusivas mantêm o caminho Unibox, mesmo com V2 selecionado."},
 {"Vars::Aimbot::General::AimFOV","Limita o ângulo em que a mira procura candidatos. Não limita a distância e não garante visibilidade ou acerto."},
 {"Vars::Aimbot::General::AutoShoot","Permite disparar quando a mira e a arma atendem às condições de tiro. Desligado, você controla o disparo."},
 {"Vars::Aimbot::General::AimType","Escolhe como a mira é aplicada: direta, suavizada, assistida ou silenciosa. Os filtros de alvo e as condições da arma continuam valendo."},
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
 if(color)return "Altera a cor e a transparência de “"+label+"” "+context+". Efeitos com paleta própria não são alterados.";
 if(choices)return "Seleciona “"+label+"” "+context+". Opções com várias marcações permitem combinar escolhas.";
 if(toggle)return "Ativa ou desativa “"+label+"” "+context+".";
 return "Ajusta “"+label+"” "+context+". O valor atual e a unidade são mostrados no controle.";
}
}
