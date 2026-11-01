// Full Comp radar, source 366fa0dc SHA256 0361867e2fb1333ffd96cf008bbae24d5f4109c3aa0ce45ffcf25146f28e6246
#pragma once
#include "../../../SDK/SDK.h"

class CFullRadar
{
	bool GetDrawPosition(CTFPlayer* pLocal, CBaseEntity* pEntity, int& x, int& y, int& z);
	void DrawBackground();
	void DrawPoints(CTFPlayer* pLocal);

public:
	void Run(CTFPlayer* pLocal);
};

ADD_FEATURE(CFullRadar, FullRadar);
