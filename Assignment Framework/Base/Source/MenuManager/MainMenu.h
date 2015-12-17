#ifndef _MAINMENU_H_
#define _MAINMENU_H_

#include "MenuManager.h"

class CGameStateManager;

class CMainMenu : public MenuManager
{
public:
	CMainMenu();
	virtual ~CMainMenu();

	virtual E_RETURN_STATE Response(UIButton::E_BUTTON_TYPE type, CGameStateManager* GSM);
};

#endif