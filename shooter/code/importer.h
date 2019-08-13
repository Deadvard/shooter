#ifndef IMPORTER_H
#define IMPORTER_H
#include <vector>
#include <glm/glm.hpp>

bool loadObj(const char* path, std::vector<glm::vec3>* vertices, std::vector<glm::vec2>* texCoords, std::vector<glm::vec3>* normals, std::vector<unsigned int>* indices);

#endif