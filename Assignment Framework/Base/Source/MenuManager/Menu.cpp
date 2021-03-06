#include "Menu.h"

Menu::Menu(void) : m_buttonList(NULL), m_type(MENU_NONE), m__background(NULL)
{
}

Menu::~Menu(void)
{
	Clear();
}

void Menu::Init(E_MENU_TYPE type, Mesh* _bgMesh, Vector3 bgPos, Vector3 bgScale)
{
	if (_bgMesh != NULL && m__background == NULL)
	{
		m__background = new CGameObject();
		CTransform* transform = new CTransform();
		transform->Init(bgPos, Vector3(), bgScale);
		m__background->Init(_bgMesh, transform);
	}
	if (m__background)
	{
		m__background->SetMesh(_bgMesh);
		CTransform transform = m__background->GetTransform();
		transform.m_translate = bgPos;
		transform.m_scale = bgScale;
	}
	m_type = type;
}

void Menu::Update(double dt)
{

}

void Menu::Clear()
{
	while (m_buttonList.size() > 0)
	{
		UIButton* _button = m_buttonList.back();
		if (_button)
		{
			delete _button;
			_button = NULL;
		}
		m_buttonList.pop_back();
	}

	if (m__background)
	{
		delete m__background;
		m__background = NULL;
	}
}

bool Menu::AddButton(UIButton* _button)
{
	m_buttonList.push_back(_button);
	return true;
}

vector<UIButton*>& Menu::GetButtonList()
{
	return m_buttonList;
}

Menu::E_MENU_TYPE Menu::GetType()
{
	return m_type;
}

CGameObject* Menu::GetBackground()
{
	return m__background;
}

void Menu::Resize(Vector3 bgPos, Vector3  bgScale)
{
	//m__background->SetPos(bgPos);
	//m__background->SetScale(bgScale);
}