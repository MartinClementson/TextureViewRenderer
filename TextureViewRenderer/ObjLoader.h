#pragma once
#include "DataStructs.h"
class ObjLoader
{
public:
	ObjLoader();
	int loadObj(const char * path, VertexData *& vData, unsigned int *& iData, unsigned int &vCount, unsigned int &iCount);

	~ObjLoader();
};

