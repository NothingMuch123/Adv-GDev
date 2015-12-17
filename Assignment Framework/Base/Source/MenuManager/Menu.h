#ifndef MENU_H
#define MENU_H

#include "UIButton.h"

class Menu 
{
public:
	enum E_MENU_TYPE
	{
		MENU_NONE = 0,
		MENU_MAIN,
		MENU_LEVEL_SELECT,
		MENU_INSTRUCTIONS,
		MENU_OPTIONS,
		MENU_CREDITS,
		MENU_EXIT,
		MENU_WIN_LEVEL,
		MENU_LOSE_LEVEL,
		MENU_PAUSE,
		NUM_MENU,
	};

protected:	// Variables
	vector<UIButton*> m_buttonList;
	E_MENU_TYPE m_type;
	CGameObject* m__background;

public:		// Functions
	Menu();
	virtual ~Menu();

	virtual void Init(E_MENU_TYPE type, Mesh* _bgMesh = NULL, Vector3 bgPos = Vector3::ZERO_VECTOR, Vector3 bgScale = Vector3(1,1));
	virtual void Update(double dt);
	void Clear();

	bool AddButton(UIButton* _button);

	void Resize(Vector3 bgPos, Vector3  bgScale);

	/*
	 * Getters
	 */
	vector<UIButton*>& GetButtonList();
	E_MENU_TYPE GetType();
	CGameObject* GetBackground();
};

#endif