#ifndef _LEVELSELECT_H_
#define _LEVELSELECT_H_

#include "MenuManager.h"

class CGameStateManager;

class COptions : public MenuManager
{
public:
	COptions();
	virtual ~COptions();

	virtual E_RETURN_STATE Response(UIButton::E_BUTTON_TYPE type, CGameStateManager* GSM);
};

#endif