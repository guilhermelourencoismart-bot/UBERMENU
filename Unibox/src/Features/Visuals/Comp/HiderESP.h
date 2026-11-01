// Ported from Amalgam-Comp 366fa0dc; original SHA256 c7802b08516abf9e80b12bd11ff03ca01f2d751d80c1bf4a3f7d01ed61bc4af9.
#pragma once
#include "../../../SDK/SDK.h"
#include <unordered_map>

struct CompHiderData
{
    Vec3 LastPosition;
    float LastMoveTime;
    bool IsHider;
    float LastUpdateTime;
};

class CFullHiderESP
{
private:
    static constexpr float CLEANUP_INTERVAL = 1.0f;             // Clean invalid players every second

    // Player tracking data
    std::unordered_map<int, CompHiderData> m_PlayerData;
    int m_iLastLifeState = 2; // Start with LIFE_DEAD (2)
    float m_flNextCleanupTime = 0.0f;

    // Helper functions
    float DistanceBetweenVectors(const Vec3& v1, const Vec3& v2);
    void ResetPlayerData();
    void CleanInvalidTargets();
    bool IsPlayerInRange(CTFPlayer* pPlayer, CTFPlayer* pLocal);
    void UpdatePlayerData(CTFPlayer* pPlayer);
    void DrawPlayerESP(CTFPlayer* pPlayer, const CompHiderData& data);

public:
    void Draw();
};

ADD_FEATURE(CFullHiderESP, FullHiderESP)
