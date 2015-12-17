#ifndef _INSTRUCTIONS_H_
#define _INSTRUCTIONS_H_

#include "MenuManager.h"

class CGameStateManager;

class CInstructions : public MenuManager
{
public:
	CInstructions();
	virtual ~CInstructions();

	virtual E_RETURN_STATE Response(UIButton::E_BUTTON_TYPE type, CGameStateManager* GSM);
};

#endif