#ifndef track_hpp
#define track_hpp

#include "actor.hpp"
#include <fstream>
#include <string>
#include <regex>

class Vehicle : public Actor {

public:
	Vehicle(const char *meshFilename, 
		const char *collider, 
		const char *settingsFilename, 
		btDiscreteDynamicsWorld* dynamicsWorld,
		Actor* wheelFR,
		Actor* wheelFL,
		Actor* wheelBR,
		Actor* wheelBL);
	~Vehicle();
	btRaycastVehicle *vehicle;
	virtual void handleUserInput(double deltaTime, GLFWwindow* window);
	virtual void updateTransform();

protected:
	float steering;
	float steeringClamp;
	float steeringIncrement;
	float engineForce;
	Mesh *meshCollider;
	btVehicleRaycaster* raycaster;
	std::map<std::string, float> settings;
	std::vector<Actor*> wheels;
	std::map<std::string, float> loadSettings(const char* settingsFilename);
};

#endif