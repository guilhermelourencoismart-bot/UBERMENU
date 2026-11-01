#pragma once
#include "../../../SDK/SDK.h"

class CCompColor
{
public:
	Color_t GetTeamColor(int iLocalTeam, int iTargetTeam, bool bRelative);
	Color_t GetEntityDrawColor(CTFPlayer* pLocal, CBaseEntity* pEntity, bool bRelative, CBaseEntity* pTarget = nullptr, int* pType = nullptr);
	Color_t GetScoreboardColor(int iIndex);
};

ADD_FEATURE(CCompColor, CompColor);
