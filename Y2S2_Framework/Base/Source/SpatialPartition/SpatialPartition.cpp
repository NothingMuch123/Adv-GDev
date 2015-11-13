#include "SpatialPartition.h"
#include "stdio.h"

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
	return theGrid[ xIndex*yNumOfGrid + yIndex ].theGridMesh;
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
	if ((index_topleft>0) && (index_topleft<xNumOfGrid*yNumOfGrid))
	{
		theGrid[ index_topleft ].AddObject( theObject );
	}

	// if part of the object is in another grid, then add it in as well.
	if ((index_bottomright>0) && (index_bottomright<xNumOfGrid*yNumOfGrid))
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
void CSpatialPartition::Update(void)
{
	for (int i=0; i<xNumOfGrid; i++)
	{
		for (int j=0; j<yNumOfGrid; j++)
		{
			// Update the Grids
			theGrid[ i*yNumOfGrid + j ].Update();
		}
	}
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
