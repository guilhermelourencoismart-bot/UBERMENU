#include "../Unibox/src/Features/ImGui/Menu/Descriptions.h"
#include <cassert>
#include <iostream>
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
 std::cout<<"8 presentation/help checks passed\n";
}
