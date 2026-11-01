// Ported from Amalgam-Comp 366fa0dc; original SHA256 406a430087bcc768eeaab10a15e2dc2ddef4c80b2680e3ca967f532d5e395b46.
#pragma once
#include "../../../SDK/SDK.h"
#include <map>

struct CompMedicInfo
{
	std::string Name;
	std::string WeaponName;
	std::string RealWeaponName;
	float UberPercentage;
	bool IsAlive;
	int TeamNumber;
};

struct CompKritzTracker
{
	float LastKritzValue = 0.0f;
	float LastUberValue = 0.0f;
	float LastUpdateTime = 0.0f;
	bool WasUsed = false;
	bool WasBuilding = false;
};

class CFullUberTracker
{
private:
	// Configuration values are now read from Vars::Comp::UberTracker::* variables

	std::map<int, float> m_PrevUber;
	std::map<int, std::string> m_Status;
	std::map<int, bool> m_UberDecreasing;
	std::map<int, float> m_MedDeathTime;
	std::map<std::string, CompKritzTracker> m_KritzTrackers;

	std::string GetMedicID(CBaseEntity* pEntity);
	std::string GetWeaponType(int iItemDefinitionIndex, bool* bIsKritzOverride = nullptr);
	Color_t GetColorForUber(float flPercentage, bool bIsAlive);
	bool DetectKritzUsage(CompKritzTracker& tracker, float flActualKritzPercentage, float flCurrentTime);
	float TranslateKritzToUber(CBaseEntity* pEntity, const std::string& sMedicID, float flActualKritzPercentage, float flCurrentTime);
	void DrawMedicInfo(const std::vector<CompMedicInfo>& vMedics, int iStartY);
	void DrawAdvantageText(const std::vector<CompMedicInfo>& vRedMedics, const std::vector<CompMedicInfo>& vBluMedics, int iLocalTeam);

public:
	void Draw();
};

ADD_FEATURE(CFullUberTracker, FullUberTracker)
