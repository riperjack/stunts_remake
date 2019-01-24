#include "camera.hpp"

void Camera::setTarget(Actor *aTarget) {
	target = aTarget;
}

glm::mat4 Camera::getThirdPersonViewMatrix() {

	btVector3 targetPosition = target->getTransform().getOrigin();

	// prevent camera shaking by using average target height
	float avgHeight = 0.0f;
	static std::deque<float> targetHeights;

	if (targetHeights.size() <= heightSmoothSampleSize) {
		targetHeights.push_back(targetPosition[1]);
	}
	else {
		targetHeights.pop_front();
	}

	avgHeight = std::accumulate(targetHeights.begin(), targetHeights.end(), 0) / float(targetHeights.size());

	cameraPosition[1] = avgHeight + 3.0f;
	
	btVector3 camToObject = targetPosition - cameraPosition;

	float cameraDistance = camToObject.length();
	float correctionFactor = 0.f;

	if (cameraDistance < minCameraDistance)
	{
		correctionFactor = 0.15*(minCameraDistance - cameraDistance) / cameraDistance;
	}
	if (cameraDistance > maxCameraDistance)
	{
		correctionFactor = 0.15*(maxCameraDistance - cameraDistance) / cameraDistance;
	}
	cameraPosition -= correctionFactor * camToObject;

	return glm::lookAt(
		glm::vec3(cameraPosition[0], cameraPosition[1], cameraPosition[2]),
		glm::vec3(targetPosition[0], targetPosition[1], targetPosition[2]),
		glm::vec3(0, 1, 0)
	);
}

glm::mat4 Camera::getProjectionMatrix()
{
	return glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 10000.0f);
}
