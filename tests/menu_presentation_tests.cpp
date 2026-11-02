#include "../Unibox/src/Features/ImGui/Menu/Descriptions.h"
#include <cassert>
#include <iostream>
#include <vector>
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
 std::cout<<"13 presentation/help checks passed\n";
}
