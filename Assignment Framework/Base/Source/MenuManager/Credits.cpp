#include "Credits.h"
#include "..\GameStateManager\GameStateManager.h"


CCredits::CCredits()
{
}


CCredits::~CCredits()
{
}

MenuManager::E_RETURN_STATE CCredits::Response(UIButton::E_BUTTON_TYPE type, CGameStateManager* GSM)
{
	switch (type)
	{
	case UIButton::BUTTON_RETURN_TO_MAIN_MENU:
		{
			GSM->PushState(new CMainMenuState());
			return E_RETURN_STATE::RS_CHANGE;
		}
		break;
	}

	return E_RETURN_STATE::RS_NONE;
}
