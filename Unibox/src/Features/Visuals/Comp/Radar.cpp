// Full Comp radar, source 366fa0dc SHA256 44606da67712a0afbfcc8f47df6289fa75011b081bc4aa61be59404821b28063
#include "CompDraw.h"
#include "CompEntities.h"
#include "CompColor.h"
#include "Radar.h"

#include "../../Players/PlayerUtils.h"
#include "../../ImGui/Menu/Menu.h"

bool CFullRadar::GetDrawPosition(CTFPlayer* pLocal, CBaseEntity* pEntity, int& x, int& y, int& z)
{
	const float flRange = std::max(1.f,Vars::Comp::Radar::Main::Range.Value);
	const float flYaw = -DEG2RAD(I::EngineClient->GetViewAngles().y);
	const float flSin = sinf(flYaw), flCos = cosf(flYaw);

	Vec3 vDelta = pLocal->GetAbsOrigin() - pEntity->GetAbsOrigin();
	Vec2 vPos = { vDelta.x * flSin + vDelta.y * flCos, vDelta.x * flCos - vDelta.y * flSin };

	switch (Vars::Comp::Radar::Main::Style.Value)
	{
	case Vars::Comp::Radar::Main::StyleEnum::Circle:
	{
		const float flDist = vDelta.Length2D();
		if (flDist > flRange)
		{
			if (!Vars::Comp::Radar::Main::DrawOutOfRange.Value)
				return false;

			vPos *= flRange / flDist;
		}
		break;
	}
	case Vars::Comp::Radar::Main::StyleEnum::Rectangle:
		if (fabs(vPos.x) > flRange || fabs(vPos.y) > flRange)
		{
			if (!Vars::Comp::Radar::Main::DrawOutOfRange.Value)
				return false;

			Vec2 a = { -flRange / vPos.x, -flRange / vPos.y };
			Vec2 b = { flRange / vPos.x, flRange / vPos.y };
			Vec2 c = { std::min(a.x, b.x), std::min(a.y, b.y) };
			vPos *= fabsf(std::max(c.x, c.y));
		}
	}

	auto& tWindowBox = Vars::Comp::Radar::Main::Window.Value;
	x = tWindowBox.x + vPos.x / flRange * tWindowBox.w / 2;
	y = tWindowBox.y + vPos.y / flRange * tWindowBox.w / 2 + tWindowBox.h / 2.f;
	z = vDelta.z;

	return true;
}

void CFullRadar::DrawBackground()
{
	auto& tWindowBox = Vars::Comp::Radar::Main::Window.Value;
	Color_t& tThemeBack = Vars::Menu::Theme::Background.Value;
	Color_t& tThemeAccent = Vars::Menu::Theme::Accent.Value;
	Color_t tColorBackground = { tThemeBack.r, tThemeBack.g, tThemeBack.b, byte(Vars::Comp::Radar::Main::BackgroundAlpha.Value) };
	Color_t tColorAccent = { tThemeAccent.r, tThemeAccent.g, tThemeAccent.b, byte(Vars::Comp::Radar::Main::LineAlpha.Value) };

	switch (Vars::Comp::Radar::Main::Style.Value)
	{
	case Vars::Comp::Radar::Main::StyleEnum::Circle:
	{
		const float flRadius = tWindowBox.w / 2.f;
		F::CompDraw.FillCircle(tWindowBox.x, tWindowBox.y + flRadius, flRadius, 100, tColorBackground);
		F::CompDraw.LineCircle(tWindowBox.x, tWindowBox.y + flRadius, flRadius, 100, tColorAccent);
		break;
	}
	case Vars::Comp::Radar::Main::StyleEnum::Rectangle:
		F::CompDraw.FillRoundRect(tWindowBox.x - tWindowBox.w / 2, tWindowBox.y, tWindowBox.w, tWindowBox.h, F::CompDraw.Scale(3), tColorBackground);
		F::CompDraw.LineRoundRect(tWindowBox.x - tWindowBox.w / 2, tWindowBox.y, tWindowBox.w, tWindowBox.h, F::CompDraw.Scale(3), tColorAccent);
	}

	F::CompDraw.Line(tWindowBox.x - tWindowBox.w / 2, tWindowBox.y + tWindowBox.h / 2, tWindowBox.x + tWindowBox.w / 2 - 1, tWindowBox.y + tWindowBox.h / 2, tColorAccent);
	F::CompDraw.Line(tWindowBox.x, tWindowBox.y, tWindowBox.x, tWindowBox.y + tWindowBox.h - 1, tColorAccent);
}

void CFullRadar::DrawPoints(CTFPlayer* pLocal)
{
	if (Vars::Comp::Radar::World::Enabled.Value)
	{
		const int iSize = Vars::Comp::Radar::World::Size.Value;

		if (Vars::Comp::Radar::World::Draw.Value & Vars::Comp::Radar::World::DrawEnum::Gargoyle)
		{
			for (auto pGargy : F::CompEntities.GetGroup(CompGroup::WORLD_GARGOYLE))
			{
				int x, y, z;
				if (GetDrawPosition(pLocal, pGargy, x, y, z))
				{
					if (Vars::Comp::Radar::World::Background.Value)
					{
						const float flRadius = sqrtf(pow(iSize, 2) * 2) / 2;
						F::CompDraw.FillCircle(x, y, flRadius, 20, Vars::Comp::Palette::Halloween.Value);
					}

					F::CompDraw.Texture(x, y, iSize, iSize, 39);
				}
			}
		}

		if (Vars::Comp::Radar::World::Draw.Value & Vars::Comp::Radar::World::DrawEnum::Spellbook)
		{
			for (auto pBook : F::CompEntities.GetGroup(CompGroup::PICKUPS_SPELLBOOK))
			{
				int x, y, z;
				if (GetDrawPosition(pLocal, pBook, x, y, z))
				{
					if (Vars::Comp::Radar::World::Background.Value)
					{
						const float flRadius = sqrtf(pow(iSize, 2) * 2) / 2;
						F::CompDraw.FillCircle(x, y, flRadius, 20, Vars::Comp::Palette::Halloween.Value);
					}

					F::CompDraw.Texture(x, y, iSize, iSize, 38);
				}
			}
		}

		if (Vars::Comp::Radar::World::Draw.Value & Vars::Comp::Radar::World::DrawEnum::Powerup)
		{
			for (auto pPower : F::CompEntities.GetGroup(CompGroup::PICKUPS_POWERUP))
			{
				int x, y, z;
				if (GetDrawPosition(pLocal, pPower, x, y, z))
				{
					if (Vars::Comp::Radar::World::Background.Value)
					{
						const float flRadius = sqrtf(pow(iSize, 2) * 2) / 2;
						F::CompDraw.FillCircle(x, y, flRadius, 20, Vars::Comp::Palette::Powerup.Value);
					}

					F::CompDraw.Texture(x, y, iSize, iSize, 37);
				}
			}
		}

		if (Vars::Comp::Radar::World::Draw.Value & Vars::Comp::Radar::World::DrawEnum::Bombs)
		{
			for (auto bBomb : F::CompEntities.GetGroup(CompGroup::WORLD_BOMBS))
			{
				int x, y, z;
				if (GetDrawPosition(pLocal, bBomb, x, y, z))
				{
					if (Vars::Comp::Radar::World::Background.Value)
					{
						const float flRadius = sqrtf(pow(iSize, 2) * 2) / 2;
						F::CompDraw.FillCircle(x, y, flRadius, 20, Vars::Comp::Palette::Halloween.Value);
					}

					F::CompDraw.Texture(x, y, iSize, iSize, 36);
				}
			}
		}

		if (Vars::Comp::Radar::World::Draw.Value & Vars::Comp::Radar::World::DrawEnum::Money)
		{
			for (auto pBook : F::CompEntities.GetGroup(CompGroup::PICKUPS_MONEY))
			{
				int x, y, z;
				if (GetDrawPosition(pLocal, pBook, x, y, z))
				{
					if (Vars::Comp::Radar::World::Background.Value)
					{
						const float flRadius = sqrtf(pow(iSize, 2) * 2) / 2;
						F::CompDraw.FillCircle(x, y, flRadius, 20, Vars::Comp::Palette::Money.Value);
					}

					F::CompDraw.Texture(x, y, iSize, iSize, 35);
				}
			}
		}

		if (Vars::Comp::Radar::World::Draw.Value & Vars::Comp::Radar::World::DrawEnum::Ammo)
		{
			for (auto pAmmo : F::CompEntities.GetGroup(CompGroup::PICKUPS_AMMO))
			{
				int x, y, z;
				if (GetDrawPosition(pLocal, pAmmo, x, y, z))
				{
					if (Vars::Comp::Radar::World::Background.Value)
					{
						const float flRadius = sqrtf(pow(iSize, 2) * 2) / 2;
						F::CompDraw.FillCircle(x, y, flRadius, 20, Vars::Comp::Palette::Ammo.Value);
					}

					F::CompDraw.Texture(x, y, iSize, iSize, 34);
				}
			}
		}

		if (Vars::Comp::Radar::World::Draw.Value & Vars::Comp::Radar::World::DrawEnum::Health)
		{
			for (auto pHealth : F::CompEntities.GetGroup(CompGroup::PICKUPS_HEALTH))
			{
				int x, y, z;
				if (GetDrawPosition(pLocal, pHealth, x, y, z))
				{
					if (Vars::Comp::Radar::World::Background.Value)
					{
						const float flRadius = sqrtf(pow(iSize, 2) * 2) / 2;
						F::CompDraw.FillCircle(x, y, flRadius, 20, Vars::Comp::Palette::Health.Value);
					}

					F::CompDraw.Texture(x, y, iSize, iSize, 33);
				}
			}
		}
	}

	if (Vars::Comp::Radar::Building::Enabled.Value)
	{
		const int iSize = Vars::Comp::Radar::Building::Size.Value;

		for (auto pEntity : F::CompEntities.GetGroup(CompGroup::BUILDINGS_ALL))
		{
			auto pBuilding = pEntity->As<CBaseObject>();

			if (!pBuilding->m_bWasMapPlaced())
			{
				auto pOwner = pBuilding->m_hBuilder().Get();
				if (pOwner)
				{
					const int nIndex = pOwner->entindex();
					if (pLocal->m_iObserverMode() == OBS_MODE_FIRSTPERSON ? pLocal->m_hObserverTarget().Get() == pOwner : nIndex == I::EngineClient->GetLocalPlayer())
					{
						if (!(Vars::Comp::Radar::Building::Draw.Value & Vars::Comp::Radar::Building::DrawEnum::Local))
							continue;
					}
					else
					{
						if (!(Vars::Comp::Radar::Building::Draw.Value & Vars::Comp::Radar::Building::DrawEnum::Prioritized && F::PlayerUtils.IsPrioritized(nIndex))
							&& !(Vars::Comp::Radar::Building::Draw.Value & Vars::Comp::Radar::Building::DrawEnum::Friends && H::Entities.IsFriend(nIndex))
							&& !(Vars::Comp::Radar::Building::Draw.Value & Vars::Comp::Radar::Building::DrawEnum::Party && H::Entities.InParty(nIndex))
							&& !(pOwner->As<CTFPlayer>()->m_iTeamNum() != pLocal->m_iTeamNum() ? Vars::Comp::Radar::Building::Draw.Value & Vars::Comp::Radar::Building::DrawEnum::Enemy : Vars::Comp::Radar::Building::Draw.Value & Vars::Comp::Radar::Building::DrawEnum::Team))
							continue;
					}
				}
			}

			int x, y, z;
			if (GetDrawPosition(pLocal, pBuilding, x, y, z))
			{
				const Color_t tColor = F::CompColor.GetEntityDrawColor(pLocal, pBuilding, Vars::Comp::Palette::Relative.Value);

				int iBounds = iSize;
				if (Vars::Comp::Radar::Building::Background.Value)
				{
					const float flRadius = sqrtf(pow(iSize, 2) * 2) / 2;
					F::CompDraw.FillCircle(x, y, flRadius, 20, tColor);
					iBounds = flRadius * 2;
				}

				switch (pBuilding->GetClassID())
				{
				case ETFClassID::CObjectSentrygun:
					F::CompDraw.Texture(x, y, iSize, iSize, 26 + pBuilding->m_iUpgradeLevel());
					break;
				case ETFClassID::CObjectDispenser:
					F::CompDraw.Texture(x, y, iSize, iSize, 30);
					break;
				case ETFClassID::CObjectTeleporter:
					F::CompDraw.Texture(x, y, iSize, iSize, pBuilding->m_iObjectMode() ? 32 : 31);
					break;
				}

				if (Vars::Comp::Radar::Building::Health.Value)
				{
					const int iMaxHealth = pBuilding->m_iMaxHealth(), iHealth = pBuilding->m_iHealth();

					float flRatio = std::clamp(float(iHealth) / std::max(iMaxHealth,1), 0.f, 1.f);
					Color_t cColor = Vars::Comp::Palette::IndicatorBad.Value.Lerp(Vars::Comp::Palette::IndicatorGood.Value, flRatio);
					F::CompDraw.FillRectPercent(x - iBounds / 2, y - iBounds / 2, 2, iBounds, flRatio, cColor, { 0, 0, 0, 255 }, ALIGN_BOTTOM, true);
				}
			}
		}
	}

	if (Vars::Comp::Radar::Player::Enabled.Value)
	{
		const int iSize = Vars::Comp::Radar::Player::Size.Value;

		for (auto pEntity : F::CompEntities.GetGroup(CompGroup::PLAYERS_ALL))
		{
			auto pPlayer = pEntity->As<CTFPlayer>();
			if (pPlayer->IsDormant() && !H::Entities.GetDormancy(pPlayer->entindex()) || !pPlayer->IsAlive() || pPlayer->IsAGhost())
				continue;

			const int nIndex = pPlayer->entindex();
			if (pLocal->m_iObserverMode() == OBS_MODE_FIRSTPERSON ? pLocal->m_hObserverTarget().Get() == pPlayer : nIndex == I::EngineClient->GetLocalPlayer())
			{
				if (!(Vars::Comp::Radar::Player::Draw.Value & Vars::Comp::Radar::Player::DrawEnum::Local))
					continue;
			}
			else
			{
				if (!(Vars::Comp::Radar::Player::Draw.Value & Vars::Comp::Radar::Player::DrawEnum::Prioritized && F::PlayerUtils.IsPrioritized(nIndex))
					&& !(Vars::Comp::Radar::Player::Draw.Value & Vars::Comp::Radar::Player::DrawEnum::Friends && H::Entities.IsFriend(nIndex))
					&& !(Vars::Comp::Radar::Player::Draw.Value & Vars::Comp::Radar::Player::DrawEnum::Party && H::Entities.InParty(nIndex))
					&& !(pPlayer->m_iTeamNum() != pLocal->m_iTeamNum() ? Vars::Comp::Radar::Player::Draw.Value & Vars::Comp::Radar::Player::DrawEnum::Enemy : Vars::Comp::Radar::Player::Draw.Value & Vars::Comp::Radar::Player::DrawEnum::Team))
					continue;
			}
			if (!(Vars::Comp::Radar::Player::Draw.Value & Vars::Comp::Radar::Player::DrawEnum::Cloaked) && pPlayer->m_flInvisibility() >= 1.f)
				continue;

			int x, y, z;
			if (GetDrawPosition(pLocal, pPlayer, x, y, z))
			{
				const Color_t tColor = F::CompColor.GetEntityDrawColor(pLocal, pPlayer, Vars::Comp::Palette::Relative.Value);

				int iBounds = iSize;
				if (Vars::Comp::Radar::Player::Background.Value)
				{
					const float flRadius = sqrtf(pow(iSize, 2) * 2) / 2;
					F::CompDraw.FillCircle(x, y, flRadius, 20, tColor);
					iBounds = flRadius * 2;
				}

				switch (Vars::Comp::Radar::Player::Icon.Value)
				{
				case Vars::Comp::Radar::Player::IconEnum::Avatars:
				{
					player_info_t pi{};
					if (I::EngineClient->GetPlayerInfo(pPlayer->entindex(), &pi) && !pi.fakeplayer)
					{
						int iType = 0; F::PlayerUtils.GetPlayerName(pPlayer->entindex(), nullptr, &iType);
						if (iType != 1)
						{
							F::CompDraw.Avatar(x, y, iSize, iSize, pi.friendsID);
							break;
						}
					}
					[[fallthrough]];
				}
				case Vars::Comp::Radar::Player::IconEnum::Portraits:
					if (int iTeam = pPlayer->IsInValidTeam())
					{
						F::CompDraw.Texture(x, y, iSize, iSize, pPlayer->m_iClass() + (iTeam == TF_TEAM_RED ? 9 : 18) - 1);
						break;
					}
					[[fallthrough]];
				case Vars::Comp::Radar::Player::IconEnum::Icons:
					F::CompDraw.Texture(x, y, iSize, iSize, pPlayer->m_iClass() - 1);
					break;
				}

				if (Vars::Comp::Radar::Player::Health.Value)
				{
					const int iMaxHealth = pPlayer->GetMaxHealth(), iHealth = pPlayer->m_iHealth();

					float flRatio = std::clamp(float(iHealth) / std::max(iMaxHealth,1), 0.f, 1.f);
					Color_t cColor = Vars::Comp::Palette::IndicatorBad.Value.Lerp(Vars::Comp::Palette::IndicatorGood.Value, flRatio);
					F::CompDraw.FillRectPercent(x - iBounds / 2, y - iBounds / 2, 2, iBounds, flRatio, cColor, { 0, 0, 0, 255 }, ALIGN_BOTTOM, true);

					if (iHealth > iMaxHealth)
					{
						const float flMaxOverheal = floorf(iMaxHealth / 10.f) * 5;
						flRatio = std::clamp((iHealth - iMaxHealth) / std::max(flMaxOverheal,1.f), 0.f, 1.f);
						cColor = Vars::Comp::Palette::IndicatorMisc.Value;
						F::CompDraw.FillRectPercent(x - iBounds / 2, y - iBounds / 2, 2, iBounds, flRatio, cColor, { 0, 0, 0, 0 }, ALIGN_BOTTOM, true);
					}
				}

				if (Vars::Comp::Radar::Player::Height.Value && std::abs(z) > 80.f)
				{
					const int m = x - iSize / 2;
					const int iOffset = z < 0 ? -5 : 5;
					const int yPos = z < 0 ? y - iBounds / 2 - 2 : y + iBounds / 2 + 2;

					F::CompDraw.FillPolygon({ Vec2(m, yPos), Vec2(m + iSize * 0.5f, yPos + iOffset), Vec2(m + iSize, yPos) }, tColor);
				}
			}
		}
	}
}

void CFullRadar::Run(CTFPlayer* pLocal)
{
	if (!Vars::Comp::Radar::Main::Enabled.Value || I::MatSystemSurface->IsCursorVisible() && !I::EngineClient->IsPlayingDemo() && !F::Menu.m_bIsOpen)
		return;

	DrawBackground();
	DrawPoints(pLocal);
}
