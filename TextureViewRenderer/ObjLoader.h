#pragma once
#include "DataStructs.h"
class ObjLoader
{
public:
	ObjLoader();
	int loadObj(const char * path, VertexData &vData);

	~ObjLoader();
};

