#include "Player.h"

Player::Player(std::string numeFisier, Shader* shader, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale):Object(numeFisier,shader, position, rotation,scale)
{
	this->SetLayer(Layer::IgnoreRaycast);
	this->bullet = new Bullet(this->GetShader());
}

void Player::SetScene(vector<Object*>* scene)
{
	this->scena = scene;
}

void Player::Shoot()
{
	this->bullet->SetRotation(glm::vec3(90, 0, 270));
	this->bullet->SetPosition(glm::vec3(GetPosition().x, GetPosition().y+0.2f, GetPosition().z-0.15f));
	this->bullet->SetScale(glm::vec3(0.0025f, 0.0025f, 0.0025f));
	this->scena->push_back(this->bullet);
}

void Player::Draw()
{
	Object::Draw();
	bullet->Draw();
}