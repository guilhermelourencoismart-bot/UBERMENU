// Ported from Amalgam-Comp 366fa0dc; original SHA256 b3c6cb6c056d90fccb450c072ceba65870ece5aa5c170f472d003dcc6c4d2cf8.
#pragma once
#include "../../../SDK/SDK.h"

class CFullMedicUberBar
{
private:
    // Helper functions
    Color_t GetUberBarColor(float uberPercentage, bool isAlive);
    void DrawUberBar(int x, int y, int width, float uberPercentage, bool isAlive);
    bool IsVisible(const Vec3& vPos);
    std::string GetWeaponType(int iItemDefinitionIndex);
    float GetUberPercentage(CTFPlayer* pMedic);

public:
    void Draw();
};

ADD_FEATURE(CFullMedicUberBar, FullMedicUberBar);
