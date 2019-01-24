#include "actor.hpp"

Actor::Actor(const char *meshFilename, btDiscreteDynamicsWorld* dynamicsWorld): dynamicsWorld(dynamicsWorld)
{
	mesh = new Mesh(meshFilename);
	setupRigidbody();
}

Actor::~Actor()
{
	delete mesh;
	delete rigidBody;
	delete motionState;
	delete collisionShape;
}

glm::mat4 Actor::getModelMatrix(){
    glm::mat4 locationMatrix = glm::translate(
		glm::mat4(1.0f), 
		glm::vec3(transform.getOrigin().getX(), transform.getOrigin().getY(), transform.getOrigin().getZ())
	);

    glm::mat4 rotationMatrix = glm::toMat4(
		glm::quat(
			transform.getRotation().getW(),
			transform.getRotation().getX(),
			transform.getRotation().getY(),
			transform.getRotation().getZ()
		)
	);

    return locationMatrix * rotationMatrix;
}

void Actor::setupRigidbody() {
	collisionShape = new btBvhTriangleMeshShape(mesh->meshInterface, false, true);
	motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, motionState, collisionShape, btVector3(0, 0, 0));
	rigidBody = new btRigidBody(groundRigidBodyCI);
	rigidBody->setContactProcessingThreshold(0.f);

	dynamicsWorld->addRigidBody(rigidBody);
}

btTransform Actor::getTransform() {
	return transform;
}

void Actor::updateTransform() {
	btTransform btTransform;
	rigidBody->getMotionState()->getWorldTransform(btTransform);
	transform = btTransform;
}

void Actor::updateTransform(const btTransform &btTransform) {
	transform = btTransform;
}

void Actor::handleUserInput(double deltaTime, GLFWwindow* window) {
}
