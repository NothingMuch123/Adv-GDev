#include "Instructions.h"
#include "..\GameStateManager\GameStateManager.h"


CInstructions::CInstructions()
{
}


CInstructions::~CInstructions()
{
}

MenuManager::E_RETURN_STATE CInstructions::Response(UIButton::E_BUTTON_TYPE type, CGameStateManager* GSM)
{
	switch (type)
	{
	case UIButton::BUTTON_RETURN_TO_MAIN_MENU:
			{
				GSM->PopState();
				return E_RETURN_STATE::RS_CHANGE;
			}
		break;
	}

	return E_RETURN_STATE::RS_NONE;
}
