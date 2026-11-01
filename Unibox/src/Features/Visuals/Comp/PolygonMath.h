#pragma once
#include <cmath>
#include <cstddef>
namespace PolygonMath
{
    enum class Kind { Invalid,Convex,Concave };
    template<class Points> Kind Classify(const Points& points)
    {
        if(points.size()<3)return Kind::Invalid;
        int winding=0;bool concave=false;double area=0.;
        for(std::size_t i=0;i<points.size();++i)
        {
            const auto& a=points[i];const auto& b=points[(i+1)%points.size()];const auto& c=points[(i+2)%points.size()];
            if(!std::isfinite(a.x)||!std::isfinite(a.y)||!std::isfinite(b.x)||!std::isfinite(b.y))return Kind::Invalid;
            area+=double(a.x)*b.y-double(a.y)*b.x;
            const double cross=(double(b.x)-a.x)*(double(c.y)-b.y)-(double(b.y)-a.y)*(double(c.x)-b.x);
            if(cross==0.)continue;
            const int sign=cross>0.?1:-1;if(winding && winding!=sign)concave=true;winding=sign;
        }
        if(!winding || area==0.)return Kind::Invalid;
        return concave?Kind::Concave:Kind::Convex;
    }
}
