#include "BoxCollider.h"

bool BoxCollider::isOverlapping1D(float x1min, float x1max, float x2min, float x2max)
{
	return x1max >= x2min && x2max >= x1min;
}

BoxCollider::BoxCollider(glm::vec3 min, glm::vec3 max)
{
	float factor = 1.0f;
	this->min = glm::vec3(min.x * factor, min.y * factor,min.z * factor);
	this->max = glm::vec3(max.x * factor, max.y * factor, max.z * factor);
}

bool BoxCollider::Intersects(BoxCollider collider)
{
	/*std::cout << collider.GetMin().x << ' ' << collider.GetMin().y << ' ' << collider.GetMin().z << std::endl;
	std::cout << collider.GetMax().x << ' ' << collider.GetMax().y << ' ' << collider.GetMax().z << std::endl;

	std::cout << this->GetMin().x << ' ' << this->GetMin().y << ' ' << this->GetMin().z << std::endl;
	std::cout << this->GetMax().x << ' ' << this->GetMax().y << ' ' << this->GetMax().z << std::endl;
	std::cout << std::endl;*/
	return isOverlapping1D(collider.GetMin().x, collider.GetMax().x, this->min.x, this->max.x) &&
		isOverlapping1D(collider.GetMin().y, collider.GetMax().y, this->min.y, this->max.y) &&
		isOverlapping1D(collider.GetMin().z, collider.GetMax().z, this->min.z, this->max.z);
}

bool BoxCollider::Intersects(glm::vec3 point1, glm::vec3 point2)
{
	return true;
	//TODO
}

void BoxCollider::SetBounds(std::vector<glm::vec3> corners)
{
	glm::vec3 mini = corners[0];
	glm::vec3 maxi = corners[0];
	for (int i = 1; i < corners.size(); ++i)
	{
		if (corners[i].x < mini.x)
		{
			mini.x = corners[i].x;
		}
		if (corners[i].y < mini.y)
		{
			mini.y = corners[i].y;
		}
		if (corners[i].z < mini.z)
		{
			mini.z = corners[i].z;
		}
		if (corners[i].x > maxi.x)
		{
			maxi.x = corners[i].x;
		}
		if (corners[i].y > maxi.y)
		{
			maxi.y = corners[i].y;
		}
		if (corners[i].z > maxi.z)
		{
			maxi.z = corners[i].z;
		}
	}
	min = mini;
	max = maxi;
}

glm::vec3 BoxCollider::GetMin()
{
	return min;
}

glm::vec3 BoxCollider::GetMax()
{
	return max;
}
