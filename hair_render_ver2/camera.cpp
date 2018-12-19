#include"camera.h"

void Camera::update()
{
	const float *pos = getPosition();
	direction[0] = regard[0] - pos[0];
	direction[1] = regard[1] - pos[1];
	direction[2] = regard[2] - pos[2];
	Normalize(direction);
}

//setfunction
void Camera::setType(CAMERA_TYPE t) { type = t; }
void Camera::setRegard(float x, float y, float z) {
	regard[0] = x;
	regard[1] = y;
	regard[2] = z;
}
void Camera::setUpvector(float x, float y, float z) {
	upvector[0] = x;
	upvector[1] = y;
	upvector[2] = z;
}
void Camera::setDirection(float x, float y, float z) {
	direction[0] = x;
	direction[1] = y;
	direction[2] = z;
}
void Camera::setPerth_size(float a, float f, float far, float near) {
	aspect = a;
	fovy = f;
	perth_size[0] = far;
	perth_size[1] = near;
}
void Camera::setOrtho_size(float r, float l, float t, float b, float f, float n)
{
	ortho_size[0] = r; ortho_size[1] = l;
	ortho_size[2] = t; ortho_size[3] = b;
	ortho_size[4] = f; ortho_size[5] = n;
}

//get function
const CAMERA_TYPE Camera::getType() { return type; }
const float* Camera::getRegard() { return regard; }
const float* Camera::getUpvector() { return upvector; }
const float* Camera::getDirection() { return direction; }
const float Camera::getAspect() { return aspect; }
const float Camera::getFovy() { return fovy; }
const float* Camera::getPerth_size() { return perth_size; }
const float* Camera::getOrtho_size() { return ortho_size; }

//camera animation
void Camera::ZoomFunc(float w) {
	float pos[3] = { getPosition()[0],getPosition()[1],getPosition()[2] };
	mouseZoom(w, direction, pos);
	setPosition(pos[0], pos[1], pos[2]);
}
void Camera::RotateFunc(float x, float y) {
	float pos[3] = { getPosition()[0],getPosition()[1],getPosition()[2] };
	mouseRotate(x, y, direction, pos, regard);
	setPosition(pos[0], pos[1], pos[2]);
}
void Camera::TranslateFunc(float x, float y) {
	float pos[3] = { getPosition()[0],getPosition()[1],getPosition()[2] };
	mouseTranslate(x, y, direction, pos);
	addPosition(pos[0], pos[1], pos[2]);
	regard[0] += pos[0]; regard[1] += pos[1]; regard[2] += pos[2];
}