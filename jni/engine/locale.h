/* (c) Alexandre D�az. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at fingership.redneboa.es        */

#ifndef H_LOCALE
#define H_LOCALE
#include "game_core.h"

enum
{
	LANGUAGE_ES=0,
	LANGUAGE_US,
	LANGUAGE_GAL,
	NUM_LANGUAGES,


	RESOURCE_STR_HAS_GANADO=0,
	RESOURCE_STR_SI_TE_HA_GUSTADO_POR_FAVOR,
	RESOURCE_STR_VOTA_EN_LA_STORE_GRACIAS,
	RESOURCE_STR_SALIR,
	RESOURCE_STR_EL_RETO_DE_LOS_X_METROS,
	RESOURCE_STR_PULSA_EN_LA_NAVE_PARA_EMPEZAR,
	RESOURCE_STR_CREADO_POR_UNSIGNED_CHAR,
	RESOURCE_STR_OPCIONES,
	RESOURCE_STR_CERRAR,
	RESOURCE_STR_ATRAS,
	RESOURCE_STR_VIBRACION,
	RESOURCE_STR_REDUCIR_GRAFICOS,
	RESOURCE_STR_SONIDOS,
	RESOURCE_STR_MUSICA,
	RESOURCE_STR_IDIOMA,
	RESOURCE_STR_REINICIAR,
	RESOURCE_STR_METROS,
	RESOURCE_STR_DISPARALE_PARA_DESTRUILA,
	RESOURCE_STR_PULSA_ENCIMA_PARA_DESTRUIRLA,
	RESOURCE_STR_ARMA_SOBRECALENTADA,
	RESOURCE_STR_RECOGELO_PARA_AYUDARLE,
	RESOURCE_STR_JEFE_FINAL,
	RESOURCE_STR_PAUSADO,
	RESOURCE_STR_PAUSAR,
	RESOURCE_STR_REANUDAR,
	RESOURCE_STR_VIDA,
	RESOURCE_STR_CREDITOS,
	RESOURCE_STR_GRAFICOS,
	RESOURCE_STR_LOGO,
	RESOURCE_STR_BETA_TESTER,
	RESOURCE_STR_AUTOR_PROGRAMACION,

	NUM_RESOURCES_STR
};

static const char m_sLocale[NUM_LANGUAGES][NUM_RESOURCES_STR][255] = {
	{
		"HAS GANADO!\0",
		"SI TE HA GUSTADO POR FAVOR\0",
		"VOTA EN LA STORE. GRACIAS!\0",
		"SALIR\0",
		"El Reto De Los %.0f Metros\0",
		"PULSA EN LA NAVE PARA EMPEZAR\0",
		"Copyright (C) 2015-2016 Alexandre Diaz\0",
		"OPCIONES\0",
		"CERRAR\0",
		"ATRAS\0",
		"Vibracion\0",
		"Reducir Graficos\0",
		"Sonidos\0",
		"Musica\0",
		"Idioma:\0",
		"REINICIAR\0",
		"Metros\0",
		"Disparale para destruirla\0",
		"Pulsa encima para destruirla\0",
		"Arma Sobrecalentada!\0",
		"Recogele para ayudarle\0",
		"Jefe Final\0",
		"PAUSADO\0",
		"PAUSAR\0",
		"REANUDAR\0",
		"Vida:\0",
		"CREDITOS\0",
		"Graficos\0",
		"Logo\0",
		"Beta-Tester\0",
		"Autor & Programacion\0",
	},
	{
		"YOU WIN!\0",
		"IF YOU LIKED PLEASE\0",
		"VOTE IN THE STORE. THANK YOU!\0",
		"EXIT\0",
		"The Challenge Of %.0f Meters\0",
		"TAP ON THE SHIP TO START\0",
		"Copyright (C) 2015-2016 Alexandre Diaz\0",
		"OPTIONS\0",
		"CLOSE\0",
		"BACK\0",
		"Vibration\0",
		"Low Graphics\0",
		"Sounds\0",
		"Music\0",
		"Language:\0",
		"RESTART\0",
		"Meters\0",
		"Shoot to destroy\0",
		"Tap above to destroy\0",
		"Weapon Overheated!\0",
		"Pick me up for help\0",
		"Final Boss\0",
		"PAUSED\0",
		"PAUSE\0",
		"RESUME\0",
		"Health:\0",
		"CREDITS\0",
		"Graphics\0",
		"Logo\0",
		"Beta-Tester\0",
		"Author & Programming\0",
	},
	{
		"GANHOU!\0",
		"SE GUSTOUCHE POR FAVOR\0",
		"VOTA NA STORE. GRACINHAS!\0",
		"SAIR\0",
		"O Reto Dos %.0f Metros\0",
		"PREME NA NAVE PARA COMEZAR\0",
		"Copyright (C) 2015-2016 Alexandre Diaz\0",
		"OPCIONS\0",
		"PECHAR\0",
		"VOLTAR\0",
		"Vibracion\0",
		"Reducir Graficos\0",
		"Sons\0",
		"Musica\0",
		"Linguaxe:\0",
		"REINICIAR\0",
		"Metros\0",
		"Disparalle para destruila\0",
		"Preme enriba para destruila\0",
		"Arma Sobrecalentada!\0",
		"Recolleo para axudarlle\0",
		"Xefe Final\0",
		"PAUSADO\0",
		"PAUSAR\0",
		"REANUDAR\0",
		"Vida:\0",
		"CREDITOS\0",
		"Graficos\0",
		"Logo\0",
		"Beta-Tester\0",
		"Autor & Programacion\0",
	},
};

class CLocale
{
public:
	static const char* getString(int resourceId) {
		CGameCore *pCore = CGameCore::getInstance();

		if (resourceId < 0 || resourceId > NUM_RESOURCES_STR)
			return 0x0;

		return m_sLocale[pCore->m_Config.m_Language][resourceId];
	}
};

#endif
