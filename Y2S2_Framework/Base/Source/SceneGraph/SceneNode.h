#pragma once
#include "node.h"
#include "Model.h"
#include "Transform.h"
//#include <list>

#include <vector>
using namespace std;

class CSceneManager;

class CSceneNode :
	public CNode
{
private:
	vector<CNode*> theChildren;

	CModel* theModel;
	CTransform* theTransform;
	int sceneNodeID;

	// Check where a line segment between two positions intersects a plane
	int GetIntersection(float fDst1, float fDst2, Vector3 P1, Vector3 P2, Vector3 &Hit);
	// Check two positions are within a box region
	int InBox(Vector3 Hit, Vector3 B1, Vector3 B2, const int Axis);

public:
	CSceneNode(void);
	CSceneNode(const int sceneNodeID);
	~CSceneNode(void);

	// Update resolution
	void Update(const CModel::RESOLUTION_TYPE type);

	// Draw this Node and its children
	void Draw(void);
	void Draw(CSceneManager* theSceneManager);

	// Set model and other info to this Node
	void SetModel(CModel* theModel);
	int SetNode(CTransform* aNewTransform, CModel* aNewModel);
	void SetSceneNodeID(const int sceneNodeID);

	int AddChild(CTransform* aNewTransform, CModel* aNewModel);
	int AddChild(const int sceneNodeID, CTransform* aNewTransform, CModel* aNewModel);

	// Get methods
	int GetSceneNodeID(void);
	CSceneNode* GetNode(const int sceneNodeID);

	// Set transformation
	void SetTranslate(const float dx, const float dy, const float dz);
	void SetRotate(const float angle, const float rx, const float ry, const float rz);

	// Apply transformation to this Node and its children
	void ApplyTranslate( const float dx, const float dy, const float dz );
	void ApplyRotate( const float angle, const float rx, const float ry, const float rz );

	// Get top left corner of the group
	Vector3 GetTopLeft(void);
	// Get bottom right corner of the group
	Vector3 GetBottomRight(void);

	// Set colour of the instance
	void SetColor(const float red, const float green, const float blue);

	// Return the number of children in this group
	int GetNumOfChild(void);
	// Get top left corner of a child
	bool GetTopLeft(const int m_iChildIndex, Vector3& vector3D_TopLeft);
	// Get bottom right corner of a child
	bool GetBottomRight(const int m_iChildIndex, Vector3& vector3D_BottomRight);

	// Set the Colour of a child
	void SetColorForChild(const int m_iChildIndex, const float red, const float green, const float blue);

	// Check a position for collision with objects and its child
	bool CheckForCollision(Vector3 pos);

	// Check two positions for collision with objects in any of the grid
	int CheckForCollision(Vector3 position_start, Vector3 position_end, Vector3 &Hit);
};
