#include "Player.h"

Player::Player(std::string numeFisier, Shader* shader, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale):Object(numeFisier,shader, position, rotation,scale)
{
	this->SetLayer(Layer::IgnoreRaycast);
}

void Player::SetScene(vector<Object*>* scene)
{
	this->scena = scene;
}

void Player::Draw()
{
	Object::Draw();
}