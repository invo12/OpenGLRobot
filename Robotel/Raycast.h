#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Object.h"
class Raycast
{
public:
	static Object* castRay(glm::vec3 origin, glm::vec3 direction, float maxDistance, std::vector<Object*>* objects);
};

