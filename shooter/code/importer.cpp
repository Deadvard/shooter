#include "importer.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

bool loadObj(const char* path, std::vector<glm::vec3>* vertices, std::vector<glm::vec2>* texCoords, std::vector<glm::vec3>* normals, std::vector<unsigned int>* indices)
{
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
			(*vertices).emplace_back(vertex);
		}
		else if(temp.substr(0, 2) == "vt")
		{
			std::istringstream stringOp(temp.substr(3));
			glm::vec2 texCoord;
			stringOp >> texCoord.x >> texCoord.y;
			(*texCoords).emplace_back(texCoord);
		}
		else if (temp.substr(0, 2) == "vn")
		{
			std::istringstream stringOp(temp.substr(3));
			glm::vec3 normal;
			stringOp >> normal.x >> normal.y >> normal.z;
			(*normals).emplace_back(normal);
		}
		else if (temp.substr(0, 2) == "f ")
		{
			std::istringstream stringOp(temp.substr(2));
			unsigned int tempIndices[9];
			stringOp
			>> tempIndices[0] >> tempIndices[1] >> tempIndices[2]
			>> tempIndices[3] >> tempIndices[4] >> tempIndices[5]
			>> tempIndices[6] >> tempIndices[7] >> tempIndices[8];

			for(int i = 0; i < 1; ++i)
			{
				(*indices).emplace_back(tempIndices[i+0]);
				(*indices).back()--;
				(*indices).emplace_back(tempIndices[i+3]);
				(*indices).back()--;
				(*indices).emplace_back(tempIndices[i+6]);
				(*indices).back()--;
			}
		}
	}

	return true;
}