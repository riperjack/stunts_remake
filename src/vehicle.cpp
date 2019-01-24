#include "vehicle.hpp"

Vehicle::Vehicle(const char *meshFilename, 
	const char *collider, 
	const char *settingsFilename, 
	btDiscreteDynamicsWorld* dynamicsWorld,
	Actor* wheelFR,
	Actor* wheelFL,
	Actor* wheelBR,
	Actor* wheelBL) :
	Actor(meshFilename, dynamicsWorld) {
	
	settings = loadSettings(settingsFilename);

	wheels.push_back(wheelFL);
	wheels.push_back(wheelFR);
	wheels.push_back(wheelBL);
	wheels.push_back(wheelBR);

	steeringClamp = settings.at("steeringClamp");
	steeringIncrement = settings.at("steeringIncrement");
	engineForce = settings.at("engineForce");

	meshCollider = new Mesh(collider);
	btScalar chassisMass(settings.at("chassisMass"));
	btVector3 chassisInertia(0.0f, 0.0f, 0.0f);
	collisionShape = new btConvexTriangleMeshShape(meshCollider->meshInterface);

	btQuaternion initalRotation = btQuaternion(0, 0, 0, 1);
	initalRotation.setRotation(btVector3(0, 1, 0), btScalar(3.14));
	motionState = new btDefaultMotionState(btTransform(initalRotation, btVector3(-4.f, 5.f, 2.f)));
	collisionShape->calculateLocalInertia(chassisMass, chassisInertia);
	btRigidBody::btRigidBodyConstructionInfo chassisRigidBodyCI(chassisMass, motionState, collisionShape, chassisInertia);
	rigidBody = new btRigidBody(chassisRigidBodyCI);

	rigidBody->setActivationState(DISABLE_DEACTIVATION);	
	dynamicsWorld->addRigidBody(rigidBody);

	raycaster = new btDefaultVehicleRaycaster(dynamicsWorld);
	btRaycastVehicle::btVehicleTuning tuning;

	tuning.m_suspensionStiffness = settings.at("suspensionStiffness");
	tuning.m_suspensionDamping = settings.at("suspensionDamping");
	tuning.m_suspensionCompression = settings.at("suspensionCompression");
	tuning.m_maxSuspensionForce = settings.at("maxSuspensionForce");
	tuning.m_maxSuspensionTravelCm = settings.at("maxSuspensionTravelCm");
	tuning.m_frictionSlip = settings.at("frictionSlip");

	vehicle = new btRaycastVehicle(tuning, rigidBody, raycaster);
	vehicle->setCoordinateSystem(0, 1, 2);
	
	btVector3 wheelDirection(0.0f, -1.0f, 0.0f);
	btVector3 wheelAxis(-1.0f, 0.0f, 0.0f);
	btScalar suspensionRestLength(settings.at("suspensionRestLength"));
	btScalar wheelRadius(0.4f);

	vehicle->addWheel(btVector3(-0.7f, 0.0f, 0.8f), wheelDirection, wheelAxis, suspensionRestLength, wheelRadius, tuning, true);
	vehicle->addWheel(btVector3(0.7f, 0.0f, 0.8f), wheelDirection, wheelAxis, suspensionRestLength, wheelRadius, tuning, true);
	vehicle->addWheel(btVector3(-0.7f, 0.0f, -1.45f), wheelDirection, wheelAxis, suspensionRestLength, wheelRadius, tuning, false);
	vehicle->addWheel(btVector3(0.7f, 0.0f, -1.45f), wheelDirection, wheelAxis, suspensionRestLength, wheelRadius, tuning, false);

	dynamicsWorld->addVehicle(vehicle);
}

Vehicle::~Vehicle() {
	delete meshCollider;
	delete raycaster;
	delete vehicle;
}

void Vehicle::handleUserInput(double deltaTime, GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		vehicle->applyEngineForce(engineForce, 0);
		vehicle->applyEngineForce(engineForce, 1);
	} 
	
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS ) {
		vehicle->applyEngineForce(-engineForce, 0);
		vehicle->applyEngineForce(-engineForce, 1);
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		steering += steeringIncrement * deltaTime;
		steering = std::fmin(steering, steeringClamp);
	}
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		steering -= steeringIncrement * deltaTime;
		steering = std::fmax(steering, -steeringClamp);
	}
	else {
		steering = 0.0f;
	}
	
	vehicle->setSteeringValue(steering, 0);
	vehicle->setSteeringValue(steering, 1);
}

void Vehicle::updateTransform() {
	Actor::updateTransform();

	for (int i = 0; i < wheels.size(); i++) {
		vehicle->updateWheelTransform(i, true);
		wheels[i]->updateTransform(vehicle->getWheelInfo(i).m_worldTransform);
	}
}

std::map<std::string, float> Vehicle::loadSettings(const char* settingsFilename) {
	std::map<std::string, float> settings = std::map<std::string, float>();
	std::ifstream infile(settingsFilename);
	
	for (std::string line; getline(infile, line); )
	{
		std::regex reg_whitespace("\\s+");
		std::vector<std::string> result{
			std::sregex_token_iterator(line.begin(), line.end(), reg_whitespace, -1), {}
		};
		settings[result[0]] = strtof((result[1]).c_str(), 0);
	}

	return settings;
}