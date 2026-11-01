#include "EnemyCam.h"
#include "../CameraWindow/CameraWindow.h"
#include <cstdlib>
#ifndef TEXTMODE
#include <ImGui/imgui.h>
#endif
// Full Comp selection and view modes, with native shared render-target ownership.
std::vector<CTFPlayer*> CEnemyCam::GetEnemyPlayers()
{
    std::vector<CTFPlayer*> enemies;

    auto pLocal = H::Entities.GetLocal();
    if (!pLocal)
        return enemies;

    int localTeam = pLocal->m_iTeamNum();

    for (auto pEntity : H::Entities.GetGroup(EntityEnum::PlayerAll))
    {
        auto pPlayer = pEntity->As<CTFPlayer>();
        if (!pPlayer || !pPlayer->IsAlive() || pPlayer->IsDormant())
            continue;

        if (pPlayer->m_iTeamNum() == localTeam)
            continue;

        // Skip cloaked spies
        if (pPlayer->InCond(TF_COND_STEALTHED))
            continue;

        if ((pPlayer->GetAbsOrigin()-pLocal->GetAbsOrigin()).Length()>Vars::EnemyCam::MaxDistance.Value) continue;
        if(Vars::EnemyCam::VisibleOnly.Value && !SDK::VisPos(pLocal,pPlayer,pLocal->GetEyePosition(),pPlayer->GetEyePosition())) continue;
        enemies.push_back(pPlayer);
    }

    return enemies;
}

CTFPlayer* CEnemyCam::FindClosestEnemy()
{
    auto pLocal = H::Entities.GetLocal();
    if (!pLocal)
        return nullptr;

    auto enemies = GetEnemyPlayers();
    if (enemies.empty())
        return nullptr;

    CTFPlayer* closest = nullptr;
    float minDist = FLT_MAX;
    Vec3 localPos = pLocal->GetAbsOrigin();

    for (auto pEnemy : enemies)
    {
        Vec3 enemyPos = pEnemy->GetAbsOrigin();
        float dist = (enemyPos - localPos).Length();

        if (dist < minDist)
        {
            minDist = dist;
            closest = pEnemy;
        }
    }

    return closest;
}

CTFPlayer* CEnemyCam::FindHealedPlayer(CTFPlayer*& outMedic)
{
    auto enemies = GetEnemyPlayers();

    for (auto pPlayer : enemies)
    {
        if (pPlayer->InCond(TF_COND_HEALTH_OVERHEALED))
        {
            // Try to find the medic healing this player
            for (auto pMedic : enemies)
            {
                if (pMedic->m_iClass() == TF_CLASS_MEDIC)
                {
                    auto pMedigun = pMedic->GetWeaponFromSlot(SLOT_SECONDARY);
                    if (pMedigun && pMedigun->GetWeaponID()==TF_WEAPON_MEDIGUN)
                    {
                        auto pHealTarget = pMedigun->As<CWeaponMedigun>()->m_hHealingTarget().Get();
                        if (pHealTarget == pPlayer)
                        {
                            outMedic = pMedic;
                            return pPlayer;
                        }
                    }
                }
            }

            // Found healed player but no medic
            outMedic = nullptr;
            return pPlayer;
        }
    }

    // No healed players, fall back to closest
    outMedic = nullptr;
    return FindClosestEnemy();
}

CTFPlayer* CEnemyCam::FindEnemyMedic(CTFPlayer*& outTarget)
{
    auto enemies = GetEnemyPlayers();

    for (auto pPlayer : enemies)
    {
        if (pPlayer->m_iClass() == TF_CLASS_MEDIC)
        {
            // Try to find healing target
            auto pMedigun = pPlayer->GetWeaponFromSlot(SLOT_SECONDARY);
            if (pMedigun && pMedigun->GetWeaponID()==TF_WEAPON_MEDIGUN)
            {
                auto pHealTarget = pMedigun->As<CWeaponMedigun>()->m_hHealingTarget().Get();
                if (pHealTarget && pHealTarget->As<CTFPlayer>())
                    outTarget = pHealTarget->As<CTFPlayer>();
                else
                    outTarget = nullptr;
            }
            else
            {
                outTarget = nullptr;
            }

            return pPlayer;
        }
    }

    outTarget = nullptr;
    return nullptr;
}

CTFPlayer* CEnemyCam::FindTopScorePlayer()
{
    auto enemies = GetEnemyPlayers();
    if (enemies.empty())
        return nullptr;

    CTFPlayer* topPlayer = nullptr;
    int maxScore = -1;

    auto pResource = H::Entities.GetResource();
    if (!pResource)
        return enemies[0]; // Fallback to first enemy

    for (auto pEnemy : enemies)
    {
        int score = pResource->m_iTotalScore(pEnemy->entindex());
        if (score > maxScore)
        {
            maxScore = score;
            topPlayer = pEnemy;
        }
    }

    return topPlayer ? topPlayer : enemies[0];
}


void CEnemyCam::CacheDrawInfo(CTFPlayer* local)
{
 EnemyView_t frame;
 const std::string map=I::EngineClient->GetLevelName();
 const float now=I::GlobalVars->curtime;
 const int mode=Vars::Comp::EnemyCam::Mode.Value;
 if(!local || !Vars::EnemyCam::Enabled.Value || map!=m_map || now<m_time || mode!=m_mode) {
  m_target.Term();m_medic.Term();m_searchTime=-1.f;m_switchTime=0;
 }
 m_map=map;m_time=now;m_mode=mode;
 if(!local || !Vars::EnemyCam::Enabled.Value || SDK::TakingScreenshot() || I::EngineVGui->IsGameUIVisible()){m_cache.Set(frame);return;}
 auto raw=m_target.Get();auto target=raw && raw->IsPlayer()?raw->As<CTFPlayer>():nullptr;
 bool valid=target && target->IsAlive() && !target->IsDormant() && !target->InCond(TF_COND_STEALTHED) && target->m_iTeamNum()!=local->m_iTeamNum();
 bool expired=Vars::Comp::EnemyCam::TrackTime.Value>0 && now-m_switchTime>=Vars::Comp::EnemyCam::TrackTime.Value;
 if((!valid || expired) && (m_searchTime<0 || now-m_searchTime>=.5f)) {
  m_searchTime=now; CTFPlayer* related=nullptr;
  switch(mode) {
   case 0:target=FindClosestEnemy();break;
   case 1:target=FindHealedPlayer(related);break;
   case 2:target=FindEnemyMedic(related);break;
   case 3:target=FindTopScorePlayer();break;
   case 4:{auto enemies=GetEnemyPlayers();target=enemies.empty()?nullptr:enemies[std::rand()%enemies.size()];break;}
   default:target=nullptr;break;
  }
  m_target=target;m_medic=related;m_switchTime=now;
  valid=target!=nullptr;
 }
 if(!valid){m_cache.Set(frame);return;}
 frame.active=true;frame.origin=target->GetEyePosition();frame.angles=target->GetEyeAngles();
 if(Vars::Comp::EnemyCam::ViewMode.Value==Vars::Comp::EnemyCam::ViewModeEnum::Offset){
  Vec3 forward,up;Math::AngleVectors(frame.angles,&forward,nullptr,&up);
  float pitchFactor=frame.angles.x>60.f?1.f+(frame.angles.x-60.f)/30.f*7.5f:1.f;
  frame.origin+=forward*(Vars::Comp::EnemyCam::OffsetX.Value*pitchFactor)+up*Vars::Comp::EnemyCam::OffsetY.Value;
 }
 auto& box=frame.window;
 box.w=std::clamp(Vars::Comp::EnemyCam::WindowWidth.Value,160,640);box.h=std::clamp(Vars::Comp::EnemyCam::WindowHeight.Value,120,480);
 const int x=Vars::Comp::EnemyCam::WindowX.Value,y=Vars::Comp::EnemyCam::WindowY.Value;
 int left=x>=0&&y>=0?std::clamp(x,0,std::max(0,H::Draw.m_nScreenW-box.w)):std::max(0,H::Draw.m_nScreenW-box.w-5);
 box.x=left+box.w/2;box.y=x>=0&&y>=0?std::clamp(y,20,std::max(20,H::Draw.m_nScreenH-box.h)):25;
 player_info_t info{};
 if(I::EngineClient->GetPlayerInfo(target->entindex(),&info))frame.title=info.name;
 frame.title+=" [class "+std::to_string(target->m_iClass())+"]";
 if(auto medic=m_medic.Get()){player_info_t related{};if(I::EngineClient->GetPlayerInfo(medic->entindex(),&related))frame.title+=" + Medic: "+std::string(related.name);}
 frame.health=target->m_iHealth();frame.maxHealth=std::max(1,target->GetMaxHealth());m_cache.Set(std::move(frame));
}
void CEnemyCam::DrawOverlay(ImDrawList* list)
{
#ifndef TEXTMODE
 const auto frame=m_cache.Get();if(!list || !frame.active || F::CameraWindow.m_bShouldDraw)return;
 const auto& box=frame.window;float x=float(box.x-box.w/2),y=float(box.y);
 list->AddRect({x-1,y-21},{x+box.w+1,y+box.h+1},IM_COL32(235,64,52,255));
 list->AddRectFilled({x,y-20},{x+box.w,y},IM_COL32(130,26,17,230));
 list->AddText({x+5,y-18},IM_COL32_WHITE,frame.title.c_str());
 auto text=std::format("HP: {}/{}",frame.health,frame.maxHealth);
 list->AddRectFilled({x+5,y+25},{x+110,y+43},IM_COL32(0,0,0,180));list->AddText({x+10,y+27},IM_COL32_WHITE,text.c_str());
#endif
}
