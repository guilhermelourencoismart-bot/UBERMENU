#pragma once
#include <boost/property_tree/ptree.hpp>
#include "../ImGui/Menu/PresentationState.h"
#include <charconv>
#include <cmath>
#include <optional>
#include <string>
#include <type_traits>
#include <algorithm>
#include <vector>
// Shared by the production serializer and isolated regression tests.
namespace ConfigCodec
{
    inline void WriteMenuState(boost::property_tree::ptree& tree, const MenuPresentation::State& state)
    {
        boost::property_tree::ptree favorites;
        for (const auto& key : state.favorites) {
            boost::property_tree::ptree item; item.put_value(key);
            favorites.push_back({"", item});
        }
        tree.put_child("Menu.Favorites", favorites);
    }
    inline MenuPresentation::State ReadMenuState(const boost::property_tree::ptree& tree)
    {
        MenuPresentation::State state;
        if (auto favorites = tree.get_child_optional("Menu.Favorites"))
            for (const auto& entry : *favorites)
                if (entry.first.empty() && entry.second.empty())
                    state.Add(entry.second.get_value<std::string>());
        return state;
    }
    template<class T> void WriteScalar(boost::property_tree::ptree& tree,const std::string& key,const T& value) { tree.put(key,value); }
    template<class T> bool ReadScalar(const boost::property_tree::ptree& tree,const std::string& key,T& value)
    {
        auto candidate=tree.get_optional<T>(key);if(!candidate)return false;
        if constexpr(std::is_floating_point_v<T>) if(!std::isfinite(*candidate))return false;
        value=*candidate;return true;
    }
    template<class Color> void WriteColor(boost::property_tree::ptree& tree,const std::string& key,const Color& value)
    {
        boost::property_tree::ptree child;
        WriteScalar(child,"r",int(value.r));WriteScalar(child,"g",int(value.g));WriteScalar(child,"b",int(value.b));WriteScalar(child,"a",int(value.a));tree.put_child(key,child);
    }
    template<class Color> void ReadColor(const boost::property_tree::ptree& tree,const std::string& key,Color& value)
    {
        if(auto child=tree.get_child_optional(key))
        {
            auto channel=[&](const char* name,auto& destination){int v=destination;if(ReadScalar(*child,name,v) && v>=0 && v<=255)destination=static_cast<std::remove_reference_t<decltype(destination)>>(v);};
            channel("r",value.r);channel("g",value.g);channel("b",value.b);channel("a",value.a);
        }
    }
    template<class Rect> void WriteWindow(boost::property_tree::ptree& tree,const std::string& key,const Rect& value)
    {
        boost::property_tree::ptree child;WriteScalar(child,"x",value.x);WriteScalar(child,"y",value.y);WriteScalar(child,"w",value.w);WriteScalar(child,"h",value.h);tree.put_child(key,child);
    }
    template<class Rect> void ReadWindow(const boost::property_tree::ptree& tree,const std::string& key,Rect& value)
    {
        if(auto child=tree.get_child_optional(key)){ReadScalar(*child,"x",value.x);ReadScalar(*child,"y",value.y);ReadScalar(*child,"w",value.w);ReadScalar(*child,"h",value.h);}
    }
    inline std::optional<int> ParseBindKey(const std::string& key,size_t count,bool noBind)
    {
        int index=0;auto result=std::from_chars(key.data(),key.data()+key.size(),index);
        if(result.ec!=std::errc() || result.ptr!=key.data()+key.size())return std::nullopt;
        if(index==-1)return index;
        if(index<0 || noBind || size_t(index)>=count)return std::nullopt;
        return index;
    }
    template<class Map,class Write> boost::property_tree::ptree WriteBindMap(const Map& map,Write writer)
    {
        boost::property_tree::ptree tree;
        for(const auto& [index,value]:map)writer(tree,std::to_string(index),value);
        return tree;
    }
    template<class Tree> auto Variables(Tree& tree)
    {
        auto variables=tree.get_child_optional("Vars");
        if(!variables)variables=tree.get_child_optional("ConVars");
        return variables;
    }
    template<class Binds> void SanitizeBindParents(Binds& binds)
    {
        for(auto& bind:binds)if(bind.m_iParent < -1 || (bind.m_iParent>=0 && size_t(bind.m_iParent)>=binds.size()))bind.m_iParent=-1;
        for(size_t i=0;i<binds.size();++i)
        {
            std::vector<bool> seen(binds.size());
            for(int parent=int(i);parent!=-1;parent=binds[parent].m_iParent)
            {
                if(parent<0 || size_t(parent)>=binds.size() || seen[parent]){binds[i].m_iParent=-1;break;}
                seen[parent]=true;
            }
        }
    }
    inline void MigrateMenuKey(boost::property_tree::ptree& root)
    {
        auto vars=Variables(root);if(!vars)return;
        const std::string key="Vars::Menu::SecondaryKey.-1";
        int value=0;if(ReadScalar(*vars,key,value) && value==0x72)vars->put(key,0x05);
    }
    inline void MigrateLegacyRadar(boost::property_tree::ptree& root)
    {
        auto variables=Variables(root);if(!variables)return;
        const std::string old="Vars::Radar::Circular",next="Vars::Comp::Radar::Main::Style";
        if(!variables->get_child_optional(next))
            if(auto circular=variables->get_child_optional(old))
            {
                boost::property_tree::ptree styles;
                for(const auto& [key,node]:*circular)if(auto value=node.get_value_optional<bool>())styles.put(key,*value?0:1);
                variables->put_child(next,styles);
            }
        const std::string window="Vars::Comp::Radar::Main::Window";
        if(!variables->get_child_optional(window) && (variables->get_child_optional("Vars::Radar::Size") || variables->get_child_optional("Vars::Radar::PositionX")))
        {
            float size=200.f;int x=20,y=200;
            ReadScalar(*variables,"Vars::Radar::Size.-1",size);
            ReadScalar(*variables,"Vars::Radar::PositionX.-1",x);ReadScalar(*variables,"Vars::Radar::PositionY.-1",y);
            size=std::max(100.f,std::min(500.f,size));
            variables->put(window+".-1.x",x+int(size/2));variables->put(window+".-1.y",y);
            variables->put(window+".-1.w",int(size));variables->put(window+".-1.h",int(size));
        }
    }
}
