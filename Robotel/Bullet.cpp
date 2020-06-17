#include "Bullet.h"

Bullet::Bullet(Shader* shader, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) :Object("Assets/projectile/bullet", shader, position, rotation, scale)
{
	this->SetScale(glm::vec3(0.0015f, 0.0015f, 0.0015f));
}

