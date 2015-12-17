#ifndef GDEV_ASSIGNMENT01_H
#define GDEV_ASSIGNMENT01_H

#include "..\SceneBase.h"
#include "..\Character\ThirdPerson.h"
#include "..\SpatialPartition\SpatialPartition.h"
#include "..\Projectile\Projectile.h"

#include <fstream>

class GameStateManager;

class AGDev_Assign01 : public SceneBase
{
public:
	enum MESH_TYPE
	{
		MESH_TEXT = 0,
		MESH_CUBE,
		MESH_SPHERE,
		MESH_CONE,

		// Object
		MESH_CHARACTER,

		// Skybox
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_GRASS_DARKGREEN,
		GEO_GRASS_LIGHTGREEN,

		//Skybox
		MESH_FLOOR,

		NUM_MESH,
	};
	AGDev_Assign01(int width = 1280, int height = 720);
	virtual ~AGDev_Assign01();

	// Functions from scenebase, do not add parameters else override will not work
	virtual void Init(int width, int height);
	virtual void Update(CGameStateManager* GSM, double dt);
	virtual void Render();
	virtual void Exit();
	virtual void Reset();
	virtual void ProcessKeys(CGameStateManager* GSM, double dt, bool* keys, Vector2 mousePos);
	virtual void ProcessMouse(CGameStateManager* GSM, double dt, float yaw, float pitch, Vector2 mousePos);

	virtual void UpdateCameraStatus(unsigned char key);
	virtual void UpdateWeaponStatus(unsigned char key, double dt = 0.0);

	void RenderSkybox();
	void RenderGround();

	void SetHUD(const bool m_bHUDmode);

	void PreRendering(Vector3 translate, Vector3 rotate, Vector3 scale, bool enableLight);
	void PostRendering();

private:
	void InitMesh();
	void InitMap();

private:
	Mesh* m_meshList[NUM_MESH];
	CGameObject* test;
	CSceneNode* nodeTest;

	CThirdPerson* m_char;

	// Spatial partition
	CSpatialPartition* m_spatialPartition;

	// Projectile
	vector<CProjectile*> m_projList;

	// Sound
	irrklang::ISoundEngine *sound;
};

#endif