#include "CompColor.h"

#include "../../../SDK/Vars.h"
#include "../../Players/PlayerUtils.h"

Color_t CCompColor::GetTeamColor(int iLocalTeam, int iTargetTeam, bool bRelative)
{
	if (bRelative)
		return iLocalTeam == iTargetTeam ? Vars::Comp::Palette::Team.Value : Vars::Comp::Palette::Enemy.Value;
	else
	{
		switch (iTargetTeam)
		{
		case TF_TEAM_RED: return Vars::Comp::Palette::TeamRed.Value;
		case TF_TEAM_BLUE: return Vars::Comp::Palette::TeamBlu.Value;
		}
	}

	return { 255, 255, 255, 255 };
}

Color_t CCompColor::GetEntityDrawColor(CTFPlayer* pLocal, CBaseEntity* pEntity, bool bRelative, CBaseEntity* pTarget, int* pType)
{
	Color_t out = GetTeamColor(pLocal->m_iTeamNum(), pEntity->m_iTeamNum(), bRelative);
	if (pType) *pType = 1;

	if (pEntity->IsPlayer())
	{
		auto pPlayer = pEntity->As<CTFPlayer>();

		if (pLocal == pPlayer)
		{
			out = Vars::Comp::Palette::Local.Value;
			if (pType) *pType = 2;
		}
		else if (H::Entities.IsFriend(pPlayer->entindex()))
		{
			if (auto tag = F::PlayerUtils.GetTag(FRIEND_TAG)) out = tag->m_tColor;
			if (pType) *pType = 3;
		}
		else if (H::Entities.InParty(pPlayer->entindex()))
		{
			if (auto tag = F::PlayerUtils.GetTag(PARTY_TAG)) out = tag->m_tColor;
			if (pType) *pType = 4;
		}
		else if (auto pTag = F::PlayerUtils.GetSignificantTag(pPlayer->entindex()))
		{
			out = pTag->m_tColor;
			if (pType) *pType = 5;
		}
	}

	if (pTarget && pTarget->entindex() == G::AimTarget.m_iEntIndex)
	{
		out = Vars::Comp::Palette::Target.Value;
		if (pType) *pType = 6;
	}

	return out;
}

Color_t CCompColor::GetScoreboardColor(int iIndex)
{
	Color_t out = { 0, 0, 0, 0 };

	if (iIndex == I::EngineClient->GetLocalPlayer())
		out = Vars::Comp::Palette::Local.Value;
	else if (H::Entities.IsFriend(iIndex))
    { if (auto tag = F::PlayerUtils.GetTag(FRIEND_TAG)) out = tag->m_tColor; }
	else if (H::Entities.InParty(iIndex))
    { if (auto tag = F::PlayerUtils.GetTag(PARTY_TAG)) out = tag->m_tColor; }
	else if (auto pTag = F::PlayerUtils.GetSignificantTag(iIndex))
		out = pTag->m_tColor;

	return out;
}
