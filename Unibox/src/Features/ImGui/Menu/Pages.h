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
	MenuPageSettings,
	MenuPageHome = MenuPresentation::HomePage
};

inline const std::vector<NavPage_t> vMenuPages = {
	{ "Aimbot", ICON_MD_GPS_FIXED, { "Geral", "Armas", "Apoio", "Indicadores" } },
	{ "Visuais", ICON_MD_VISIBILITY, { "ESP Unibox", "Câmera", "Cenário", "Interface", "ESP Comp" } },
	{ "Combate avançado (HvH)", ICON_MD_BOLT, { "Ritmo de comandos", "Ângulos" } },
	{ "Utilidades", ICON_MD_TUNE, { "Movimento", "Automação", "Bots e navegação", "Filas" } },
	{ "Monitoramento", ICON_MD_GPP_MAYBE, { "Suspeitos", "Detecção" } },
	{ "Jogadores", ICON_MD_GROUP, { "Sala", "Marcadores", "Marcados", "Registros", "Avisos" } },
	{ "Skins", ICON_MD_PALETTE, { "Armas", "Mochila" } },
	{ "Configurações", ICON_MD_SETTINGS, { "Arquivos", "Atalhos", "Materiais", "Utilidades" } },
	{ "Início", ICON_MD_HOME, { "Essencial", "Favoritos", "Explorar tudo" } }
};

namespace Pages
{
	bool Draw(int iPage, int iSub);
	void Search(const std::string& sSearch);
}
