#include "ObjLoader.h"
#include <vector>


ObjLoader::ObjLoader()
{
}

int ObjLoader::loadObj(const char * path, VertexData *& vData, unsigned int *& iData, unsigned int &vCount, unsigned int &iCount, float scale, bool openGL)
{
	FILE * file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return 0;
	}



	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< DirectX::XMFLOAT3 > temp_vertices;
	std::vector< DirectX::XMFLOAT2 > temp_uvs;
	std::vector< DirectX::XMFLOAT3 > temp_normals;
	std::vector< DirectX::XMFLOAT3 > out_vertices;
	std::vector< DirectX::XMFLOAT2 > out_uvs;
	std::vector< DirectX::XMFLOAT3 > out_normals;

	vertexIndices.reserve(25000); uvIndices.reserve(25000); normalIndices.reserve(25000);

	temp_vertices.reserve(25000000);
	temp_uvs.reserve(25000000);
	temp_normals.reserve(25000000);
	out_vertices.reserve(25000000);
	out_uvs.reserve(25000000);
	out_normals.reserve(25000000);

	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;
		if (strcmp(lineHeader, "v") == 0) {
			DirectX::XMFLOAT3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			DirectX::XMFLOAT2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			DirectX::XMFLOAT3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);


		}
	}

	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		DirectX::XMFLOAT3 vertex = temp_vertices[vertexIndex - 1];
		out_vertices.push_back(vertex);
	}
	for (unsigned int i = 0; i < uvIndices.size(); i++) {
		unsigned int uvIndex = uvIndices[i];
		DirectX::XMFLOAT2 uv = temp_uvs[uvIndex - 1];
		out_uvs.push_back(uv);
	}
	for (unsigned int i = 0; i < normalIndices.size(); i++) {
		unsigned int normalIndex = normalIndices[i];
		DirectX::XMFLOAT3 normal = temp_normals[normalIndex - 1];
		out_normals.push_back(normal);
	}


	vData = new VertexData[vertexIndices.size()];
	iData = new unsigned int[vertexIndices.size()];
	vCount = vertexIndices.size();
	iCount = vCount;
	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		DirectX::XMStoreFloat3(&vData[i].pos, DirectX::XMVectorScale(DirectX::XMLoadFloat3(&out_vertices[i]), scale));
		vData[i].normal = out_normals[i];
		vData[i].UV = out_uvs[i];
		if (openGL)
		{
			vData[i].pos.z *= -1.0;
			vData[i].normal.z *= -1.0;
			vData[i].UV.x = vData[i].UV.x - 1.0;
		}

		iData[i] = i;
	}

	if (openGL)
	{
		for (size_t i = 2; i < vertexIndices.size(); i += 3)
		{ 
			unsigned int temp = iData[i];
			iData[i] = iData[i - 1];
			iData[i - 1] = temp;
		}
	}

	return 1;
}


ObjLoader::~ObjLoader()
{
}
