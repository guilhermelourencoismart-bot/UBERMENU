#pragma once
#include "Shell.h"

enum EMenuPage
{
	MenuPageAimbot,
	MenuPageVisuals,
	MenuPageHvH,
	MenuPageMisc,
	MenuPageAnticheat,
	MenuPagePlayers,
	MenuPageSkins,
	MenuPageSettings
};

inline const std::vector<NavPage_t> vMenuPages = {
	{ "Aimbot", ICON_MD_GPS_FIXED, { "Geral", "Armas", "Apoio", "Indicadores" } },
	{ "Visuais", ICON_MD_VISIBILITY, { "ESP Unibox", "Câmera e interface", "Cenário", "Interface", "ESP Comp" } },
	{ "HvH", ICON_MD_BOLT, { "Ticks", "Ângulos" } },
	{ "Utilidades", ICON_MD_TUNE, { "Movimento e utilidades", "Automação", "Navegação e acompanhamento", "Filas de partidas" } },
	{ "Monitoramento", ICON_MD_GPP_MAYBE, { "Suspeitos", "Detecção" } },
	{ "Jogadores", ICON_MD_GROUP, { "Sala", "Marcadores", "Marcados", "Registros", "Avisos" } },
	{ "Skins", ICON_MD_PALETTE, { "Armas", "Mochila" } },
	{ "Configurações", ICON_MD_SETTINGS, { "Arquivos", "Atalhos", "Materiais", "Utilidades" } }
};

namespace Pages
{
	bool Draw(int iPage, int iSub);
	void Search(const std::string& sSearch);
}
