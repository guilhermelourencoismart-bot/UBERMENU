#pragma once
#include <cmath>
#include <cstddef>
namespace PolygonMath
{
    template<class Points> bool IsConvex(const Points& points)
    {
        if(points.size()<3)return false;
        int winding=0;
        for(std::size_t i=0;i<points.size();++i)
        {
            const auto& a=points[i];const auto& b=points[(i+1)%points.size()];const auto& c=points[(i+2)%points.size()];
            if(!std::isfinite(a.x)||!std::isfinite(a.y))return false;
            const double cross=(double(b.x)-a.x)*(double(c.y)-b.y)-(double(b.y)-a.y)*(double(c.x)-b.x);
            if(cross==0.)continue;
            const int sign=cross>0.?1:-1;if(winding && winding!=sign)return false;winding=sign;
        }
        return winding!=0;
    }
}
