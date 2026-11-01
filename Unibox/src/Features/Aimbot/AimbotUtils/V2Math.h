#pragma once
#include <algorithm>
#include <cmath>
#include <limits>
namespace V2Math
{
    inline bool Normalize(float input,float& output)
    {
        output=0.f;if(!std::isfinite(input))return false;
        output=std::fmod(input,360.f);if(output>180.f)output-=360.f;if(output<-180.f)output+=360.f;return true;
    }
    template<class Angle> float CalcFov(const Angle& from,const Angle& to)
    {
        float p1,y1,p2,y2;
        if(!Normalize(from.x,p1)||!Normalize(from.y,y1)||!Normalize(to.x,p2)||!Normalize(to.y,y2)) return std::numeric_limits<float>::infinity();
        constexpr double rad=0.017453292519943295;
        double dot=std::cos(p1*rad)*std::cos(p2*rad)*std::cos((y1-y2)*rad)+std::sin(p1*rad)*std::sin(p2*rad);
        return float(std::acos(std::clamp(dot,-1.0,1.0))/rad);
    }
    inline bool LowerArc(float speed,float gravity,float distance,float height,float& pitch,float& flight)
    {
        pitch=flight=0.f;
        if(!std::isfinite(speed)||!std::isfinite(gravity)||!std::isfinite(distance)||!std::isfinite(height)||speed<=0||gravity<0||distance<0)return false;
        if(gravity==0){pitch=std::atan2(height,distance);flight=std::hypot(distance,height)/speed;return std::isfinite(flight);}
        if(distance<.0001f)return false;
        const double v2=double(speed)*speed,g=gravity,d=distance,z=height;
        const double discriminant=v2*v2-g*(g*d*d+2*z*v2);
        if(discriminant<0)return false;
        const double angle=std::atan((v2-std::sqrt(discriminant))/(g*d));
        pitch=float(angle);flight=float(d/(std::cos(angle)*speed));
        return std::isfinite(pitch)&&std::isfinite(flight)&&flight>=0;
    }
    constexpr bool PrioritizedBefore(bool a,bool b){return a&&!b;}
}
