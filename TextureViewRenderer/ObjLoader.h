#pragma once
#include "DataStructs.h"
class ObjLoader
{
public:
	ObjLoader();
	int loadObj(const char * path, VertexData *& vData, unsigned int *& iData, unsigned int &vCount, unsigned int &iCount, float scale = 1.0f, bool openGL = true);

	~ObjLoader();
};

