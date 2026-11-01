#pragma once
#include "../../../SDK/SDK.h"
#include "../../ImGui/IndicatorCache.h"
struct ImDrawList;
struct EnemyView_t
{
    bool active=false; Vec3 origin={},angles={}; WindowBox_t window={};
    std::string title; int health=0,maxHealth=1;
};
class CEnemyCam
{
    CIndicatorCache<EnemyView_t> m_cache;
    EHANDLE m_target, m_medic;
    float m_switchTime=0.f,m_searchTime=-1.f,m_time=-1.f;
    int m_mode=-1;
    std::string m_map;
    std::vector<CTFPlayer*> GetEnemyPlayers();
    CTFPlayer* FindClosestEnemy();
    CTFPlayer* FindHealedPlayer(CTFPlayer*& medic);
    CTFPlayer* FindEnemyMedic(CTFPlayer*& target);
    CTFPlayer* FindTopScorePlayer();
public:
    void CacheDrawInfo(CTFPlayer* local);
    EnemyView_t GetView() { return m_cache.Get(); }
    void DrawOverlay(ImDrawList* list);
};
ADD_FEATURE(CEnemyCam, EnemyCam);
