#include "importer.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

bool loadObj(const char* path, std::vector<glm::vec3>* vertices, std::vector<glm::vec2>* texCoords, std::vector<glm::vec3>* normals, std::vector<unsigned int>* indices)
{
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_texCoords;
	std::vector<glm::vec3> temp_normals;
	std::vector<unsigned int> vert_indices;
	std::vector<unsigned int> texCoord_indices;
	std::vector<unsigned int> normal_indices;

	std::ifstream in;
	in.open(path);

	if(!in.is_open())
		return false;

	std::string temp;
	while (std::getline(in, temp))
	{
		if(temp.substr(0, 2) == "v ")
		{
			std::istringstream stringOp(temp.substr(2));
			glm::vec3 vertex;
			stringOp >> vertex.x >> vertex.y >> vertex.z;
			temp_vertices.emplace_back(vertex);
		}
		else if(temp.substr(0, 2) == "vt")
		{
			std::istringstream stringOp(temp.substr(3));
			glm::vec2 texCoord;
			stringOp >> texCoord.x >> texCoord.y;
			temp_texCoords.emplace_back(texCoord);
		}
		else if (temp.substr(0, 2) == "vn")
		{
			std::istringstream stringOp(temp.substr(3));
			glm::vec3 normal;
			stringOp >> normal.x >> normal.y >> normal.z;
			temp_normals.emplace_back(normal);
		}
		else if (temp.substr(0, 2) == "f ")
		{
			std::istringstream stringOp(temp.substr(2));
			unsigned int tempIndices[9];
			char crapValue;
			stringOp
			>> tempIndices[0] >> crapValue >> tempIndices[1] >> crapValue >> tempIndices[2]
			>> tempIndices[3] >> crapValue >> tempIndices[4] >> crapValue >> tempIndices[5]
			>> tempIndices[6] >> crapValue >> tempIndices[7] >> crapValue >> tempIndices[8];

			vert_indices.emplace_back(tempIndices[0]);
			vert_indices.emplace_back(tempIndices[3]);
			vert_indices.emplace_back(tempIndices[6]);
			
			texCoord_indices.emplace_back(tempIndices[1]);
			texCoord_indices.emplace_back(tempIndices[4]);
			texCoord_indices.emplace_back(tempIndices[7]);
			
			normal_indices.emplace_back(tempIndices[2]);
			normal_indices.emplace_back(tempIndices[5]);
			normal_indices.emplace_back(tempIndices[8]);
		}
	}

	for (int i = 0; i < vert_indices.size(); ++i)
	{
		unsigned int vertexIndex = vert_indices[i];
		vertices->emplace_back(temp_vertices[vertexIndex - 1]);

		unsigned int texCoordIndex = texCoord_indices[i];
		texCoords->emplace_back(temp_texCoords[texCoordIndex - 1]);

		unsigned int normalIndex = normal_indices[i];
		normals->emplace_back(temp_normals[normalIndex - 1]);

		indices->emplace_back(i);
	}

	return true;
}