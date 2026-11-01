#include "CompDraw.h"
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
void CCompDraw::Draw(ImDrawList* list)
{
#ifndef TEXTMODE
    if (!list) return;
    for (const auto& command:m_frame.Get())
    {
        const auto color=IM_COL32(command.color.r,command.color.g,command.color.b,command.color.a);
        const auto& a=command.points.front();
        if (command.type==CompDrawCommand::Type::Text)
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
