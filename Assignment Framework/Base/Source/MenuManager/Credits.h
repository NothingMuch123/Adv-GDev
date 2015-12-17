#ifndef _CREDITS_H_
#define _CREDITS_H_

#include "MenuManager.h"

class CGameStateManager;

class CCredits : public MenuManager
{
public:
	CCredits();
	virtual ~CCredits();

	virtual E_RETURN_STATE Response(UIButton::E_BUTTON_TYPE type, CGameStateManager* GSM);
};

#endif