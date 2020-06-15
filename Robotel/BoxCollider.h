#pragma once
#include <glm/glm.hpp>
#include<iostream>
#include<vector>
#include<math.h>
class BoxCollider
{
private:
	glm::vec3 min;
	glm::vec3 max;
	bool isOverlapping1D(float x1min, float x1max, float x2min, float x2max);
public:
	BoxCollider(glm::vec3 min, glm::vec3 max);
	bool Intersects(BoxCollider collider);
	void SetBounds(std::vector<glm::vec3> corners);
	glm::vec3 GetMin();
	glm::vec3 GetMax();
};

