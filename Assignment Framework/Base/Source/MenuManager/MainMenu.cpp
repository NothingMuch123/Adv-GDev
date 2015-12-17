#include "MainMenu.h"
#include "..\GameStateManager\GameStateManager.h"


CMainMenu::CMainMenu()
{
}


CMainMenu::~CMainMenu()
{
}

MenuManager::E_RETURN_STATE CMainMenu::Response(UIButton::E_BUTTON_TYPE type, CGameStateManager* GSM)
{
	switch (type)
	{
	case UIButton::BUTTON_NEW_GAME:
		{
			GSM->PushState(new CPlayState());
			return E_RETURN_STATE::RS_CHANGE;
		}
		break;
	case UIButton::BUTTON_OPTIONS:
		{
			GSM->PushState(new COptionsState());
			return E_RETURN_STATE::RS_CHANGE;
		}
		break;
	case UIButton::BUTTON_INSTRUCTIONS:
		{
			GSM->PushState(new CInstructionsState());
			return E_RETURN_STATE::RS_CHANGE;
		}
		break;
	case UIButton::BUTTON_CREDITS:
		{
			GSM->PushState(new CCreditsState());
			return E_RETURN_STATE::RS_CHANGE;
		}
		break;
	case UIButton::BUTTON_EXIT:
		{
			GSM->Quit();
			return E_RETURN_STATE::RS_CHANGE;
		}
		break;
	}

	return E_RETURN_STATE::RS_NONE;
}
