#include "SpatialPartition.h"
#include "stdio.h"
#include <algorithm>

struct MyStruct
{
	float key;
	int indexValue;

	MyStruct(float k, int iv) : key(k), indexValue(iv)
	{
	}
};

struct less_than_key
{
	inline bool operator() (const MyStruct& struct1, const MyStruct& struct2)
	{
		return (struct1.key < struct2.key);
	}
};

vector<MyStruct> vec;

/********************************************************************************
 Constructor
 ********************************************************************************/
CSpatialPartition::CSpatialPartition(void)
: theGrid(NULL)
, xSize(0)
, ySize(0)
, xGridSize(0)
, yGridSize(0)
, xNumOfGrid(0)
, yNumOfGrid(0)
{
}

/********************************************************************************
 Destructor
 ********************************************************************************/
CSpatialPartition::~CSpatialPartition(void)
{
	for (int i=0; i<xNumOfGrid; i++)
	{
		for (int j=0; j<yNumOfGrid; j++)
		{
			theGrid[ i*yNumOfGrid + j ].DeleteObjects();
		}
	}
	delete [] theGrid;
}

/********************************************************************************
 Initialise the spatial partition
 ********************************************************************************/
bool CSpatialPartition::Init(const int xSize, const int ySize, const int xNumOfGrid, const int yNumOfGrid)
{
	if ((xSize>0) && (ySize>0) 
		&& (xNumOfGrid>0) && (yNumOfGrid>0))
	{
		this->xNumOfGrid = xNumOfGrid;
		this->yNumOfGrid = yNumOfGrid;
		this->xGridSize = xSize;
		this->yGridSize = ySize;
		this->xSize = xGridSize * xNumOfGrid;
		this->ySize = yGridSize * yNumOfGrid;

		// Create an array of grids
		theGrid = new CGrid[ xNumOfGrid*yNumOfGrid ];

		// Initialise the array of grids
		for (int i=0; i<xNumOfGrid; i++)
		{
			for (int j=0; j<yNumOfGrid; j++)
			{
				theGrid[ i*yNumOfGrid + j ].Init(i, j, xSize, ySize);
			}
		}
		return true;
	}
	return false;
}

/********************************************************************************
 Get xNumOfGrid
 ********************************************************************************/
int CSpatialPartition::GetxNumOfGrid(void)
{
	return xNumOfGrid;
}

/********************************************************************************
 Get yNumOfGrid
 ********************************************************************************/
int CSpatialPartition::GetyNumOfGrid(void)
{
	return yNumOfGrid;
}

/********************************************************************************
 Get a particular grid
 ********************************************************************************/
CGrid CSpatialPartition::GetGrid(const int xIndex, const int yIndex)
{
	return theGrid[ xIndex*yNumOfGrid + yIndex ];
}

/********************************************************************************
 Get a particular grid's Mesh
 ********************************************************************************/
Mesh* CSpatialPartition::GetGridMesh(const int xIndex, const int yIndex)
{
	if (theGrid[xIndex*yNumOfGrid + yIndex].m_bDisplayed)
	{
		return theGrid[xIndex*yNumOfGrid + yIndex].theGridMesh;
	}
	else
	{
		return NULL;
	}
}

/********************************************************************************
 Set a particular grid
 ********************************************************************************/
void CSpatialPartition::SetGridMesh(const int xIndex, const int yIndex, Mesh* theGridMesh)
{
	theGrid[ xIndex*yNumOfGrid + yIndex ].SetMesh( theGridMesh );
}

/********************************************************************************
 Add a new object model
 ********************************************************************************/
void CSpatialPartition::AddObject(CSceneNode* theObject)
{
	// Get the indices of the 2 values of each position
	int index_topleft_x = ((int) theObject->GetTopLeft().x / (xSize*xNumOfGrid));
	int index_topleft_z = ((int) theObject->GetTopLeft().z / (ySize*yNumOfGrid));
	int index_bottomright_x = ((int) theObject->GetBottomRight().x / (xSize*xNumOfGrid));
	int index_bottomright_z = ((int) theObject->GetBottomRight().z / (ySize*yNumOfGrid));

	// Calculate the index of each position
	int index_topleft = index_topleft_x*yNumOfGrid + index_topleft_z;
	int index_bottomright = index_bottomright_x*yNumOfGrid + index_bottomright_z;

	// Add them to each grid
	if ((index_topleft>=0) && (index_topleft<xNumOfGrid*yNumOfGrid))
	{
		theGrid[ index_topleft ].AddObject( theObject );
	}

	// if part of the object is in another grid, then add it in as well.
	if ((index_bottomright>=0) && (index_bottomright<xNumOfGrid*yNumOfGrid))
	{
		if (index_topleft != index_bottomright)
		{
			theGrid[ index_bottomright ].AddObject( theObject );
		}
	}
}

/********************************************************************************
 Calculate the squared distance from camera to a grid's centrepoint
 ********************************************************************************/
float CSpatialPartition::CalculateDistanceSquare(Vector3* theCameraPosition, const int xIndex, const int yIndex)
{
	float xDistance = (xIndex * xNumOfGrid + (xSize / 2)) - theCameraPosition->x;
	float yDistance = (yIndex * yNumOfGrid + (ySize / 2)) - theCameraPosition->z;

	return (float) ( xDistance*xDistance + yDistance*yDistance );
}

float CSpatialPartition::CalculateDistanceSquare(Vector3 pos, Vector3 dir, const int xIndex, const int yIndex)
{
	float xDist = (xIndex * xGridSize + (xGridSize * 0.5)) - pos.x;
	float yDist = (yIndex * yGridSize + (yGridSize * 0.5)) - pos.z;
	return (float) (xDist*xDist + yDist*yDist);
}

/********************************************************************************
 Render the spatial partition
 ********************************************************************************/
void CSpatialPartition::Render(Vector3* theCameraPosition)
{
	for (int i=0; i<xNumOfGrid; i++)
	{
		for (int j=0; j<yNumOfGrid; j++)
		{
			// Render the Grid and its objects
			//glPushMatrix();
				// Check the distance for this grid then decide the LOD to display
				if (theCameraPosition)
				{
					float distanceGrid2Camera = CalculateDistanceSquare( theCameraPosition, i, j );
					if (distanceGrid2Camera < 900)
					{
						theGrid[ i*yNumOfGrid + j ].RenderObjects(0);
					}
					else if (distanceGrid2Camera < 3600)
					{
						theGrid[ i*yNumOfGrid + j ].RenderObjects(1);
					}
					else
					{
						theGrid[ i*yNumOfGrid + j ].RenderObjects(2);
					}
				}
			//glPopMatrix();
		}
	}

	for (int i=0; i<xNumOfGrid; i++)
	{
		for (int j=0; j<yNumOfGrid; j++)
		{
			// Render the Grid and its objects
			//glPushMatrix();
				theGrid[ i*yNumOfGrid + j ].Render();
			//glPopMatrix();
		}
	}
}

/********************************************************************************
 Update the spatial partition
 ********************************************************************************/
void CSpatialPartition::Update(Vector3 cameraPos, Vector3 cameraNormal)
{
	for (int i=0; i<xNumOfGrid; i++)
	{
		for (int j=0; j<yNumOfGrid; j++)
		{
			// Update the Grids
			theGrid[ i*yNumOfGrid + j ].Update();
		}
	}

	/*
	// Check for occlusion
	findNearestGrid(cameraPos, cameraNormal);

	occulusionChecker.SetScreenCoordinate(cameraPos);
	occulusionChecker.SetScreenNormal(cameraNormal);

#if _DEBUG
	cout << "Camera pos: " << cameraPos << endl;
#endif

	if (vec.size() > 1)
	{
		// Use the nearest grid as the reference grid
		Vector3 posReference_TopLeft = occulusionChecker.GetProjectedCoordinate(theGrid[vec[0].indexValue].GetTopLeft());
		Vector3 posReference_BottomRight = occulusionChecker.GetProjectedCoordinate(theGrid[vec[0].indexValue].GetBottomRight());

#if _DEBUG
		cout << "0. Before projection: " << theGrid[vec[0].indexValue].GetTopLeft() << ", " << theGrid[vec[0].indexValue].GetBottomRight() << endl;
		cout << "0. After projection: " << posReference_TopLeft << ", " << posReference_BottomRight << endl;
#endif

		// Set the nearest grid's display boolean flag to true first
		theGrid[vec[0].indexValue].m_bDisplayed = true;

		for (int i = 1; i < vec.size(); ++i)
		{
			Vector3 posCheck_TopLeft = occulusionChecker.GetProjectedCoordinate(theGrid[vec[i].indexValue].GetTopLeft());
			Vector3 posCheck_BottomRight = occulusionChecker.GetProjectedCoordinate(theGrid[vec[i].indexValue].GetBottomRight());

#if _DEBUG
			cout << i << ". Before projection: " << theGrid[vec[i].indexValue].GetTopLeft() << ", " << theGrid[vec[i].indexValue].GetBottomRight() << endl;
			cout << i << ". After projection: " << posCheck_TopLeft << ", " << posReference_BottomRight << endl;
			cout << "========== Start of Occulusion ==========" << endl;
#endif

			// Set the grid's display boolean flag to false first
			theGrid[vec[i].indexValue].m_bDisplayed = false;

			// Check the top left corner of a grid against the reference positions
			if (posReference_TopLeft.x > posCheck_TopLeft.x)
			{
#if _DEBUG
				cout << "posReference_TopLeft.x set from " << posReference_TopLeft.x << "to " << posCheck_TopLeft.x << endl;
#endif
				posReference_TopLeft.x = posCheck_TopLeft.x;
				theGrid[vec[i].indexValue].m_bDisplayed = true;
			}
			if (posReference_TopLeft.z > posCheck_TopLeft.z)
			{
#if _DEBUG
				cout << "posReference_TopLeft.z set from " << posReference_TopLeft.z << "to " << posCheck_TopLeft.z << endl;
#endif
				posReference_TopLeft.z = posCheck_TopLeft.z;
				theGrid[vec[i].indexValue].m_bDisplayed = true;
			}

			// Check the bottom right corner of a grid against the reference position
			if (posReference_BottomRight.x < posCheck_BottomRight.x)
			{
#if _DEBUG
				cout << "posReference_BottomRight.x set from " << posReference_BottomRight.x << "to " << posCheck_BottomRight.x << endl;
#endif
				posReference_BottomRight.x = posCheck_BottomRight.x;
				theGrid[vec[i].indexValue].m_bDisplayed = true;
			}
			if (posReference_BottomRight.z < posCheck_BottomRight.z)
			{
#if _DEBUG
				cout << "posReference_BottomRight.z set from " << posReference_BottomRight.z << "to " << posCheck_BottomRight.z << endl;
#endif
				posReference_BottomRight.z = posCheck_BottomRight.z;
				theGrid[vec[i].indexValue].m_bDisplayed = true;
			}
#if _DEBUG
			cout << "========== End of Occulusion ==========" << endl;
			cout << "Current Reference: " << posReference_TopLeft << ", " << posReference_BottomRight << endl << endl;
#endif
		}
#if _DEBUG
		cout << endl << "Printout of vec" << endl;
		for (int i = 0; i < vec.size(); ++i)
		{
			cout << i << ": " << theGrid[vec[i].indexValue].GetTopLeft() << ", " << theGrid[vec[i].indexValue].GetBottomRight() << theGrid[vec[i].indexValue].m_bDisplayed << endl;
		}
#endif
		vec.clear();
	}
	*/
}


/********************************************************************************
 PrintSelf
 ********************************************************************************/
void CSpatialPartition::PrintSelf()
{
	cout << "* CSpatialPartition::PrintSelf() ***********************************************" << endl;
	cout << "xSize        : " << xSize << endl;
	cout << "ySize        : " << ySize << endl;
	cout << "xNumOfGrid   : " << xNumOfGrid << endl;
	cout << "xNumOfGrid   : " << xNumOfGrid << endl;
	if (theGrid)
	{
		cout << "theGrid : OK" << endl;
		cout << "Printing out theGrid below: " << endl;
		for (int i=0; i<xNumOfGrid; i++)
		{
			for (int j=0; j<yNumOfGrid; j++)
			{
				theGrid[ i*yNumOfGrid + j ].PrintSelf();
			}
		}
	}
	else
		cout << "theGrid : NULL" << endl;
	cout << "********************************************************************************" << endl;
}

void CSpatialPartition::findNearestGrid(Vector3 pos, Vector3 dir)
{
	float dist = -1.f;

	for (int i = 0; i < xNumOfGrid; ++i)
	{
		for (int j = 0; j < yNumOfGrid; ++j)
		{
			dist = CalculateDistanceSquare(pos, dir, i, j);
			vec.push_back(MyStruct(dist, i*yNumOfGrid + j));
		}
	}

#if _DEBUG
	// Display list of grids before sorting
	cout << "Before sort" << endl;
	for (int i = 0; i < vec.size(); ++i)
	{
		cout << i << ": index = " << vec[i].indexValue << ", key = " << vec[i].key << " => " << theGrid[vec[i].indexValue].m_bDisplayed << endl;
	}
#endif

	// Sort vector
	sort(vec.begin(), vec.end(), less_than_key());

#if _DEBUG
	// DIsplay list of grids after sorting
	cout << "After sort" << endl;
	for (int i = 0; i < vec.size(); ++i)
	{
		cout << i << ": index = " << vec[i].indexValue << ", key = " << vec[i].key << " => " << theGrid[vec[i].indexValue].m_bDisplayed << endl;
	}
#endif
}
