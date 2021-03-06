#pragma once
#include "DataStructs.h"
#include <fstream>
class ObjLoader
{
public:
	ObjLoader();
	int loadObj(const char * path, VertexData *& vData, unsigned int *& iData, unsigned int &vCount, unsigned int &iCount, float scale = 1.0f, bool openGL = true);
	void CalculateTangentArray(unsigned int vertexCount, const DirectX::XMFLOAT3 *vertex, const DirectX::XMFLOAT3 *normal,
		const DirectX::XMFLOAT2 *texcoord, unsigned int indexCount, unsigned int * indexList, DirectX::XMFLOAT3 *tangent);
	~ObjLoader();
	int LoadTangentData(const char * path,DirectX::XMFLOAT3* destArray , int numVerts);
};

