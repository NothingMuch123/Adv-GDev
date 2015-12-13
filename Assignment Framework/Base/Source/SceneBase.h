#ifndef SCENEBASE_H
#define SCENEBASE_H

#include "Scene.h"
#include "irrKlang.h"
#include "Mtx44.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include <vector>
#include "LoadHmap.h"
#include "Obj.h"

class SceneBase : public Scene
{
public:
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,
		U_NUMLIGHTS,
		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_LIGHT1_TYPE,
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_COLOR_TEXTURE_ENABLED1,
		U_COLOR_TEXTURE1,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,

		// Fog
		U_FOG_COLOR,
		U_FOG_START,
		U_FOG_END,
		U_FOG_DENSITY,
		U_FOG_TYPE,
		U_FOG_ENABLE,
		U_TOTAL,
	};

	SceneBase(int width = 1280, int height = 720);
	virtual ~SceneBase(void);

	virtual void Init();
	virtual void Init(int width, int height);
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	virtual void Reset();

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderMeshIn2D(Mesh *mesh, bool enableLight, float size = 1.0f, float x = 0.0f, float y = 0.0f, float rotate = 0.f);

protected:
	unsigned m_vertexArrayID;
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Camera3* m_camera; // Pointer to the current view

	MS modelStack;
	MS viewStack;
	MS projectionStack;

	static const int MAX_LIGHT = 2;
	Light lights[MAX_LIGHT];

	bool m_lightEnabled;
	bool fogEnabled;

	float m_fps;
	float m_fov; // Used for changing FOV

	// Screen width and height
	int m_window_width, m_window_height;
};

#endif