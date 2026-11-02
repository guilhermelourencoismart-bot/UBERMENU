#include "../Unibox/src/Features/ImGui/Menu/Descriptions.h"
#include <cassert>
#include <iostream>
#include <vector>
#include "../Unibox/src/Features/ImGui/Menu/PresentationState.h"
int main() {
 assert(std::string(MenuPTBR::Text("Target color"))=="Cor do alvo selecionado");
 assert(std::string(MenuPTBR::Text("Vars::Comp::Palette::Target"))=="Vars::Comp::Palette::Target");
 assert(std::string(MenuPTBR::Text("##stable-id"))=="##stable-id");
 const char* first=MenuPTBR::Text("Enemy color"); MenuPTBR::Text("Target color");
 assert(std::string(first)=="Cor dos inimigos");
 assert(MenuPTBR::Help("Vars::Comp::Palette::Target","Target color",false,true,false).find("Não confirma")!=std::string::npos);
 assert(MenuPTBR::Help("Vars::Menu::SecondaryKey","Secondary key",false,false,false).find("Mouse4")!=std::string::npos);
 assert(MenuPTBR::Help("Vars::Aimbot::General::Solver","Aimbot solver",false,false,true).find("Unibox")!=std::string::npos);
 assert(!MenuPTBR::Help("Vars::Comp::Radar::Main::Range","Range",false,false,false).empty());
 struct Variable{std::vector<const char*> m_vNames,m_vValues;size_t m_iType;};
 Variable asset{{"Target color"},{"None","Target color"},typeid(std::string).hash_code()};
 MenuPTBR::LocalizePresentation(asset);
 assert(std::string(asset.m_vNames.front())=="Cor do alvo selecionado");
 assert(std::string(asset.m_vValues[0])=="None" && std::string(asset.m_vValues[1])=="Target color");
 Variable choice{{"Target color"},{"None"},typeid(int).hash_code()};MenuPTBR::LocalizePresentation(choice);
 assert(std::string(choice.m_vValues.front())=="Nenhum");
 assert(MenuPTBR::SearchText("DISTÂNCIA e AÇÃO")=="distancia e acao");
 assert(MenuPTBR::SearchText("Ângulos")==MenuPTBR::SearchText("angulos"));
 assert(std::string(MenuPTBR::Text("Dead ringer"))=="Dead Ringer preparado");
 assert(std::string(MenuPTBR::Text("Sentry"))=="Torre automática");
 assert(std::string(MenuPTBR::Text("Unsimulated"))=="Sem atualização de movimento");
 assert(MenuPTBR::ChoiceHelp("Vars::Aimbot::General::AimType",MenuPTBR::Text("Plain")).find("não influenciam")!=std::string::npos);
 assert(MenuPTBR::ChoiceHelp("Vars::Aimbot::General::Ignore",MenuPTBR::Text("Dead ringer")).find("Spies")!=std::string::npos);
 assert(MenuPTBR::ChoiceHelp("Vars::Skins::Name",MenuPTBR::Text("Plain")).empty());
 MenuPresentation::State favorites;
 assert(favorites.Add("Vars::Aimbot::General::AimFOV"));
 assert(!favorites.Add("Vars::Aimbot::General::AimFOV"));
 assert(!favorites.Add("arbitrary-key"));
 assert(favorites.Contains("Vars::Aimbot::General::AimFOV"));
 assert(favorites.Remove("Vars::Aimbot::General::AimFOV") && favorites.favorites.empty());
 for(size_t i=0;i<MenuPresentation::MaxFavorites;++i) assert(favorites.Add("Vars::Test::"+std::to_string(i)));
 assert(!favorites.Add("Vars::Test::overflow"));
 const auto critHelp=MenuPTBR::Help("Vars::CritHack::ForceCrits","Force crits",true,false,false);
 assert(critHelp.find("Não funciona em toda arma")!=std::string::npos);
 assert(critHelp.find("100%")!=std::string::npos && critHelp.find("servidor")!=std::string::npos);
 assert(MenuPTBR::Help("Vars::CritHack::CritEffects","Crit effects",true,false,false).find("não cria dano")!=std::string::npos);
 assert(MenuPTBR::Help("Vars::Radar::Players","Radar players",true,false,false).find("não basta")!=std::string::npos);
 assert(std::string(MenuPTBR::Text("Force crits"))=="Tentar críticos");
 std::cout<<"30 presentation/help checks and 64 favorite capacity checks passed\n";
}
