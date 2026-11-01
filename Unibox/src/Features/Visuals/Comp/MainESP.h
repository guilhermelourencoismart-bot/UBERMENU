// Adapted from Amalgam-Comp 366fa0dcf5e5caa722ec270e6f87015adf381c50; see docs/THIRD-PARTY-NOTICES.md.
#pragma once
#include "../../../SDK/SDK.h"

Enum(CompESPText, Top, Bottom, Right, Health, Uber)

struct CompMainPlayerCache
{
	bool m_bWasDrawn=false;
	float m_flAlpha = 1.f;
	std::vector<std::tuple<int, std::string, Color_t, Color_t>> m_vText = {};
	Color_t m_tColor = {};
	bool m_bBox = false;
	bool m_bBones = false;

	bool m_bHealthBar = false;
	bool m_bUberBar = false;
	int m_iClassIcon = 0;
	CHudTexture* m_pWeaponIcon = nullptr;
	float m_flHealth = 1.f;
	float m_flUber = 0.f;
};

struct CompMainBuildingCache
{
	bool m_bWasDrawn=false;
	float m_flAlpha = 1.f;
	std::vector<std::tuple<int, std::string, Color_t, Color_t>> m_vText = {};
	Color_t m_tColor = {};
	bool m_bBox = false;

	bool m_bHealthBar = false;
	float m_flHealth = 1.f;
};

struct CompMainWorldCache
{
	bool m_bWasDrawn=false;
	float m_flAlpha = 1.f;
	std::vector<std::tuple<int, std::string, Color_t, Color_t>> m_vText = {};
	Color_t m_tColor = {};
	bool m_bBox = false;
};

class CFullMainESP
{
private:
	void StorePlayers(CTFPlayer* pLocal);
	void StoreBuildings(CTFPlayer* pLocal);
	void StoreProjectiles(CTFPlayer* pLocal);
	void StoreObjective(CTFPlayer* pLocal);
	void StoreWorld();

	void DrawPlayers();
	void DrawBuildings();
	void DrawWorld();

	Color_t GetColor(CTFPlayer* pLocal, CBaseEntity* pEntity);
	bool GetDrawBounds(CBaseEntity* pEntity, float& x, float& y, float& w, float& h);
	void DrawBones(CTFPlayer* pPlayer, matrix3x4* aBones, std::vector<int> vecBones, Color_t clr);

	// Per-frame ESP data cache (cleared every frame in Store)
	std::unordered_map<CBaseEntity*, CompMainPlayerCache> m_mCompMainPlayerCache = {};
	std::unordered_map<CBaseEntity*, CompMainBuildingCache> m_mCompMainBuildingCache = {};
	std::unordered_map<CBaseEntity*, CompMainWorldCache> m_mCompMainWorldCache = {};

public:
    bool HasHealthBar(CBaseEntity* entity) const { auto i=m_mPlayerCache.find(entity); return i!=m_mPlayerCache.end() && i->second.m_bWasDrawn && i->second.m_bHealthBar; }
    bool HasUberBar(CBaseEntity* entity) const { auto i=m_mPlayerCache.find(entity); return i!=m_mPlayerCache.end() && i->second.m_bWasDrawn && i->second.m_bUberBar; }
    bool HasBox(CBaseEntity* entity) const {
        auto player=m_mPlayerCache.find(entity); if(player!=m_mPlayerCache.end())return player->second.m_bWasDrawn && player->second.m_bBox;
        auto building=m_mBuildingCache.find(entity); if(building!=m_mBuildingCache.end())return building->second.m_bWasDrawn && building->second.m_bBox;
        auto world=m_mWorldCache.find(entity);return world!=m_mWorldCache.end() && world->second.m_bWasDrawn && world->second.m_bBox;
    }
	void Store(CTFPlayer* pLocal);
	void Draw();
};

ADD_FEATURE(CFullMainESP, FullMainESP);
