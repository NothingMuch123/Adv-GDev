#include "Options.h"
#include "..\GameStateManager\GameStateManager.h"


COptions::COptions()
{
}


COptions::~COptions()
{
}

MenuManager::E_RETURN_STATE COptions::Response(UIButton::E_BUTTON_TYPE type, CGameStateManager* GSM)
{
	switch (type)
	{
	case UIButton::BUTTON_RETURN_TO_MAIN_MENU:
		{
			GSM->S_MUSIC = !GSM->S_MUSIC;
			GSM->PopState();
			return E_RETURN_STATE::RS_CHANGE;
		}
		break;
	}

	return E_RETURN_STATE::RS_NONE;
}
