#include "gameobject.h"

//getä÷êî
std::string &GameObject::getName() { return name; }
const float* GameObject::getPosition() { return position; }
const float* GameObject::getRotation() { return rotation; }
const float* GameObject::getScale() { return scale; }
const bool GameObject::getActive() { return active; }
const bool GameObject::getSelect() { return select; }

//setä÷êî
void GameObject::onSelect() { select = true; }
void GameObject::offSelect() { select = false; }
void GameObject::setName(const char* n) { name = n; }
void GameObject::setPosition(float x, float y, float z) {
	position[0] = x;
	position[1] = y;
	position[2] = z;
}
void GameObject::setRotation(float x, float y, float z) {
	rotation[0] = x;
	rotation[1] = y;
	rotation[2] = z;
}
void GameObject::setScale(float x, float y, float z) {
	scale[0] = x;
	scale[1] = y;
	scale[2] = z;
}
void GameObject::addPosition(float x, float y, float z) {
	position[0] += x;
	position[1] += y;
	position[2] += z;
}
void GameObject::addRotation(float x, float y, float z) {
	rotation[0] += x;
	rotation[1] += y;
	rotation[2] += z;
}
void GameObject::addScale(float x, float y, float z) {
	scale[0] += x;
	scale[1] += y;
	scale[2] += z;
}
void GameObject::setPrePosition() {
	preposition[0] = position[0];
	preposition[1] = position[1];
	preposition[2] = position[2];
}