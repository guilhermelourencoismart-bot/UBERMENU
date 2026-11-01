#pragma once
#include <boost/property_tree/ptree.hpp>
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
    template<class T> void WriteScalar(boost::property_tree::ptree& tree,const std::string& key,const T& value) { tree.put(key,value); }
    template<class T> bool ReadScalar(const boost::property_tree::ptree& tree,const std::string& key,T& value)
    {
        auto candidate=tree.get_optional<T>(key);if(!candidate)return false;
        if constexpr(std::is_floating_point_v<T>) if(!std::isfinite(*candidate))return false;
        value=*candidate;return true;
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
