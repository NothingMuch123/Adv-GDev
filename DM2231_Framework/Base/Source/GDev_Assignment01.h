#ifndef GDEV_ASSIGNMENT01_H
#define GDEV_ASSIGNMENT01_H

#include "Projectile.h"
#include "Minimap.h"
#include "SceneBase.h"
#include "Character.h"
#include "Target.h"
#include "AmmoCrate.h"

#include "Object\GameObject.h"

class GDev_Assignment01 : public SceneBase
{
public:
	static float MAX_CHANGE_WEAPON_TIMER;
	static float MAX_SCOPE_TIMER;
	static float MAX_SPAWN_TARGET_TIMER;

	enum WEAPON_ACTION
	{
		WA_NIL = 0,
		WA_FIRE,
		WA_RELOAD,
		WA_CHANGEWEAPON_NEXT,
		WA_CHANGEWEAPON_PREV,
		WA_SCOPE,
		WA_TOTAL,
	};

	GDev_Assignment01();
	~GDev_Assignment01();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	virtual void Reset();
	virtual void UpdateCameraStatus(unsigned char key);
	virtual void UpdateWeaponStatus(unsigned char key, double dt = 0.0);

	void SetHUD(const bool m_bHUDmode);
	void RenderSkyPlane();
	void RenderTerrain();
	void RenderTextInWorld();
	void RenderObject();
	void Render2D();
	void RenderProjectile(CProjectile *p);
	void RenderWeapon(CWeapon *weapon);
	void RenderAmmo(CWeapon *weapon);
	void RenderEntity(Mesh *mesh, bool enableLight, Vector2 minimapPos, Vector2 objectPos, float rotate);

	void SpawnTarget();

	CProjectile *FetchProj();

private:
	CMinimap *m_cMinimap; // Handle to the minimap

	std::vector<CProjectile*> projectileList;
	std::vector<CTarget*> targetList;
	std::vector<CAmmoCrate*> ammocrateList;
	float FireRateCounter, ReloadTimer, ChangeWeaponTimer, ScopeTimer, SpawnTargetTimer; // Timers
	bool reloading, scope;
	int score;
	double recoil;
	bool reloadTranslation;

	// Character
	CCharacter *currentChar;

	// Sound
	irrklang::ISoundEngine *sound;
};

#endif