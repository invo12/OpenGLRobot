#include "Raycast.h"
#include <map>

Object* Raycast::castRay(glm::vec3 origin, glm::vec3 direction, float maxDistance, std::vector<Object*>* objects)
{
	direction = glm::normalize(direction);
	float dist = maxDistance / 100;
	glm::vec3 point1 = origin;
	glm::vec3 point2 = point1 + dist * direction;
	for (int i = 0; i < 100; ++i)
	{
		for (int j = 0; j < objects->size(); ++j)
		{
			if ((*objects)[j]->GetCollider()->Intersects(point1, point2) && (*objects)[j]->IsActive() && (*objects)[j]->GetLayer() != Layer::IgnoreRaycast)
 				return (*objects)[j];
		}
		point1 = point2;
		point2 = point1 + dist * direction;
	}
	return nullptr;
}
