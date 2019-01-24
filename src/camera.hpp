#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <queue>
#include <iostream>
#include <numeric>
#include <glm.hpp>
#include "actor.hpp"

class Camera {

public:
	void setTarget(Actor *aTarget);
	glm::mat4 getThirdPersonViewMatrix();
	glm::mat4 getProjectionMatrix();

private:
	Actor *target;
	btVector3 cameraPosition = btVector3(1, 10, -20);
	btVector3 cameraTargetPosition;
	float cameraHeight = 5.0f;
	float minCameraDistance = 5.f;
	float maxCameraDistance = 20.f;
	int heightSmoothSampleSize = 200;
};

#endif
