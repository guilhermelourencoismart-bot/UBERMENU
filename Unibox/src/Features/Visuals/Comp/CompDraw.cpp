#include "CompDraw.h"
#include <cmath>
#ifndef TEXTMODE
#include <ImGui/imgui.h>
#endif
void CCompDraw::Line(int x,int y,int x2,int y2,Color_t color)
{ m_pending.push_back({CompDrawCommand::Type::Line, {{float(x),float(y)},{float(x2),float(y2)}},color}); }
void CCompDraw::FillRect(int x,int y,int w,int h,Color_t color)
{ m_pending.push_back({CompDrawCommand::Type::FilledRectangle, {{float(x),float(y)},{float(x+w),float(y+h)}},color}); }
void CCompDraw::LineRect(int x,int y,int w,int h,Color_t color)
{ m_pending.push_back({CompDrawCommand::Type::Rectangle, {{float(x),float(y)},{float(x+w),float(y+h)}},color}); }
void CCompDraw::String(const Font_t& font,int x,int y,Color_t color,EAlign align,const char* text)
{
    if (!text) return;
    CompDrawCommand command{CompDrawCommand::Type::Text,{{float(x),float(y)}},color};
    command.text=text; command.fontSize=float(font.m_nTall); command.align=align;
    m_pending.push_back(std::move(command));
}
void CCompDraw::FillPolygon(std::vector<Vertex_t> vertices,Color_t color)
{
    CompDrawCommand command{CompDrawCommand::Type::Polygon,{},color};
    for (auto& vertex:vertices) command.points.emplace_back(vertex.m_Position.x,vertex.m_Position.y);
    if (command.points.size()>=3) m_pending.push_back(std::move(command));
}
void CCompDraw::LinePolygon(std::vector<Vertex_t> vertices,Color_t color)
{
    CompDrawCommand command{CompDrawCommand::Type::Outline,{},color};
    for (auto& vertex:vertices) command.points.emplace_back(vertex.m_Position.x,vertex.m_Position.y);
    if (command.points.size()>=3) m_pending.push_back(std::move(command));
}
void CCompDraw::FillCircle(int x,int y,float radius,int segments,Color_t color)
{
    std::vector<Vertex_t> vertices;
    segments=std::clamp(segments,3,256);
    for (int i=0;i<segments;++i)
    {
        float angle=i*6.28318530718f/segments;
        vertices.emplace_back(Vec2(x+std::cos(angle)*radius,y+std::sin(angle)*radius));
    }
    FillPolygon(std::move(vertices),color);
}
void CCompDraw::Draw(ImDrawList* list)
{
#ifndef TEXTMODE
    if (!list) return;
    for (const auto& command:m_frame.Get())
    {
        const auto color=IM_COL32(command.color.r,command.color.g,command.color.b,command.color.a);
        const auto& a=command.points.front();
        if (command.type==CompDrawCommand::Type::Polygon || command.type==CompDrawCommand::Type::Outline)
        {
            std::vector<ImVec2> points; points.reserve(command.points.size());
            for (auto& point:command.points) points.emplace_back(point.x,point.y);
            if (command.type==CompDrawCommand::Type::Polygon) list->AddConvexPolyFilled(points.data(),int(points.size()),color);
            else list->AddPolyline(points.data(),int(points.size()),color,ImDrawFlags_Closed,1.f);
        }
        else if (command.type==CompDrawCommand::Type::Text)
        {
            auto font=ImGui::GetFont();
            const auto size=font->CalcTextSizeA(command.fontSize,FLT_MAX,0,command.text.c_str());
            ImVec2 position(a.x,a.y);
            if (command.align==ALIGN_TOP || command.align==ALIGN_CENTER || command.align==ALIGN_BOTTOM) position.x-=size.x*.5f;
            if (command.align==ALIGN_TOPRIGHT || command.align==ALIGN_RIGHT || command.align==ALIGN_BOTTOMRIGHT) position.x-=size.x;
            if (command.align==ALIGN_LEFT || command.align==ALIGN_CENTER || command.align==ALIGN_RIGHT) position.y-=size.y*.5f;
            if (command.align==ALIGN_BOTTOMLEFT || command.align==ALIGN_BOTTOM || command.align==ALIGN_BOTTOMRIGHT) position.y-=size.y;
            list->AddText(font,command.fontSize,position,color,command.text.c_str());
        }
        else if (command.points.size()==2)
        {
            const auto& b=command.points[1];
            if (command.type==CompDrawCommand::Type::Line) list->AddLine({a.x,a.y},{b.x,b.y},color);
            if (command.type==CompDrawCommand::Type::Rectangle) list->AddRect({a.x,a.y},{b.x,b.y},color);
            if (command.type==CompDrawCommand::Type::FilledRectangle) list->AddRectFilled({a.x,a.y},{b.x,b.y},color);
        }
    }
#endif
}
