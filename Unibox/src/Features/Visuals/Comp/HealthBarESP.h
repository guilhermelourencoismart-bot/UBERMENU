// Complete Comp port, source 366fa0dc, SHA256 4156d801e71914160dbc2da701885bab53ea4b7314092771580b62d5378a9f56.
#pragma once
#include "../../../SDK/SDK.h"

class CFullHealthBarESP
{
private:
    // Configuration constants from lua script
    static constexpr int ALPHA = 70;
    static constexpr int BAR_HEIGHT = 10;
    static constexpr int BAR_WIDTH = 90;
    static constexpr float MAX_DISTANCE_SQR = 3500.0f * 3500.0f;
    static constexpr bool MEDIC_MODE = true;
    
    // Overheal color (blue) from lua script
    static constexpr Color_t OVERHEAL_COLOR = {71, 166, 255, 255};
    
    // Helper functions
    Color_t GetHealthBarColor(int health, int maxHealth);
    void DrawHealthBar(int x, int y, int width, int health, int maxHealth);
    void DrawHealthPolygon(const Vec3& playerPos, int health, int maxHealth);
    bool IsVisible(const Vec3& vPos);

public:
    void Draw();
};

ADD_FEATURE(CFullHealthBarESP, FullHealthBarESP);