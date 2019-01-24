#include "utility.hpp"
#include "actor.hpp"
#include "camera.hpp"
#include "renderer.hpp"
#include "physics.hpp"
#include "vehicle.hpp"

int main()
{
	Utility systemUtility = Utility();
	GLFWwindow* window = systemUtility.createWindow();
	Physics* physics = new Physics();
	Camera* camera = new Camera();
	Renderer renderer = Renderer();

	std::vector<Actor*> scene;

	const char *wheelModelFilename = "assets/models/wheel.obj";
	
	Actor* wheelFR = new Actor(wheelModelFilename, physics->dynamicsWorld);
	scene.push_back(wheelFR);
	
	Actor* wheelFL = new Actor(wheelModelFilename, physics->dynamicsWorld);
	scene.push_back(wheelFL);
	
	Actor* wheelBR = new Actor(wheelModelFilename, physics->dynamicsWorld);
	scene.push_back(wheelBR);
	
	Actor* wheelBL = new Actor(wheelModelFilename, physics->dynamicsWorld);
	scene.push_back(wheelBL);

	Vehicle* vehicle = new Vehicle("assets/models/car01.obj", "assets/models/car01_collider.obj", "assets/settings/car01.txt", physics->dynamicsWorld, wheelFR, wheelFL, wheelBR, wheelBL);
	scene.push_back(vehicle);

	Actor* road = new Actor("assets/models/road.obj", physics->dynamicsWorld);
	scene.push_back(road);
	
	Actor* terrain = new Actor("assets/models/terrain.obj", physics->dynamicsWorld);
	scene.push_back(terrain);
	
	camera->setTarget(vehicle);

	double deltaTime;

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS 
		&& glfwWindowShouldClose(window) == 0) {

		double frameStartTime = glfwGetTime();

		for (int i = 0; i < scene.size(); i++) {
			scene[i]->updateTransform();
			scene[i]->handleUserInput(deltaTime, window);
		}
		
		renderer.render(scene, camera);

		glfwSwapBuffers(window);
		glfwPollEvents();

		double frameEndTime = glfwGetTime();
		deltaTime = frameEndTime - frameStartTime;

		physics->dynamicsWorld->stepSimulation(deltaTime, 1);
	}

	glfwTerminate();

	delete wheelFR;
	delete wheelFL;
	delete wheelBR;
	delete wheelBL;
	delete vehicle;
	delete road;
	delete terrain;
	delete physics;
	delete camera;

    return 0;
}
