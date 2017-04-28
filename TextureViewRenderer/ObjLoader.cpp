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
	DirectX::XMFLOAT4 * out_tangents;

	vertexIndices.reserve(25000); uvIndices.reserve(25000); normalIndices.reserve(25000);

	temp_vertices.reserve(2500000);
	temp_uvs.reserve(2500000);
	temp_normals.reserve(2500000);
	out_vertices.reserve(2500000);
	out_uvs.reserve(2500000);
	out_normals.reserve(2500000);

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
	out_tangents = new DirectX::XMFLOAT4[vertexIndices.size()];

	for (unsigned int i = 0; i < vertexIndices.size(); i++)
		iData[i] = i;
	
	CalculateTangentArray(vertexIndices.size(), out_vertices.data(), out_normals.data(), out_uvs.data(), iCount, iData, out_tangents);

	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		DirectX::XMStoreFloat3(&vData[i].pos, DirectX::XMVectorScale(DirectX::XMLoadFloat3(&out_vertices[i]), scale));
		vData[i].normal = out_normals[i];
		vData[i].UV = out_uvs[i];
		vData[i].tangent = DirectX::XMFLOAT4(out_tangents[i].x, out_tangents[i].y, out_tangents[i].z, out_tangents[i].w);
		if (openGL)
		{
			vData[i].pos.z *= -1.0;
			vData[i].normal.z *= -1.0;
			vData[i].tangent.z *= -1.0;
			vData[i].UV.y = 1.0 - vData[i].UV.y;
		}
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

	if (out_tangents != nullptr)
		delete[] out_tangents;
	return 1;
}

void ObjLoader::CalculateTangentArray(unsigned int vertexCount, const DirectX::XMFLOAT3 *vertex, const DirectX::XMFLOAT3 *normal,
	const DirectX::XMFLOAT2 *texcoord, unsigned int indexCount, unsigned int * indexList, DirectX::XMFLOAT4 *tangent)
{
	DirectX::XMFLOAT3 *tan1 = new DirectX::XMFLOAT3[vertexCount * 2];
	DirectX::XMFLOAT3 *tan2 = tan1 + vertexCount;
	ZeroMemory(tan1, vertexCount * sizeof(DirectX::XMFLOAT3) * 2);

	for (long a = 0; a < indexCount; a+=3)
	{
		long i1 = indexList[a];
		long i2 = indexList[a+1];
		long i3 = indexList[a+2];

		DirectX::XMFLOAT3 v1 = vertex[i1];
		DirectX::XMFLOAT3 v2 = vertex[i2];
		DirectX::XMFLOAT3 v3 = vertex[i3];

		DirectX::XMFLOAT2 w1 = texcoord[i1];
		DirectX::XMFLOAT2 w2 = texcoord[i2];
		DirectX::XMFLOAT2 w3 = texcoord[i3];

		float x1 = v2.x - v1.x;
		float x2 = v3.x - v1.x;
		float y1 = v2.y - v1.y;
		float y2 = v3.y - v1.y;
		float z1 = v2.z - v1.z;
		float z2 = v3.z - v1.z;

		float s1 = w2.x - w1.x;
		float s2 = w3.x - w1.x;
		float t1 = w2.y - w1.y;
		float t2 = w3.y - w1.y;

		float r = 1.0F / (s1 * t2 - s2 * t1);
		DirectX::XMFLOAT3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
			(t2 * z1 - t1 * z2) * r);
		DirectX::XMFLOAT3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
			(s1 * z2 - s2 * z1) * r);

		DirectX::XMStoreFloat3(&tan1[i1], DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&tan1[i1]), DirectX::XMLoadFloat3(&sdir)));
		DirectX::XMStoreFloat3(&tan1[i2], DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&tan1[i2]), DirectX::XMLoadFloat3(&sdir)));
		DirectX::XMStoreFloat3(&tan1[i3], DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&tan1[i3]), DirectX::XMLoadFloat3(&sdir)));
		
		DirectX::XMStoreFloat3(&tan2[i1], DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&tan2[i1]), DirectX::XMLoadFloat3(&sdir)));
		DirectX::XMStoreFloat3(&tan2[i2], DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&tan2[i2]), DirectX::XMLoadFloat3(&sdir)));
		DirectX::XMStoreFloat3(&tan2[i3], DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&tan2[i3]), DirectX::XMLoadFloat3(&sdir)));

		//tan1[i1] += sdir;
		//tan1[i2] += sdir;
		//tan1[i3] += sdir;

		//tan2[i1] += tdir;
		//tan2[i2] += tdir;
		//tan2[i3] += tdir;
	}

	for (long a = 0; a < vertexCount; a++)
	{
		DirectX::XMVECTOR n = DirectX::XMLoadFloat3(&normal[a]);
		DirectX::XMVECTOR t = DirectX::XMLoadFloat3(&tan1[a]);
		
		// Gram-Schmidt orthogonalize, Normalize((t - n * Dot(n, t)))
		DirectX::XMStoreFloat4(&tangent[a],
			DirectX::XMVector3Normalize(
				DirectX::XMVectorSubtract(t, DirectX::XMVectorScale(n, DirectX::XMVector3Dot(n, t).m128_f32[0])
				)
			)
		);
	
		// Calculate handedness tangent[a].w = (Dot(Cross(n, t), tan2[a]) < 0.0F) ? -1.0F : 1.0F;
		tangent[a].w = (DirectX::XMVector3Dot(DirectX::XMVector3Cross(n, t), DirectX::XMLoadFloat3(&tan2[a])).m128_f32[0] < 0.0F) ? -1.0F : 1.0F;

		//if ((DirectX::XMVector3Dot(DirectX::XMVector3Cross(n, t), DirectX::XMLoadFloat3(&tan2[a])).m128_f32[0] < 0.0F))
		//	tangent[a].z *= -1;
	}

	delete[] tan1;
}


ObjLoader::~ObjLoader()
{
}
