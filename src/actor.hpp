//
//  GameObject.hpp
//  stunts
//
//  Created by Marcin Sewastianowicz on 4/10/18.
//  Copyright © 2018 Marcin Sewastianowicz. All rights reserved.
//
#ifndef actor_hpp
#define actor_hpp

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <vector>
#include <glfw3.h>
#include <glm.hpp>
#include <ext.hpp>
#include <btBulletDynamicsCommon.h>
#include "mesh.hpp"

class Actor
{
public:
	Actor();
	Actor(const char *meshFilename, btDiscreteDynamicsWorld* dynamicsWorld);
	~Actor();

	Mesh* mesh;
    glm::mat4 getModelMatrix();
	btTransform getTransform();
	void setupRigidbody();
	virtual void updateTransform();
	virtual void updateTransform(const btTransform &btTransform);
	virtual void handleUserInput(double deltaTime, GLFWwindow* window);

protected:
	btTransform transform;
	btDiscreteDynamicsWorld* dynamicsWorld;
	btRigidBody* rigidBody;
	btDefaultMotionState* motionState;
	btCollisionShape* collisionShape;
};

#endif
