#include "CompDraw.h"
#include "PolygonMath.h"
#include <cmath>
#include "../ESP/ESP.h"
#include "../../Players/SteamProfileCache.h"
#ifndef TEXTMODE
#include <ImGui/imgui.h>
#endif

void CCompDraw::Record(CompDrawCommand command) {
    if(command.points.empty())return;
    for(auto& p:command.points)if(!std::isfinite(p.x)||!std::isfinite(p.y))return;
    command.color.a=byte(std::clamp(float(command.color.a)*m_alpha,0.f,255.f));
    m_pending.push_back(std::move(command));
}
void CCompDraw::LineRectOutline(int x,int y,int w,int h,Color_t color,Color_t outline) {
    LineRect(x-1,y-1,w+2,h+2,outline); LineRect(x+1,y+1,w-2,h-2,outline); LineRect(x,y,w,h,color);
}
void CCompDraw::DrawHudTexture(int x,int y,float scale,const CHudTexture* icon,Color_t color) {
    if(!icon || !std::isfinite(scale) || scale<=0.f)return;
    CompDrawCommand command{CompDrawCommand::Type::Image,{{float(x),float(y)},{x+icon->Width()*scale,y+icon->Height()*scale}},color};
    if(icon->bRenderUsingFont && icon->hFont)command.text=std::format("#font:{}:{}",(unsigned long)icon->hFont,(unsigned)(unsigned char)icon->cCharacterInFont);
    else { command.text=icon->szTextureFile; command.uv0={icon->texCoords[0],icon->texCoords[1]};command.uv1={icon->texCoords[2],icon->texCoords[3]}; }
    if(command.text.empty())return;
    PrepareESPIconTexture(command.text);
    Record(std::move(command));
}

void CCompDraw::Line(int x,int y,int x2,int y2,Color_t color)
{ Record({CompDrawCommand::Type::Line, {{float(x),float(y)},{float(x2),float(y2)}},color}); }
void CCompDraw::FillRect(int x,int y,int w,int h,Color_t color)
{ Record({CompDrawCommand::Type::FilledRectangle, {{float(x),float(y)},{float(x+w),float(y+h)}},color}); }
void CCompDraw::LineRect(int x,int y,int w,int h,Color_t color)
{ Record({CompDrawCommand::Type::Rectangle, {{float(x),float(y)},{float(x+w),float(y+h)}},color}); }
void CCompDraw::String(const Font_t& font,int x,int y,Color_t color,EAlign align,const char* text)
{
    if (!text) return;
    CompDrawCommand command{CompDrawCommand::Type::Text,{{float(x),float(y)}},color};
    command.text=text; command.fontSize=float(font.m_nTall); command.align=align;
    Record(std::move(command));
}
void CCompDraw::FillPolygon(std::vector<Vertex_t> vertices,Color_t color)
{
    CompDrawCommand command{CompDrawCommand::Type::Polygon,{},color};
    for (auto& vertex:vertices) command.points.emplace_back(vertex.m_Position.x,vertex.m_Position.y);
    if (command.points.size()>=3) Record(std::move(command));
}
void CCompDraw::LinePolygon(std::vector<Vertex_t> vertices,Color_t color)
{
    CompDrawCommand command{CompDrawCommand::Type::Outline,{},color};
    for (auto& vertex:vertices) command.points.emplace_back(vertex.m_Position.x,vertex.m_Position.y);
    if (command.points.size()>=3) Record(std::move(command));
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

namespace {
void AlignBox(int& x,int& y,int w,int h,EAlign align) {
 if(align==ALIGN_TOP || align==ALIGN_CENTER || align==ALIGN_BOTTOM) x-=w/2;
 if(align==ALIGN_TOPRIGHT || align==ALIGN_RIGHT || align==ALIGN_BOTTOMRIGHT) x-=w;
 if(align==ALIGN_LEFT || align==ALIGN_CENTER || align==ALIGN_RIGHT) y-=h/2;
 if(align==ALIGN_BOTTOMLEFT || align==ALIGN_BOTTOM || align==ALIGN_BOTTOMRIGHT) y-=h;
}
}
void CCompDraw::Avatar(int x,int y,int w,int h,uint32_t account,EAlign align) {
 AlignBox(x,y,w,h,align); F::SteamProfileCache.TouchAvatar(account);
 CompDrawCommand command{CompDrawCommand::Type::Avatar,{{float(x),float(y)},{float(x+w),float(y+h)}},Color_t(255,255,255,255)};
 command.account=account; Record(std::move(command));
}
void CCompDraw::Texture(int x,int y,int w,int h,int id,EAlign align) {
 AlignBox(x,y,w,h,align);
 static const char* classes[]={"scout","sniper","soldier","demo","medic","heavy","pyro","spy","engineer"};
 std::string path;
 if(id<0) return;
 if(id<9) path=std::string("hud/leaderboard_class_")+classes[id]+".vtf";
 else if(id<27) path=std::string("vgui/class_portraits/")+classes[(id-9)%9]+(id<18?"_red.vtf":"_blue.vtf");
 else {
  static const char* icons[]={"obj_sentrygun","obj_sentrygun2","obj_sentrygun3","obj_dispenser","obj_teleporter_entry","obj_teleporter_exit","medic_health_cross","ico_ammo","ico_cash","pumpkin_bomb","ico_powerup","spellbook","gargoyle"};
  if(id>39) return;
  if(auto icon=H::Draw.GetIcon(icons[id-27])) { DrawHudTexture(x,y,std::min(float(w)/std::max(icon->Width(),1),float(h)/std::max(icon->Height(),1)),icon,{255,255,255,255}); return; }
  return;
 }
 CompDrawCommand command{CompDrawCommand::Type::Image,{{float(x),float(y)},{float(x+w),float(y+h)}},Color_t(255,255,255,255)};
 PrepareESPIconTexture(path);command.text=std::move(path);Record(std::move(command));
}
void CCompDraw::LineCircle(int x,int y,float radius,int segments,Color_t color) {
 std::vector<Vertex_t> vertices;segments=std::clamp(segments,3,256);
 for(int i=0;i<segments;++i){float a=i*6.28318530718f/segments;vertices.emplace_back(Vec2(x+std::cos(a)*radius,y+std::sin(a)*radius));}
 LinePolygon(std::move(vertices),color);
}
void CCompDraw::FillRoundRect(int x,int y,int w,int h,int radius,Color_t color) {
 std::vector<Vertex_t> vertices; float r=float(std::clamp(radius,0,std::max(0,std::min(w,h)/2)));
 for(int corner=0;corner<4;++corner)for(int i=0;i<=8;++i){float a=(corner*90.f+i*90.f/8.f)*.01745329252f;
  float cx=corner<2?x+w-r:x+r,cy=(corner==0||corner==3)?y+r:y+h-r;
  vertices.emplace_back(Vec2(cx+std::sin(a)*r,cy-std::cos(a)*r));}
 FillPolygon(std::move(vertices),color);
}
void CCompDraw::LineRoundRect(int x,int y,int w,int h,int radius,Color_t color) {
 // Record the same rounded boundary, then switch the command to an outline.
 auto count=m_pending.size();FillRoundRect(x,y,w,h,radius,color);
 if(m_pending.size()>count)m_pending.back().type=CompDrawCommand::Type::Outline;
}
void CCompDraw::FillRectPercent(int x,int y,int w,int h,float fraction,Color_t color,Color_t background,EAlign align,bool adjust) {
 fraction=std::clamp(fraction,0.f,1.f);
 if(!adjust) FillRect(x-1,y-1,w+2,h+2,background);
 int nw=w,nh=h;
 if(align==ALIGN_LEFT)nw=int(w*fraction);
 else if(align==ALIGN_RIGHT){nw=int(w*fraction);x+=w-nw;}
 else if(align==ALIGN_TOP)nh=int(h*fraction);
 else if(align==ALIGN_BOTTOM){nh=int(h*fraction);y+=h-nh;}
 if(adjust)FillRect(x-1,y-1,nw+2,nh+2,background);
 FillRect(x,y,nw,nh,color);
}

void CCompDraw::Draw(ImDrawList* list)
{
#ifndef TEXTMODE
    if (!list) return;
    for (const auto& command:m_frame.Get())
    {
        const auto color=IM_COL32(command.color.r,command.color.g,command.color.b,command.color.a);
        const auto& a=command.points.front();
        if(command.type==CompDrawCommand::Type::Image || command.type==CompDrawCommand::Type::Avatar) {
            void* texture=command.type==CompDrawCommand::Type::Avatar ? F::SteamProfileCache.GetAvatarTexture(command.account) : GetESPIconTexture(command.text);
            const auto& b=command.points[1];
            if(texture) list->AddImage(reinterpret_cast<ImTextureID>(texture),{a.x,a.y},{b.x,b.y},{command.uv0.x,command.uv0.y},{command.uv1.x,command.uv1.y},color);
            else if(command.type==CompDrawCommand::Type::Avatar) list->AddRectFilled({a.x,a.y},{b.x,b.y},IM_COL32(128,128,128,255));
            else list->AddCircleFilled({(a.x+b.x)*.5f,(a.y+b.y)*.5f},std::min(b.x-a.x,b.y-a.y)*.4f,color);
        }
        else if (command.type==CompDrawCommand::Type::Polygon || command.type==CompDrawCommand::Type::Outline)
        {
            std::vector<ImVec2> points; points.reserve(command.points.size());
            for (auto& point:command.points) points.emplace_back(point.x,point.y);
            if (command.type==CompDrawCommand::Type::Polygon) {
                const auto kind=PolygonMath::Classify(points);
                if(kind==PolygonMath::Kind::Convex)list->AddConvexPolyFilled(points.data(),int(points.size()),color);
                else if(kind==PolygonMath::Kind::Concave)list->AddConcavePolyFilled(points.data(),int(points.size()),color);
            }
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
