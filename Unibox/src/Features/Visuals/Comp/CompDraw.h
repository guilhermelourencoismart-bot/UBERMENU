#pragma once
#include "../../../SDK/SDK.h"
#include "../../ImGui/IndicatorCache.h"
struct ImDrawList;
struct CompDrawCommand
{
    enum class Type { Line, Rectangle, FilledRectangle, Text, Polygon, Outline, Image, Avatar } type;
    std::vector<Vec2> points;
    Color_t color;
    std::string text;
    float fontSize = 14.f;
    int align = ALIGN_TOPLEFT;
    uint32_t account = 0;
};
class CCompDraw
{
    std::vector<CompDrawCommand> m_pending;
    CIndicatorCache<std::vector<CompDrawCommand>> m_frame;
public:
    void Begin() { m_pending.clear(); }
    void Publish() { m_frame.Set(std::move(m_pending)); m_pending.clear(); }
    void Draw(ImDrawList* list);
    void Line(int x, int y, int x2, int y2, Color_t color);
    void FillRect(int x, int y, int w, int h, Color_t color);
    void LineRect(int x, int y, int w, int h, Color_t color);
    void FillPolygon(std::vector<Vertex_t> vertices, Color_t color);
    void LinePolygon(std::vector<Vertex_t> vertices, Color_t color);
    void FillCircle(int x, int y, float radius, int segments, Color_t color);
    void LineCircle(int x,int y,float radius,int segments,Color_t color);
    void FillRoundRect(int x,int y,int w,int h,int radius,Color_t color);
    void LineRoundRect(int x,int y,int w,int h,int radius,Color_t color);
    void FillRectPercent(int x,int y,int w,int h,float fraction,Color_t color,Color_t background,EAlign align,bool adjust);
    void Texture(int x,int y,int w,int h,int id,EAlign align=ALIGN_CENTER);
    void Avatar(int x,int y,int w,int h,uint32_t account,EAlign align=ALIGN_CENTER);
    float Scale(float value=1.f) { return H::Draw.Scale(value); }
    void String(const Font_t& font, int x, int y, Color_t color, EAlign align, const char* text);
    Vec2 GetTextSize(const char* text, const Font_t& font) { return H::Draw.GetTextSize(text, font); }
};
ADD_FEATURE(CCompDraw, CompDraw);
