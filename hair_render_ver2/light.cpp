#include"light.h"
//set function
void Light::setType(LIGHT_TYPE t) { type = t; }
void Light::setEnergy(float value) {
	CLAMP(value, 0.0f, 10.0f);
	energy = value;
}
void Light::setDistance(float value) {
	CLAMP(value, 0.0f, 100000.0f);
	distance = value;
}
void Light::setVector(float x, float y, float z) {
	vector[0] = x;
	vector[1] = y;
	vector[2] = z;
	Normalize(vector);
}
void Light::setSpot_size(float value) {
	CLAMP(value, 180.0f, 1.0f);
	spot_size = value;
}
void Light::setBlend(float value) {
	CLAMP(value, 1.0f, 0.0f);
	blend = value;
}
void Light::setSize(float x, float y) {
	CLAMP(x, 0.0f, 100000.0f);
	CLAMP(y, 0.0f, 100000.0f);
	size[0] = x; size[1] = y;
}
void Light::setGamma(float value) {
	CLAMP(value, 2.0f, 0.0001f);
	gamma = value;
}
void Light::setColor(float r, float g, float b) {
	CLAMP(r, 0.0f, 1.0f);
	CLAMP(g, 0.0f, 1.0f);
	CLAMP(b, 0.0f, 1.0f);
	color[0] = r;
	color[1] = g;
	color[2] = b;
}

//get function
const LIGHT_TYPE Light::getType() { return type; }
const float Light::getEnergy() { return energy; }
const float Light::getDistance() { return distance; }
const float* Light::getVector() { return vector; }
const float Light::getSpot_size() { return spot_size; }
const float Light::getBlend() { return blend; }
const float* Light::getSize() { return size; }
const float Light::getGamma() { return gamma; }
const float* Light::getColor() { return color; }