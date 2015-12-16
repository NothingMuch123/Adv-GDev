#ifndef UIBUTTON_H
#define UIBUTTON_H

#include "..\Object\GameObject.h"
#include <vector>

using std::vector;

class UIButton : public CGameObject
{
	public:
		enum E_BUTTON_STATE_TYPE
		{
			UP_STATE,
			DOWN_STATE,
			HOVER_STATE,
			NUM_BUTTON_STATES
		};
		enum E_BUTTON_TYPE
		{
			BUTTON_NONE = 0,
			// Universal buttons
			BUTTON_RETURN_TO_MAIN_MENU,

			// Main menu buttons
			BUTTON_NEW_GAME, // New game
			BUTTON_CONTINUE, // Continue game
			BUTTON_LEVEL_SELECT,
			BUTTON_INSTRUCTIONS,
			BUTTON_CREDITS,
			BUTTON_EXIT,

			// Lose level menu buttons
			BUTTON_RETRY,

			// Win level menu buttons
			BUTTON_NEXT_STAGE,

			// Level select menu buttons
			BUTTON_START,
			BUTTON_LEVEL,

			// Pause
			BUTTON_RESUME,

			NUM_BUTTONS,
		};
		static Mesh* s_m_offMeshList[NUM_BUTTONS];	// Not hover
		static Mesh* s_m_onMeshList[NUM_BUTTONS];	// Hover

	private:
		E_BUTTON_STATE_TYPE m_state;
		E_BUTTON_TYPE m_type;
		int m_levelID;

	public:
		UIButton(E_BUTTON_TYPE type = BUTTON_NONE, Mesh* _mesh = NULL, Vector3 pos = Vector3::ZERO_VECTOR, Vector3 scale = Vector3(1,1,1));
		~UIButton();

		static void InitMeshLists(vector<Mesh*> meshList);
	
		void Init(E_BUTTON_TYPE type, Mesh* _mesh, Vector3 pos, Vector3 scale);
		void InitLevel(int levelID, Mesh* _mesh, Vector3 pos, Vector3 scale);
		void UpdateState(bool pressed, float mouseX, float mouseY);

		void Resize(Vector3 pos, Vector3 scale);

		/*
		 * Setters
		 */
		void SetState(E_BUTTON_STATE_TYPE type);

		/*
		 * Getters
		 */
		E_BUTTON_STATE_TYPE GetState(void);
		E_BUTTON_TYPE GetType(void);
		int GetLevelID();
};

#endif