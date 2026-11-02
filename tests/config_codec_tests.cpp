#include "../Unibox/src/Features/Configs/ConfigCodec.h"
#include <boost/property_tree/json_parser.hpp>
#include <sstream>
#include <map>
#include <iostream>
#include <cstdlib>
static int checks=0;
static void Check(bool result,const char* message){++checks;if(!result){std::cerr<<"FAIL: "<<message<<'\n';std::exit(1);}}
int main()
{
    using namespace ConfigCodec;
    boost::property_tree::ptree tree;
    WriteScalar(tree,"enabled",true);WriteScalar(tree,"factor",25.5f);WriteScalar(tree,"name",std::string("config with accents: ação"));
    std::ostringstream stream;boost::property_tree::write_json(stream,tree);
    boost::property_tree::ptree restored;std::istringstream input(stream.str());boost::property_tree::read_json(input,restored);
    bool enabled=false;float factor=0;std::string name;
    Check(ReadScalar(restored,"enabled",enabled)&&enabled,"boolean roundtrip");
    Check(ReadScalar(restored,"factor",factor)&&factor==25.5f,"float roundtrip");
    Check(ReadScalar(restored,"name",name)&&name=="config with accents: ação","UTF-8 string roundtrip");
    int retained=42;Check(!ReadScalar(restored,"missing",retained)&&retained==42,"old configs retain new defaults");
    restored.put("bad","garbage");Check(!ReadScalar(restored,"bad",retained)&&retained==42,"invalid scalar does not replace previous value");
    restored.put("infinite","nan");Check(!ReadScalar(restored,"infinite",factor)&&factor==25.5f,"nonfinite config rejected");
    Check(ParseBindKey("-1",0,true)==-1,"default bind supported");
    Check(ParseBindKey("1",2,false)==1,"valid bound override");
    for(const auto& key:{"2","-2","x","1suffix","99999999999999999999",""})Check(!ParseBindKey(key,2,false),"invalid bind key rejected");
    Check(!ParseBindKey("0",2,true),"NOBIND cannot acquire overrides");
    std::map<int,int> map{{-1,10},{0,20},{1,30}};
    auto encoded=WriteBindMap(map,[](auto& t,const auto& key,const auto& value){WriteScalar(t,key,value);});
    Check(encoded.get<int>("-1")==10&&encoded.get<int>("0")==20&&encoded.get<int>("1")==30,"bound values preserve IDs and defaults");
    std::istringstream old(R"({"ConVars":{"Vars::Radar::Circular":{"-1":false,"0":true},"Vars::Radar::Size":{"-1":200},"Vars::Radar::PositionX":{"-1":20},"Vars::Radar::PositionY":{"-1":200}}})");
    boost::property_tree::ptree legacy;boost::property_tree::read_json(old,legacy);MigrateLegacyRadar(legacy);
    auto vars=Variables(legacy);
    Check(bool(vars),"legacy ConVars root supported");
    Check(vars->get<int>("Vars::Comp::Radar::Main::Style.-1")==1&&vars->get<int>("Vars::Comp::Radar::Main::Style.0")==0,"legacy circular mode migrated per bind");
    Check(vars->get<int>("Vars::Comp::Radar::Main::Window.-1.x")==120&&vars->get<int>("Vars::Comp::Radar::Main::Window.-1.y")==200,"legacy top-left position converted to native centered window");
    vars->put("Vars::Comp::Radar::Main::Style.-1",0);MigrateLegacyRadar(legacy);
    Check(vars->get<int>("Vars::Comp::Radar::Main::Style.-1")==0,"migration does not overwrite modern settings");
    struct Bind{int m_iParent=-1;};std::vector<Bind> binds{{1},{0},{99},{-9},{4}};SanitizeBindParents(binds);
    Check(binds[0].m_iParent==-1&&binds[2].m_iParent==-1&&binds[3].m_iParent==-1&&binds[4].m_iParent==-1,"cycles self-links and invalid parents normalized");
    std::vector<Bind> valid{{-1},{0},{1}};SanitizeBindParents(valid);Check(valid[2].m_iParent==1&&valid[1].m_iParent==0,"valid bind ancestry preserved");
    bool failed=false;try{std::istringstream malformed("{ broken");boost::property_tree::read_json(malformed,tree);}catch(const boost::property_tree::json_parser_error&){failed=true;}
    Check(failed,"malformed JSON rejected by production parser");
    struct Color{unsigned char r,g,b,a;};Color color{0,42,255,128};WriteColor(tree,"color",color);Color copy{};ReadColor(tree,"color",copy);
    Check(copy.r==0&&copy.g==42&&copy.b==255&&copy.a==128,"RGBA numeric byte roundtrip");
    tree.put("color.r",-1);tree.put("color.g",300);tree.put("color.b","invalid");tree.put("color.a",99);ReadColor(tree,"color",copy);
    Check(copy.r==0&&copy.g==42&&copy.b==255&&copy.a==99,"invalid channels retained, valid alpha updated");
    struct Window{int x,y,w,h;};Window window{-200,30,300,250};WriteWindow(tree,"window",window);Window rect{};ReadWindow(tree,"window",rect);
    Check(rect.x==-200&&rect.y==30&&rect.w==300&&rect.h==250,"camera/radar window roundtrip");
    tree.put("window.w","invalid");tree.erase("missing-window");ReadWindow(tree,"window",rect);ReadWindow(tree,"missing-window",rect);
    Check(rect.w==300&&rect.h==250,"invalid or absent window members retain defaults");
    std::map<int,Color> colorBinds{{-1,{1,2,3,4}},{1,{10,20,30,40}}};
    auto colors=WriteBindMap(colorBinds,[](auto& t,const auto& k,const auto& c){WriteColor(t,k,c);});Color bound{};ReadColor(colors,"1",bound);
    Check(bound.r==10&&bound.a==40,"per-bind module colors serialize through production codec");
    boost::property_tree::ptree oldKeys;
    oldKeys.put("Vars.Vars::Menu::SecondaryKey.-1",0x72);MigrateMenuKey(oldKeys);
    Check(oldKeys.get<int>("Vars.Vars::Menu::SecondaryKey.-1")==5,"stock F3 migrates to Mouse4");
    oldKeys.put("Vars.Vars::Menu::SecondaryKey.-1",0x73);MigrateMenuKey(oldKeys);
    Check(oldKeys.get<int>("Vars.Vars::Menu::SecondaryKey.-1")==0x73,"custom shortcut retained");
    boost::property_tree::ptree absent;MigrateMenuKey(absent);Check(absent.empty(),"absent settings retain new defaults");
    std::cout<<checks<<" config/bind codec checks passed\n";
}
